#include "expander.h"
#include "dllmain.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/timer.h"
#include "struct/ExecutionContext.h"

#include <shellapi.h>

BOOL  g_cliOptionPortableMode;                              // whether cmd line option /portable is set
DWORD g_cliDebugOptions;                                    // bit mask of specified CLI debug options

extern MqlInstanceList               g_mqlInstances;        // all MQL program instances
extern std::vector<DWORD>            g_threads;             // all known threads executing MQL programs
extern std::vector<uint>             g_threadsPrograms;     // the last MQL program executed by a thread
extern std::vector<TICK_TIMER_DATA*> g_tickTimers;          // all registered ticktimers
extern CRITICAL_SECTION              g_expanderMutex;       // mutex for Expander-wide locking


/**
 * DLL entry point.
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
 * Handler for DLL_PROCESS_ATTACH events.
 * Code in this function must be safe under DLL loader lock.
 *
 * @return BOOL - success status
 */
BOOL WINAPI onProcessAttach() {
   InitializeCriticalSection(&g_expanderMutex);

   g_mqlInstances.   reserve(128);                    // TODO: replace global state by getter/setter functions
   g_threads.        reserve(512);
   g_threadsPrograms.reserve(512);
   g_tickTimers.     reserve(32);

   // parse command line arguments                    // TODO: replace global state by lazy-initialized getters
   int argc;
   LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
   if (!argv) return !error(ERR_WIN32_ERROR + GetLastError(), "CommandLineToArgvW()");

   for (int i=1; i < argc; i++) {
      if (StrStartsWith(argv[i], L"/portable")) {     // The terminal accepts any argument starting with prefix "/portable",
         g_cliOptionPortableMode = TRUE;              // e.g. "/portable-poo" activates portable mode, too.
         continue;                                    // This test mirrors that unusual behavior.
      }
      if (StrCompare(argv[i], L"/rsf:debug-ec")) {
         g_cliDebugOptions |= DEBUG_EXECUTION_CONTEXT;
         continue;
      }
      if (StrCompare(argv[i], L"/rsf:debug-accountnumber")) {
         g_cliDebugOptions |= DEBUG_ACCOUNT_NUMBER;
         continue;
      }
      if (StrCompare(argv[i], L"/rsf:debug-accountserver")) {
         g_cliDebugOptions |= DEBUG_ACCOUNT_SERVER;
         continue;
      }
      if (StrCompare(argv[i], L"/rsf:debug-objectcreate")) {
         g_cliDebugOptions |= DEBUG_OBJECT_CREATE;
         continue;
      }
      if (StrCompare(argv[i], L"/rsf:debug-indicatorlist")) {
         g_cliDebugOptions |= DEBUG_INDICATOR_LIST;
         continue;
      }
   }
   LocalFree(argv);

   // launch independant worker thread for custom initializations
   HANDLE hThread = CreateThread(NULL, 0, onExpanderStart, NULL, 0, NULL);
   if (hThread) CloseHandle(hThread);
   else         error(ERR_WIN32_ERROR + GetLastError(), "CreateThread()");    // report error but don't fail

   return TRUE;
}


/**
 * Handler for DLL_PROCESS_DETACH events.
 *
 * @param  BOOL isTerminating - whether the DLL is detached because of process termination
 *
 * @return BOOL - success status
 */
BOOL WINAPI onProcessDetach(BOOL isTerminating) {
   if (!isTerminating) {
      DeleteCriticalSection(&g_expanderMutex);
      ReleaseTickTimers();
      ReleaseWindowProperties();
   }
   return TRUE;
}


/**
 * Executes start tasks outside of the DLL loader lock (runs in a separate thread).
 * These are independant tasks not required for a working DLL.
 *
 * @param  void* lpParam - thread data passed by CreateThread()
 *
 * @return DWORD - error status
 */
DWORD WINAPI onExpanderStart(void* lpParam) {
   // lock the DLL in memory (production only, debug is immediately released for testing)
   const wchar* dllName = GetExpanderFileNameW();
   BOOL isProduction = StrEndsWithI(dllName, L"rsfMT4Expander.dll");
   if (isProduction) {
      HMODULE hModule = NULL;
      GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCTSTR)onProcessAttach, &hModule);
   }

   // customize the terminal (production only as customization will subclass MT4 windows)
   if (isProduction) {
      CustomizeTerminal();
   }
   return NO_ERROR;
}
