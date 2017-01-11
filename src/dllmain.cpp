#include "expander.h"


std::vector<ContextChain> contextChains  (64);                       // all MQL programs (index = program id)
std::vector<DWORD>        threads        (64);                       // all threads executing MQL programs
std::vector<uint>         threadsPrograms(64);                       // the last MQL program executed by a thread
uint                      lastUIThreadProgram;                       // the last MQL program executed by the UI thread
CRITICAL_SECTION          terminalLock;                              // application wide lock


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
   threads        .resize(0);
   threadsPrograms.resize(0);
   contextChains  .resize(1);                               // index[0] stays empty (zero wouldn't be a valid MQL program id)
   InitializeCriticalSection(&terminalLock);
   return(TRUE);
}


/**
 * Handler for DLL_PROCESS_DETACH events.
 */
BOOL WINAPI onProcessDetach() {
   DeleteCriticalSection(&terminalLock);
   RemoveTickTimers();
   return(TRUE);
}
