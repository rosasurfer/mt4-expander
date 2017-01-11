#include "expander.h"

//#include <string>
//#include <iostream>
//#include <fstream>
//#include <cstdlib>


/**
 * Process a C string debug message.
 *
 * @return int - 0 (zero)
 */
int _debug(const char* fileName, const char* funcName, int line, const char* format, ...) {
   va_list args;
   va_start(args, format);
   __debug(fileName, funcName, line, format, args);
   va_end(args);
   return(0);
}


/**
 * Process a C++ string debug message.
 *
 * @return int - 0 (zero)
 */
int _debug(const char* fileName, const char* funcName, int line, const string &format, ...) {
   va_list args;
   va_start(args, format);
   __debug(fileName, funcName, line, format.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Send a formatted string to the debugger output.
 *
 * @param  char*   fileName - Name der Datei, in der der Aufruf erfolgt
 * @param  char*   funcName - Name der Funktion, in der der Aufruf erfolgt
 * @param  int     line     - Zeile, in der der Aufruf erfolgt
 * @param  char*   format   - Formatstring mit Platzhaltern für alle weiteren Parameter
 * @param  va_list args     - weitere Parameter
 */
void __debug(const char* fileName, const char* funcName, int line, const char* format, const va_list &args) {
   if (!format) format = "(null)";

   // (1) zuerst alle explizit angegebenen Argumente in einen String transformieren (ab format)
   size_t size = _vscprintf(format, args) + 1;                                   // +1 für das terminierende '\0'
   char* msg = (char*) alloca(size);                                             // auf dem Stack
   vsprintf_s(msg, size, format, args);

   // Parameter fileName zerlegen: nur der einfache Dateiname {basename.ext} wird benötigt
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // (2) dann die impliziten Location-Infos vorn anfügen
   char* locationFormat = "Metatrader::%s%s::%s(%d)  %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;     // +1 für das terminierende '\0'
   char* buffer = (char*) alloca(size);                                          // auf dem Stack
   sprintf_s(buffer, size, locationFormat, baseName, ext, funcName, line, msg);

   OutputDebugString(buffer);
}


/**
 * Process a DLL warning with a C string log message.
 *
 * @return int - 0 (zero)
 */
int _warn(const char* fileName, const char* funcName, int line, int error, const char* format, ...) {
   va_list args;
   va_start(args, format);
   __warn(fileName, funcName, line, error, format, args);
   va_end(args);
   return(0);
}


/**
 * Process a DLL warning with a C++ string log message.
 *
 * @return int - 0 (zero)
 */
int _warn(const char* fileName, const char* funcName, int line, int error, const string &format, ...) {
   va_list args;
   va_start(args, format);
   __warn(fileName, funcName, line, error, format.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Process a DLL warning and send it to OutputDebugString(). If the warning occurred during a call from MQL the error
 * is stored in the program's EXECUTION_CONTEXT. Must be called via the <tt>warn(...)</tt> macro.
 *
 * @param  char*   fileName  - file where the macro was called
 * @param  char*   funcName  - function where the macro was called
 * @param  int     line      - line where the macro was called
 * @param  int     error     - error code
 * @param  char*   msgFormat - message with format codes for further parameters
 * @param  va_list msgArgs   - further message parameters
 */
void __warn(const char* fileName, const char* funcName, int line, int error, const char* msgFormat, const va_list &msgArgs) {
   if (!msgFormat) msgFormat = "(null)";

   // create message with the specified parameters
   size_t size = _vscprintf(msgFormat, msgArgs) + 1;                                   // +1 for the terminating '\0'
   char* msg = (char*) alloca(size);                                                   // on the stack
   vsprintf_s(msg, size, msgFormat, msgArgs);

   // extract {baseName}.{ext} from fileName
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // add location infos
   char* locationFormat = "Metatrader::%s%s::%s(%d)  WARN: %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;           // +1 for the terminating '\0'
   char* locationMsg = (char*) alloca(size);                                           // on the stack
   sprintf_s(locationMsg, size, locationFormat, baseName, ext, funcName, line, msg);

   char* fullMsg;

   // add error details (if any)
   if (error) {
      char* errorFormat  = "%s  [%s]";
      const char* sError = ErrorToStr(error);
      size = _scprintf(errorFormat, locationMsg, sError) + 1;                          // +1 for the terminating '\0'
      fullMsg = (char*) alloca(size);                                                  // on the stack
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
 * Process a DLL error with a C string log message.
 *
 * @return int - 0 (zero)
 */
int _error(const char* fileName, const char* funcName, int line, int error, const char* format, ...) {
   va_list args;
   va_start(args, format);
   __error(fileName, funcName, line, error, format, args);
   va_end(args);
   return(0);
}


/**
 * Process a DLL error with a C++ string log message.
 *
 * @return int - 0 (zero)
 */
int _error(const char* fileName, const char* funcName, int line, int error, const string &format, ...) {
   va_list args;
   va_start(args, format);
   __error(fileName, funcName, line, error, format.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Process a DLL error and send it to OutputDebugString(). If the error occurred during a call from MQL the error
 * is stored in the program's EXECUTION_CONTEXT. Must be called via the <tt>error(...)</tt> macro.
 *
 * @param  char*   fileName  - file where the macro was called
 * @param  char*   funcName  - function where the macro was called
 * @param  int     line      - line where the macro was called
 * @param  int     error     - error code
 * @param  char*   msgFormat - message with format codes for further parameters
 * @param  va_list msgArgs   - further message parameters
 */
void __error(const char* fileName, const char* funcName, int line, int error, const char* msgFormat, const va_list &msgArgs) {
   if (!error) return;
   if (!msgFormat) msgFormat = "(null)";

   // create message with the specified parameters
   size_t size = _vscprintf(msgFormat, msgArgs) + 1;                                   // +1 for the terminating '\0'
   char* msg = (char*) alloca(size);                                                   // on the stack
   vsprintf_s(msg, size, msgFormat, msgArgs);

   // extract {baseName}.{ext} from fileName
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // add location infos and error details
   char* locationFormat = "Metatrader::%s%s::%s(%d)  ERROR: %s  [%s]";
   const char* sError   = ErrorToStr(error);
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg, sError) + 1;   // +1 for the terminating '\0'
   char* fullMsg = (char*) alloca(size);                                               // on the stack
   sprintf_s(fullMsg, size, locationFormat, baseName, ext, funcName, line, msg, sError);

   // look-up the current thread's last associated MQL program
   DWORD currentThread = GetCurrentThreadId();
   int currentThreadIndex=-1, currentThreadLastProgram=0;
   size = threads.size();
   for (uint i=0; i < size; i++) {
      if (threads[i] == currentThread) {                             // thread found
         currentThreadIndex       = i;                               // keep thread index and last MQL program
         currentThreadLastProgram = threadsPrograms[i];
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
 * Pseudo-Funktionen, die ihrem Namen entsprechende feste Werte zurückzugeben.
 *
 * @param  ... all parameters are ignored
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
 * Pseudo-Funktionen, die ihrem Namen entsprechende variable Werte zurückzugeben.
 * Außer dem ersten werden alle übergebenen Parameter ignoriert.
 */
bool   WINAPI _bool  (bool   value, ...) { return(value); }
char   WINAPI _char  (char   value, ...) { return(value); }
int    WINAPI _int   (int    value, ...) { return(value); }
float  WINAPI _float (float  value, ...) { return(value); }
double WINAPI _double(double value, ...) { return(value); }
BOOL   WINAPI _BOOL  (BOOL   value, ...) { return(value); }


/**
 *
 */
std::istream& getLine(std::istream &is, string &line) {
   // The characters in the stream are read one-by-one using std::streambuf. This is faster than reading them one-by-one using
   // std::istream. Code that uses streambuf this way must be guarded by a sentry object. The sentry object performs various
   // tasks, such as thread synchronization and updating the stream state.
   //
   // @see  http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf/6089413#6089413

   // CR     = 0D     = 13       = \r       Mac
   // LF     = 0A     = 10       = \n       Linux
   // CRLF   = 0D0A   = 13,10    = \r\n     Windows
   // CRCRLF = 0D0D0A = 13,13,10 = \r\r\n   Netscape, Windows XP Notepad bug (not yet tested)

   std::istream::sentry se(is, true);
   std::streambuf* sb = is.rdbuf();
   line.clear();

   for (;;) {
      int ch = sb->sbumpc();
      switch (ch) {
         case '\n':
            goto endloop;

         case '\r':
            if (sb->sgetc() == '\n')
               sb->sbumpc();
            goto endloop;

         case EOF:
            // handle the case when the last line has no line ending
            if (line.empty())
               is.setstate(std::ios::eofbit);
            goto endloop;

         default:
            line += (char)ch;
      }
   }
   endloop:
   return(is);

   /*
   string   fileName = GetTerminalPath() +"\\tester\\"+ ec->programName +".ini";
   std::ifstream fs(fileName.c_str());
   if (!fs) return(error(ERR_FILE_CANNOT_OPEN, "cannot open file %s", DoubleQuoteStr(fileName.c_str())));

   string line;
   uint n = 0;

   debug("reading file %s...", DoubleQuoteStr(fileName.c_str()));
   while (!getLine(fs, line).eof()) {
      ++n;
      debug("line %d: %s (%d)", n, line.c_str(), line.length());
   }
   fs.close();
   debug("file contains %d line(s)", n);
   */
}


/**
 *
 */
int WINAPI Test() {

   typedef std::vector<int> IntVector;
   IntVector ints(1);

   ints.push_back(1);
   ints.push_back(1);
   ints.push_back(1);

   IntVector::iterator it = ints.begin();
   ints.erase(ints.begin() + 1);

   debug("capacity(ints)=%d  size(ints)=%d", ints.capacity(), ints.size());
   return(0);

   string str("Hello world");
   int len = str.length();
   char* test = new char[len+1];
   str.copy(test, len);
   test[len] = '\0';
   delete[] test;
   return(0);

   debug("sizeof(EXECUTION_CONTEXT) = %d", sizeof(EXECUTION_CONTEXT));
   /*
   debug("error.code=%d  error.message=%s", error->code, error->message);
   error->code    = 200;
   char* msg      = "200: OK";
   int   bufSize  = strlen(msg)+1;
   char* buffer   = new char[bufSize];
   strcpy_s(buffer, bufSize, msg);
   error->message = buffer;
   */

   /*
   auto_ptr<char> p(new char(10));
   int len = strlen(p.get());
   */
   return(0);
   //#pragma EXPORT
}
