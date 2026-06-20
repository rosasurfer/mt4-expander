#include "expander.h"
#include "lib/conversion.h"
#include "lib/helper.h"
#include "lib/string.h"


/**
 * Return the classname of the specified window. Simplified version of user32::GetClassNameA().
 *
 * @param  HWND hWnd - window handle
 *
 * @return char* - classname or a NULL pointer in case of errors
 */
char* WINAPI GetClassNameA(HWND hWnd) {
   char className[256];                         // max. possible value
   if (!::GetClassNameA(hWnd, className, countof(className))) {
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
   wchar className[256];                        // max. possible value
   if (!::GetClassNameW(hWnd, className, countof(className))) {
      return (wchar*)!error(ERR_WIN32_ERROR + GetLastError(), "GetClassNameW(hWnd=%p)", hWnd);
   }
   return wsdup(className);                     // caller must free()
   #pragma EXPANDER_EXPORT
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
   if (!IsWindow(hWnd)) return _EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: %p (not a valid window)", hWnd));

   /**
    * A local function as substitute for missing lambda support in C++03.
    * On error a window was destroyed cross-thread. Such a window is reported as "gone".
    *
    * @return int - number of reported child windows
    */
   struct local {
      static int ProcessWindow(HWND hWnd, HWND hWndParent, HWND hWndDesktop, BOOL isRoot, BOOL recursive, uint level) {
         uint indentLevel = (level > 0) ? (level-1) : 0;
         string spaces(indentLevel * 2, ' ');
         char* marker = level ? "-> " : "";

         SetLastError(NO_ERROR);
         wchar* wndClass = GetClassNameW(hWnd);
         if (!wndClass) return _NULL(debug_raw("  %s%s%p: (gone)  [%s]", spaces.c_str(), marker, hWnd, ErrorToStrA(GetLastError())));

         SetLastError(NO_ERROR);
         wchar* wndTitle = GetInternalWindowTextW(hWnd);
         if (!wndTitle) return _NULL(debug_raw("  %s%s%p: (gone)  [%s]", spaces.c_str(), marker, hWnd, ErrorToStrA(GetLastError())), bfree(wndClass));

         const char* sType = "";
         if      (hWnd == hWndDesktop)       sType = " (desktop)";
         else if (hWndParent == hWndDesktop) sType = " (top-level)";

         SetLastError(NO_ERROR);
         int ctrlId = GetDlgCtrlID(hWnd);
         if (!ctrlId) {
            int error = GetLastError();
            if (error) return _NULL(debug_raw("  %s%s%p: (gone)  [%s]", spaces.c_str(), marker, hWnd, ErrorToStrA(error)), bfree(wndClass, wndTitle));
         }
         if (!hWndParent || hWndParent == hWndDesktop) {
            ctrlId = 0;
         }
         char* sCtrlId = asformat(" (id %d)", ctrlId);

         debug_raw("  %s%s%p: %S \"%S\"%s%s", spaces.c_str(), marker, hWnd, wndClass, wndTitle, sType, ctrlId ? sCtrlId : "");
         free(wndClass, wndTitle, sCtrlId);
         int count = !isRoot;

         // enumerate child windows
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

   // indicate if no child windows were found to visually distinguish from error conditions
   if (!count) debug_raw("  -> (no child windows)");

   return count;
   #pragma EXPANDER_EXPORT
}
