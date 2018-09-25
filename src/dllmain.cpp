#include "expander.h"
#include "lib/datetime.h"
#include "struct/xtrade/ExecutionContext.h"

#include <vector>


std::vector<ContextChain> g_contextChains  (64);                     // all context chains (i.e. MQL programs, index = program id)
std::vector<DWORD>        g_threads        (64);                     // all known threads executing MQL programs
std::vector<uint>         g_threadsPrograms(64);                     // the last MQL program executed by a thread
uint                      g_lastUIThreadProgram;                     // the last MQL program executed by the UI thread
CRITICAL_SECTION          g_terminalMutex;                           // mutex for application-wide locking


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
void WINAPI onProcessAttach();
void WINAPI onProcessDetach();


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fReason, LPVOID lpReserved) {
   switch (fReason) {
      case DLL_PROCESS_ATTACH: onProcessAttach(); break;
      case DLL_THREAD_ATTACH :                    break;
      case DLL_THREAD_DETACH :                    break;
      case DLL_PROCESS_DETACH: onProcessDetach(); break;
   }
   return(TRUE);
}


/**
 * Handler for DLL_PROCESS_ATTACH events.
 */
void WINAPI onProcessAttach() {
   g_threads        .resize(0);
   g_threadsPrograms.resize(0);
   g_contextChains  .resize(1);                             // index[0] stays empty (zero wouldn't be a valid MQL program id)
   InitializeCriticalSection(&g_terminalMutex);
}


/**
 * Handler for DLL_PROCESS_DETACH events.
 */
void WINAPI onProcessDetach() {
   DeleteCriticalSection(&g_terminalMutex);
   RemoveTickTimers();
}
