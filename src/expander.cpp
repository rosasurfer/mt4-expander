#include "expander.h"
#include "lib/conversion.h"
#include "lib/executioncontext.h"
#include "lib/string.h"

extern MqlInstanceList g_mqlInstances;       // all MQL program instances


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
   if ((uint)data < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter data: 0x%p (not a valid pointer)", data);
   if (size < 1)                       return !error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size);

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
         return !error(ERR_INVALID_PARAMETER, "invalid parameter mode: %d (not a valid dump mode)", mode);
   }

   _debug(fileName, funcName, line, "%s", ss.str().c_str());
   return 0;
}


/**
 * Print a message to the system debugger.
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
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander        %s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);
   return NO_ERROR;
}


/**
 * Process a logmessage of level L_DEBUG.
 *
 * @param  char* fileName - file name of the call
 * @param  char* funcName - function name of the call
 * @param  int   line     - line of the call
 * @param  int   error    - error code of the message
 * @param  char* message  - message with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - the passed error code
 */
int __cdecl _debug(const char* fileName, const char* funcName, int line, int error, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander        %s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // add the error code at the end (if any)
   if (error) {
      char* newMsg = asformat("%s  [%s]", fullMsg, ErrorToStrA(error));
      free(fullMsg);
      fullMsg = newMsg;
   }

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);
   return error;
}


/**
 * Process a logmessage of level L_INFO.
 *
 * @param  char* fileName - file name of the call
 * @param  char* funcName - function name of the call
 * @param  int   line     - line of the call
 * @param  char* message  - message with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - the passed error code
 */
int __cdecl _info(const char* fileName, const char* funcName, int line, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander INFO   %s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);
   return NO_ERROR;
}


/**
 * Process a logmessage of level L_INFO.
 *
 * @param  char* fileName - file name of the call
 * @param  char* funcName - function name of the call
 * @param  int   line     - line of the call
 * @param  int   error    - error code of the message
 * @param  char* message  - message with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - the passed error code
 */
int __cdecl _info(const char* fileName, const char* funcName, int line, int error, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander INFO   %s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // add the error code at the end (if any)
   if (error) {
      char* newMsg = asformat("%s  [%s]", fullMsg, ErrorToStrA(error));
      free(fullMsg);
      fullMsg = newMsg;
   }

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);
   return error;
}


/**
 * Process a logmessage of level L_NOTICE.
 *
 * @param  char* fileName - file name of the call
 * @param  char* funcName - function name of the call
 * @param  int   line     - line of the call
 * @param  char* message  - message with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - the passed error code
 */
int __cdecl _notice(const char* fileName, const char* funcName, int line, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander NOTICE %s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);
   return NO_ERROR;
}


/**
 * Process a logmessage of level L_NOTICE.
 *
 * @param  char* fileName - file name of the call
 * @param  char* funcName - function name of the call
 * @param  int   line     - line of the call
 * @param  int   error    - error code of the message
 * @param  char* message  - message with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - the passed error code
 */
int __cdecl _notice(const char* fileName, const char* funcName, int line, int error, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander NOTICE %s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // add the error code at the end (if any)
   if (error) {
      char* newMsg = asformat("%s  [%s]", fullMsg, ErrorToStrA(error));
      free(fullMsg);
      fullMsg = newMsg;
   }

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);
   return error;
}


/**
 * Process a logmessage of level L_WARN.
 *
 * @param  char* fileName - file name of the call
 * @param  char* funcName - function name of the call
 * @param  int   line     - line of the call
 * @param  char* message  - message with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - the passed error code
 */
int __cdecl _warn(const char* fileName, const char* funcName, int line, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander WARN   %s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);

   // store the warning in the EXECUTION_CONTEXT of the currently executed MQL program
   if (uint pid = GetLastThreadProgram()) {
      ContextChain &chain = *g_mqlInstances[pid];
      uint size = chain.size();
      if (size && chain[0]) {                                        // master context (if available)
         chain[0]->dllWarning = NO_ERROR;
         //ec_SetDllWarningMsg(ec, formattedMsg);
      }
      if (size > 1 && chain[1]) {                                    // main context (if available)
         chain[1]->dllWarning = NO_ERROR;
         //ec_SetDllWarningMsg(ec, formattedMsg);
      }
   }
   return NO_ERROR;
}


/**
 * Process a logmessage of level L_WARN.
 *
 * @param  char* fileName - file name of the call
 * @param  char* funcName - function name of the call
 * @param  int   line     - line of the call
 * @param  int   errore   - error code of the message
 * @param  char* message  - message with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - the passed error code
 */
