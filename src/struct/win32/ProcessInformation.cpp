/**
 * Win32 struct PROCESS_INFORMATION
 *
 * struct PROCESS_INFORMATION {
 *    HANDLE hProcess;              //  4
 *    HANDLE hThread;               //  4
 *    DWORD  dwProcessId;           //  4
 *    DWORD  dwThreadId;            //  4
 * };                               // 16 byte = int[4]
 */
#include "expander.h"


/**
 * Gibt das Process-Handle einer PROCESS_INFORMATION zurück.
 *
 * @param  PROCESS_INFORMATION* pi
 *
 * @return HANDLE
 */
HANDLE WINAPI pi_hProcess(const PROCESS_INFORMATION* pi) {
   if ((uint)pi < MIN_VALID_POINTER) return((HANDLE)error(ERR_INVALID_PARAMETER, "invalid parameter pi: 0x%p (not a valid pointer)", pi));
   return(pi->hProcess);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das Thread-Handle einer PROCESS_INFORMATION zurück.
 *
 * @param  PROCESS_INFORMATION* pi
 *
 * @return HANDLE
 */
HANDLE WINAPI pi_hThread(const PROCESS_INFORMATION* pi) {
   if ((uint)pi < MIN_VALID_POINTER) return((HANDLE)error(ERR_INVALID_PARAMETER, "invalid parameter pi: 0x%p (not a valid pointer)", pi));
   return(pi->hThread);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Process-ID einer PROCESS_INFORMATION zurück.
 *
 * @param  PROCESS_INFORMATION* pi
 *
 * @return DWORD
 */
DWORD WINAPI pi_ProcessId(const PROCESS_INFORMATION* pi) {
   if ((uint)pi < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter pi: 0x%p (not a valid pointer)", pi));
   return(pi->dwProcessId);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Thread-ID einer PROCESS_INFORMATION zurück.
 *
 * @param  PROCESS_INFORMATION* pi
 *
 * @return DWORD
 */
DWORD WINAPI pi_ThreadId(const PROCESS_INFORMATION* pi) {
   if ((uint)pi < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter pi: 0x%p (not a valid pointer)", pi));
   return(pi->dwThreadId);
   #pragma EXPANDER_EXPORT
}
