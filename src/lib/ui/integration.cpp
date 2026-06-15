#include "expander.h"
#include "lib/helper.h"
#include "lib/terminal.h"
#include "lib/ui/integration.h"
#include "lib/ui/menu.h"

#include <commctrl.h>


#define MAIN_WINDOW_SUBCLASS_ID     1              // subclass identifier for the main window
#define CHART_WINDOW_SUBCLASS_ID    2              // subclass identifier for chart windows

static HHOOK hMainWndSendMsgHook = NULL;           // hook handle


/**
 * Setup UI integration of the Expander. The first call is always from a non-UI thread (DLL loader).
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetupUiIntegration() {
   // Some integration tasks must run in the UI thread, some tasks must NOT run there to not delay startup.

   if (!IsUIThread()) {
      static BOOL done = FALSE;
      if (!done) {
         done = TRUE;
         // perform configured modifications
         if (!CustomizeTerminal()) return FALSE;

         // register a hook in the UI thread and continue there
         if (hMainWndSendMsgHook) return FALSE;
         hMainWndSendMsgHook = SetWindowsHookEx(WH_CALLWNDPROC, MainWindowSendMessageHook, NULL, GetUIThreadId());
         if (!hMainWndSendMsgHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx()");

         // wake-up the UI thread
         SendMessageTimeout(GetTerminalMainWindow(), WM_NULL, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 3000, NULL);
      }
   }
   else {
      static BOOL done = FALSE;
      if (!done) {
         done = TRUE;
         if (!SubclassMainWindow())   return FALSE;      // subclass the terminal main window
         if (!SubclassChartWindows()) return FALSE;      // subclass all current and future chart windows
      }
   }
   return TRUE;
}


/**
 * Hook function for messages sent to the terminal main window. Triggers Expander integration tasks which require
 * execution in the UI thread, and immediately removes itself.
 *
 * @param  int    code   - below 0 (zero) if the hook should skip the message
 * @param  WPARAM wParam - whether the message was sent by the current thread
 * @param  LPARAM lParam - pointer to message details
 *
 * @return LRESULT - return value of CallNextHookEx()
 */
static LRESULT CALLBACK MainWindowSendMessageHook(int code, WPARAM wParam, LPARAM lParam) {
   if (code >= 0) {
      if (hMainWndSendMsgHook) {
         HHOOK hHook = hMainWndSendMsgHook;
         hMainWndSendMsgHook = NULL;
         if (!UnhookWindowsHookEx(hHook)) error(ERR_WIN32_ERROR + GetLastError(), "UnhookWindowsHookEx(hHook=0x%p)", hHook);
         SetupUiIntegration();
      }
   }
   return CallNextHookEx(hMainWndSendMsgHook, code, wParam, lParam);   // NULL after the hook was removed
}


/**
 * Subclass the terminal main window.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassMainWindow() {
   if (!SetWindowSubclass(GetTerminalMainWindow(), MainWindowSubclassProc, MAIN_WINDOW_SUBCLASS_ID, 0)) {
      return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowSubclass()");
   }
   return TRUE;
}


/**
 * The main window's subclassing window procedure. Executed in the UI thread.
 *
 * @param  HWND      hWnd       - subclassed window receiving the message
 * @param  uint      msg        - sent message
 * @param  WPARAM    wParam     - additional message info
 * @param  LPARAM    lParam     - additional message info
 * @param  UINT_PTR  subclassId - subclass identifier
 * @param  DWORD_PTR data       - user data as passed to SetWindowSubclass()
 *
 * @return LRESULT - depends on the message sent
 */
static LRESULT CALLBACK MainWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data) {
   switch (msg) {
      case WM_COMMAND: {
         static DWORD debugOptions = GetDebugOptions();
         if (debugOptions & OPTION_DEBUG_WM_COMMAND) debug("WM_COMMAND  id=%d  lParam=0x%p", wParam, lParam);
         break;
      }

      case WM_INITMENUPOPUP: {
         HMENU hMenu = (HMENU)wParam;
         BOOL isSystemMenu = HIWORD(lParam);
         if (!isSystemMenu && IsChartTemplatesMenu(hMenu)) {
            RebuildChartTemplatesMenu(hMenu);
         }
         break;
      }

      case WM_QUERYENDSESSION: {                   // Windows: "Are you ready to shut down?"
         if (lParam & ENDSESSION_LOGOFF) {}        // user logoff
         else                            {}        // system shutdown/restart
         debug("WM_QUERYENDSESSION  %s", lParam & ENDSESSION_LOGOFF ? "logoff" : "shutdown");
         break;
      }

      case WM_ENDSESSION: {                        // workaround for terminal bug https://github.com/rosasurfer/mt4-expander/issues/26
         if (wParam) {                             // Windows: "Logoff/shutdown is happening now. You have ~5 seconds."
            if (lParam & ENDSESSION_LOGOFF) {}     // user logoff
            else                            {}     // system shutdown/restart
            debug("WM_ENDSESSION  %s", lParam & ENDSESSION_LOGOFF ? "logoff" : "shutdown");
         }//else                                   // logoff/shutdown was cancelled
         break;
      }

      case WM_NCDESTROY: {
         RemoveWindowSubclass(hWnd, MainWindowSubclassProc, subclassId);
         break;
      }
   }
   return DefSubclassProc(hWnd, msg, wParam, lParam);
}


/**
 * Subclass all current and future chart windows.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassChartWindows() {
   return TRUE;
}


/**
 * A chart window's subclassing window procedure. Executed in the UI thread.
 *
 * @param  HWND      hWnd       - subclassed window receiving the message
 * @param  uint      msg        - sent message
 * @param  WPARAM    wParam     - additional message info
 * @param  LPARAM    lParam     - additional message info
 * @param  UINT_PTR  subclassId - subclass identifier
 * @param  DWORD_PTR data       - user data as passed to SetWindowSubclass()
 *
 * @return LRESULT - depends on the message sent
 */
static LRESULT CALLBACK ChartWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data) {
   return DefSubclassProc(hWnd, msg, wParam, lParam);
}


/**
 * Customize the UI of the terminal as configured. Runs in a non-UI thread to not delay terminal startup.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI CustomizeTerminal() {
   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return FALSE;

   // find the toolbar
   HWND hToolbar = GetDlgItem(hWndMain, IDC_TOOLBAR);
   if (!hToolbar) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(MainWindow, IDC_TOOLBAR)");

   // find and remove a search box control (contains the "Community" button, builds > 509)
   HWND hSearchCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_SEARCHBOX);
   if (hSearchCtrl) {
      PostMessageA(hSearchCtrl, WM_CLOSE, 0, 0);      // no redrawing needed
      return TRUE;
   }

   // find and remove an independent "Community" button (builds <= 509)
   HWND hBtnCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_COMMUNITY_BUTTON);
   if (hBtnCtrl) {
      PostMessageA(hBtnCtrl, WM_CLOSE, 0, 0);         // no redrawing needed
   }
   return TRUE;
}
