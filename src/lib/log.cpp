#include "expander.h"
#include "lib/string.h"
#include "struct/rsf/ExecutionContext.h"

#include <fstream>

extern MqlProgramList g_mqlPrograms;               // all MQL programs: vector<ContextChain> with index = program id


/**
 * Send a message to the log appenders of the specified program.
 *
 * @param  EXECUTION_CONTEXT* ec               - execution context of the program
 * @param  char*              message          - log message
 * @param  int                error [optional] - error to log (default: none)
 *
 * @return BOOL - success status
 */
BOOL WINAPI LogA(EXECUTION_CONTEXT* ec, const char* message, int error) {
   if ((uint)ec < MIN_VALID_POINTER)      return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->pid)                          return(error(ERR_INVALID_PARAMETER, "invalid execution context: ec.pid=0  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (g_mqlPrograms.size() <= ec->pid)   return(error(ERR_ILLEGAL_STATE,     "invalid execution context: ec.pid=%d (no such program)  ec=%s", ec->pid, EXECUTION_CONTEXT_toStr(ec)));
   if ((uint)message < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter message: 0x%p (not a valid pointer)", message));

   EXECUTION_CONTEXT* master = (*g_mqlPrograms[ec->pid])[0];
   if (!master->logEnabled) return(debug("%-13s  logging is disabled", master->programName));

   //if (master->logToDebugEnabled)                         // send the message to the system debugger (if configured)

   if (master->logToCustomEnabled) {                        // log the message to a custom logfile (if configured)
      std::ofstream* log = master->customLog;
      if (!log || !log->is_open()) return(error(ERR_ILLEGAL_STATE, "invalid execution context (ec.customLog is not an open stream)  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

      *log << message << std::endl;
   }
   else debug("%-13s  custom logging is disabled", master->programName);

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Enable or disable a program's custom logfile.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              filename - if a non-empty string is passed custom logging to the specified file is enabled;
 *                                       if a NULL pointer or an empty string is passed custom logging is disabled
 * @return BOOL - success status
 */
BOOL WINAPI SetCustomLogA(EXECUTION_CONTEXT* ec, const char* filename) {
   if (            (uint)ec       < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (filename && (uint)filename < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter filename: 0x%p (not a valid pointer)", filename));
   if (!ec->pid)                                       return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (g_mqlPrograms.size() <= ec->pid)                return(error(ERR_ILLEGAL_STATE,     "invalid execution context: ec.pid=%d (no such program)  ec=%s", ec->pid, EXECUTION_CONTEXT_toStr(ec)));

   ContextChain &chain = *g_mqlPrograms[ec->pid];
   if (ec != chain[1]) return(error(ERR_ACCESS_DENIED, "cannot modify custom logging from an MQL library,  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   EXECUTION_CONTEXT* master = chain[0];

   if (filename && *filename) {
      // enable custom logging
      std::ofstream* log = master->customLog;
      if (!log) {
         log = master->customLog = ec->customLog = new std::ofstream();
         debug("%-13s  new ofstream instance created", master->programName);
      }
      if (!StrCompare(filename, master->customLogFilename)) {
         if (log->is_open()) {
            log->close();
            debug("%-13s  file \"%s\" closed", master->programName, master->customLogFilename);
         }
         ec_SetCustomLogFilename(ec, filename);
      }
      if (!log->is_open()) {
         log->open(master->customLogFilename, std::ios::app);
         if (!log->is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "%-13s  opening \"%s\" failed (%s)", master->programName, master->customLogFilename, strerror(errno)));
         debug("%-13s  file \"%s\" opened", master->programName, master->customLogFilename);
      }
      ec_SetLogToCustomEnabled(ec, TRUE);
   }
   else {
      // disable custom logging
      ec_SetLogToCustomEnabled(ec, FALSE);
      if (master->customLog && master->customLog->is_open()) {
         master->customLog->close();
         debug("%-13s  file \"%s\" closed", master->programName, master->customLogFilename);
      }
   }

   return(TRUE);
   #pragma EXPANDER_EXPORT
}
