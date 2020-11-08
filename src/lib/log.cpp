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

   // check and apply the configured loglevels
   if (!master->loglevel     || master->loglevel    ==LOG_OFF) return(FALSE);
   if (!master->loglevelFile || master->loglevelFile==LOG_OFF) return(FALSE);
   if (level < master->loglevelFile || level==LOG_OFF)         return(FALSE);

   // check whether to use an existing logger or a logbuffer
   BOOL useLogger    = (master->logger && strlen(master->logFilename));
   BOOL useLogBuffer = (!useLogger && master->programInitFlags & INIT_BUFFERED_LOG);
   if (!useLogger && !useLogBuffer) return(FALSE);                                        // logger and buffered log are inactive

   // open a closed logger
   if (useLogger && !master->logger->is_open()) {
      if (!IsFileA(master->logFilename)) {
         char drive[MAX_DRIVE], dir[MAX_DIR];                                             // extract the directory part of logFilename
         _splitpath(master->logFilename, drive, dir, NULL, NULL);
         if (CreateDirectoryA(string(drive).append(dir), MKDIR_PARENT))                   // make sure the directory exists
            return(FALSE);
      }
      master->logger->open(master->logFilename, std::ios::binary|std::ios::app);          // open the logfile
      if (!master->logger->is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "opening of \"%s\" failed (%s)", master->logFilename, strerror(errno)));
      if (master->logBuffer && master->logBuffer->size()) {
         uint size = master->logBuffer->size();
         for (uint i=0; i < size; ++i) {
            string* entry = (*master->logBuffer)[i];
            *master->logger << *entry << NL;                                              // flush existing logbuffer entries
            delete entry;
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
   string sLoglevel(level==LOG_INFO ? "": LoglevelDescriptionA(level));                   // loglevel (INFO is blanked out)
   string sExecPath(ec->programName); sExecPath.append("::");                             // execution path
   if (ec->moduleType == MT_LIBRARY) sExecPath.append(ec->moduleName).append("::");       //
   string sMessage(message);




                                                                 // TODO: process linebreaks
   string sError; if (error) sError.append("  [").append(ErrorToStr(error)).append("]");  // error description

   if (ec->testing) {                                                                     // generate the appropriate time string
      size_t bufSize = 20;
      char* buffer = (char*)alloca(bufSize);
      gmtimeFormat(buffer, bufSize, time, "%Y-%m-%d %H:%M:%S");                           // time with seconds
      ss << "Tester " << buffer;
   }
   else {
      SYSTEMTIME st; GetLocalTime(&st);
      size_t bufSize = 20;
      char* buffer = (char*)alloca(bufSize);
      localtimeFormat(buffer, bufSize, st, "%Y-%m-%d %H:%M:%S");
      ss << buffer << "." << std::setw(3) << std::setfill('0') << st.wMilliseconds;       // time with milliseconds
   }
   ss << " " << std::setw(6) << std::setfill(' ') << std::left << sLoglevel << " " << ec->symbol << "," << PeriodDescription(ec->timeframe) << "  " << sExecPath << sMessage << sError;

   // write the log entry to logfile or logbuffer
   if (useLogger) *master->logger << ss.str() << std::endl;
   else           master->logBuffer->push_back(new string(ss.str()));

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
   if (strlen(filename) > MAX_PATH)                    return(error(ERR_INVALID_PARAMETER, "too long parameter filename: \"%s\" (max. %d chars)", filename, MAX_PATH));
   if (!ec->pid)                                       return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (g_mqlPrograms.size() <= ec->pid)                return(error(ERR_ILLEGAL_STATE,     "invalid execution context: ec.pid=%d (no such program)  ec=%s", ec->pid, EXECUTION_CONTEXT_toStr(ec)));

   ContextChain &chain = *g_mqlPrograms[ec->pid];
   EXECUTION_CONTEXT* master = chain[0];

   if (filename && *filename) {
      // enable the file logger
      std::ofstream* log = master->logger;
      if (!log) log = master->logger = ec->logger = new std::ofstream();

      // close a different previous logfile
      if (!StrCompare(filename, master->logFilename)) {
         if (log->is_open()) log->close();
      }
      ec_SetLogFilename(ec, filename);

      // open the new logfile if the logfile appender is not disabled
      if (master->loglevel!=LOG_OFF && master->loglevelFile!=LOG_OFF) {
         if (!log->is_open()) {
            if (!IsFileA(filename)) {
               char drive[MAX_DRIVE], dir[MAX_DIR];                              // extract the directory part of logFilename
               _splitpath(filename, drive, dir, NULL, NULL);
               if (CreateDirectoryA(string(drive).append(dir), MKDIR_PARENT))    // make sure the directory exists
                  return(FALSE);
            }
            log->open(filename, std::ios::binary|std::ios::app);                 // open the logfile
            if (!log->is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "opening of \"%s\" failed (%s)", filename, strerror(errno)));
            if (master->logBuffer && master->logBuffer->size()) {
               uint size = master->logBuffer->size();
               for (uint i=0; i < size; ++i) {
                  string* entry = (*master->logBuffer)[i];
                  *master->logger << *entry << NL;                               // flush existing logbuffer entries
                  delete entry;
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
