#pragma once
#include "expander.h"

int    WINAPI EnumChildWindowsToDebug(HWND hWnd, BOOL recursive = FALSE);
char*  WINAPI GetClassNameA(HWND hWnd);
wchar* WINAPI GetClassNameW(HWND hWnd);
