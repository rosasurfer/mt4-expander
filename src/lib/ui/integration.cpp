#include "expander.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/thread.h"
#include "lib/window.h"
#include "lib/ui/integration.h"
#include "lib/ui/menu.h"

#include <commctrl.h>

#define MAIN_WINDOW_SUBCLASS_ID     1                 // subclass identifier for the terminal main window
#define CHART_WINDOW_SUBCLASS_ID    2                 // subclass identifier for chart windows
#define CHART_FRAME_SUBCLASS_ID     3                 // subclass identifier for chart frames (painting areas)

static HHOOK hUiThreadHook    = NULL;                 // hook handles
static HHOOK hWindowEventHook = NULL;


/**
 * Integrate the Expander in the terminal process. Called two times: first from a non-UI thread (worker in DLL loader),
 * then from the UI thread.
 *
 * @return BOOL - success status
 */
BOOL WINAPI IntegrateExpander() {
   // Some integration tasks MUST run in the UI thread. Some MUST NOT not run there. Some may run anywhere.

   // if not in the UI thread
   if (!IsUiThread()) {
      static bool done = false;
      if (!done) {
         done = true;
         if (!CustomizeTerminal()) return FALSE;      // perform configured modifications
         if (!HookUiThread())      return FALSE;      // continue in the UI thread
      }
      return TRUE;
   }

   // if in the UI thread
   static bool done = false;
   if (!done) {
      done = true;
      if (!SubclassMainWindow())   return FALSE;
      if (!SubclassChartWindows()) return FALSE;
      if (!HookWindowEvents())     return FALSE;      // in the UI thread and after MT4 installed its own blocking hook
   }
   return TRUE;
}


/**
 * Register a hook in the UI thread and trigger it. Called from a non-UI thread to run code in the UI thread.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI HookUiThread() {
   // register a hook for messages sent to any window owned by the UI thread
   hUiThreadHook = SetWindowsHookEx(WH_CALLWNDPROC, UiThreadHookProc, NULL, GetUiThreadId());
   if (!hUiThreadHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx(UiThreadHookProc)");
   if (GetDebugFeatures() & DEBUG_FEATURE_HOOKS) debug("hook registered");

   // trigger the UI thread
   SetLastError(ERROR_SUCCESS);
   if (!SendMessageTimeout(GetTerminalMainWindow(), WM_NULL, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 3000, NULL)) {
      warn(ERR_WIN32_ERROR + GetLastError(), "SendMessageTimeout()");
   }
   return TRUE;
}


/**
 * Hook procedure for messages sent to any window owned by the UI thread. Continues Expander integration and
 * removes itself.
 *
 * @param  int    code   - below 0 (zero) if the hook should skip the message
 * @param  WPARAM wParam - whether the message was sent by the current thread
 * @param  LPARAM lParam - pointer to message details
 *
 * @return LRESULT - return value of CallNextHookEx()
 */
static LRESULT CALLBACK UiThreadHookProc(int code, WPARAM wParam, LPARAM lParam) {
   if (hUiThreadHook) {
      if (GetDebugFeatures() & DEBUG_FEATURE_HOOKS) debug("called");

      if (!UnhookWindowsHookEx(hUiThreadHook)) error(ERR_WIN32_ERROR + GetLastError(), "UnhookWindowsHookEx(hUiThreadHook=0x%p)", hUiThreadHook);
      hUiThreadHook = NULL;

      IntegrateExpander();                         // continue integration in the UI thread
   }
   return CallNextHookEx(NULL, code, wParam, lParam);
}


/**
 * Register a hook for window events for windows owned by the UI thread.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI HookWindowEvents() {
   if (!hWindowEventHook) {
      hWindowEventHook = SetWindowsHookEx(WH_CBT, WindowEventsHookProc, NULL, GetUiThreadId());
      if (!hWindowEventHook) return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowsHookEx(WH_CBT)");
      if (GetDebugFeatures() & DEBUG_FEATURE_HOOKS) debug("hook registered");
   }
   return TRUE;
}


/**
 * Hook procedure receiving window events for windows owned by the UI thread. Called in the UI thread.
 *
 * @param  int    type   - event type; below 0 (zero) if the hook should skip the event
 * @param  WPARAM wParam - whether the event was sent by the current thread
 * @param  LPARAM lParam - pointer to event details
 *
 * @return LRESULT - whether the system should allow or prevent the event operation (depends on the event type)
 */
