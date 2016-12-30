/**
 *
 */
#include "header/Expander.h"


HWND    last_hWnd;
WNDPROC origWndProc;


/**
 * Custom window procedure
 */
LRESULT WINAPI CustomWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

   debug("hWnd=%d, msg=%d, wParam=%p, lParam=%p", hWnd, msg, wParam, lParam);

   return(CallWindowProc(origWndProc, hWnd, msg, wParam, lParam));
}


/**
 *
 */
BOOL WINAPI SubclassWindow(HWND hWnd) {
   if (!IsWindow(hWnd))                    return(error(ERR_INVALID_PARAMETER, "invalid parameter hWnd=%d (not a window)", hWnd));
   DWORD processId; GetWindowThreadProcessId(hWnd, &processId);
   if (processId != GetCurrentProcessId()) return(error(ERR_INVALID_PARAMETER, "window hWnd=%d not owned by the current process", hWnd));

   origWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)CustomWndProc);
   if (!origWndProc) return(error(ERR_WIN32_ERROR+GetLastError(), "SetWindowLong() failed"));

   last_hWnd = hWnd;
   debug("replaced window procedure 0x%p with 0x%p", origWndProc, CustomWndProc);
   return(TRUE);
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI UnsubclassWindow(HWND hWnd) {
   if (!hWnd)             return(error(ERR_INVALID_PARAMETER, "invalid parameter hWnd=%d (not a window)", hWnd));
   if (hWnd != last_hWnd) return(error(ERR_INVALID_PARAMETER, "invalid parameter hWnd=%d (not a subclassed window)", hWnd));

   if (!SetWindowLong(hWnd, GWL_WNDPROC, (LONG)origWndProc))
      return(error(ERR_WIN32_ERROR+GetLastError(), "failed to restore original window procedure"));

   last_hWnd   = NULL;
   origWndProc = NULL;

   debug("original window procedure restored");
   return(TRUE);
   #pragma EXPORT
}
