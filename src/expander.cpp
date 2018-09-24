#include "expander.h"
#include "lib/conversion.h"
#include "lib/helper.h"
#include "struct/xtrade/ExecutionContext.h"

#include <vector>


// external declarations for error management
extern std::vector<ContextChain> g_contextChains;                    // all context chains (= MQL programs, index = program id)
extern std::vector<DWORD>        g_threads;                          // all known threads executing MQL programs
extern std::vector<uint>         g_threadsPrograms;                  // the last MQL program executed by a thread


/**
 * Dump data from a buffer to the debugger output console.
 *
 * @param  char* fileName        - name of the file where the dump operation occurred
 * @param  char* funcName        - name of the function where the dump operation occurred
 * @param  int   line            - line number in the file where the dump operation occurred
 * @param  void* data            - pointer to the data to dump
 * @param  uint  size            - size of the data to dump in bytes
 * @param  uint  mode [optional] - mode controling the way of dumping (default: human-readable character dump)
 *                                 DUMPMODE_HEX:  output a hex dump
 *                                 DUMPMODE_CHAR: output a readable character representation
 * @return int - 0 (NULL)
 */
int _dump(const char* fileName, const char* funcName, int line, const void* data, uint size, uint mode/*=DUMPMODE_CHAR*/) {
   if ((uint)data < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter data: 0x%p (not a valid pointer)", data));
   if (size < 1)                       return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));

   char* bytes = (char*) data;
   std::stringstream ss;

   switch (mode) {
      case DUMPMODE_HEX:
         ss << std::hex << std::uppercase;
         for (uint i=0; i < size; i++) {
            ss << std::setw(2) << std::setfill('0') << (int) bytes[i] << " ";
            if (i%4 == 3)
               ss << " ";
         }
         break;

      case DUMPMODE_CHAR:
         for (uint i=0; i < size; i++) {
            char c = bytes[i];
            if      (c == 0) c = '…';           // substitute NUL characters
            else if (c < 33) c = '•';           // substitute CTRL characters
            ss << c;
         }
         break;

      default:
         return(error(ERR_INVALID_PARAMETER, "invalid parameter mode: %d (not a valid dump mode)", mode));
   }
   ss << std::dec << std::nouppercase << " (" << (int)size << " bytes)";

   _debug(fileName, funcName, line, "%s", ss.str().c_str());
   return(0);
}


/**
 * Print a C string to the debugger output console.
 *
 * @param  char* fileName - name of the file where the debug operation occurred
 * @param  char* funcName - name of the function where the debug operation occurred
 * @param  int   line     - line number in the file where the debug operation occurred
 *
 * @return int - 0 (NULL)
 */
int _debug(const char* fileName, const char* funcName, int line, const char* format, ...) {
   va_list args;
   va_start(args, format);
   __debug(fileName, funcName, line, format, args);
   va_end(args);
   return(0);
}


/**
 * Print a std::string to the debugger output console.
 *
 * @param  char* fileName - name of the file where the debug operation occurred
 * @param  char* funcName - name of the function where the debug operation occurred
 * @param  int   line     - line number in the file where the debug operation occurred
 *
 * @return int - 0 (NULL)
 */
