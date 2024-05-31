#include "expander.h"
#include "lib/conversion.h"
#include "lib/executioncontext.h"
#include "lib/helper.h"
#include "lib/string.h"


extern MqlInstanceList g_mqlInstances;       // all MQL program instances

extern BOOL g_debugAccountNumber;            // whether cmd line option /rsf:debug-accountnumber is set
extern BOOL g_debugAccountServer;            // whether cmd line option /rsf:debug-accountserver is set
extern BOOL g_debugExecutionContext;         // whether cmd line option /rsf:debug-ec is set
extern BOOL g_debugObjectCreate;             // whether cmd line option /rsf:debug-objectcreate is set


/**
 * Whether cmd line option "/rsf:debug-accountnumber" is set.
 *
 * @return BOOL
 */
BOOL WINAPI IsDebugAccountNumber() {
   return(g_debugAccountNumber);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether cmd line option "/rsf:debug-accountserver" is set.
 *
 * @return BOOL
 */
BOOL WINAPI IsDebugAccountServer() {
   return(g_debugAccountServer);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether cmd line option "/rsf:debug-ec" is set.
 *
 * @return BOOL
 */
BOOL WINAPI IsDebugEC() {
   return(g_debugExecutionContext);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether cmd line option "/rsf:debug-objectcreate" is set.
 *
 * @return BOOL
 */
BOOL WINAPI IsDebugObjectCreate() {
   return(g_debugObjectCreate);
   #pragma EXPANDER_EXPORT
}


/**
 * Dump data from a buffer to the debugger output.
 *
 * @param  char* fileName        - file name of the call
 * @param  char* funcName        - function name of the call
 * @param  int   line            - line number of the call
 * @param  void* data            - pointer to the data to dump
 * @param  uint  size            - size of the data to dump in bytes
 * @param  uint  mode [optional] - mode controling the output format (default: hex dump)
 *                                 DUMPMODE_HEX:  output a hex dump
 *                                 DUMPMODE_CHAR: output a readable character representation
 * @return int - 0 (NULL)
 */
int __cdecl _dump(const char* fileName, const char* funcName, int line, const void* data, uint size, uint mode/*=DUMPMODE_HEX*/) {
   if ((uint)data < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter data: 0x%p (not a valid pointer)", data));
   if (size < 1)                       return(!error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));

   char* bytes = (char*) data;
   std::ostringstream ss;

   switch (mode) {
      case DUMPMODE_HEX:
         ss << "dumping " << (int)size << " byte:" << NL;
         ss << std::hex << std::uppercase;
         for (uint i=0; i < size; i++) {
            ss << std::setw(2) << std::setfill('0') << (int)bytes[i] << " ";
            if      (i%16 == 15) ss << NL;      // a linebreak every 16 byte
            else if (i% 4 ==  3) ss << " ";     // a space separator every 4 byte
         }
         break;

      case DUMPMODE_CHAR:
         ss << "dumping " << (int)size << " chars:" << NL;
         for (uint i=0; i < size; i++) {
            char c = bytes[i];
            if      (c == 0) c = '…';           // substitute NUL characters
            else if (c < 33) c = '•';           // substitute CTRL characters
            ss << c;
         }
         break;

      default:
         return(!error(ERR_INVALID_PARAMETER, "invalid parameter mode: %d (not a valid dump mode)", mode));
   }

   _debug(fileName, funcName, line, "%s", ss.str().c_str());
   return(0);
}


/**
 * Print a C string to the debugger output console.
 *
 * @param  char* fileName - name of the file where the debug operation occurred
 * @param  char* funcName - name of the function where the debug operation occurred
 * @param  int   line     - line number in the file where the debug operation occurred
 * @param  char* message  - string with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - 0 (NULL)
 */
int __cdecl _debug(const char* fileName, const char* funcName, int line, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME], ext[MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander::%s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   OutputDebugStringA(fullMsg);                    // see limitations at http://www.unixwiz.net/techtips/outputdebugstring.html
   free(fullMsg);
   return(NULL);
}


/**
 * Process a notice message.
 *
 * @param  char* fileName   - file name of the call
 * @param  char* funcName   - function name of the call
 * @param  int   line       - line of the call
 * @param  int   error_code - error code of the notice
 * @param  char* message    - message with format codes for additional parameters
 * @param        ...        - variable number of additional parameters
 *
 * @return int - the passed 'error_code'
 */
int __cdecl _notice(const char* fileName, const char* funcName, int line, int error_code, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME], ext[MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander::%s%s::%s(%d)  NOTICE: %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // add the error code at the end (if any)
   if (error_code) {
      char* newMsg = asformat("%s  [%s]", fullMsg, ErrorToStrA(error_code));
      free(fullMsg);
      fullMsg = newMsg;
   }
   OutputDebugStringA(fullMsg);
   free(fullMsg);

   return(error_code);
}


/**
 * Process a warning message.
 *
 * @param  char* fileName   - file name of the call
 * @param  char* funcName   - function name of the call
 * @param  int   line       - line of the call
 * @param  int   error_code - error code of the warning
 * @param  char* message    - message with format codes for additional parameters
 * @param        ...        - variable number of additional parameters
 *
 * @return int - the passed 'error_code'
 */
int __cdecl _warn(const char* fileName, const char* funcName, int line, int error_code, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME], ext[MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander::%s%s::%s(%d)  WARN: %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // add the error code at the end (if any)
   if (error_code) {
      char* newMsg = asformat("%s  [%s]", fullMsg, ErrorToStrA(error_code));
      free(fullMsg);
      fullMsg = newMsg;
   }
   OutputDebugStringA(fullMsg);
   free(fullMsg);

   // store the warning in the EXECUTION_CONTEXT of the currently executed MQL program
   if (uint pid = GetLastThreadProgram()) {
      ContextChain &chain = *g_mqlInstances[pid];
      uint size = chain.size();
      if (size && chain[0]) {                                        // master context (if available)
         chain[0]->dllWarning = error_code;
         //ec_SetDllWarningMsg(ec, formattedMsg);
      }
      if (size > 1 && chain[1]) {                                    // main context (if available)
         chain[1]->dllWarning = error_code;
         //ec_SetDllWarningMsg(ec, formattedMsg);
      }
   }
   return(error_code);
}


/**
 * Process an error status and a message. Eithout an error (NO_ERROR) the function does nothing.
 *
 * @param  char* fileName   - file name of the call
 * @param  char* funcName   - function name of the call
 * @param  int   line       - line of the call
 * @param  int   error_code - error code
 * @param  char* message    - message with format codes for additional parameters
 * @param        ...        - variable number of additional parameters
 *
 * @return int - the passed 'error_code'
 */
int __cdecl _error(const char* fileName, const char* funcName, int line, int error_code, const char* message, ...) {
   if (error_code) {
      const char* msg = message;
      if (!msg)  message = "(null)";
      if (!*msg) message = "(empty)";

      // format the variable parameters
      va_list args;
      va_start(args, message);
      char* formattedMsg = _asformat(msg, args);
      va_end(args);

      // insert the call location at the beginning: {basename.ext(line)}
      char baseName[MAX_FNAME], ext[MAX_EXT];
      if (!fileName) baseName[0] = ext[0] = '\0';
      else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
      char* fullMsg = asformat("MT4Expander::%s%s::%s(%d)  ERROR: %s  [%s]", baseName, ext, funcName, line, formattedMsg, ErrorToStrA(error_code));
      free(formattedMsg);

      OutputDebugStringA(fullMsg);
      free(fullMsg);

      // store the error in the EXECUTION_CONTEXT of the currently executed MQL program
      if (uint pid = GetLastThreadProgram()) {
         ContextChain &chain = *g_mqlInstances[pid];
         uint size = chain.size();
         if (size && chain[0]) {                                     // master context (if available)
            chain[0]->dllError = error_code;
            //ec_SetDllErrorMsg(ec, formattedMsg);
         }
         if (size > 1 && chain[1]) {                                 // main context (if available)
            chain[1]->dllError = error_code;
            //ec_SetDllErrorMsg(ec, formattedMsg);
         }
      }
   }
   return(error_code);
}
