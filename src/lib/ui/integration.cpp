#include "expander.h"
#include "lib/helper.h"
#include "lib/terminal.h"
#include "lib/ui/integration.h"
#include "lib/ui/menu.h"

#include <commctrl.h>


#define MAIN_WINDOW_SUBCLASS_ID     1                 // subclass identifier for the main window
#define CHART_WINDOW_SUBCLASS_ID    2                 // subclass identifier for chart windows

static HHOOK hUiThreadHook = NULL;                    // hook handles
static HHOOK hWindowEventHook = NULL;                 //


/**
 * Setup UI integration of the Expander. Called two times: once from a non-UI thread, once from the UI thread.
 * The first call is always from a non-UI thread (worker in DLL loader).
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetupUiIntegration() {
   // Some integration tasks must run in the UI thread. Some must not run there. Some may run anywhere.

   // register a hook for window events
   static BOOL success = RegisterWindowEventHook();   // no full synchronization needed
   if (!success) return FALSE;

   // if in a non-UI thread
   if (!IsUIThread()) {
      static BOOL done = FALSE;
      if (!done) {                                    // no full synchronization needed
         done = TRUE;
         if (!CustomizeTerminal()) return FALSE;      // perform configured modifications
         if (!NotifyUiThread())    return FALSE;      // continue in the UI thread
      }
      return TRUE;
   }

   // if in the UI thread
   static BOOL done = FALSE;
   if (!done) {                                       // fully synchronized
      done = TRUE;
      if (!SubclassMainWindow())   return FALSE;      // subclass the terminal main window
      if (!SubclassChartWindows()) return FALSE;      // subclass all chart windows
   }
   return TRUE;
}


/**
 * Register a hook for window related events of the UI thread.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI RegisterWindowEventHook() {
   if (!hWindowEventHook) {
      hWindowEventHook = SetWindowsHookExW(WH_CBT, WindowEventHook, NULL, GetUIThreadId());
      if (!hWindowEventHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx(WindowEventHook)");
   }
   return TRUE;
}


/**
 * Hook procedure (listener) receiving window related events of the UI thread. Called in the UI thread.
 *
 * @param  int    type   - event type; below 0 (zero) if the hook should skip the event
 * @param  WPARAM wParam - whether the event was sent by the current thread
 * @param  LPARAM lParam - pointer to event details
 *
 * @return LRESULT - whether the system should allow or prevent the event operation (depends on the event type)
 */
static LRESULT CALLBACK WindowEventHook(int type, WPARAM wParam, LPARAM lParam) {
   // This hook (a WH_CBT hook) receives a large amount of messages and must be as fast as possible.

   switch (type) {
      case HCBT_CREATEWND: {                 // A window is about to be created.
         HWND hWnd = (HWND)wParam;
         //debug("HCBT_CREATEWND  %p", hWnd);
         break;
      }
      case HCBT_DESTROYWND: break;           // A window is about to be destroyed.
      case HCBT_MINMAX:     break;           // A window is about to be minimized or maximized.
      case HCBT_MOVESIZE:   break;           // A window is about to be moved or sized.
      case HCBT_SETFOCUS:   break;           // A window is about to receive the keyboard focus.
   }
   return CallNextHookEx(hWindowEventHook, type, wParam, lParam);
}


/**
 * Register a hook in the UI thread and trigger it. Called from a non-UI thread to run code in the UI thread.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI NotifyUiThread() {
   // register a hook for sent messages
   hUiThreadHook = SetWindowsHookEx(WH_CALLWNDPROC, UiThreadHook, NULL, GetUIThreadId());
   if (!hUiThreadHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx(UiThreadHook)");

   // trigger the UI thread
   SetLastError(ERROR_SUCCESS);
   if (!SendMessageTimeout(GetTerminalMainWindow(), WM_NULL, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 3000, NULL)) {
      warn(ERR_WIN32_ERROR + GetLastError(), "SendMessageTimeout()");  // don't fail, as the hook is OK
   }
   return TRUE;
}


/**
 * Hook procedure (listener) for messages sent to the terminal main window (UI thread). Continues Expander integration and
 * removes itself.
 *
 * @param  int    code   - below 0 (zero) if the hook should skip the message
 * @param  WPARAM wParam - whether the message was sent by the current thread
 * @param  LPARAM lParam - pointer to message details
 *
 * @return LRESULT - return value of CallNextHookEx()
 */
static LRESULT CALLBACK UiThreadHook(int code, WPARAM wParam, LPARAM lParam) {
   if (code >= 0) {
      if (hUiThreadHook) {
         HHOOK hHook = hUiThreadHook;
         hUiThreadHook = NULL;
         if (!UnhookWindowsHookEx(hHook)) error(ERR_WIN32_ERROR + GetLastError(), "UnhookWindowsHookEx(hHook=0x%p)", hHook);
         SetupUiIntegration();                                    // continue Expander integration
      }
   }
   return CallNextHookEx(hUiThreadHook, code, wParam, lParam);    // will be NULL after the hook was removed
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
         } // else                                 // logoff/shutdown was cancelled
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
 * Subclass a single chart window. Executed in the UI thread.
 *
 * @param  HWND hWnd - chart window
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassChartWindow(HWND hWnd) {
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
 * Customize the UI of the terminal as configured. Executed in a non-UI thread to not delay terminal startup.
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