int _debug(const char* fileName, const char* funcName, int line, const string& format, ...) {
   va_list args;
   va_start(args, format);
   __debug(fileName, funcName, line, format.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Print a formatted string to the debugger output console.
 *
 * @param  char*   fileName - name of the file where the debug operation occurred
 * @param  char*   funcName - name of the function where the debug operation occurred
 * @param  int     line     - line number in the file where the debug operation occurred
 * @param  char*   format   - string with codes to format additional parameters
 * @param  va_list args     - additional parameters
 */
void __debug(const char* fileName, const char* funcName, int line, const char* format, const va_list& args) {
   if (!format) format = "(null)";

   // format the parameters of the argument list
   uint size = _vscprintf(format, args) + 1;                                     // +1 for the terminating '\0'
   char* msg = (char*)alloca(size);                                              // on the stack
   vsprintf_s(msg, size, format, args);

   // get the simple file name: {basename.ext}
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // insert the call location at the beginning of the message
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;     // +1 for the terminating '\0'
   char* buffer = (char*)alloca(size);                                           // on the stack
   sprintf_s(buffer, size, locationFormat, baseName, ext, funcName, line, msg);

   // @see  http://www.unixwiz.net/techtips/outputdebugstring.html
   OutputDebugString(buffer);
}


/**
 * Process a C string warning.
 *
 * @return int - 0 (NULL)
 */
int _warn(const char* fileName, const char* funcName, int line, int code, const char* format, ...) {
   va_list args;
   va_start(args, format);
   __warn(fileName, funcName, line, code, format, args);
   va_end(args);
   return(0);
}


/**
 * Process a std::string warning.
 *
 * @return int - 0 (NULL)
 */
int _warn(const char* fileName, const char* funcName, int line, int error, const string& format, ...) {
   va_list args;
   va_start(args, format);
   __warn(fileName, funcName, line, error, format.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Send a formatted warning message to the debugger output. If the warning occurred during a call from MQL the warning is
 * stored in the program's EXECUTION_CONTEXT.
 *
 * @param  char*   fileName  - file name of the call
 * @param  char*   funcName  - function name of the call
 * @param  int     line      - line of the call
 * @param  int     code      - error code of the warning
 * @param  char*   msgFormat - message with format codes for additional parameters
 * @param  va_list msgArgs   - additional parameters
 */
void __warn(const char* fileName, const char* funcName, int line, int code, const char* msgFormat, const va_list& msgArgs) {
   if (!msgFormat) msgFormat = "(null)";

   // create message with the specified parameters
   uint size = _vscprintf(msgFormat, msgArgs) + 1;                                     // +1 for the terminating '\0'
   char* msg = (char*)alloca(size);                                                    // on the stack
   vsprintf_s(msg, size, msgFormat, msgArgs);

   // get the simple file name: {basename.ext}
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // insert the call location at the beginning of the message
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  WARN: %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;           // +1 for the terminating '\0'
   char* locationMsg = (char*)alloca(size);                                            // on the stack
   sprintf_s(locationMsg, size, locationFormat, baseName, ext, funcName, line, msg);

   char* fullMsg;

   // add error details (if any)
   if (code) {
      char* errorFormat  = "%s  [%s]";
      const char* sError = ErrorToStr(code);
      size = _scprintf(errorFormat, locationMsg, sError) + 1;                          // +1 for the terminating '\0'
      fullMsg = (char*)alloca(size);                                                   // on the stack
      sprintf_s(fullMsg, size, errorFormat, locationMsg, sError);
   }
   else {
      fullMsg = locationMsg;
   }

   BOOL inMqlCall = FALSE;
   if (inMqlCall) {
      //ec_SetDllWarning   (ec, error  );
      //ec_SetDllWarningMsg(ec, fullMsg);
   }
   OutputDebugString(fullMsg);
}


/**
 * Process a C string error.
 *
 * @return int - 0 (NULL)
 */
int _error(const char* fileName, const char* funcName, int line, int code, const char* format, ...) {
   va_list args;
   va_start(args, format);
   __error(fileName, funcName, line, code, format, args);
   va_end(args);
   return(0);
}


/**
 * Process a std::string error.
 *
 * @return int - 0 (NULL)
 */
int _error(const char* fileName, const char* funcName, int line, int code, const string& format, ...) {
   va_list args;
   va_start(args, format);
   __error(fileName, funcName, line, code, format.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Send a formatted error message to the debugger output. If the error occurred during a call from MQL the error is stored in
 * the program's EXECUTION_CONTEXT.
 *
 * @param  char*   fileName  - file name of the call
 * @param  char*   funcName  - function name of the call
 * @param  int     line      - line of the call
 * @param  int     code      - error code
 * @param  char*   msgFormat - message with format codes for additional parameters
 * @param  va_list msgArgs   - additional parameters
 */
void __error(const char* fileName, const char* funcName, int line, int code, const char* msgFormat, const va_list& msgArgs) {
   if (!code) return;
   if (!msgFormat) msgFormat = "(null)";

   // create message with the specified parameters
   int size = _vscprintf(msgFormat, msgArgs) + 1;                                      // +1 for the terminating '\0'
   char* msg = (char*)alloca(size);                                                    // on the stack
   vsprintf_s(msg, size, msgFormat, msgArgs);

   // get the simple file name: {basename.ext}
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // insert the call location at the beginning of the message
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  ERROR: %s  [%s]";
   const char* sError   = ErrorToStr(code);
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg, sError) + 1;   // +1 for the terminating '\0'
   char* fullMsg = (char*) alloca(size);                                               // on the stack
   sprintf_s(fullMsg, size, locationFormat, baseName, ext, funcName, line, msg, sError);

   // look-up the current thread's last associated MQL program
   DWORD currentThread = GetCurrentThreadId();
   uint currentThreadIndex=-1, currentThreadLastProgramIndex=0;
   size = g_threads.size();
   for (int i=0; i < size; i++) {
      if (g_threads[i] == currentThread) {                           // thread found
         currentThreadIndex            = i;                          // keep thread index and last MQL program
         currentThreadLastProgramIndex = g_threadsPrograms[i];
         break;
      }
   }

   // We only store an error in the EXECUTION_CONTEXT if this context is the current or last context processed by the DLL.
   BOOL contextAccessible = FALSE;

   if (currentThreadIndex == -1) {
      // thread unknown/not found: Not much we can do. We could just have entered SyncMainContext_init() or we could be in a
      //                           function callback called from a non-MQL thread (if that's possible).
   }
   else {
      // the current thread executed MQL before
      if (IsUIThread()) {
         // We are in a function callback executed in the UI thread or in a MQL indicator (or one of it's submodules).
         // If in a function callback the main context of the indicator may be invalid (during init cycle).
         //contextAccessible = TRUE;                       // let's see if/when this crashes
      }
      else {
         // We are in an expert or a script (or one of it's submodules) with read/write access to the main context.
         //contextAccessible = TRUE;
      }

      if (contextAccessible) {
         if (g_contextChains.size() > currentThreadLastProgramIndex && g_contextChains[currentThreadLastProgramIndex][0]) {
            EXECUTION_CONTEXT* ec = g_contextChains[currentThreadLastProgramIndex][0];
            ec_SetDllError(ec, code);
            //ec_SetDllErrorMsg(ec, fullMsg);
         }
      }
   }
   OutputDebugString(fullMsg);
}


/**
 * Helper functions returning fixed values. All parameters are ignored.
 */
int         WINAPI _CLR_NONE    (...) { return(CLR_NONE    ); }
int         WINAPI _EMPTY       (...) { return(EMPTY       ); }
const char* WINAPI _EMPTY_STR   (...) { return(""          ); }
HWND        WINAPI _INVALID_HWND(...) { return(INVALID_HWND); }
int         WINAPI _NULL        (...) { return(NULL        ); }
bool        WINAPI _true        (...) { return(true        ); }
BOOL        WINAPI _TRUE        (...) { return(TRUE        ); }
bool        WINAPI _false       (...) { return(false       ); }
BOOL        WINAPI _FALSE       (...) { return(FALSE       ); }


/**
 * Helper functions returning variable values. All parameters except the first one are ignored.
 */
bool        WINAPI _bool        (bool   value, ...) { return(value); }
BOOL        WINAPI _BOOL        (BOOL   value, ...) { return(value); }
char        WINAPI _char        (char   value, ...) { return(value); }
int         WINAPI _int         (int    value, ...) { return(value); }
float       WINAPI _float       (float  value, ...) { return(value); }
double      WINAPI _double      (double value, ...) { return(value); }
