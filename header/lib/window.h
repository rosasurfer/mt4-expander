#pragma once
#include "expander.h"


/**
 * Return the regular styles of the specified window.
 *
 * @param  HWND hWnd
 *
 * @return DWORD - regular window styles
 */
__forceinline DWORD WINAPI GetWindowStyles(HWND hWnd) {
   return GetWindowLongPtrW(hWnd, GWL_STYLE);
}


/**
 * Return the extended styles of the specified window.
 *
 * @param  HWND hWnd
 *
 * @return DWORD - extended window styles
 */
__forceinline DWORD WINAPI GetWindowStylesEx(HWND hWnd) {
   return GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
}