static LRESULT CALLBACK WindowEventsHookProc(int type, WPARAM wParam, LPARAM lParam) {
   static DWORD debugFeatures = GetDebugFeatures();
   static bool done = false;
   if (!done) {
      if (debugFeatures & DEBUG_FEATURE_HOOKS) debug("called");
      done = true;
   }

   switch (type) {
      case HCBT_CREATEWND: {
         HWND hWnd = (HWND)wParam;
         CREATESTRUCT* cs = ((CBT_CREATEWND*)lParam)->lpcs;
         uint ctrlId = (uint)cs->hMenu;
         if (debugFeatures & DEBUG_FEATURE_CREATE_WINDOW) debug(" HCBT_CREATEWND  %p  %S", hWnd, getClassNameW(hWnd).c_str());

         // call previous hooks first (MT4 overrides/disables subclassing)
         LRESULT denied = CallNextHookEx(hWindowEventHook, type, wParam, lParam);
         if (denied) {
            notice("HCBT_CREATEWND  %p  %S  denied by previous hook", hWnd, getClassNameW(hWnd).c_str());
            return denied;
         }

         // get the MDI container
         static HWND hWndMdi = GetTerminalMdiWindow();
         if (!hWndMdi) return NULL;

         // subclass chart windows
         if (cs->hwndParent == hWndMdi && cs->style & WS_CHILD) {
            if (ctrlId >= IDC_MDICLIENT_CHART1 && ctrlId < IDC_MDICLIENT_CHART1 + CHARTS_MAX) {
               SubclassChartWindow(hWnd);
            }
         }

         // subclass chart frames (aka the painting area)
         else if (ctrlId == IDC_MDICLIENT_CHART_FRAME && cs->style & WS_CHILD) {
            if (GetAncestor(cs->hwndParent, GA_PARENT) == hWndMdi) {
               SubclassChartFrame(cs->hwndParent, hWnd);
            }
         }
         return NULL;   // not denied by previous hooks
      }
   }
   return CallNextHookEx(hWindowEventHook, type, wParam, lParam);
}


/**
 * Subclass the terminal main window.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassMainWindow() {
   if (!IsUiThread()) return !error(ERR_ILLEGAL_STATE, "not in UI thread");

   HWND hWnd = GetTerminalMainWindow();
   if (!hWnd) return FALSE;

   if (GetPropW(hWnd, PROP_WINDOW_SUBCLASSED)) {
      warn("terminal main window %p already subclassed", hWnd);
      return TRUE;
   }
   if (!SetWindowSubclass(hWnd, MainWindowSubclassProc, MAIN_WINDOW_SUBCLASS_ID, 0)) {
      return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowSubclass()");
   }
   SetPropW(hWnd, PROP_WINDOW_SUBCLASSED, (HANDLE)1);

   static DWORD debugFeatures = GetDebugFeatures();
   if (debugFeatures & DEBUG_FEATURE_SUBCLASS) debug("terminal main window %p subclassed", hWnd);
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
   static DWORD debugFeatures = GetDebugFeatures();
   static uint WM_MT4EXPANDER = MT4ExpanderMsg();

   // process MT4Expander messages
   if (msg == WM_MT4EXPANDER) {
      switch (wParam) {
         case ID_UI_CALLBACK: {                                // executes a task in the UI thread
            JOB* job = (JOB*)lParam;
            if (!job) return !error(ERR_INVALID_POINTER, "WM_MT4EXPANDER  id=ID_CALLBACK  invalid parameter job: NULL");
            return job->run();
         }
      }
      return DefSubclassProc(hWnd, msg, wParam, lParam);
   }

   // process regular messages
   switch (msg) {
      case WM_COMMAND: {
         if (debugFeatures & DEBUG_FEATURE_WM_COMMAND) debug("WM_COMMAND  id=%d  lParam=0x%p", LOWORD(wParam), lParam);
         break;
      }

      case WM_INITMENUPOPUP: {
         HMENU hMenu = (HMENU)wParam;
         BOOL isSystemMenu = HIWORD(lParam);

         if (!isSystemMenu && IsChartTemplatesMenu(hMenu)) {
            if (debugFeatures & DEBUG_FEATURE_CHART_TEMPLATES) debug("WM_INITMENUPOPUP \"Chart->Templates\"");
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
 * Subclass all chart windows.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassChartWindows() {
   if (!IsUiThread()) return !error(ERR_ILLEGAL_STATE, "not in UI thread");

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
 * Subclass a single chart window and it's chart frame (if it exists).
 *
 * @param  HWND hWnd - chart window
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassChartWindow(HWND hWnd) {
   if (!IsUiThread()) return !error(ERR_ILLEGAL_STATE, "not in UI thread");
   static DWORD debugFeatures = GetDebugFeatures();

   // subclass the chart window
   if (GetPropW(hWnd, PROP_WINDOW_SUBCLASSED)) {
      if (debugFeatures & DEBUG_FEATURE_SUBCLASS) debug("chart window %p already subclassed", hWnd);
   }
   else {
      if (!SetWindowSubclass(hWnd, ChartWindowSubclassProc, CHART_WINDOW_SUBCLASS_ID, 0)) {
         return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowSubclass()");
      }
      SetPropW(hWnd, PROP_WINDOW_SUBCLASSED, (HANDLE)1);
      if (debugFeatures & DEBUG_FEATURE_SUBCLASS) debug("chart window %p subclassed", hWnd);
   }

   // check for and handle an existing chart frame (may not yet exist)
   if (HWND hChartFrame = GetDlgItem(hWnd, IDC_MDICLIENT_CHART_FRAME)) {
      return SubclassChartFrame(hWnd, hChartFrame);
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
   static DWORD debugFeatures = GetDebugFeatures();

   switch (msg) {
      case WM_COMMAND: {
         if (debugFeatures & DEBUG_FEATURE_WM_COMMAND) {
            debug("WM_COMMAND  %p  \"%S\"  id=%d  lParam=0x%p", hWnd, getInternalWindowTextW(hWnd).c_str(), LOWORD(wParam), lParam);
         }
         break;
      }

      case WM_INITMENUPOPUP: {
         HMENU hMenu = (HMENU)wParam;
         BOOL isSystemMenu = HIWORD(lParam);

         if (!isSystemMenu && IsChartTemplatesMenu(hMenu)) {
            if (debugFeatures & DEBUG_FEATURE_CHART_TEMPLATES) debug("WM_INITMENUPOPUP \"Chart->Templates\"");
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
 * Subclass a chart frame (aka a chart's painting area).
 *
 * @param  HWND hWndChart      - containing chart window
 * @param  HWND hWndChartFrame - chart frame
 *
 * @return BOOL - success status
 */
