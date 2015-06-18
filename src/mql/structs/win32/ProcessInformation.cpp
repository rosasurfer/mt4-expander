/**
 * Win32 structure PROCESS_INFORMATION
 *
 * struct PROCESS_INFORMATION {
 *    HANDLE hProcess;           //  4
 *    HANDLE hThread;            //  4
 *    DWORD  processId;          //  4
 *    DWORD  threadId;           //  4
 * } pi;                         // 16 byte = int[4]
 *
 *
 * MQL-Importdeklarationen:
 * ------------------------
 * int pi_hProcess (/*PROCESS_INFORMATION/int pi[]);
 * int pi_hThread  (/*PROCESS_INFORMATION/int pi[]);
 * int pi_ProcessId(/*PROCESS_INFORMATION/int pi[]);
 * int pi_ThreadId (/*PROCESS_INFORMATION/int pi[]);
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt das Process-Handle einer PROCESS_INFORMATION zurück.
 *
 * @param  PROCESS_INFORMATION* pi
 *
 * @return HANDLE
 *
 *
 * @mql    int pi_hProcess(int pi[]);
 */
HANDLE WINAPI pi_hProcess(PROCESS_INFORMATION* pi) {
   if ((uint)pi < MIN_VALID_POINTER) return((HANDLE)debug("invalid parameter pi = 0x%p (not a valid pointer)", pi));
   return(pi->hProcess);
   #pragma EXPORT
}


/**
 * Gibt das Thread-Handle einer PROCESS_INFORMATION zurück.
 *
 * @param  PROCESS_INFORMATION* pi
 *
 * @return HANDLE
 *
 *
 * @mql    int pi_hThread(int pi[]);
 */
HANDLE WINAPI pi_hThread(PROCESS_INFORMATION* pi) {
   if ((uint)pi < MIN_VALID_POINTER) return((HANDLE)debug("invalid parameter pi = 0x%p (not a valid pointer)", pi));
   return(pi->hThread);
   #pragma EXPORT
}


/**
 * Gibt die Process-ID einer PROCESS_INFORMATION zurück.
 *
 * @param  PROCESS_INFORMATION* pi
 *
 * @return DWORD
 *
 *
 * @mql    int pi_ProcessId(int pi[]);
 */
DWORD WINAPI pi_ProcessId(PROCESS_INFORMATION* pi) {
   if ((uint)pi < MIN_VALID_POINTER) return(debug("invalid parameter pi = 0x%p (not a valid pointer)", pi));
   return(pi->dwProcessId);
   #pragma EXPORT
}


/**
 * Gibt die Thread-ID einer PROCESS_INFORMATION zurück.
 *
 * @param  PROCESS_INFORMATION* pi
 *
 * @return DWORD
 *
 *
 * @mql    int pi_ThreadId(int pi[]);
 */
DWORD WINAPI pi_ThreadId(PROCESS_INFORMATION* pi) {
   if ((uint)pi < MIN_VALID_POINTER) return(debug("invalid parameter pi = 0x%p (not a valid pointer)", pi));
   return(pi->dwThreadId);
   #pragma EXPORT
}