#include "expander.h"
#include "lib/helper.h"
#include "lib/terminal.h"
#include "lib/ui/integration.h"
#include "lib/ui/menu.h"

#include <commctrl.h>


#define MAIN_WINDOW_SUBCLASS_ID     1              // subclass identifier for the main window
#define CHART_WINDOW_SUBCLASS_ID    2              // subclass identifier for chart windows

static HHOOK hMainWindowSendMessageHook = NULL;    // hook handle


/**
 * Setup UI integration of the Expander. The first call is always from a non-UI thread (worker in DLL loader).
 * Some integration tasks must run in the UI thread, some must not run there, some may run anywhere.
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetupUiIntegration() {
   // register a hook for window events in the UI thread (should be the very first task)
   static HHOOK hWindowEventsHook = SetWindowsHookExW(WH_CBT, WindowEventsHook, NULL, GetUIThreadId());
   if (!hWindowEventsHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx(WindowEventsHook)");

   // if in a non-UI thread
   if (!IsUIThread()) {
      static BOOL done = FALSE;
      if (!done) {
         done = TRUE;
         // perform configured modifications
         if (!CustomizeTerminal()) return FALSE;

         // register a hook in the UI thread and continue there
         if (hMainWindowSendMessageHook) return FALSE;
         hMainWindowSendMessageHook = SetWindowsHookEx(WH_CALLWNDPROC, MainWindowSendMessageHook, NULL, GetUIThreadId());
         if (!hMainWindowSendMessageHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx(MainWindowSendMessageHook)");
         // wake-up the UI thread
         SendMessageTimeout(GetTerminalMainWindow(), WM_NULL, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 3000, NULL);
      }
      return TRUE;
   }

   // if in the UI thread
   static BOOL done = FALSE;
   if (!done) {
      done = TRUE;
      if (!SubclassMainWindow())   return FALSE;      // subclass the terminal main window
      if (!SubclassChartWindows()) return FALSE;      // subclass existing chart windows
   }
   return TRUE;
}


/**
 * Hook function receiving window related events of the UI thread (beneath many others). The system calls this function
 * before activating, creating, destroying, minimizing, maximizing, moving, or sizing a window.
 *
 * @param  int    type   - event type; below 0 (zero) if the hook should skip the event
 * @param  WPARAM wParam - whether the event was sent by the current thread
 * @param  LPARAM lParam - pointer to event details
 *
 * @return LRESULT - whether the system should allow or prevent the event operation (depends on the event type)
 */
static LRESULT CALLBACK WindowEventsHook(int type, WPARAM wParam, LPARAM lParam) {
   switch (type) {
      case HCBT_CREATEWND: {           // A window is about to be created.
         HWND hWnd = (HWND)wParam;
         debug("HCBT_CREATEWND  %p", hWnd);
         break;
      }
      case HCBT_DESTROYWND: break;     // A window is about to be destroyed.
      case HCBT_MINMAX:     break;     // A window is about to be minimized or maximized.
      case HCBT_MOVESIZE:   break;     // A window is about to be moved or sized.
      case HCBT_SETFOCUS:   break;     // A window is about to receive the keyboard focus.
   }
   return CallNextHookEx(NULL, type, wParam, lParam);
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
      if (hMainWindowSendMessageHook) {
         HHOOK hHook = hMainWindowSendMessageHook;
         hMainWindowSendMessageHook = NULL;
         if (!UnhookWindowsHookEx(hHook)) error(ERR_WIN32_ERROR + GetLastError(), "UnhookWindowsHookEx(hHook=0x%p)", hHook);
         SetupUiIntegration();
      }
   }
   return CallNextHookEx(hMainWindowSendMessageHook, code, wParam, lParam);   // NULL after the hook was removed
}


/**
 * Subclass the terminal main window.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassMainWindow() {
   if (!IsUIThread()) return !error(ERR_ILLEGAL_STATE, "must run in the UI thread");

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
 * Subclass all existing chart windows. Executed in the UI thread.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassChartWindows() {
   if (!IsUIThread()) return !error(ERR_ILLEGAL_STATE, "must run in the UI thread");
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
   if (IsUIThread()) return !error(ERR_ILLEGAL_STATE, "must not run in the UI thread");

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
