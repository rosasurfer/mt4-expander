#include "expander.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/win32.h"
#include "lib/ui/integration.h"
#include "lib/ui/menu.h"

#include <commctrl.h>

#define MAIN_WINDOW_SUBCLASS_ID     1                 // subclass identifier for the main window
#define CHART_WINDOW_SUBCLASS_ID    2                 // subclass identifier for chart windows

#define PROP_WINDOW_SUBCLASSED      L"rsf.MT4Expander.WindowSubclassed"

static HHOOK hUiThreadHook    = NULL;                 // hook handles
static HHOOK hWindowEventHook = NULL;

static BOOL subclassChartWindows = TRUE;              // whether to subclass chart windows

/**
 * Setup UI integration of the Expander. Called two times: once from a non-UI thread, once from the UI thread.
 * The first call is always from a non-UI thread (worker in DLL loader).
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetupUiIntegration() {
   // Some integration tasks must run in the UI thread. Some must not run there. Some may run anywhere.

   // if in a non-UI thread
   if (!IsUiThread()) {
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
      if (!SubclassMainWindow())      return FALSE;
      if (!SubclassChartWindows())    return FALSE;
      if (!RegisterWindowEventHook()) return FALSE;   // in the UI thread and after MT4 installed its own blocking hook
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
      hWindowEventHook = SetWindowsHookEx(WH_CBT, WindowEventHook, NULL, GetUiThreadId());
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
   switch (type) {
      case HCBT_CREATEWND: {                                  // a window is about to be created
         HWND hWnd = (HWND)wParam;
         CREATESTRUCT* cs = ((CBT_CREATEWND*)lParam)->lpcs;

         // log if the debug feature is enabled
         static DWORD debugOptions = GetDebugOptions();
         if (debugOptions & OPTION_DEBUG_CREATE_WINDOW) {
            debug(" HCBT_CREATEWND  %p  %S", hWnd, getClassNameW(hWnd).c_str());
         }

         // call previous hooks first (MT4 overrides/disables subclassing)
         LRESULT denied = CallNextHookEx(hWindowEventHook, type, wParam, lParam);
         if (denied) {
            notice("HCBT_CREATEWND  %p  %S  denied by previous hook", hWnd, getClassNameW(hWnd).c_str());
            return denied;
         }

         // subclass chart windows if the feature is enabled (after MT4 hook)
         if (subclassChartWindows) {
            static HWND hWndMdi = NULL; if (!hWndMdi) hWndMdi = GetTerminalMdiWindow();      // retry on error
            uint ctrlId = (uint)cs->hMenu;

            if (hWndMdi && cs->hwndParent == hWndMdi && cs->style & WS_CHILD) {
               if (ctrlId >= IDC_MDICLIENT_CHART1 && ctrlId < IDC_MDICLIENT_CHART1 + 128) {  // current limit: 100
                  SubclassChartWindow(hWnd);
               }
            }
         }
         return NULL;
      }
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
   hUiThreadHook = SetWindowsHookEx(WH_CALLWNDPROC, UiThreadHook, NULL, GetUiThreadId());
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
   if (!IsUiThread()) return !error(ERR_ILLEGAL_STATE, "must run in the UI thread");

   HWND hWnd = GetTerminalMainWindow();
   if (!hWnd) return FALSE;

   if (GetPropW(hWnd, PROP_WINDOW_SUBCLASSED)) {
      warn("main window %p already subclassed", hWnd);      // accepted but an issue: we want to know
      return TRUE;
   }
   if (!SetWindowSubclass(hWnd, MainWindowSubclassProc, MAIN_WINDOW_SUBCLASS_ID, 0)) {
      return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowSubclass()");
   }
   SetPropW(hWnd, PROP_WINDOW_SUBCLASSED, (HANDLE)1);

   static DWORD debugOptions = GetDebugOptions();
   if (debugOptions & OPTION_DEBUG_SUBCLASS) {
      debug("main window %p subclassed", hWnd);
   }
   return TRUE;
}

/**
 * Main window subclassing procedure. Processes all messages for the window. Executed in the UI thread.
 *
 * @param  HWND      hWnd       - window receiving the message
 * @param  uint      msg        - received message
 * @param  WPARAM    wParam     - additional message info
 * @param  LPARAM    lParam     - additional message info
 * @param  UINT_PTR  subclassId - subclass identifier
 * @param  DWORD_PTR data       - user data as passed to SetWindowSubclass()
 *
 * @return LRESULT - depends on the message
 */
