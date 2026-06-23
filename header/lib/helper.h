#pragma once
#include "expander.h"

BOOL        WINAPI IsDebugBuild();

char*       WINAPI GetInternalWindowTextA(HWND hWnd);
wchar*      WINAPI GetInternalWindowTextW(HWND hWnd);
string      WINAPI getInternalWindowTextA(HWND hWnd);
wstring     WINAPI getInternalWindowTextW(HWND hWnd);
DWORD       WINAPI GetLastWin32Error();
DWORD       WINAPI GetPressedVirtualKeys(DWORD flags/*= F_VK_ALL*/);
DWORD       WINAPI GetUIThreadId();
char*       WINAPI GetWindowTextA(HWND hWnd);
wchar*      WINAPI GetWindowTextW(HWND hWnd);
BOOL        WINAPI IsCustomTimeframe(int timeframe);
BOOL        WINAPI IsStandardTimeframe(int timeframe);
BOOL        WINAPI IsProgramType(int type);
BOOL        WINAPI IsUIThread(DWORD threadId = NULL);
BOOL        WINAPI IsVirtualKeyDown(int key);
BOOL        WINAPI IsWindowAreaVisible(HWND hWnd);
string      WINAPI MakeChartTitle(const string &symbol, uint timeframe);
uint        WINAPI MT4InternalMsg();
uint        WINAPI WM_MT4();

HANDLE      WINAPI GetWindowPropertyA(HWND hWnd, const char* name);
BOOL        WINAPI SetWindowPropertyA(HWND hWnd, const char* name, HANDLE value);
HANDLE      WINAPI RemoveWindowPropertyA(HWND hWnd, const char* name);
BOOL        WINAPI EnumWindowPropertiesA(HWND hWnd, const char* prefix);
BOOL        WINAPI EnumWindowPropertiesW(HWND hWnd, const wchar* prefix);

int         WINAPI GetWindowIntegerA(HWND hWnd, const char* name);
double      WINAPI GetWindowDoubleA(HWND hWnd, const char* name);
const char* WINAPI GetWindowStringA(HWND hWnd, const char* name);

BOOL        WINAPI SetWindowIntegerA(HWND hWnd, const char* name, int value);
BOOL        WINAPI SetWindowDoubleA(HWND hWnd, const char* name, double value);
BOOL        WINAPI SetWindowStringA(HWND hWnd, const char* name, const char* value);

int         WINAPI RemoveWindowIntegerA(HWND hWnd, const char* name);
double      WINAPI RemoveWindowDoubleA(HWND hWnd, const char* name);
char*       WINAPI RemoveWindowStringA(HWND hWnd, const char* name);

void        WINAPI ReleaseWindowProperties();
