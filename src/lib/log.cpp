#include "expander.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/conversion.h"
#include "lib/string.h"
#include "struct/rsf/ExecutionContext.h"

#include <ctime>
#include <fstream>

extern MqlInstanceList g_mqlInstances;             // all MQL program instances : vector<ContextChain*> with index = instance id aka pid


/**
 * Append a log message to a program's logfile. The caller is responsible for filtering messages by loglevel.
 *
 * @param  EXECUTION_CONTEXT* ec         - execution context of the program
 * @param  time32             serverTime - server time as a Unix timestamp (used in tests only, modelled)
 * @param  char*              message    - log message
 * @param  int                error      - error linked to the message (if any)
 * @param  int                level      - log level of the message
 *
 * @return BOOL - success status
 */
BOOL WINAPI AppendLogMessageA(EXECUTION_CONTEXT* ec, time32 serverTime, const char* message, int error, int level) {
   if ((uint)ec < MIN_VALID_POINTER)      return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->pid)                          return(!error(ERR_INVALID_PARAMETER, "invalid execution context: ec.pid=0  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (g_mqlInstances.size() <= ec->pid)  return(!error(ERR_ILLEGAL_STATE,     "invalid execution context: ec.pid=%d (no such instance)  ec=%s", ec->pid, EXECUTION_CONTEXT_toStr(ec)));
   if ((uint)message < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter message: 0x%p (not a valid pointer)", message));
   if (level == LOG_OFF)                  return(FALSE);

   EXECUTION_CONTEXT* master = (*g_mqlInstances[ec->pid])[0];
   if (master->superContext) master = (*g_mqlInstances[master->superContext->pid])[0];    // use a super context (if any)

   // check whether to use an existing logger or a logbuffer
   BOOL useLogger    = (master->logger && strlen(master->logFilename));
   BOOL useLogBuffer = (!useLogger && master->programInitFlags & INIT_BUFFERED_LOG);
   if (!useLogger && !useLogBuffer) return(FALSE);                                        // logger and buffered log are inactive

   // open a closed logger
   if (useLogger && !master->logger->is_open()) {
      if (!IsFileA(master->logFilename, MODE_SYSTEM)) {
         char drive[MAX_DRIVE], dir[MAX_DIR];                                             // extract the directory part of logFilename
         _splitpath(master->logFilename, drive, dir, NULL, NULL);
         string path = string(drive).append(dir);
         if (CreateDirectoryA(path.c_str(), MODE_SYSTEM|MODE_MKPARENT)) return(FALSE);    // make sure the directory exists
      }
      master->logger->open(master->logFilename, std::ios::binary|std::ios::app);          // open the logfile
      if (!master->logger->is_open()) return(!error(ERR_WIN32_ERROR+GetLastError(), "opening of \"%s\" failed (%s)", master->logFilename, strerror(errno)));
      if (master->logBuffer && master->logBuffer->size()) {
         uint size = master->logBuffer->size();
         for (uint i=0; i < size; ++i) {
            string entry = (*master->logBuffer)[i];
            *master->logger << entry << NL;                                               // append existing logbuffer entries
         }
         master->logger->flush();
         master->logBuffer->clear();
      }
   }
   else if (useLogBuffer && !master->logBuffer) {
      master->logBuffer = ec->logBuffer = new LogBuffer();
      master->logBuffer->reserve(16);
   }

   // compose the log entry
   std::ostringstream ss;
   string sLoglevel(level==LOG_DEBUG ? "": LoglevelDescriptionA(level));                  // loglevel (LOG_DEBUG is blanked out)
   string sExecPath(ec->programName); sExecPath.append("::");                             // execution path
   if (ec->moduleType == MT_LIBRARY) sExecPath.append(ec->moduleName).append("::");       //
   string sMessage(message); StrReplace(StrReplace(sMessage, "\r\n", " "), "\n", " ");    // replace linebreaks with spaces
   string sError; if (error) sError.append("  [").append(ErrorToStrA(error)).append("]"); // append error description

   if (ec->testing) {                                                                     // tester:
      ss << "T " << gmtTimeFormat(serverTime, "%Y-%m-%d %H:%M:%S");                       // prepend prefix "T" followed by the passed tester time (seconds only)
   }
   else {
      SYSTEMTIME st = getSystemTime();                                                    // online:
      time32 gmtTime = SystemTimeToUnixTime32(st);                                        // prepend current time with milliseconds
      ss << localTimeFormat(gmtTime, "%Y-%m-%d %H:%M:%S") << "." << std::setw(3) << std::setfill('0') << st.wMilliseconds;
   }
   ss << "  " << std::setfill(' ') << std::setw(6) << std::left << sLoglevel << "  " << ec->symbol << "," << std::setw(3) << std::left << PeriodDescriptionA(ec->timeframe) << "  " << sExecPath << sMessage << sError;

   // write the log entry to logfile or logbuffer
   if (useLogger) *master->logger << ss.str() << std::endl;
   else            master->logBuffer->push_back(ss.str());

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Set a program's log filename.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              filename - if a non-empty string is passed logging to the specified file is enabled;
 *                                       if an empty string or a NULL pointer are passed the logfile is disabled
 * @return BOOL - success status
 */
BOOL WINAPI SetLogfileA(EXECUTION_CONTEXT* ec, const char* filename) {
   if ((uint)ec < MIN_VALID_POINTER)                   return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (filename && (uint)filename < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter filename: 0x%p (not a valid pointer)", filename));
   if (strlen(filename) > MAX_PATH)                    return(!error(ERR_INVALID_PARAMETER, "too long parameter filename: \"%s\" (max. %d chars)", filename, MAX_PATH));
   if (!ec->pid)                                       return(!error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (g_mqlInstances.size() <= ec->pid)               return(!error(ERR_ILLEGAL_STATE,     "invalid execution context: ec.pid=%d (no such instance)  ec=%s", ec->pid, EXECUTION_CONTEXT_toStr(ec)));

   ContextChain &chain = *g_mqlInstances[ec->pid];
   EXECUTION_CONTEXT* master = chain[0];
   if (master->superContext) return(TRUE);                                       // ignore the call if in iCustom()

   if (filename && *filename) {
      // enable the file logger
      std::ofstream* log = master->logger;
      if (!log) log = master->logger = ec->logger = new std::ofstream();

      // close a previous logfile with a different name
      if (!StrCompare(filename, master->logFilename)) {
         if (log->is_open()) log->close();
      }
      ec_SetLogFilename(ec, filename);

      // open the new logfile if the logfile appender is not disabled
      if (master->loglevel!=LOG_OFF && master->loglevelFile!=LOG_OFF) {
         if (!log->is_open()) {
            if (!IsFileA(filename, MODE_SYSTEM)) {
               char drive[MAX_DRIVE], dir[MAX_DIR];                              // extract the directory part of filename
               _splitpath(filename, drive, dir, NULL, NULL);
               string path = string(drive).append(dir);                          // make sure the directory exists
               if (CreateDirectoryA(path.c_str(), MODE_SYSTEM|MODE_MKPARENT)) return(FALSE);
            }
            log->open(filename, std::ios::binary|std::ios::app);                 // open the logfile
            if (!log->is_open()) return(!error(ERR_WIN32_ERROR+GetLastError(), "opening of \"%s\" failed (%s)", filename, strerror(errno)));

            if (master->logBuffer && master->logBuffer->size()) {
               uint size = master->logBuffer->size();
               for (uint i=0; i < size; ++i) {
                  string entry = (*master->logBuffer)[i];
                  *master->logger << entry << NL;                                // append existing logbuffer entries
               }
               master->logger->flush();
               master->logBuffer->clear();
            }
         }
      }
   }
   else {
      // close the logfile but keep an existing instance (we may be in an init cycle)
      if (master->logger && master->logger->is_open())
         master->logger->close();
      ec_SetLogFilename(ec, filename);
   }

   return(TRUE);
   #pragma EXPANDER_EXPORT
}