static BOOL WINAPI SubclassChartFrame(HWND hWndChart, HWND hWndChartFrame) {
   if (!IsUiThread()) return !error(ERR_ILLEGAL_STATE, "not in UI thread");
   static DWORD debugFeatures = GetDebugFeatures();

   if (GetPropW(hWndChartFrame, PROP_WINDOW_SUBCLASSED)) {
      if (debugFeatures & DEBUG_FEATURE_SUBCLASS) debug(" chart window %p chart %p already subclassed", hWndChart, hWndChartFrame);
   }
   else {
      if (!SetWindowSubclass(hWndChartFrame, ChartFrameSubclassProc, CHART_FRAME_SUBCLASS_ID, 0)) {
         return !error(ERR_WIN32_ERROR + GetLastError(), "SetWindowSubclass()");
      }
      SetPropW(hWndChartFrame, PROP_WINDOW_SUBCLASSED, (HANDLE)1);
      if (debugFeatures & DEBUG_FEATURE_SUBCLASS) debug(" chart window %p chart %p subclassed", hWndChart, hWndChartFrame);

      // improve redrawing performance of child windows
      SetWindowStyles(hWndChartFrame, GetWindowStyles(hWndChartFrame) | WS_CLIPCHILDREN);
   }
   return TRUE;
}


/**
 * Chart frame subclassing procedure. Processes all messages for a window. Executed in the UI thread.
 *
 * @param  HWND      hWnd       - chart frame receiving the message (painting area)
 * @param  uint      msg        - received message
 * @param  WPARAM    wParam     - additional message info
 * @param  LPARAM    lParam     - additional message info
 * @param  UINT_PTR  subclassId - subclass identifier
 * @param  DWORD_PTR data       - user data as passed to SetWindowSubclass()
 *
 * @return LRESULT - depends on the message
 */
static LRESULT CALLBACK ChartFrameSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data) {
   static DWORD debugFeatures = GetDebugFeatures();

   switch (msg) {
      case WM_COMMAND: {
         if (debugFeatures & DEBUG_FEATURE_WM_COMMAND) {
            debug("WM_COMMAND  %p  \"%S\"  id=%d  lParam=0x%p", hWnd, getInternalWindowTextW(GetAncestor(hWnd, GA_PARENT)).c_str(), LOWORD(wParam), lParam);
         }
         break;
      }

      case WM_NCDESTROY: {
         RemoveWindowSubclass(hWnd, ChartFrameSubclassProc, subclassId);
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
   if (IsUiThread()) return !error(ERR_ILLEGAL_STATE, "not in UI thread");

   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return FALSE;

   // find the toolbar
   HWND hToolbar = GetDlgItem(hWndMain, IDC_TOOLBAR);
   if (!hToolbar) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(MainWindow, IDC_TOOLBAR)");

   // find and remove a search box control (contains the "Community" button, builds > 509)
   HWND hSearchCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_SEARCHBOX);
   if (hSearchCtrl) {
      PostMessageA(hSearchCtrl, WM_CLOSE, 0, 0);   // a DestroyWindow() in the UI thread would delay startup
      return TRUE;
   }

   // find and remove a separate "Community" button (builds <= 509)
   HWND hBtnCtrl = GetDlgItem(hToolbar, IDC_TOOLBAR_COMMUNITY_BUTTON);
   if (hBtnCtrl) {
      PostMessageA(hBtnCtrl, WM_CLOSE, 0, 0);      // a DestroyWindow() in the UI thread would delay startup
   }
   return TRUE;
}
