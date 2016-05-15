/**
 *
 */
#include "Expander.h"


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
   if (!IsWindow(hWnd))                    return(debug("ERROR:  invalid parameter hWnd=%d (not a window)", hWnd));
   DWORD processId; GetWindowThreadProcessId(hWnd, &processId);
   if (processId != GetCurrentProcessId()) return(debug("ERROR:  window hWnd=%d not owned by the current process", hWnd));

   origWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)CustomWndProc);
   if (!origWndProc)
      return(debug("ERROR: SetWindowLong() failed with error %d", GetLastError()));

   last_hWnd = hWnd;
   debug("replaced window procedure 0x%p with 0x%p", origWndProc, CustomWndProc);
   return(TRUE);
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI UnsubclassWindow(HWND hWnd) {
   if (!hWnd)             return(debug("ERROR:  invalid parameter hWnd=%d (not a window)", hWnd));
   if (hWnd != last_hWnd) return(debug("ERROR:  invalid parameter hWnd=%d (not a subclassed window)", hWnd));

   if (!SetWindowLong(hWnd, GWL_WNDPROC, (LONG)origWndProc))
      return(debug("failed to restore original window procedure with error %d", GetLastError()));

   last_hWnd   = NULL;
   origWndProc = NULL;

   debug("original window procedure restored");
   return(TRUE);
   #pragma EXPORT
}
