#include "expander.h"
#include "lib/helper.h"
#include "lib/terminal.h"
#include "lib/ui/integration.h"
#include "lib/ui/menu.h"

#include <commctrl.h>


static HHOOK g_hUiHook = NULL;                        // hook handle to subclass the terminal main window from a non-UI thread
static const uint g_subclassId = 1;                   // main window subclass identifier


/**
 * Setup UI integration of the Expander.
 */
void WINAPI SetupUiIntegration() {
   CustomizeTerminal();
}


/**
 * Customize the UI of the terminal.
 */
void WINAPI CustomizeTerminal() {
   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return;
   BOOL isUiThread = IsUIThread();

   SubclassMainWindow(hWndMain, isUiThread);          // subclass the terminal main window
   SubclassChartWindows(hWndMain, isUiThread);        // subclass all current and future chart windows

   // get the toolbar
   HWND hToolbar = GetDlgItem(hWndMain, IDC_TOOLBAR);
   if (!hToolbar) {
      error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(MainWindow, IDC_TOOLBAR) terminal toolbar not found");
      return;
   }

   // a local function as substitute for missing lambda support in C++03
   struct local {
      static void CloseWindow(HWND hCtrl, BOOL isUiThread) {
         if (isUiThread) {
            DestroyWindow(hCtrl);                     // must be called from the UI thread
         }
         else {
            PostMessageA(hCtrl, WM_CLOSE, 0, 0);
            while (IsWindow(hCtrl)) Sleep(100);       // TODO: so ugly, move to the UI thread
         }
      }
   };

   // find and remove a search box control (contains the "Community" button)
   if (HWND hSearchCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_SEARCHBOX)) {
      local::CloseWindow(hSearchCtrl, isUiThread);
      if (!RedrawWindow(hToolbar, NULL, NULL, RDW_ERASE|RDW_INVALIDATE)) {
         error(ERR_WIN32_ERROR + GetLastError(), "RedrawWindow(IDC_TOOLBAR)");
         return;
      }
   }
   // if search box control not found, find/remove an independent "Community" button
   else if (HWND hBtnCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_COMMUNITY_BUTTON)) {
      local::CloseWindow(hBtnCtrl, isUiThread);
   }
}


/**
 * Subclass the terminal main window.
 *
 * @param  HWND hWndMain   - handle of the terminal main window
 * @param  BOOL isUiThread - whether the function is executed by the UI thread
 *
 * @return BOOL - success status
 */
BOOL WINAPI SubclassMainWindow(HWND hWndMain, BOOL isUiThread) {
   DWORD_PTR data = 0;        // guard against multiple calls
   if (GetWindowSubclass(hWndMain, MainWindowSubclassProc, g_subclassId, &data)) {
      return TRUE;
   }
   if (isUiThread) {          // on the UI thread we can call SetWindowSubclass() directly
      if (!SetWindowSubclass(hWndMain, MainWindowSubclassProc, g_subclassId, 0)) {
         return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowSubclass()");
      }
   }
   else {                     // otherwise we must install a hook to be executed by the UI thread
      g_hUiHook = SetWindowsHookEx(WH_CALLWNDPROC, UiHookProc, NULL, GetUIThreadId());
      if (!g_hUiHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx()");

      // wake-up the UI thread with a non-blocking SendMessage()
      SetLastError(ERROR_SUCCESS);
      if (!SendMessageTimeout(hWndMain, WM_NULL, 0, 0, SMTO_ABORTIFHUNG|SMTO_NOTIMEOUTIFNOTHUNG, 3000, NULL)) {
         debug(ERR_WIN32_ERROR + GetLastError(), "SendMessageTimeout()");
      }
   }
   return TRUE;
}


/**
 * Subclass all current and future chart windows.
 *
 * @param  HWND hWndMain   - handle of the terminal main window
 * @param  BOOL isUiThread - whether the function is executed by the UI thread
 *
 * @return BOOL - success status
 */
BOOL WINAPI SubclassChartWindows(HWND hWndMain, BOOL isUiThread) {
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
LRESULT CALLBACK MainWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data) {
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
LRESULT CALLBACK ChartWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data) {
   return DefSubclassProc(hWnd, msg, wParam, lParam);
}


/**
 * Callback function registered by SetWindowsHookEx() to run in the UI thread.
 *
 * @param  int    code   - whether the hook procedure must process the message
 * @param  WPARAM wParam - whether the message was sent by the current thread
 * @param  LPARAM lParam - pointer to message details
 *
 * @return LRESULT - return value of CallNextHookEx()
 */
LRESULT CALLBACK UiHookProc(int code, WPARAM wParam, LPARAM lParam) {
   if (code >= 0) {
      if (g_hUiHook) {
         HHOOK hHook = g_hUiHook;
         g_hUiHook = NULL;
         if (!UnhookWindowsHookEx(hHook)) {
            error(ERR_WIN32_ERROR + GetLastError(), "UnhookWindowsHookEx(hHook=0x%p)", hHook);
         }
      }
      if (!SetWindowSubclass(GetTerminalMainWindow(), MainWindowSubclassProc, g_subclassId, 0)) {
         error(ERR_WIN32_ERROR + GetLastError(), "SetWindowSubclass()");
      }
   }
   return CallNextHookEx(NULL, code, wParam, lParam);
}
