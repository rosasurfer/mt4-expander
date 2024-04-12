#include "expander.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/timer.h"
#include "lib/lock/Lock.h"
#include "struct/rsf/ExecutionContext.h"

#include <shellapi.h>

LPWSTR* g_argv;               // Unicode array of command line arguments. The 1st element contains the program name, each subsequent element one argument.
int     g_argc;               // size of g_argv

extern CRITICAL_SECTION              g_terminalMutex;       // mutex for application-wide locking
extern Locks                         g_locks;               // a map holding pointers to fine-granular locks

extern MqlInstanceList               g_mqlInstances;        // all MQL program instances
extern std::vector<DWORD>            g_threads;             // all known threads executing MQL programs
extern std::vector<uint>             g_threadsPrograms;     // the last MQL program executed by a thread
extern std::vector<TICK_TIMER_DATA*> g_tickTimers;          // all registered ticktimers


//
// Win32 Mutex            = system-wide mutex
// Win32 CRITICAL_SECTION = process-wide mutex
// https://docs.microsoft.com/en-us/windows/desktop/Sync/critical-section-objects
//
//
// Speed benchmark and especially: ...if the spin count expires, the mutex for the critical section will be allocated.
// https://stackoverflow.com/questions/800383/what-is-the-difference-between-mutex-and-critical-section
//
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

   InitializeCriticalSection(&g_terminalMutex);

   // get and store the command line arguments
   g_argv = CommandLineToArgvW(GetCommandLineW(), &g_argc);
   if (!g_argv) return(!error(ERR_WIN32_ERROR+GetLastError(), "CommandLineToArgvW()"));

   // the production version of the DLL is locked in memory
   const char* dllName = GetExpanderFileNameA();
   if (StrEndsWith(dllName, "rsfMT4Expander.dll")) {
      HMODULE hModule = NULL;
      GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCTSTR)onProcessAttach, &hModule);
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

   DeleteCriticalSection(&g_terminalMutex);
   ReleaseTickTimers();
   ReleaseWindowProperties();

   for (Locks::iterator it=g_locks.begin(), end=g_locks.end(); it != end; ++it) {
      delete it->second;
   }
   g_locks.clear();
   return(TRUE);
}
