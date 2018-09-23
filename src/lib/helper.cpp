#include "expander.h"
#include "lib/string.h"

extern "C" {
#include "etc/md5.h"
}


/**
 * Gibt die Speicheradresse eines MQL-Bool-Arrays zurück.
 *
 * @param  BOOL values[] - MQL-Bool-Array (in MetaTrader als Integer-Array implementiert)
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 */
uint WINAPI GetBoolsAddress(const BOOL values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zurück.
 *
 * @param  int values[] - MQL-Integer-Array
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 */
uint WINAPI GetIntsAddress(const int values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zurück.
 *
 * @param  double values[] - MQL-Double-Array
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 */
uint WINAPI GetDoublesAddress(const double values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den letzten aufgetretenen Windows-Fehler des aktuellen Threads zurück. Wrapper für kernel32::GetLastError(),
 * da MQL eine Funktion desselben Namens definiert.
 *
 * @return int - Fehlercode
 */
int WINAPI GetLastWin32Error() {
   return(GetLastError());
   #pragma EXPANDER_EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen MetaTrader-Standard-Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 */
BOOL WINAPI IsStdTimeframe(int timeframe) {
   switch (timeframe) {
      case PERIOD_M1 : return(TRUE);
      case PERIOD_M5 : return(TRUE);
      case PERIOD_M15: return(TRUE);
      case PERIOD_M30: return(TRUE);
      case PERIOD_H1 : return(TRUE);
      case PERIOD_H4 : return(TRUE);
      case PERIOD_D1 : return(TRUE);
      case PERIOD_W1 : return(TRUE);
      case PERIOD_MN1: return(TRUE);
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen benutzerdefinierten Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 */
BOOL WINAPI IsCustomTimeframe(int timeframe) {
   if (timeframe <= 0)
      return(FALSE);
   return(!IsStdTimeframe(timeframe));
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das Handle des Terminal-Hauptfensters zurück.
 *
 * @return HWND - Handle oder 0, falls ein Fehler auftrat
 */
HWND WINAPI GetApplicationWindow() {
   static HWND hWnd;

   if (!hWnd) {
      HWND  hWndNext = GetTopWindow(NULL);
      DWORD processId, myProcessId=GetCurrentProcessId();

      // alle Top-Level-Windows durchlaufen
      while (hWndNext) {
         GetWindowThreadProcessId(hWndNext, &processId);
         if (processId == myProcessId) {

            // ClassName des Fensters ermitteln
            int   size = 255;
            char* className = (char*) alloca(size);            // auf dem Stack
            int   copied = GetClassName(hWndNext, className, size);
            if (!copied) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetClassName() 0 chars copied"));

            while (copied >= size-1) {                         // GetClassName() gibt die Anzahl der kopierten Zeichen zurück
               size <<= 1;                                     // (ohne \0). Bei size-1 ist unklar, ob der String genau in den
               className = (char*) alloca(size);               // Buffer paßte oder nicht.
               copied    = GetClassName(hWndNext, className, size);
            }
            if (!copied) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetClassName() 0 chars copied"));

            // Klasse mit der Klasse des Terminal-Hauptfensters vergleichen
            if (strcmp(className, "MetaQuotes::MetaTrader::4.00") == 0)
               break;
         }
         hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
      }
      if (!hWndNext) error(ERR_RUNTIME_ERROR, "cannot find application main window");
      hWnd = hWndNext;
   }

   return(hWnd);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die ID des Userinterface-Threads zurück.
 *
 * @return DWORD - Thread-ID (nicht das Thread-Handle) oder 0, falls ein Fehler auftrat
 */
DWORD WINAPI GetUIThreadId() {
   static DWORD uiThreadId;

   if (!uiThreadId) {
      HWND hWnd = GetApplicationWindow();
      if (hWnd)
         uiThreadId = GetWindowThreadProcessId(hWnd, NULL);
   }
   return(uiThreadId);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob der aktuelle Thread der UI-Thread ist.
 *
 * @return BOOL
 */
BOOL WINAPI IsUIThread() {
   return(GetCurrentThreadId() == GetUIThreadId());
   #pragma EXPANDER_EXPORT
}


/**
 * Wrapper für die Win32-API-Funktion GetProp(). Gibt den Wert einer Window-Property zurück.
 *
 * @param  HWND  hWnd   - Fensterhandle
 * @param  char* lpName - Property-Name
 *
 * @return HANDLE - Property-Value
 */
HANDLE WINAPI GetWindowProperty(HWND hWnd, const char* lpName) {
   return(GetProp(hWnd, lpName));
   #pragma EXPANDER_EXPORT
}


/**
 * Wrapper für die Win32-API-Funktion RemoveProp(). Gibt den Wert einer Window-Property zurück und löscht sie.
 *
 * @param  HWND  hWnd   - Fensterhandle
 * @param  char* lpName - Property-Name
 *
 * @return HANDLE - Property-Value
 */
HANDLE WINAPI RemoveWindowProperty(HWND hWnd, const char* lpName) {
   return(RemoveProp(hWnd, lpName));
   #pragma EXPANDER_EXPORT
}


/**
 * Wrapper für die Win32-API-Funktion SetProp(). Setzt eine Window-Property.
 *
 * @param  HWND   hWnd   - Fensterhandle
 * @param  char*  lpName - Property-Name
 * @param  HANDLE value  - Property-Value
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SetWindowProperty(HWND hWnd, const char* lpName, HANDLE value) {
   return(SetProp(hWnd, lpName, value));
   #pragma EXPANDER_EXPORT
}


/**
 * Shifted die Werte eines IndicatorBuffers um eine Anzahl von Bars nach hinten. Die ältesten Werte verfallen.
 *
 * @param  double buffer[]   - MQL-Double-Array (IndicatorBuffer)
 * @param  int    bufferSize - Größe des Arrays
 * @param  int    bars       - Anzahl der zu shiftenden Bars
 * @param  double emptyValue - Initialisierungswert für freiwerdende Bufferelemente
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI ShiftIndicatorBuffer(double buffer[], int bufferSize, int bars, double emptyValue) {
   if (buffer && (uint)buffer < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (bufferSize < 0)                             return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d", bufferSize));
   if (bars < 0)                                   return(error(ERR_INVALID_PARAMETER, "invalid parameter bars: %d", bars));
   if (!bufferSize || !bars) return(TRUE);

   MoveMemory((void*)&buffer[0], &buffer[bars], (bufferSize-bars)*sizeof(buffer[0]));

   for (int i=bufferSize-bars; i < bufferSize; i++) {
      buffer[i] = emptyValue;
   }
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Copy the symbol-timeframe description as in the title bar of a chart window to the specified buffer. If the buffer is too
 * small the string in the buffer is truncated. The string is always terminated with a NULL character.
 *
 * @param  char* symbol
 * @param  uint  timeframe
 * @param  char* buffer
 * @param  uint  bufferSize
 *
 * @return uint - Amount of copied characters not counting the terminating NULL character or {bufferSize} if the buffer is
 *                too small and the string in the buffer was truncated.
 *                NULL in case of an error.
 */
uint WINAPI GetChartDescription(const char* symbol, uint timeframe, char* buffer, uint bufferSize) {
   uint symbolLength = strlen(symbol);
   if (!symbolLength || symbolLength > MAX_SYMBOL_LENGTH) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol: %s", DoubleQuoteStr(symbol)));
   if (!buffer)                                           return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: %p", buffer));
   if ((int)bufferSize <= 0)                              return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d", bufferSize));

   char* szTimeframe;

   switch (timeframe) {
      case PERIOD_M1 : szTimeframe = "M1";      break;               // 1 minute
      case PERIOD_M5 : szTimeframe = "M5";      break;               // 5 minutes
      case PERIOD_M15: szTimeframe = "M15";     break;               // 15 minutes
      case PERIOD_M30: szTimeframe = "M30";     break;               // 30 minutes
      case PERIOD_H1 : szTimeframe = "H1";      break;               // 1 hour
      case PERIOD_H4 : szTimeframe = "H4";      break;               // 4 hour
      case PERIOD_D1 : szTimeframe = "Daily";   break;               // 1 day
      case PERIOD_W1 : szTimeframe = "Weekly";  break;               // 1 week
      case PERIOD_MN1: szTimeframe = "Monthly"; break;               // 1 month
      default:
         return(error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", timeframe));
   }

   // create the result in a temporary buffer
   char* format = "%s,%s";
   uint  size   = symbolLength + strlen(szTimeframe) + 2;            // symbol + 1 + timeframe + \0
   char* result = (char*)alloca(size);                               // on the stack
   int copied = sprintf_s(result, size, format, symbol, szTimeframe);
   if (copied <= 0) return(error(ERR_WIN32_ERROR+GetLastError(), "=> sprintf_s() => %d chars copied", copied));

   // copy the result to the destination buffer
   uint len = strlen(result);                                        // len should be equal to size-1

   if (len < bufferSize) {                                           // destination buffer is large enough
      strncpy(buffer, result, len);
      buffer[len] = 0;
      return(len);
   }

   strncpy(buffer, result, bufferSize-1);                            // destination buffer is too small
   buffer[bufferSize-1] = 0;
   return(bufferSize);
}


/**
 * Calculate the MD5 hash of the input.
 *
 * @param  void* input  - buffer with binary content
 * @param  uint  length - length of the content in bytes
 *
 * @return char* - MD5 hash or a NULL pointer in case of errors
 */
char* WINAPI MD5Hash(const void* input, uint length) {
   if ((uint)input < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter input: 0x%p (not a valid pointer)", input));
   if (length < 1)                      return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter length: %d", length));

   MD5_CTX context;
   MD5_INIT(&context);
   MD5_UPDATE(&context, input, length);
   uchar buffer[16];                                                 // on the stack
   MD5_FINAL((uchar*)&buffer, &context);                             // fill buffer with binary MD5 hash (16 bytes)

   std::stringstream ss;                                             // convert hash to hex string (32 chars)
   ss << std::hex;
   for (uint i=0; i < 16; i++) {
      ss << std::setw(2) << std::setfill('0') << (int)buffer[i];
   }
   return(copychars(ss.str()));                                      // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}


/**
 * Calculate the MD5 hash of a C string (ANSI).
 *
 * @param  char* input - C input string
 *
 * @return char* - MD5 hash or a NULL pointer in case of errors
 */
char* WINAPI MD5HashA(const char* input) {
   if ((uint)input < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter input: 0x%p (not a valid pointer)", input));

   return(MD5Hash(input, strlen(input)));
   #pragma EXPANDER_EXPORT
}


/**
 * Ermittelt eine eindeutige Message-ID für den String "MetaTrader4_Internal_Message".
 *
 * @return uint - Message ID im Bereich 0xC000 bis 0xFFFF oder 0, falls ein Fehler auftrat.
 */
uint WINAPI MT4InternalMsg() {
   static uint msgId;
   if (!msgId) {
      msgId = RegisterWindowMessageA("MetaTrader4_Internal_Message");
      if (!msgId) return(error(ERR_WIN32_ERROR+GetLastError(), "=> RegisterWindowMessage()"));
   }
   return(msgId);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a terminal configuration value as a boolean. Queries the global and the local configuration with the local configu-
 * ration superseding the global one. Boolean values can be expressed by "0" or "1", "On" or "Off", "Yes" or "No" and "true" or
 * "false" (case insensitive). An empty value of an existing key is considered FALSE and a numeric value is considered TRUE if
 * its nominal value is non-zero. Trailing configuration comments (text following the ";" character) are ignored.
 *
 * @param  char* section      - configuration section name
 * @param  char* key          - configuration key
 * @param  BOOL  defaultValue - alternative value to return if the specified value was not found
 *
 * @return BOOL - configuration value
 */
BOOL WINAPI GetConfigBool(const char* section, const char* key, BOOL defaultValue/*=FALSE*/) {
   // Es ist schneller, immer globale und lokale Konfiguration auszuwerten (intern jeweils nur ein Aufruf von GetPrivateProfileString()).
   //BOOL result = GetGlobalConfigBool(section, key, defaultValue);
   //return(GetLocalConfigBool (section, key, result));
}


/**
 *
 */
std::istream& getLine(std::istream& is, string& line) {
   // The characters in the stream are read one-by-one using std::streambuf. This is faster than reading them one-by-one using
   // std::istream. Code that uses streambuf this way must be guarded by a sentry object. The sentry object performs various
   // tasks, such as thread synchronization and updating the stream state.
   //
   // @see  http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf/6089413#6089413
   //
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
