#include "expander.h"
#include "lib/helper.h"
#include "lib/terminal.h"
#include "lib/ui/integration.h"
#include "lib/ui/menu.h"

#include <commctrl.h>


const uint MAIN_WINDOW_SUBCLASS_ID  = 1;              // subclass identifier for the main window
const uint CHART_WINDOW_SUBCLASS_ID = 2;              // subclass identifier for chart windows

static HHOOK g_hUiHook = NULL;                        // hook handle to switch to the UI thread


/**
 * Setup UI integration of the Expander.
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetupUiIntegration() {
   // make sure we run in the UI thread
   if (!IsUIThread()) {
      if (g_hUiHook) return FALSE;
      g_hUiHook = SetWindowsHookEx(WH_CALLWNDPROC, UiThreadProc, NULL, GetUIThreadId());
      if (!g_hUiHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx()");

      HWND hWndMain = GetTerminalMainWindow();
      if (!hWndMain) return FALSE;

      // wake-up the UI thread with a non-blocking SendMessage()
      SetLastError(ERROR_SUCCESS);
      if (!SendMessageTimeout(hWndMain, WM_NULL, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 3000, NULL)) {
         debug(ERR_WIN32_ERROR + GetLastError(), "SendMessageTimeout()");
      }
      return TRUE;
   }

   // we run in the UI thread
   if (!SubclassMainWindow())   return FALSE;      // subclass the terminal main window
   if (!SubclassChartWindows()) return FALSE;      // subclass all current and future chart windows
   if (!CustomizeTerminal())    return FALSE;      // customize the terminal as configured
   return TRUE;
}


/**
 * Callback function registered by SetWindowsHookEx(). Runs in the UI thread.
 *
 * @param  int    code   - whether the hook procedure must process the message
 * @param  WPARAM wParam - whether the message was sent by the current thread
 * @param  LPARAM lParam - pointer to message details
 *
 * @return LRESULT - return value of CallNextHookEx()
 */
static LRESULT CALLBACK UiThreadProc(int code, WPARAM wParam, LPARAM lParam) {
   if (code >= 0) {
      if (g_hUiHook) {
         HHOOK hHook = g_hUiHook;
         g_hUiHook = NULL;
         if (!UnhookWindowsHookEx(hHook)) {
            error(ERR_WIN32_ERROR + GetLastError(), "UnhookWindowsHookEx(hHook=0x%p)", hHook);
         }
      }
      SetupUiIntegration();
   }
   return CallNextHookEx(NULL, code, wParam, lParam);
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
 * Customize the UI of the terminal as configured.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI CustomizeTerminal() {
   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return FALSE;

   // get the toolbar
   HWND hToolbar = GetDlgItem(hWndMain, IDC_TOOLBAR);
   if (!hToolbar) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(MainWindow, IDC_TOOLBAR) not found");

   // find and remove a search box control (contains the "Community" button)
   if (HWND hSearchCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_SEARCHBOX)) {
      if (!DestroyWindow(hSearchCtrl)) {
         return !error(ERR_WIN32_ERROR + GetLastError(), "DestroyWindow(IDC_TOOLBAR_SEARCHBOX)");;
      }
      if (!RedrawWindow(hToolbar, NULL, NULL, RDW_ERASE|RDW_INVALIDATE)) {
         return !error(ERR_WIN32_ERROR + GetLastError(), "RedrawWindow(IDC_TOOLBAR)");;
      }
      return TRUE;
   }

   // if search box control not found, find/remove an independent "Community" button
   HWND hBtnCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_COMMUNITY_BUTTON);
   if (hBtnCtrl && !DestroyWindow(hBtnCtrl)) {
      return !error(ERR_WIN32_ERROR + GetLastError(), "DestroyWindow(IDC_TOOLBAR_COMMUNITY_BUTTON)");;
   }
   return TRUE;
}
