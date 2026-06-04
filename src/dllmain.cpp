#include "expander.h"
#include "dllmain.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/timer.h"
#include "struct/ExecutionContext.h"

extern MqlInstanceList               g_mqlInstances;        // all MQL program instances
extern std::vector<DWORD>            g_threads;             // all known threads executing MQL programs
extern std::vector<uint>             g_threadsPrograms;     // the last MQL program executed by a thread
extern std::vector<TICK_TIMER_DATA*> g_tickTimers;          // all registered ticktimers
extern CRITICAL_SECTION              g_expanderMutex;       // mutex for Expander-wide locking


/**
 * DLL entry point. Code in this function must be safe under the DLL loader-lock.
 *
 * @param  HMODULE hModule
 * @param  DWORD   reason
 * @param  LPVOID  reserved
 *
 * @return BOOL
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
   switch (reason) {
      case DLL_PROCESS_ATTACH:
         DisableThreadLibraryCalls(hModule);
         return onProcessAttach();

      case DLL_PROCESS_DETACH:
         onProcessDetach((BOOL)reserved);
         break;
   }
   return TRUE;
}


/**
 * Handler for DLL_PROCESS_ATTACH events. Code in this function must be safe under the DLL loader-lock.
 *
 * @return BOOL - success status
 */
static BOOL WINAPI onProcessAttach() {
   InitializeCriticalSection(&g_expanderMutex);

   g_mqlInstances.   reserve(128);        // TODO: replace global state by getters/setters with local state
   g_threads.        reserve(512);
   g_threadsPrograms.reserve(512);
   g_tickTimers.     reserve(32);

   // launch worker thread for custom initializations
   HMODULE hModule = NULL;                // increase ref-count so the DLL can't be unloaded before the thread finishes
   GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)onProcessAttach, &hModule);

   HANDLE hThread = CreateThread(NULL, 0, ExpanderStartThread, hModule, 0, NULL);
   if (hThread) {
      CloseHandle(hThread);
   }
   else {
      error(ERR_WIN32_ERROR + GetLastError(), "CreateThread(\"ExpanderStart\")");
      FreeLibrary(hModule);               // on error decrease ref-count
   }
   return TRUE;
}


/**
 * Handler for DLL_PROCESS_DETACH events. Code in this function must be safe under the DLL loader-lock.
 *
 * @param  BOOL isTerminating - whether the DLL is detached because of process termination
 *
 * @return BOOL - success status
 */
static BOOL WINAPI onProcessDetach(BOOL isTerminating) {
   if (!isTerminating) {
      DeleteCriticalSection(&g_expanderMutex);
      ReleaseTickTimers();
      ReleaseWindowProperties();
   }
   return TRUE;
}


/**
 * Executes MT4Expander start tasks in a separate thread (outside of the DLL loader-lock).
 * These are independant tasks not required for the DLL to load.
 *
 * @param  void* lpParam - DLL module handle
 *
 * @return DWORD - thread exit status
 */
static DWORD WINAPI ExpanderStartThread(void* lpParam) {
   const wchar* dllName = GetExpanderFileNameW();
   BOOL isProduction = StrEndsWithI(dllName, L"rsfMT4Expander.dll");

   if (!isProduction) {                                        // nothing to do, decrease ref-count and terminate the thread
      FreeLibraryAndExitThread((HMODULE)lpParam, NO_ERROR);    // this never returns
   }

   // production: lock the DLL in memory
   HMODULE hModule = NULL;
   GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCTSTR)ExpanderStartThread, &hModule);

   // start a background thread to monitor user logoff/system shutdown events
   HANDLE hThread = CreateThread(NULL, 0, SessionMonitorThread, hModule, 0, NULL);
   if (hThread) CloseHandle(hThread);
   else         error(ERR_WIN32_ERROR + GetLastError(), "CreateThread(\"SessionMonitor\")");

   // customize the UI
   CustomizeTerminal();
   return NO_ERROR;
}


/**
 * Create a hidden top-level window and run the message loop to capture WM_QUERYENDSESSION/WM_ENDSESSION messages.
 * This is a workaround for terminal bug https://github.com/rosasurfer/mt4-expander/issues/26#.
 *
 * @param  void* lpParam - DLL module handle
 *
 * @return DWORD - thread exit status
 */
static DWORD WINAPI SessionMonitorThread(void* lpParam) {
   HINSTANCE hInstance = (HINSTANCE)lpParam;

   // register the window class
   const char* className = "SessionMonitorClass";
   WNDCLASSEXA wc = {};
   wc.cbSize        = sizeof(WNDCLASSEXA);
   wc.lpfnWndProc   = SessionMonitorWndProc;
   wc.hInstance     = hInstance;
   wc.lpszClassName = className;    // no icon, cursor, background - invisible
   if (!RegisterClassExA(&wc)) {
      return error(ERR_WIN32_ERROR + GetLastError(), "RegisterClassExA(\"%s\")", className);
   }

   // create a message-only window
   HWND hWnd = CreateWindowExA(
      0,                            // no extended styles
      className,
      "MT4Expander session monitor",
      0,                            // no style needed
      0, 0, 0, 0,                   // position/size irrelevant
      NULL,                         // no parent => top-level window
      NULL,                         // no menu
      hInstance,
      NULL                          // no creation param
   );
   if (!hWnd) {
      UnregisterClassA(className, hInstance);
      return error(ERR_WIN32_ERROR + GetLastError(), "CreateWindowExA(\"%s\")", className);
   }

   // run the message loop
   MSG msg;
   while (GetMessage(&msg, NULL, 0, 0) > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   // clean-up
   DestroyWindow(hWnd);
   UnregisterClassA(className, hInstance);
   return NO_ERROR;
}


/**
 * SessionMonitor window procedure - runs on the SessionMonitor thread.
 *
 * @param  HWND   hWnd   - window receiving the message
 * @param  uint   msg    - sent message
 * @param  WPARAM wParam - additional message info
 * @param  LPARAM lParam - additional message info
 *
 * @return LRESULT - depends on the message sent
 */
static LRESULT CALLBACK SessionMonitorWndProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam) {
   switch (msg) {
      case WM_QUERYENDSESSION: {                   // Windows: "Are you ready to shut down?"
         if (lParam & ENDSESSION_LOGOFF) {}        // user logoff
         else                            {}        // system shutdown/restart
         debug("WM_QUERYENDSESSION  %s", lParam & ENDSESSION_LOGOFF ? "logoff" : "shutdown");
         return TRUE;                              // we are ready
      }

      case WM_ENDSESSION: {
         if (wParam) {                             // Windows: "Logoff/shutdown is happening now. You have ~5 seconds."
            if (lParam & ENDSESSION_LOGOFF) {}     // user logoff
            else                            {}     // system shutdown/restart
            debug("WM_ENDSESSION  %s", lParam & ENDSESSION_LOGOFF ? "logoff" : "shutdown");
            // do something...
         }
         else /*!wParam*/ {}                       // logoff/shutdown was cancelled
         return 0;
      }

      default:
         return DefWindowProc(hWnd, msg, wParam, lParam);
   }
}
