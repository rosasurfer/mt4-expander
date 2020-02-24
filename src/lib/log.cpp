#include "expander.h"
#include "struct/rsf/ExecutionContext.h"

#include <fstream>

extern MqlProgramList g_mqlPrograms;               // all MQL programs: vector<ContextChain> with index = program id


/**
 * Send a message to the configured log appenders.
 *
 * @param  uint  pid              - id of the MQL program to use for log configuration
 * @param  char* message          - log message
 * @param  int   error [optional] - error to log (default: none)
 *
 * @return BOOL - success status
 */
BOOL WINAPI Logger(uint pid, const char* message, int error) {
   if ((int)pid < 1)                      return(_FALSE(error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d", pid)));
   if (g_mqlPrograms.size() <= pid)       return(_FALSE(error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (no such program)", pid)));
   if ((uint)message < MIN_VALID_POINTER) return(_FALSE(error(ERR_INVALID_PARAMETER, "invalid parameter message: 0x%p (not a valid pointer)", message)));
   if (!*message)                         return(_FALSE(error(ERR_INVALID_PARAMETER, "illegal length of parameter message: \"\" (0)")));
   debug("feature not implemented");
   return(FALSE);

   using namespace std;

   EXECUTION_CONTEXT* ec = (*g_mqlPrograms[pid])[0];              // master context

   if (ec->logEnabled) {
      // send the message to the system debugger
      if (ec->logToDebugEnabled) {
      }

      // send the message to a custom logfile
      if (ec->logToCustomEnabled) {
         if (!ec->customLogFile) {
            if (!ec->customLogFilename || !*ec->customLogFilename) {
               ec->logToCustomEnabled = FALSE;
               return(_FALSE(error(ERR_ILLEGAL_STATE, "illegal logfile configuration: \"%s\"", ec->customLogFilename)));
            }
            ec->customLogFile = new ofstream();
         }

         ofstream* logfile = ec->customLogFile;
         if (!logfile->is_open()) logfile->open(ec->customLogFilename);


      }
   }

   debug("feature not implemented");
   return(TRUE);
   #pragma EXPANDER_EXPORT
}

