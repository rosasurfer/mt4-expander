#include "expander.h"
#include "struct/xtrade/ExecutionContext.h"
#include "util/datetime.h"

#include <vector>


std::vector<ContextChain> g_contextChains  (64);                     // all context chains (i.e. MQL programs, index = program id)
std::vector<DWORD>        g_threads        (64);                     // all known threads executing MQL programs
std::vector<uint>         g_threadsPrograms(64);                     // the last MQL program executed by a thread
uint                      g_lastUIThreadProgram;                     // the last MQL program executed by the UI thread
CRITICAL_SECTION          g_terminalLock;                            // application wide lock


// forward declarations
BOOL WINAPI onProcessAttach();
BOOL WINAPI onProcessDetach();


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fReason, LPVOID lpReserved) {
   BOOL result = TRUE;

   switch (fReason) {
      case DLL_PROCESS_ATTACH: result = onProcessAttach(); break;
      case DLL_THREAD_ATTACH :                             break;
      case DLL_THREAD_DETACH :                             break;
      case DLL_PROCESS_DETACH: result = onProcessDetach(); break;
   }
   return(result);
}


/**
 * Handler for DLL_PROCESS_ATTACH events.
 */
BOOL WINAPI onProcessAttach() {
   g_threads        .resize(0);
   g_threadsPrograms.resize(0);
   g_contextChains  .resize(1);                             // index[0] stays empty (zero wouldn't be a valid MQL program id)
   InitializeCriticalSection(&g_terminalLock);
   return(TRUE);
}


/**
 * Handler for DLL_PROCESS_DETACH events.
 */
BOOL WINAPI onProcessDetach() {
   DeleteCriticalSection(&g_terminalLock);
   RemoveTickTimers();
   return(TRUE);
}
