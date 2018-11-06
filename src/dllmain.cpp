#include "expander.h"
#include "lib/datetime.h"
#include "lib/lock/Lock.h"
#include "struct/xtrade/ExecutionContext.h"


extern std::vector<ContextChain> g_contextChains;                    // all context chains (i.e. MQL programs, index = program id)
extern std::vector<DWORD>        g_threads;                          // all known threads executing MQL programs
extern std::vector<uint>         g_threadsPrograms;                  // the last MQL program executed by a thread

extern CRITICAL_SECTION          g_terminalMutex;                    // mutex for application-wide locking
extern Locks                     g_locks;                            // a map holding pointers to fine-granular locks


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


/**
 * Handler for DLL_PROCESS_ATTACH events.
 */
void WINAPI onProcessAttach() {
   g_contextChains  .resize(1);                             // chains[0] is unused (zero is not a valid MQL program id)
   g_threads        .resize(0);
   g_threadsPrograms.resize(0);
   InitializeCriticalSection(&g_terminalMutex);
}


/**
 * Handler for DLL_PROCESS_DETACH events.
 */
void WINAPI onProcessDetach() {
   DeleteCriticalSection(&g_terminalMutex);
   RemoveTickTimers();

   for (Locks::iterator i=g_locks.begin(); i != g_locks.end(); ++i) {
      delete i->second;
   }
   g_locks.clear();
}


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
   switch (reason) {
      case DLL_PROCESS_ATTACH: onProcessAttach(); break;
      case DLL_THREAD_ATTACH :                    break;
      case DLL_THREAD_DETACH :                    break;
      case DLL_PROCESS_DETACH: onProcessDetach(); break;
   }
   return(TRUE);
}
