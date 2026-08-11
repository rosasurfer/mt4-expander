#include "expander.h"
#include "lib/string.h"
#include "struct/ExecutionContext.h"


#define MQL_PROGRAM_NAME "Rules Monitor"


/**
 * Create the status panel.
 *
 * @param  uint pid - indicator pid
 *
 * @return HWND - window handle
 */
HWND WINAPI RulesMonitor_CreateStatusPanel(uint pid) {
   // get the EXECUTION_CONTEXT
   if ((int)pid <= 0) return (HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid);
   if (!ec) return NULL;
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAME)) return (HWND)!error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

   return NULL;
   #pragma EXPANDER_EXPORT
}


/**
 * Remove the status panel.
 *
 * @param  uint pid - indicator pid
 *
 * @return HWND - window handle
 */
HWND WINAPI RulesMonitor_RemoveStatusPanel(uint pid) {
   // get the EXECUTION_CONTEXT
   if ((int)pid <= 0) return (HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid);
   if (!ec) return NULL;
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAME)) return (HWND)!error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

   return NULL;
   #pragma EXPANDER_EXPORT
}


/**
 * Update the status panel.
 *
 * @param  uint pid - indicator pid
 *
 * @return HWND - window handle
 */
HWND WINAPI RulesMonitor_UpdateStatusPanel(uint pid) {
   // get the EXECUTION_CONTEXT
   if ((int)pid <= 0) return (HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid);
   if (!ec) return NULL;
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAME)) return (HWND)!error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

   return NULL;
   #pragma EXPANDER_EXPORT
}