static LRESULT CALLBACK MainWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data) {
   static DWORD debugOptions = GetDebugOptions();

   switch (msg) {
      case WM_COMMAND: {
         if (debugOptions & OPTION_DEBUG_WM_COMMAND) debug("WM_COMMAND  id=%d  lParam=0x%p", LOWORD(wParam), lParam);
         break;
      }

      case WM_INITMENUPOPUP: {
         HMENU hMenu = (HMENU)wParam;
         BOOL isSystemMenu = HIWORD(lParam);

         if (!isSystemMenu && IsChartTemplatesMenu(hMenu)) {
            if (debugOptions & OPTION_DEBUG_CHART_TEMPLATES) {
               debug("WM_INITMENUPOPUP \"Chart->Templates\"");
            }
            RebuildChartTemplatesMenu(hMenu);                  // DefSubclassProc()
         }                                                     // - adds MFT_OWNERDRAW to mi.fType of all items
         break;                                                // - sets mi.dwItemData of all items to a shared (same) pointer
      }

      case WM_QUERYENDSESSION: {                               // Windows: "Are you ready to shut down?"
         if (lParam & ENDSESSION_LOGOFF) {}                    // user logoff
         else                            {}                    // system shutdown/restart
         debug("WM_QUERYENDSESSION %s", lParam & ENDSESSION_LOGOFF ? "logoff" : "shutdown");
         break;
      }

      case WM_ENDSESSION: {                                    // workaround for terminal bug https://github.com/rosasurfer/mt4-expander/issues/26
         if (wParam) {                                         // Windows: "Logoff/shutdown is happening now. You have ~5 seconds."
            if (lParam & ENDSESSION_LOGOFF) {}                 // user logoff
            else                            {}                 // system shutdown/restart
            debug("WM_ENDSESSION %s", lParam & ENDSESSION_LOGOFF ? "logoff" : "shutdown");
         } // else                                             // logoff/shutdown was cancelled
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
 * Subclass all chart windows if the feature is enabled. Executed in the UI thread.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassChartWindows() {
   if (!IsUiThread())         return !error(ERR_ILLEGAL_STATE, "must run in the UI thread");
   if (!subclassChartWindows) return TRUE;

   // subclass existing chart windows
   HWND hWndMdi = GetTerminalMdiWindow();
   if (!hWndMdi) return FALSE;

   HWND hWndChart = GetDlgItem(hWndMdi, IDC_MDICLIENT_CHART1);
   if (!hWndChart) {
      if (GetLastError() == ERROR_CONTROL_ID_NOT_FOUND) {
         return TRUE;                                       // no chart currently open
      }
      return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(MDIClient, IDC_MDICLIENT_CHART1)");
   }

   int i = 0;
   while (hWndChart) {
      if (!SubclassChartWindow(hWndChart)) return FALSE;
      i++;
      hWndChart = GetDlgItem(hWndMdi, IDC_MDICLIENT_CHART1 + i);
   }

   // new chart windows will be subclassed by the CBT hook (MT4 prevents class subclassing)
   return TRUE;
}

/**
 * Subclass a single chart window if the feature is enabled. Executed in the UI thread.
 *
 * @param  HWND hWnd - chart window
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassChartWindow(HWND hWnd) {
   if (!IsUiThread())         return !error(ERR_ILLEGAL_STATE, "must run in the UI thread");
   if (!subclassChartWindows) return TRUE;

   if (GetPropW(hWnd, PROP_WINDOW_SUBCLASSED)) {
      notice("chart window %p already subclassed", hWnd);   // accepted but should not happen
      return TRUE;
   }
   if (!SetWindowSubclass(hWnd, ChartWindowSubclassProc, CHART_WINDOW_SUBCLASS_ID, 0)) {
      return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowSubclass()");
   }
   SetPropW(hWnd, PROP_WINDOW_SUBCLASSED, (HANDLE)1);

   static DWORD debugOptions = GetDebugOptions();
   if (debugOptions & OPTION_DEBUG_SUBCLASS) {
      debug("chart window %p subclassed", hWnd);
   }
   return TRUE;
}

/**
 * Chart window subclassing procedure. Processes all messages for a window. Executed in the UI thread.
 *
 * @param  HWND      hWnd       - chart window receiving the message
 * @param  uint      msg        - received message
 * @param  WPARAM    wParam     - additional message info
 * @param  LPARAM    lParam     - additional message info
 * @param  UINT_PTR  subclassId - subclass identifier
 * @param  DWORD_PTR data       - user data as passed to SetWindowSubclass()
 *
 * @return LRESULT - depends on the message
 */
static LRESULT CALLBACK ChartWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data) {
   static DWORD debugOptions = GetDebugOptions();

   switch (msg) {
      case WM_COMMAND: {
         if (debugOptions & OPTION_DEBUG_WM_COMMAND) {
            debug("WM_COMMAND  %p  \"%S\"  id=%d  lParam=0x%p", hWnd, getInternalWindowTextW(hWnd).c_str(), LOWORD(wParam), lParam);
         }
         break;
      }

      case WM_INITMENUPOPUP: {
         HMENU hMenu = (HMENU)wParam;
         BOOL isSystemMenu = HIWORD(lParam);

         if (!isSystemMenu && IsChartTemplatesMenu(hMenu)) {
            if (debugOptions & OPTION_DEBUG_CHART_TEMPLATES) {
               debug("WM_INITMENUPOPUP \"Chart->Templates\"");
            }
            RebuildChartTemplatesMenu(hMenu);                  // DefSubclassProc()
         }                                                     // - adds MFT_OWNERDRAW to mi.fType of all items
         break;                                                // - sets mi.dwItemData of all items to a shared (same) pointer
      }

      case WM_NCDESTROY: {
         RemoveWindowSubclass(hWnd, ChartWindowSubclassProc, subclassId);
         break;
      }
   }
   return DefSubclassProc(hWnd, msg, wParam, lParam);
}

/**
 * Customize the UI of the terminal. Executed in a non-UI thread to not delay terminal startup.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI CustomizeTerminal() {
   if (IsUiThread()) return !error(ERR_ILLEGAL_STATE, "must not run in the UI thread");

   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return FALSE;

   // find the toolbar
   HWND hToolbar = GetDlgItem(hWndMain, IDC_TOOLBAR);
   if (!hToolbar) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(MainWindow, IDC_TOOLBAR)");

   // find and remove a search box control (contains the "Community" button, builds > 509)
   HWND hSearchCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_SEARCHBOX);
   if (hSearchCtrl) {
      PostMessageA(hSearchCtrl, WM_CLOSE, 0, 0);   // a DestroyWindow() in the UI thread delays startup
      return TRUE;
   }

   // find and remove a separate "Community" button (builds <= 509)
   HWND hBtnCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_COMMUNITY_BUTTON);
   if (hBtnCtrl) {
      PostMessageA(hBtnCtrl, WM_CLOSE, 0, 0);      // a DestroyWindow() in the UI thread delays startup
   }
   return TRUE;
}
