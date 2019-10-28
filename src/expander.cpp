#include "expander.h"
#include "lib/conversion.h"
#include "lib/executioncontext.h"
#include "lib/helper.h"
#include "lib/string.h"


extern MqlProgramList g_mqlPrograms;                     // all MQL programs: vector<ContextChain> with index = program id


/**
 * Dump data from a buffer to the debugger output console.
 *
 * @param  char* fileName        - name of the file where the dump operation occurred
 * @param  char* funcName        - name of the function where the dump operation occurred
 * @param  int   line            - line number in the file where the dump operation occurred
 * @param  void* data            - pointer to the data to dump
 * @param  uint  size            - size of the data to dump in bytes
 * @param  uint  mode [optional] - mode controling the way of dumping (default: hex dump)
 *                                 DUMPMODE_HEX:  output a hex dump
 *                                 DUMPMODE_CHAR: output a readable character representation
 * @return int - 0 (NULL)
 */
int WINAPI _dump(const char* fileName, const char* funcName, int line, const void* data, uint size, uint mode/*=DUMPMODE_HEX*/) {
   if ((uint)data < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter data: 0x%p (not a valid pointer)", data));
   if (size < 1)                       return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));

   char* bytes = (char*) data;
   std::stringstream ss;

   switch (mode) {
      case DUMPMODE_HEX:
         ss << "dumping " << (int)size << " bytes:" << NL;
         ss << std::hex << std::uppercase;
         for (uint i=0; i < size; i++) {
            ss << std::setw(2) << std::setfill('0') << (int)bytes[i] << " ";
            if      (i%16 == 15) ss << NL;
            else if (i% 4 ==  3) ss << " ";
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
         return(error(ERR_INVALID_PARAMETER, "invalid parameter mode: %d (not a valid dump mode)", mode));
   }

   _debug(fileName, funcName, line, "%s", ss.str().c_str());
   return(0);
}


/**
 * Print a string to the debugger output console.
 *
 * @param  char* fileName  - name of the file where the debug operation occurred
 * @param  char* funcName  - name of the function where the debug operation occurred
 * @param  int   line      - line number in the file where the debug operation occurred
 * @param  char* msgFormat - message string with format codes for additional parameters
 * @param        ...       - variable number of additional parameters
 *
 * @return int - 0 (NULL)
 */
int WINAPI _debug(const char* fileName, const char* funcName, int line, const char* msgFormat, ...) {
   if (!msgFormat)  msgFormat = "(null)";
   if (!*msgFormat) msgFormat = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, msgFormat);
   char* msg = strformat(msgFormat, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME], ext[MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = strformat("MT4Expander::%s%s::%s(%d)  %s", baseName, ext, funcName, line, msg);

   OutputDebugString(fullMsg);      // @see  limitations at http://www.unixwiz.net/techtips/outputdebugstring.html
   free(msg);
   free(fullMsg);
   return(NULL);
}


/**
 * Process a warning message.
 *
 * @param  char* fileName   - file name of the call
 * @param  char* funcName   - function name of the call
 * @param  int   line       - line of the call
 * @param  int   error_code - error code of the warning
 * @param  char* msgFormat  - message with format codes for additional parameters
 * @param        ...        - variable number of additional parameters
 */
int WINAPI _warn(const char* fileName, const char* funcName, int line, int error_code, const char* msgFormat, ...) {
   if (!msgFormat)  msgFormat = "(null)";
   if (!*msgFormat) msgFormat = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, msgFormat);
   char* msg = strformat(msgFormat, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME], ext[MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* newMsg = strformat("MT4Expander::%s%s::%s(%d)  WARN: %s", baseName, ext, funcName, line, msg);
   free(msg);
   msg = newMsg;

   // add the error code at the end (if any)
   if (error_code) {
      newMsg = strformat("%s  [%s]", msg, ErrorToStr(error_code));
      free(msg);
      msg = newMsg;
   }
   OutputDebugString(msg);
   free(msg);

   // store the warning in the EXECUTION_CONTEXT of the currently executed MQL program
   if (uint pid = GetLastThreadProgram()) {
      ContextChain& chain = *g_mqlPrograms[pid];
      uint size = chain.size();
      if (size && chain[0]) {                                        // master context (if available)
         chain[0]->dllWarning = error_code;
         //ec_SetDllWarningMsg(ec, msg);
      }
      if (size > 1 && chain[1]) {                                    // main context (if available)
         chain[1]->dllWarning = error_code;
         //ec_SetDllWarningMsg(ec, msg);
      }
   }
   return(NULL);
}


/**
 * Process an error message.
 *
 * @param  char* fileName   - file name of the call
 * @param  char* funcName   - function name of the call
 * @param  int   line       - line of the call
 * @param  int   error_code - error code
 * @param  char* msgFormat  - message with format codes for additional parameters
 * @param        ...        - variable number of additional parameters
 *
 * @return int - 0 (NULL)
 */
int WINAPI _error(const char* fileName, const char* funcName, int line, int error_code, const char* msgFormat, ...) {
   if (!error_code) return(NULL);
   if (!msgFormat)  msgFormat = "(null)";
   if (!*msgFormat) msgFormat = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, msgFormat);
   char* msg = strformat(msgFormat, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME], ext[MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = strformat("MT4Expander::%s%s::%s(%d)  ERROR: %s  [%s]", baseName, ext, funcName, line, msg, ErrorToStr(error_code));

   OutputDebugString(fullMsg);
   free(msg);
   free(fullMsg);

   // store the error in the EXECUTION_CONTEXT of the currently executed MQL program
   if (uint pid = GetLastThreadProgram()) {
      ContextChain& chain = *g_mqlPrograms[pid];
      uint size = chain.size();
      if (size && chain[0]) {                                        // master context (if available)
         chain[0]->dllError = error_code;
         //ec_SetDllErrorMsg(ec, msg);
      }
      if (size > 1 && chain[1]) {                                    // main context (if available)
         chain[1]->dllError = error_code;
         //ec_SetDllErrorMsg(ec, msg);
      }
   }
   return(NULL);
}
