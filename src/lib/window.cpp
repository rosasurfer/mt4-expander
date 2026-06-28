#include "expander.h"
#include "lib/conversion.h"
#include "lib/string.h"
#include "lib/window.h"

#include <map>

typedef std::map<string, int>    IntegerMap;             // map = associative array
typedef std::map<string, double> DoubleMap;              // new elements are added "by-copy"
typedef std::map<string, string> StringMap;              //

static IntegerMap intProperties;                         // a map with integers stored as window properties
static DoubleMap  doubleProperties;                      // a map with doubles stored as window properties
static StringMap  stringProperties;                      // a map with strings stored as window properties


/**
 * Return the classname of the specified window. Simplified version of user32::GetClassNameA().
 *
 * @param  HWND hWnd - window handle
 *
 * @return char* - classname or a NULL pointer in case of errors
 */
char* WINAPI GetClassNameA(HWND hWnd) {
   string className = getClassNameA(hWnd);
   return sdup(className.c_str());              // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return the classname of the specified window. Simplified version of user32::GetClassNameA().
 *
 * @param  HWND hWnd - window handle
 *
 * @return string - classname or an empty string in case of errors
 */
string WINAPI getClassNameA(HWND hWnd) {
   char className[256];                         // max. possible value
   if (!GetClassNameA(hWnd, className, countof(className))) {
      return _empty_str(error(ERR_WIN32_ERROR + GetLastError(), "GetClassNameA(hWnd=%p)", hWnd));
   }
   return string(className);
}


/**
 * Return the classname of the specified window. Simplified version of user32::GetClassNameW().
 *
 * @param  HWND hWnd - window handle
 *
 * @return wchar* - classname or a NULL pointer in case of errors
 */
wchar* WINAPI GetClassNameW(HWND hWnd) {
   wstring className = getClassNameW(hWnd);
   return wsdup(className.c_str());             // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return the classname of the specified window. Simplified version of user32::GetClassNameW().
 *
 * @param  HWND hWnd - window handle
 *
 * @return wstring - classname or an empty string in case of errors; call GetLastError() for details
 */
wstring WINAPI getClassNameW(HWND hWnd) {
   wchar className[256];                        // max. possible value
   if (!GetClassNameW(hWnd, className, countof(className))) {
      return _empty_wstr(error(ERR_WIN32_ERROR + GetLastError(), "GetClassNameW(hWnd=%p)", hWnd));
   }
   return wstring(className);
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function gets the text as a response to a WM_GETTEXT message.
 *
 * @param  HWND hWnd - window handle
 *
 * @return char* - text (may be empty) or a NULL pointer in case of errors
 */
char* WINAPI GetWindowTextA(HWND hWnd) {
   wchar* utf16Text = GetWindowTextW(hWnd);
   if (!utf16Text) return NULL;

   char* ansiText = utf16ToAnsi(utf16Text);
   free(utf16Text);

   return ansiText;                                      // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function gets the text as a response to a WM_GETTEXT message.
 *
 * @param  HWND hWnd - window handle
 *
 * @return wchar* - text (may be empty) or a NULL pointer in case of errors
 */
wchar* WINAPI GetWindowTextW(HWND hWnd) {
   wchar* buffer = NULL;
   int chars = 64, copiedChars = chars;

   SetLastError(NO_ERROR);
   while (copiedChars >= chars-1) {                      // if (length == chars-1) the string may have been truncated
      chars <<= 1;                                       // double the size (starts with 128 chars)
      buffer = (wchar*) alloca(chars * sizeof(wchar));
      copiedChars = GetWindowTextW(hWnd, buffer, chars);
   }
   if (!copiedChars && GetLastError()) return (wchar*)!error(ERR_WIN32_ERROR + GetLastError(), "GetWindowTextW()");

   return wsdup(buffer);                                 // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function reads the text directly from the window structure, it doesn't send a WM_GETTEXT message.
 *
 * @param  HWND hWnd - window handle
 *
 * @return char* - text or a NULL pointer in case of errors
 */
char* WINAPI GetInternalWindowTextA(HWND hWnd) {
   SetLastError(NO_ERROR);
   string text = getInternalWindowTextA(hWnd);
   if (GetLastError()) return NULL;

   return sdup(text.c_str());                            // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function reads the text directly from the window structure, it doesn't send a WM_GETTEXT message.
 *
 * @param  HWND hWnd - window handle
 *
 * @return string - text or an empty string in case of errors; call GetLastError() for details
 */
string WINAPI getInternalWindowTextA(HWND hWnd) {
   wstring text = getInternalWindowTextW(hWnd);
   return utf16ToAnsi(text);
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function reads the text directly from the window structure, it doesn't send a WM_GETTEXT message.
 *
 * @param  HWND hWnd - window handle
 *
 * @return wchar* - text or a NULL pointer in case of errors
 */
wchar* WINAPI GetInternalWindowTextW(HWND hWnd) {
   SetLastError(NO_ERROR);
   wstring text = getInternalWindowTextW(hWnd);
   if (GetLastError()) return NULL;

   return wsdup(text.c_str());                           // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return the text of the specified window's title bar. If the window is a control the text of the control is obtained.
 * This function reads the text directly from the window structure, it doesn't send a WM_GETTEXT message.
 *
 * @param  HWND hWnd - window handle
 *
 * @return wstring - text or an empty string in case of errors; call GetLastError() for details
 */
wstring WINAPI getInternalWindowTextW(HWND hWnd) {
   wchar* buffer = NULL;
   int chars = 64, copiedChars = chars;

   SetLastError(NO_ERROR);
   while (copiedChars >= chars-1) {                      // if (length == chars-1) the string may have been truncated
      chars <<= 1;                                       // double the length (starts with 128 chars)
      buffer = (wchar*) alloca(chars * sizeof(wchar));
      copiedChars = InternalGetWindowText(hWnd, buffer, chars);
   }
   if (!copiedChars && GetLastError()) return _empty_wstr(error(ERR_WIN32_ERROR + GetLastError(), "InternalGetWindowText()"));

   return wstring(buffer);
}


/**
 * Alias of user32::GetPropA()
 *
 * Retrieves a named value stored in the property list of the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - property name
 *
 * @return HANDLE - the stored value or NULL if no such property exists in the window
 */
HANDLE WINAPI GetWindowPropertyA(HWND hWnd, const char* name) {
   return GetPropA(hWnd, name);
   #pragma EXPANDER_EXPORT
}


/**
 * Alias of user32::SetPropA()
 *
 * Sets a named value stored in the property list of the specified window.
 *
 * @param  HWND   hWnd  - window handle
 * @param  char*  name  - property name
 * @param  HANDLE value - property value
 *
 * @return BOOL - success status
 */
BOOL WINAPI SetWindowPropertyA(HWND hWnd, const char* name, HANDLE value) {
   BOOL success = SetPropA(hWnd, name, value);
   if (!success) error(ERR_WIN32_ERROR + GetLastError(), "SetPropA()  name=\"%s\"", name);
   return success;
   #pragma EXPANDER_EXPORT
}


/**
 * Alias of user32::RemovePropA()
 *
 * Removes a named value from the property list of the specified window.
 *
 * @param  HWND   hWnd  - window handle
 * @param  char*  name  - property name
 *
 * @return HANDLE - the removed value or NULL if no such property exists in the window
 */
HANDLE WINAPI RemoveWindowPropertyA(HWND hWnd, const char* name) {
   return RemovePropA(hWnd, name);
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
   if (!IsWindow(hWnd))                return !error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   string key = to_string(hWnd).append("|").append(name);
   IntegerMap::iterator result = intProperties.find(key);

   if (result != intProperties.end()) {
      return result->second;
   }
   return NULL;
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
   if (!IsWindow(hWnd))                return !error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   string key = to_string(hWnd).append("|").append(name);
   DoubleMap::iterator result = doubleProperties.find(key);

   if (result != doubleProperties.end()) {
      return result->second;
   }
   return NULL;
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
   if (!IsWindow(hWnd))                return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   string key = to_string(hWnd).append("|").append(name);
   StringMap::iterator result = stringProperties.find(key);

   if (result != stringProperties.end()) {
      return result->second.c_str();
   }
   return NULL;
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
   if (!IsWindow(hWnd))                return !error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   string key = to_string(hWnd).append("|").append(name);
   intProperties[key] = value;

   return TRUE;
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
   if (!IsWindow(hWnd))                return !error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   string key = to_string(hWnd).append("|").append(name);
   doubleProperties[key] = value;

   return TRUE;
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
   if (!IsWindow(hWnd))                 return !error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER)  return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");
   if ((uint)value < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value);

   string key = to_string(hWnd).append("|").append(name);
   stringProperties[key] = value;

   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Remove a named integer value linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - integer name
 *
 * @return int - removed value or NULL if the name was not found or in case of errors
 */
int WINAPI RemoveWindowIntegerA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return !error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   string key = to_string(hWnd).append("|").append(name);
   IntegerMap::iterator result = intProperties.find(key);

   if (result != intProperties.end()) {
      int value = result->second;
      intProperties.erase(result);
      return value;
   }
   return NULL;
   #pragma EXPANDER_EXPORT
}


/**
 * Remove a named double value linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - double name
 *
 * @return double - removed value or NULL if the name was not found or in case of errors
 */
double WINAPI RemoveWindowDoubleA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return !error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   string key = to_string(hWnd).append("|").append(name);
   DoubleMap::iterator result = doubleProperties.find(key);

   if (result != doubleProperties.end()) {
      double value = result->second;
      doubleProperties.erase(result);
      return value;
   }
   return NULL;
   #pragma EXPANDER_EXPORT
}


/**
 * Remove a named string linked to the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - double name
 *
 * @return char* - removed string or a NULL pointer if the name was not found or in case of errors
 */
char* WINAPI RemoveWindowStringA(HWND hWnd, const char* name) {
   if (!IsWindow(hWnd))                return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);
   if ((uint)name < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   string key = to_string(hWnd).append("|").append(name);
   StringMap::iterator result = stringProperties.find(key);

   if (result != stringProperties.end()) {
      string value = result->second;
      stringProperties.erase(result);           // invalidates result and releases result->second
      return sdup(value.c_str());               // caller must free()
   }
   return NULL;
   #pragma EXPANDER_EXPORT
}


/**
 * Release all stored window properties. Called from DLL::onProcessDetach() only.
 */
void WINAPI ReleaseWindowProperties() {
   intProperties.clear();
   doubleProperties.clear();
   stringProperties.clear();
}


/**
 * Enumerate the child windows of a window and send details to the debug output.
 *
 * @param  HWND hWnd                 - window to process; if NULL all top-level windows are enumerated
 * @param  BOOL recursive [optional] - whether to enumerate childs recursively (default: no)
 *
 * @return int - number of reported child windows or EMPTY (-1) in case of errors
 */
int WINAPI EnumChildWindowsToDebug(HWND hWnd, BOOL recursive/*=FALSE*/) {
   HWND hWndDesktop = GetDesktopWindow();
   if (!hWnd) hWnd = hWndDesktop;
   if (!IsWindow(hWnd)) return _EMPTY(error(ERR_WIN32_ERROR + ERROR_INVALID_WINDOW_HANDLE, "hWnd=%p", hWnd));

   struct local {
      /**
       * On error a window was destroyed cross-thread. Such a window is reported as "gone" and not included in the return value.
       *
       * @return int - number of reported child windows
       */
      static int ProcessWindow(HWND hWnd, HWND hWndParent, HWND hWndDesktop, BOOL isRoot, BOOL recursive, uint level) {
         uint indentLevel = (level > 0) ? (level-1) : 0;
         string spaces(indentLevel * 2, ' ');
         char* marker = level ? "-> " : "";

         SetLastError(NO_ERROR);
         wstring wndClass = getClassNameW(hWnd);
         if (GetLastError()) return debug_raw("  %s%s%p: (gone)  [%s]", spaces.c_str(), marker, hWnd, ErrorToStrA(GetLastError()));

         wstring wndTitle = getInternalWindowTextW(hWnd);
         if (GetLastError()) return debug_raw("  %s%s%p: (gone)  [%s]", spaces.c_str(), marker, hWnd, ErrorToStrA(GetLastError()));

         const char* sType = "";
         if      (hWnd == hWndDesktop)       sType = " (desktop)";
         else if (hWndParent == hWndDesktop) sType = " (top-level)";

         int ctrlId = GetDlgCtrlID(hWnd);
         if (!ctrlId && GetLastError()) return debug_raw("  %s%s%p: (gone)  [%s]", spaces.c_str(), marker, hWnd, ErrorToStrA(GetLastError()));

         if (!hWndParent || hWndParent == hWndDesktop) {
            ctrlId = 0;
         }
         char* sCtrlId = asformat(" (id %d)", ctrlId);

         debug_raw("  %s%s%p: %S \"%S\"%s%s", spaces.c_str(), marker, hWnd, wndClass.c_str(), wndTitle.c_str(), sType, ctrlId ? sCtrlId : "");
         free(sCtrlId);

         // enumerate child windows                         // TODO: convert to EnumWindows/EnumChildWindows
         int count = !isRoot;
         if (isRoot || recursive) {
            HWND hWndNext = GetWindow(hWnd, GW_CHILD);      // no more error handling as the window was successfully reported
            while (hWndNext != 0) {
               count += local::ProcessWindow(hWndNext, hWnd, hWndDesktop, FALSE, recursive, level+1);
               hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
            }
         }
         return count;
      }
   };

   // process the window
   debug("for HWND %p", hWnd);
   HWND hWndParent = GetAncestor(hWnd, GA_PARENT);
   int count = local::ProcessWindow(hWnd, hWndParent, hWndDesktop, TRUE, recursive, 0);

   // report no child windows to visually distinguish from errors
   if (!count) debug_raw("  -> (no child windows)");

   return count;
   #pragma EXPANDER_EXPORT
}


/**
 * Enumerates the properties of the specified window.
 *
 * @param  HWND  hWnd - window handle
 * @param  char* name - limiting name prefix of properties to print
 *
 * @return int - number of reported window properties or EMPTY (-1) in case of errors
 */
int WINAPI EnumWindowPropertiesA(HWND hWnd, const char* prefix) {
   if (prefix && (uint)prefix < MIN_VALID_POINTER) {
      return _EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter prefix: 0x%p (not a valid pointer)", prefix));
   }
   return EnumWindowPropertiesW(hWnd, prefix ? ansiToUtf16(string(prefix)).c_str() : NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Enumerates the properties of the specified window.
 *
 * @param  HWND   hWnd - window handle
 * @param  wchar* name - limiting name prefix of properties to print
 *
 * @return int - number of reported window properties or EMPTY (-1) in case of errors
 */
int WINAPI EnumWindowPropertiesW(HWND hWnd, const wchar* prefix) {
   if (prefix && (uint)prefix < MIN_VALID_POINTER) return _EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter prefix: 0x%p (not a valid pointer)", prefix));

   struct local {
      /**
       * @param  HWND      hWnd   - window whose property list is being enumerated
       * @param  wchar*    name   - property name
       * @param  HANDLE    value  - property value
       * @param  ULONG_PTR lpData - user arguments as passed by the outer function
       *
       * @return BOOL - whether to continue enumeration with the next property
       */
      static BOOL CALLBACK EnumPropsProc(HWND hwnd, wchar* name, HANDLE value, ULONG_PTR lpData) {
         ARGS* args = (ARGS*)lpData;
         const wchar* prefix = args->prefix;

         if (prefix && !*prefix) {
            prefix = NULL;
         }
         if (!prefix || StrStartsWith(name, prefix)) {
            debug_raw("  \"%S\" = 0x%p (%d)", name, value, value);
            args->count++;
         }
         return TRUE;
      }
   };
   struct ARGS {
      __In_    const wchar* prefix;
      __InOut_ int          count;
   } args = { prefix, 0 };

   debug("for HWND %p", hWnd);

   SetLastError(NO_ERROR);
   int result = EnumPropsExW(hWnd, local::EnumPropsProc, (LPARAM)&args);
   if (result == -1 && GetLastError()) return _EMPTY(error(ERR_WIN32_ERROR + GetLastError(), "EnumPropsExW()"));

   // report no properties to visually distinguish from errors
   if (!args.count) debug_raw("  (no window properties)");
   return args.count;
   #pragma EXPANDER_EXPORT
}


/**
 * Get the message id of the internal MT4 messages.
 *
 * @return uint - message id in the range from 0xC000 to 0xFFFF, or NULL (0) in case of errors
 */
uint WINAPI MT4InternalMsg() {
   static uint msgId = RegisterWindowMessageW(L"MetaTrader4_Internal_Message");
   if (!msgId) return !error(ERR_WIN32_ERROR + GetLastError(), "RegisterWindowMessageW()");
   return msgId;
}


/**
 * Alias of MT4InternalMsg()
 *
 * Get the message id of the internal MT4 messages.
 *
 * @return uint - message id in the range from 0xC000 to 0xFFFF, or NULL (0) in case of errors
 */
uint WINAPI WM_MT4() {
   return MT4InternalMsg();
   #pragma EXPANDER_EXPORT
}


/**
 * Get the message id for internal MT4Expander messages.
 *
 * @return uint - message id in the range from 0xC000 to 0xFFFF, or NULL (0) in case of errors
 */
uint WINAPI MT4ExpanderMsg() {
   static uint msgId = RegisterWindowMessageW(L"rsf.MT4Expander.Message");
   if (!msgId) return !error(ERR_WIN32_ERROR + GetLastError(), "RegisterWindowMessageW()");
   return msgId;
}


/**
 * Alias of MT4ExpanderMsg()
 *
 * Get the message id for internal MT4Expander messages.
 *
 * @return uint - message id in the range from 0xC000 to 0xFFFF, or NULL (0) in case of errors
 */
uint WINAPI WM_MT4EXPANDER() {
   return MT4ExpanderMsg();
   #pragma EXPANDER_EXPORT
}
