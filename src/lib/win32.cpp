#include "expander.h"
#include "lib/string.h"


/**
 * Return the classname of the specified window. Simplified version of user32::GetClassNameA().
 *
 * @param  HWND hWnd - window handle
 *
 * @return char* - classname or a NULL pointer in case of errors
 */
char* WINAPI GetClassNameA(HWND hWnd) {
   uint bufSize = 256;                          // max. possible value
   char* className = (char*) alloca(bufSize * sizeof(char));

   if (!::GetClassNameA(hWnd, className, bufSize)) {
      return (char*)!error(ERR_WIN32_ERROR + GetLastError(), "GetClassNameA(hWnd=%p)", hWnd);
   }
   return sdup(className);                      // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return the classname of the specified window. Simplified version of user32::GetClassNameW().
 *
 * @param  HWND hWnd - window handle
 *
 * @return wchar* - classname or a NULL pointer in case of errors
 */
wchar* WINAPI GetClassNameW(HWND hWnd) {
   uint bufSize = 256;                          // max. possible value
   wchar* className = (wchar*) alloca(bufSize * sizeof(wchar));

   if (!::GetClassNameW(hWnd, className, bufSize)) {
      return (wchar*)!error(ERR_WIN32_ERROR + GetLastError(), "GetClassNameW(hWnd=%p)", hWnd);
   }
   return wsdup(className);                     // caller must free()
   #pragma EXPANDER_EXPORT
}
