#include "expander.h"
#include "util/helpers.h"
#include "util/toString.h"

#include <vector>


// external declarations for error management
extern std::vector<DWORD> g_threads;                                 // all known threads executing MQL programs
extern std::vector<uint>  g_threadsPrograms;                         // the last MQL program executed by a thread


/**
 * Process a C string debug message.
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
 * Process a std::string debug message.
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
 * Send a formatted debug message to the debugger output.
 *
 * @param  char*   fileName - file name of the call
 * @param  char*   funcName - function name of the call
 * @param  int     line     - line of the call
 * @param  char*   format   - message with format codes for additional parameters
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
   char* locationFormat = "Metatrader::%s%s::%s(%d)  %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;     // +1 for the terminating '\0'
   char* buffer = (char*)alloca(size);                                           // on the stack
   sprintf_s(buffer, size, locationFormat, baseName, ext, funcName, line, msg);

   // output
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
   char* locationFormat = "Metatrader::%s%s::%s(%d)  WARN: %s";
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
   char* locationFormat = "Metatrader::%s%s::%s(%d)  ERROR: %s  [%s]";
   const char* sError   = ErrorToStr(code);
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg, sError) + 1;   // +1 for the terminating '\0'
   char* fullMsg = (char*) alloca(size);                                               // on the stack
   sprintf_s(fullMsg, size, locationFormat, baseName, ext, funcName, line, msg, sError);

   // look-up the current thread's last associated MQL program
   DWORD currentThread = GetCurrentThreadId();
   int currentThreadIndex=-1, currentThreadLastProgram=0;
   size = g_threads.size();
   for (int i=0; i < size; i++) {
      if (g_threads[i] == currentThread) {                           // thread found
         currentThreadIndex       = i;                               // keep thread index and last MQL program
         currentThreadLastProgram = g_threadsPrograms[i];
         break;
      }
   }

   if (currentThreadIndex == -1) {
      // Thread unknown/not found: Not much we can do. We could just have entered SyncMainContext() or
      //                           we could be in a function callback called from a new thread (if that's possible).
   }
   else {
      // Thread executed MQL before
      if (IsUIThread()) {
         // Wir sind in einer Callback-Funktion oder im letzten im UI-Thread ausgeführten Indikator und dort u.U. in einer
         // Library oder einem via iCustom() geladenen weiteren Indikator.
         //
         // Vorsicht: Der Hauptkontext des letzten Root-Programms kann ungültig sein.
      }
      else {
         // Wir sind im Expert oder Script des Threads und dort u.U. in einer Library oder einem via iCustom() geladenen weiteren
         // Indikator. Auf den Hauptkontext des Root-Programms kann lesend/schreibend zugegriffen werden.
      }

      if (BOOL inMqlCall=FALSE) {
         //ec_SetDllError   (ec, error  );
         //ec_SetDllErrorMsg(ec, fullMsg);
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
bool        WINAPI _false       (...) { return(false       ); }
BOOL        WINAPI _TRUE        (...) { return(TRUE        ); }
BOOL        WINAPI _FALSE       (...) { return(FALSE       ); }


/**
 * Helper functions returning variable values. All parameters except the first one are ignored.
 */
bool        WINAPI _bool        (bool   value, ...) { return(value); }
char        WINAPI _char        (char   value, ...) { return(value); }
int         WINAPI _int         (int    value, ...) { return(value); }
float       WINAPI _float       (float  value, ...) { return(value); }
double      WINAPI _double      (double value, ...) { return(value); }
BOOL        WINAPI _BOOL        (BOOL   value, ...) { return(value); }


/*
HWND          WINAPI GetApplicationWindow();
uint          WINAPI GetBoolsAddress         (const BOOL values[]);
uint          WINAPI GetChartDescription     (const char* symbol, uint timeframe, char* buffer, uint bufferSize);
uint          WINAPI GetDoublesAddress       (const double values[]);
datetime      WINAPI GetGmtTime();
uint          WINAPI GetIntsAddress          (const int values[]);
int           WINAPI GetLastWin32Error();
datetime      WINAPI GetLocalTime();
uint          WINAPI GetTerminalBuild();
const string& WINAPI getTerminalPath();
const char*   WINAPI GetTerminalVersion();
BOOL          WINAPI GetTerminalVersion      (uint* major, uint* minor, uint* hotfix, uint* build);
DWORD         WINAPI GetUIThreadId();
HANDLE        WINAPI GetWindowProperty       (HWND hWnd, const char* lpName);
BOOL          WINAPI IsCustomTimeframe       (int timeframe);
BOOL          WINAPI IsStdTimeframe          (int timeframe);
BOOL          WINAPI IsUIThread();
uint          WINAPI MT4InternalMsg();
HANDLE        WINAPI RemoveWindowProperty    (HWND hWnd, const char* lpName);
BOOL          WINAPI SetWindowProperty       (HWND hWnd, const char* lpName, HANDLE value);
BOOL          WINAPI ShiftIndicatorBuffer    (double buffer[], int bufferSize, int bars, double emptyValue);
*/
