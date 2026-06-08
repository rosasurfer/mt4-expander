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

   if (HANDLE hThread = CreateThread(NULL, 0, ExpanderStartThread, hModule, 0, NULL)) {
      CloseHandle(hThread);
   }
   else {
      error(ERR_WIN32_ERROR + GetLastError(), "CreateThread(\"ExpanderStart\")");
      FreeLibrary(hModule);               // on error only (triggers DLL unloading after return)
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
 * These are independant tasks not required for the DLL to work.
 *
 * @param  void* lpParam - DLL module handle
 *
 * @return DWORD - thread exit status
 */
static DWORD WINAPI ExpanderStartThread(void* lpParam) {
   HMODULE hModule = (HMODULE)lpParam;

   const wchar* dllName = GetExpanderFileNameW();
   BOOL isProduction = StrEndsWithI(dllName, L"rsfMT4Expander.dll");

   if (!isProduction) {                            // nothing to do
      FreeLibraryAndExitThread(hModule, NO_ERROR); // decrease ref-count and terminate the thread (never returns)
   }
   PinDllToMemory();                               // otherwise we keep the DLL in memory until process termination

   CustomizeTerminal();
   return NO_ERROR;
}


/**
 * Pin the DLL to memory until the process terminates, so it can't be unloaded by calling FreeLibrary().
 *
 * @return BOOL - success status
 */
BOOL WINAPI PinDllToMemory() {
   HMODULE hModule = NULL;

   static BOOL pinned = GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCTSTR)PinDllToMemory, &hModule);
   if (!pinned) {
      static BOOL warned = (BOOL)warn(ERR_WIN32_ERROR + GetLastError(), "GetModuleHandleExA()");
   }
   return pinned;
   #pragma EXPANDER_EXPORT
}
