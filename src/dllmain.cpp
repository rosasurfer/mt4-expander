#include "expander.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/timer.h"
#include "struct/rsf/ExecutionContext.h"

#include <shellapi.h>

BOOL  g_cliOptionPortableMode;                              // whether cmd line option /portable is set
DWORD g_cliDebugOptions;                                    // bit mask of specified CLI debug options

extern MqlInstanceList               g_mqlInstances;        // all MQL program instances
extern std::vector<DWORD>            g_threads;             // all known threads executing MQL programs
extern std::vector<uint>             g_threadsPrograms;     // the last MQL program executed by a thread
extern std::vector<TICK_TIMER_DATA*> g_tickTimers;          // all registered ticktimers
extern CRITICAL_SECTION              g_expanderMutex;       // mutex for Expander-wide locking


//
// Win32 Mutex            = system-wide mutex
// Win32 CRITICAL_SECTION = process-wide mutex
// https://docs.microsoft.com/en-us/windows/desktop/Sync/critical-section-objects
//
// Speed benchmark and especially: ...if the spin count expires, the mutex for the critical section will be allocated.
// https://stackoverflow.com/questions/800383/what-is-the-difference-between-mutex-and-critical-section
//
// Exception safety:
// http://progblogs.blogspot.com/2012/02/critical-section-vs-mutex.html
//


// forward declarations
BOOL WINAPI onProcessAttach();
BOOL WINAPI onProcessDetach(BOOL isTerminating);


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
      case DLL_PROCESS_ATTACH: onProcessAttach();               break;
      case DLL_THREAD_ATTACH :                                  break;
      case DLL_THREAD_DETACH :                                  break;
      case DLL_PROCESS_DETACH: onProcessDetach((BOOL)reserved); break;
   }
   return(TRUE);
}


/**
 * Handler for DLL_PROCESS_ATTACH events.
 *
 * @return BOOL
 */
BOOL WINAPI onProcessAttach() {
   g_mqlInstances   .reserve(128);
   g_threads        .reserve(512);
   g_threadsPrograms.reserve(512);
   g_tickTimers     .reserve(32);

   InitializeCriticalSection(&g_expanderMutex);

   // parse command line arguments
   int argc;
   LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
   if (!argv) return(!error(ERR_WIN32_ERROR+GetLastError(), "CommandLineToArgvW()"));

   for (int i=1; i < argc; i++) {
      if (StrStartsWith(argv[i], L"/portable")) {
         // The terminal also enables portable mode if a command line parameter just *starts* with prefix "/portable".
         // For example passing parameter "/portablepoo" enables portable mode, too. This test mirrors that behavior.
         g_cliOptionPortableMode = TRUE;
         continue;
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
   }
   LocalFree(argv);

   // lock the production version of the DLL in memory
   const char* dllName = GetExpanderFileNameA();
   if (StrEndsWithI(dllName, "rsfMT4Expander.dll")) {
      HMODULE hModule = NULL;
      GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCTSTR)onProcessAttach, &hModule);
   }
   return(TRUE);
}


/**
 * Handler for DLL_PROCESS_DETACH events.
 *
 * @param  BOOL isTerminating - whether the DLL is detached because the process is terminating
 *
 * @return BOOL
 */
BOOL WINAPI onProcessDetach(BOOL isTerminating) {
   if (isTerminating) return(TRUE);

   DeleteCriticalSection(&g_expanderMutex);
   ReleaseTickTimers();
   ReleaseWindowProperties();

   return(TRUE);
}
