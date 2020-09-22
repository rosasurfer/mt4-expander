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
 * @param  char*              message - log message
 * @param  int                error   - error linked to the message (if any)
 * @param  int                level   - log level of the message
 *
 * @return BOOL - success status
 */
BOOL WINAPI AppendLogMessageA(EXECUTION_CONTEXT* ec, const char* message, int error, int level) {
   if ((uint)ec < MIN_VALID_POINTER)      return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->pid)                          return(error(ERR_INVALID_PARAMETER, "invalid execution context: ec.pid=0  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (g_mqlPrograms.size() <= ec->pid)   return(error(ERR_ILLEGAL_STATE,     "invalid execution context: ec.pid=%d (no such program)  ec=%s", ec->pid, EXECUTION_CONTEXT_toStr(ec)));
   if ((uint)message < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter message: 0x%p (not a valid pointer)", message));

   EXECUTION_CONTEXT* master = (*g_mqlPrograms[ec->pid])[0];

   // check the configured loglevels
   if (!master->loglevel     || master->loglevel==LOG_OFF)     return(FALSE);
   if (!master->loglevelFile || master->loglevelFile==LOG_OFF) return(FALSE);
   if (level < master->loglevelFile || level==LOG_OFF)         return(FALSE);

   // check the logger status
   std::ofstream* log = master->logger;
   if (!log) return(FALSE);                                                                     // logger not instantiated => logger is inactive

   if (!log->is_open()) {
      if (!strlen(master->logFilename) ) return(FALSE);                                         // logfile not set => logger is inactive
      log->open(master->logFilename, std::ios::app);
      if (!log->is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "opening of \"%s\" failed (%s)", master->logFilename, strerror(errno)));
   }

   // compose parts of the final log entry
   SYSTEMTIME st; GetSystemTime(&st);
   tm tt = {};
   tt.tm_year  = st.wYear - 1900;               // years since 1900
   tt.tm_mon   = st.wMonth - 1;                 // months since January:   0..11
   tt.tm_mday  = st.wDay;                       // day of the month:       1..31
   tt.tm_hour  = st.wHour;                      // hours since midnight:   0..23
   tt.tm_min   = st.wMinute;                    // minutes of the hour:    0..59
   tt.tm_sec   = st.wSecond;                    // seconds of the minute:  0..59
   tt.tm_isdst = -1;                            // have the CRT compute whether DST is in effect
   datetime gmtime = _mkgmtime(&tt);
   const char* sTime = LocalTimeFormatA(gmtime, "%Y-%m-%d %H:%M:%S");                           // formatted time with seconds

   string sLoglevel(level==LOG_INFO ? "": LoglevelDescriptionA(level));                         // loglevel (INFO is blanked out)
   string sTester(ec->testing ? "Tester::" : "");                                               // tester identification
   string sExecPath(ec->programName); sExecPath.append("::");                                   // execution path
   if (ec->moduleType == MT_LIBRARY) sExecPath.append(ec->moduleName).append("::");
   string sError; if (error) sError.append("  [").append(ErrorToStr(error)).append("]");        // error description

   // write the log entry to the file
   *log << sTime << "." << std::setw(3) << std::setfill('0') << st.wMilliseconds << " " << std::setw(6) << sLoglevel << " "
        << sTester << ec->symbol << "," << PeriodDescription(ec->timeframe) << "  "
        << sExecPath << message << sError << std::endl;

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
      if (!log) log = master->logger = ec->logger = new std::ofstream();

      // close a previous logfile
      if (!StrCompare(filename, master->logFilename)) {
         if (log->is_open()) log->close();
      }
      ec_SetLogFilename(ec, filename);

      // open the new logfile if loglevels are configured
      if (master->loglevel && master->loglevel!=LOG_OFF) {
         if (master->loglevelFile || master->loglevelFile!=LOG_OFF) {
            if (!log->is_open()) {
               log->open(master->logFilename, std::ios::app);
               if (!log->is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "opening of \"%s\" failed (%s)", master->logFilename, strerror(errno)));
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
