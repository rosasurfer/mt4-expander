#include "expander.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/conversion.h"
#include "lib/string.h"
#include "struct/rsf/ExecutionContext.h"

#include <fstream>
#include <time.h>

extern MqlProgramList g_mqlPrograms;               // all MQL programs: vector<ContextChain*> with index = program id


/**
* Append a log message to a program's logfile.
 *
 * @param  EXECUTION_CONTEXT* ec      - execution context of the program
 * @param  datetime           time    - current time (used only in tester)
 * @param  char*              message - log message
 * @param  int                error   - error linked to the message (if any)
 * @param  int                level   - log level of the message
 *
 * @return BOOL - success status
 */
BOOL WINAPI AppendLogMessageA(EXECUTION_CONTEXT* ec, datetime time, const char* message, int error, int level) {
   if ((uint)ec < MIN_VALID_POINTER)      return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->pid)                          return(error(ERR_INVALID_PARAMETER, "invalid execution context: ec.pid=0  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (g_mqlPrograms.size() <= ec->pid)   return(error(ERR_ILLEGAL_STATE,     "invalid execution context: ec.pid=%d (no such program)  ec=%s", ec->pid, EXECUTION_CONTEXT_toStr(ec)));
   if ((uint)message < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter message: 0x%p (not a valid pointer)", message));

   EXECUTION_CONTEXT* master = (*g_mqlPrograms[ec->pid])[0];

   // check the configured loglevels
   if (!master->loglevel     || master->loglevel    ==LOG_OFF) return(FALSE);
   if (!master->loglevelFile || master->loglevelFile==LOG_OFF) return(FALSE);
   if (level < master->loglevelFile || level==LOG_OFF)         return(FALSE);

   // check the logger status
   std::ofstream* log = master->logger;
   if (!log) return(FALSE);                                                                  // logger not instantiated => logger is inactive

   if (!log->is_open()) {
      if (!strlen(master->logFilename) ) return(FALSE);                                      // logfile not set => logger is inactive
      log->open(master->logFilename, std::ios::app);
      if (!log->is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "opening of \"%s\" failed (%s)", master->logFilename, strerror(errno)));
   }

   // compose the parts of the final log entry
   string sLoglevel(level==LOG_INFO ? "": LoglevelDescriptionA(level));                      // loglevel (INFO is blanked out)
   string sExecPath(ec->programName); sExecPath.append("::");                                // execution path
   if (ec->moduleType == MT_LIBRARY) sExecPath.append(ec->moduleName).append("::");
   string sError; if (error) sError.append("  [").append(ErrorToStr(error)).append("]");     // error description

   // generate the appropriate timestring and write the log entry to the file
   if (ec->testing) {
      size_t bufSize = 20;
      char* sTime = (char*)alloca(bufSize);
      gmtimeFormat(sTime, bufSize, time, "%Y-%m-%d %H:%M:%S");                               // time with seconds
      *log << "Tester " << sTime << " ";
   }
   else {
      SYSTEMTIME st; GetSystemTime(&st);
      size_t bufSize = 20;
      char* sTime = (char*)alloca(bufSize);
      localtimeFormat(sTime, bufSize, st, "%Y-%m-%d %H:%M:%S");
      *log << sTime << "." << std::setw(3) << std::setfill('0') << st.wMilliseconds << " ";  // time with milliseconds
   }

   *log << std::setw(6) << std::setfill(' ') << sLoglevel << " " << ec->symbol << "," << PeriodDescription(ec->timeframe) << "  " << sExecPath << message << sError << std::endl;

   // @see  https://www.codeguru.com/cpp/cpp/date_time/routines/article.php/c1615/Extended-Time-Format-Functions-with-Milliseconds.htm
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
   if ((uint)ec < MIN_VALID_POINTER)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (filename && (uint)filename < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter filename: 0x%p (not a valid pointer)", filename));
   if (!ec->pid)                                       return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (g_mqlPrograms.size() <= ec->pid)                return(error(ERR_ILLEGAL_STATE,     "invalid execution context: ec.pid=%d (no such program)  ec=%s", ec->pid, EXECUTION_CONTEXT_toStr(ec)));

   ContextChain &chain = *g_mqlPrograms[ec->pid];
   EXECUTION_CONTEXT* master = chain[0];

   if (filename && *filename) {
      // enable the file logger
      std::ofstream* log = master->logger;
      if (!log) {
         log = master->logger = ec->logger = new std::ofstream();
         debug("logger instance created");
      }

      // close a different previous logfile
      if (!StrCompare(filename, master->logFilename)) {
         if (log->is_open()) log->close();
      }
      ec_SetLogFilename(ec, filename);
      debug("new logfile: \"%s\"", filename);
      debug("ec=%s", EXECUTION_CONTEXT_toStr(master));

      // open the new logfile if the logfile appender is not disabled
      if (master->loglevel!=LOG_OFF && master->loglevelFile!=LOG_OFF) {
         debug("logging+logfile appender are not disabled");
         if (!log->is_open()) {
            if (!IsFileA(filename)) {
               char drive[_MAX_DRIVE], dir[_MAX_DIR];                            // extract the directory part of logFilename
               _splitpath(filename, drive, dir, NULL, NULL);
               if (CreateDirectoryA(string(drive).append(dir), MKDIR_PARENT))    // make sure the directory exists
                  return(FALSE);
            }
            log->open(filename, std::ios::app);                                  // open the logfile
            if (!log->is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "opening of \"%s\" failed (%s)", filename, strerror(errno)));
            debug("logfile opened: \"%s\"", filename);
         }
         else {
            debug("logfile already open: \"%s\"", filename);
         }
      }
      else {
         debug("logging|logfile appender are disabled");
      }
   }
   else {
      // close the logfile but keep an existing instance (we may be in an init cycle)
      if (master->logger && master->logger->is_open()) {
         master->logger->close();
         debug("previous logfile closed");
      }
      ec_SetLogFilename(ec, filename);
      debug("empty logfile: \"%s\" (%s)", filename, master->programName);
   }

   return(TRUE);
   #pragma EXPANDER_EXPORT
}
