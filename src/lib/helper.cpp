#include "expander.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include <map>

extern "C" {
   #include "etc/md5.h"
}


typedef std::map<string, int>    IntegerMap;       // all data is copied into the container
typedef std::map<string, double> DoubleMap;
typedef std::map<string, string> StringMap;

IntegerMap integerProperties;                      // a map with integers stored as window properties
DoubleMap  doubleProperties;                       // a map with doubles stored as window properties
StringMap  stringProperties;                       // a map with strings stored as window properties


/**
 * Return the last Windows error of the current thread and make it accessible to MQL.
 *
 * @return int - error code
 */
int WINAPI GetLastWin32Error() {
   return(GetLastError());
   #pragma EXPANDER_EXPORT
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function obtains the text directly from the window structure and doesn't send a WM_GETTEXT message.
 *
 * @param  HWND  hWnd - window handle
 *
 * @return char* - text (may be empty) or a NULL pointer in case of errors
 *
 * Note: The caller is responsible for releasing the string's memory after usage with "free()".
 */
char* WINAPI GetInternalWindowTextA(HWND hWnd) {
   wchar* unicodeText = GetInternalWindowTextW(hWnd);
   if (!unicodeText) return(NULL);

   char* ansiText = strdup(unicodeToAnsi(wstring(unicodeText)).c_str());   // on the heap
   free(unicodeText);

   return(ansiText);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function obtains the text directly from the window structure and doesn't send a WM_GETTEXT message.
 *
 * @param  HWND  hWnd - window handle
 *
 * @return wchar* - text (may be empty) or a NULL pointer in case of errors
 *
 * Note: The caller is responsible for releasing the string's memory after usage with "free()".
 */
wchar* WINAPI GetInternalWindowTextW(HWND hWnd) {
   if (!IsWindow(hWnd)) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));

   SetLastError(NO_ERROR);
   wchar* buffer = NULL;
   uint size=32, length=size;

   while (length >= size-1) {                               // if (length == size-1) the string may have been truncated
      free(buffer);
      size <<= 1;                                           // double the size
      buffer = (wchar*)malloc(size*2);                      // on the heap
      length = InternalGetWindowText(hWnd, buffer, size);
   }

   if (!length) {
      if (DWORD error = GetLastError()) {
         free(buffer);
         return((wchar*)!error(ERR_WIN32_ERROR+error, "->InternalGetWindowText()"));
      }
   }
   return(buffer);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function gets the text as a response to a WM_GETTEXT message.
 *
 * @param  HWND  hWnd - window handle
 *
 * @return char* - text (may be empty) or a NULL pointer in case of errors
 *
 * Note: The caller is responsible for releasing the string's memory after usage with "free()".
 */
char* WINAPI GetWindowTextA(HWND hWnd) {
   wchar* unicodeText = GetWindowTextW(hWnd);
   if (!unicodeText) return(NULL);

   char* ansiText = strdup(unicodeToAnsi(wstring(unicodeText)).c_str());   // on the heap
   free(unicodeText);

   return(ansiText);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function gets the text as a response to a WM_GETTEXT message.
 *
 * @param  HWND  hWnd - window handle
 *
 * @return wchar* - text (may be empty) or a NULL pointer in case of errors
 *
 * Note: The caller is responsible for releasing the string's memory after usage with "free()".
 */
wchar* WINAPI GetWindowTextW(HWND hWnd) {
   if (!IsWindow(hWnd)) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));

   SetLastError(NO_ERROR);
   wchar* buffer = NULL;
   uint size=32, length=size;

   while (length >= size-1) {                               // if (length == size-1) the string may have been truncated
      free(buffer);
      size <<= 1;                                           // double the size
      buffer = (wchar*)malloc(size*2);                      // on the heap
      length = GetWindowTextW(hWnd, buffer, size);
   }

   if (!length) {
      if (DWORD error = GetLastError()) {
         free(buffer);
         return((wchar*)!error(ERR_WIN32_ERROR+error, "->GetWindowTextW()"));
      }
   }
   return(buffer);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified value is a MetaTrader standard timeframe identifier.
 *
 * @param  int timeframe
 *
 * @return BOOL
 */
BOOL WINAPI IsStandardTimeframe(int timeframe) {
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
 * Whether the specified value is a custom timeframe identifier.
 *
 * @param  int timeframe
 *
 * @return BOOL
 */
BOOL WINAPI IsCustomTimeframe(int timeframe) {
   if (timeframe <= 0)
      return(FALSE);
   return(!IsStandardTimeframe(timeframe));
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified value is a MQL program type.
 *
 * @param  int type
 *
 * @return BOOL
 */
BOOL WINAPI IsProgramType(int type) {
   switch (type) {
      case PT_INDICATOR: return(TRUE);
      case PT_EXPERT:    return(TRUE);
      case PT_SCRIPT:    return(TRUE);
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a virtual key is up or down at the time the function is called.
 *
 * @param  int vKey - one of 256 possible virtual-key codes
 *
 * @return BOOL
 *
 * @see  https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
 */
BOOL WINAPI IsVirtualKeyDown(int vKey) {
   return(GetAsyncKeyState(vKey) & 0x8000);     // check the most significant bit of the SHORT result
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified thread is the application's UI thread.
 *
 * @param  DWORD threadId [optional] - thread id (default: the current thread)
 *
 * @return BOOL
 */
BOOL WINAPI IsUIThread(DWORD threadId/*= NULL*/) {
   if (!threadId)
      threadId = GetCurrentThreadId();
   return(threadId == GetUIThreadId());
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
      HWND hWnd = GetTerminalMainWindow();
      if (hWnd)
         uiThreadId = GetWindowThreadProcessId(hWnd, NULL);
   }
   return(uiThreadId);
   #pragma EXPANDER_EXPORT
}


/**
 * Store a named integer value and link it to the specified window.
 *
 * @param  HWND  hWnd  - window handle
 * @param  char* name  - integer name
 * @param  int   value - integer value
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetWindowIntegerA(HWND hWnd, const char* name, int value) {
   if (!IsWindow(hWnd))                return(!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));

   string key = to_string(hWnd).append("|").append(name);
   integerProperties[key] = value;

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Store a named double value and link it to the specified window.
 *
 * @param  HWND   hWnd  - window handle
 * @param  char*  name  - integer name
 * @param  double value - double value
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetWindowDoubleA(HWND hWnd, const char* name, double value) {
   if (!IsWindow(hWnd))                return(!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));

   string key = to_string(hWnd).append("|").append(name);
   doubleProperties[key] = value;

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Store a named string and link it to the specified window.
 *
 * @param  HWND  hWnd  - window handle
 * @param  char* name  - string identifier
 * @param  char* value - string (must not be a NULL pointer)
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetWindowStringA(HWND hWnd, const char* name, const char* value) {
   if (!IsWindow(hWnd))                 return(!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER)  return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                          return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));
   if ((uint)value < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));

   string key = to_string(hWnd).append("|").append(name);
   stringProperties[key] = value;

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a named integer value linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - integer name
 *
 * @return int - stored value or NULL if the name was not found or in case of errors
 */
int WINAPI GetWindowIntegerA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return(!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));

   string key = to_string(hWnd).append("|").append(name);
   IntegerMap::iterator result = integerProperties.find(key);

   if (result != integerProperties.end())
      return(result->second);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a named double value linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - double name
 *
 * @return double - stored value or NULL if the name was not found or in case of errors
 */
double WINAPI GetWindowDoubleA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return(!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));

   string key = to_string(hWnd).append("|").append(name);
   DoubleMap::iterator result = doubleProperties.find(key);

   if (result != doubleProperties.end())
      return(result->second);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a named string linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - string identifier
 *
 * @return char* - stored string or a NULL pointer if the name was not found or in case of errors
 */
const char* WINAPI GetWindowStringA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));

   string key = to_string(hWnd).append("|").append(name);
   StringMap::iterator result = stringProperties.find(key);

   if (result != stringProperties.end())
      return(result->second.c_str());
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Remove and return a named integer value linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - integer name
 *
 * @return int - removed value or NULL if the name was not found or in case of errors
 */
int WINAPI RemoveWindowIntegerA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return(!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));

   string key = to_string(hWnd).append("|").append(name);
   IntegerMap::iterator result = integerProperties.find(key);

   if (result != integerProperties.end()) {
      int value = result->second;
      integerProperties.erase(result);
      return(value);
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Remove and return a named double value linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - double name
 *
 * @return double - removed value or NULL if the name was not found or in case of errors
 */
double WINAPI RemoveWindowDoubleA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return(!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));

   string key = to_string(hWnd).append("|").append(name);
   DoubleMap::iterator result = doubleProperties.find(key);

   if (result != doubleProperties.end()) {
      double value = result->second;
      doubleProperties.erase(result);
      return(value);
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Remove and return a named string linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - double name
 *
 * @return char* - removed string or a NULL pointer if the name was not found or in case of errors
 */
const char* WINAPI RemoveWindowStringA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd));
   if ((uint)name < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)"));

   string key = to_string(hWnd).append("|").append(name);
   StringMap::iterator result = stringProperties.find(key);

   if (result != stringProperties.end()) {
      string value = result->second;
      stringProperties.erase(result);           // invalidates result and releases result->second
      return(strdup(value.c_str()));            // TODO: close memory leak
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Release all stored window values. Called from DLL::onProcessDetach().
 */
void WINAPI ReleaseWindowProperties() {
   integerProperties.clear();
   doubleProperties.clear();
   stringProperties.clear();
}


/**
 * Compose a <symbol>,<timeframe> description of the specified parameters (format as in chart title bar, e.g. "EURUSD,Daily")
 * and copy it to the passed buffer. If the buffer is too small the string in the buffer is truncated. The string is always
 * terminated with a null character.
 *
 * @param  char* symbol
 * @param  uint  timeframe
 * @param  char* buffer
 * @param  uint  bufferSize
 *
 * @return uint - Amount of copied characters not counting the terminating null character or the passed parameter 'bufferSize'
 *                if the buffer is too small and the string in the buffer was truncated. NULL in case of errors.
 */
uint WINAPI ComposeChartDescription(const char* symbol, uint timeframe, char* buffer, uint bufferSize) {
   uint symbolLength = strlen(symbol);
   if (!symbolLength || symbolLength > MAX_SYMBOL_LENGTH) return(!error(ERR_INVALID_PARAMETER, "invalid parameter symbol: %s", DoubleQuoteStr(symbol)));
   if (!buffer)                                           return(!error(ERR_INVALID_PARAMETER, "invalid parameter buffer: %p", buffer));
   if ((int)bufferSize <= 0)                              return(!error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d", bufferSize));

   char* sTimeframe = NULL;

   switch (timeframe) {
      case PERIOD_M1 : sTimeframe = "M1";      break;
      case PERIOD_M5 : sTimeframe = "M5";      break;
      case PERIOD_M15: sTimeframe = "M15";     break;
      case PERIOD_M30: sTimeframe = "M30";     break;
      case PERIOD_H1 : sTimeframe = "H1";      break;
      case PERIOD_H4 : sTimeframe = "H4";      break;
      case PERIOD_D1 : sTimeframe = "Daily";   break;
      case PERIOD_W1 : sTimeframe = "Weekly";  break;
      case PERIOD_MN1: sTimeframe = "Monthly"; break;
      default:
         return(!error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", timeframe));
   }
   uint chars = snprintf(buffer, bufferSize, "%s,%s", symbol, sTimeframe);

   if (chars < 0 || chars==bufferSize) {
      buffer[chars-1] = 0;
      return(bufferSize);
   }
   return(chars);
}


/**
 * Calculate the MD5 hash of the input.
 *
 * @param  void* input  - buffer with binary content
 * @param  uint  length - length of the content in bytes
 *
 * @return char* - MD5 hash or a NULL pointer in case of errors
 */
const char* WINAPI MD5Hash(const void* input, uint length) {
   if ((uint)input < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter input: 0x%p (not a valid pointer)", input));
   if (length < 1)                      return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter length: %d", length));

   MD5_CTX context;
   MD5_INIT(&context);
   MD5_UPDATE(&context, input, length);
   uchar buffer[16];                                                 // on the stack
   MD5_FINAL((uchar*)&buffer, &context);                             // fill buffer with binary MD5 hash (16 bytes)

   std::ostringstream ss;                                            // convert hash to hex string (32 chars)
   ss << std::hex;
   for (uint i=0; i < 16; i++) {
      ss << std::setw(2) << std::setfill('0') << (int)buffer[i];
   }
   return(strdup(ss.str().c_str()));                                 // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}


/**
 * Calculate the MD5 hash of a string.
 *
 * @param  char* input - input string
 *
 * @return char* - MD5 hash or a NULL pointer in case of errors
 */
const char* WINAPI MD5HashA(const char* input) {
   if ((uint)input < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter input: 0x%p (not a valid pointer)", input));

   return(MD5Hash(input, strlen(input)));
   #pragma EXPANDER_EXPORT
}


/**
 * Get a unique message id for the string "MetaTrader4_Internal_Message".
 *
 * @return uint - message id in the range from 0xC000 to 0xFFFF or 0 (zero) in case of errors
 */
uint WINAPI MT4InternalMsg() {
   static uint msgId;
   if (!msgId) {
      msgId = RegisterWindowMessageA("MetaTrader4_Internal_Message");
      if (!msgId) return(!error(ERR_WIN32_ERROR+GetLastError(), "->RegisterWindowMessage()"));
   }
   return(msgId);
   #pragma EXPANDER_EXPORT
}


/**
 * Alias of MT4InternalMsg()
 *
 * Get a unique message id for the string "MetaTrader4_Internal_Message".
 *
 * @return uint - message id in the range from 0xC000 to 0xFFFF or 0 (zero) in case of errors
 */
uint WINAPI WM_MT4() {
   return(MT4InternalMsg());
   #pragma EXPANDER_EXPORT
}