int __cdecl _warn(const char* fileName, const char* funcName, int line, int error, const char* message, ...) {
   const char* msg = message;
   if (!msg)  msg = "(null)";
   if (!*msg) msg = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander WARN   %s%s::%s(%d)  %s", baseName, ext, funcName, line, formattedMsg);
   free(formattedMsg);

   // add the error code at the end (if any)
   if (error) {
      char* newMsg = asformat("%s  [%s]", fullMsg, ErrorToStrA(error));
      free(fullMsg);
      fullMsg = newMsg;
   }

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);

   // store the warning in the EXECUTION_CONTEXT of the currently executed MQL program
   if (uint pid = GetLastThreadProgram()) {
      ContextChain &chain = *g_mqlInstances[pid];
      uint size = chain.size();
      if (size && chain[0]) {                                        // master context (if available)
         chain[0]->dllWarning = error;
         //ec_SetDllWarningMsg(ec, formattedMsg);
      }
      if (size > 1 && chain[1]) {                                    // main context (if available)
         chain[1]->dllWarning = error;
         //ec_SetDllWarningMsg(ec, formattedMsg);
      }
   }
   return error;
}


/**
 * Process a logmessage of level L_ERROR. Without an error code (no error) the function does nothing.
 *
 * @param  char* fileName - file name of the call
 * @param  char* funcName - function name of the call
 * @param  int   line     - line of the call
 * @param  int   error    - error code of the message
 * @param  char* message  - message with format codes for additional parameters
 * @param        ...      - variable number of additional parameters
 *
 * @return int - the passed error code
 */
int __cdecl _error(const char* fileName, const char* funcName, int line, int error, const char* message, ...) {
   if (!error) return 0;

   const char* msg = message;
   if (!msg)  message = "(null)";
   if (!*msg) message = "(empty)";

   // format the variable parameters
   va_list args;
   va_start(args, message);
   char* formattedMsg = _asformat(msg, args);
   va_end(args);

   // insert the call location at the beginning: {basename.ext(line)}
   char baseName[MAX_FNAME] = {}, ext[MAX_EXT] = {};
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, MAX_FNAME, ext, MAX_EXT);
   char* fullMsg = asformat("MT4Expander ERROR  %s%s::%s(%d)  %s  [%s]", baseName, ext, funcName, line, formattedMsg, ErrorToStrA(error));
   free(formattedMsg);

   // pass the message to the system debugger
   OutputDebugStringA(fullMsg);
   free(fullMsg);

   // store the error in the EXECUTION_CONTEXT of the currently executed MQL program
   if (uint pid = GetLastThreadProgram()) {
      ContextChain &chain = *g_mqlInstances[pid];
      uint size = chain.size();
      if (size && chain[0]) {                                     // master context (if available)
         chain[0]->dllError = error;
         //ec_SetDllErrorMsg(ec, formattedMsg);
      }
      if (size > 1 && chain[1]) {                                 // main context (if available)
         chain[1]->dllError = error;
         //ec_SetDllErrorMsg(ec, formattedMsg);
      }
   }
   return error;
}


// Helper functions returning constant values. All parameters are ignored.
int         __cdecl _EMPTY       (...) { return EMPTY;        }      // only __cdecl supports variadics
int         __cdecl _EMPTY_VALUE (...) { return EMPTY_VALUE;  }
const char* __cdecl _EMPTY_STR   (...) { return "";           }
HWND        __cdecl _INVALID_HWND(...) { return INVALID_HWND; }
int         __cdecl _NULL        (...) { return NULL;         }
bool        __cdecl _true        (...) { return true;         }
BOOL        __cdecl _TRUE        (...) { return TRUE;         }
bool        __cdecl _false       (...) { return false;        }
BOOL        __cdecl _FALSE       (...) { return FALSE;        }
color       __cdecl _CLR_NONE    (...) { return CLR_NONE;     }
color       __cdecl _NaC         (...) { return NaC;          }
time32      __cdecl _NaT32       (...) { return NaT;          }
time64      __cdecl _NaT64       (...) { return NaT;          }


// Helper functions returning variable values. All parameters except the first one are ignored.
bool   __cdecl _bool  (bool   value, ...) { return value; }          // only __cdecl supports variadics
BOOL   __cdecl _BOOL  (BOOL   value, ...) { return value; }
char   __cdecl _char  (char   value, ...) { return value; }
int    __cdecl _int   (int    value, ...) { return value; }
float  __cdecl _float (float  value, ...) { return value; }
double __cdecl _double(double value, ...) { return value; }


/**
 * A no-op for explicitly loading the Expander from MQL. If the MQL framework is used it will implicitly
 * load the Expander during MQL program initialization.
 */
void WINAPI LoadMT4Expander() {
   // nothing to do here
   #pragma EXPANDER_EXPORT
}
