#pragma once
#include "expander.h"

BOOL        WINAPI IsDebugBuild();

uint        WINAPI ComposeChartTitle(const char* symbol, uint timeframe, char* buffer, uint bufferSize);
char*       WINAPI GetInternalWindowTextA(HWND hWnd);
wchar*      WINAPI GetInternalWindowTextW(HWND hWnd);
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
const char* WINAPI MD5Hash(const void* input, uint length);
const char* WINAPI MD5HashA(const char* input);
uint        WINAPI MT4InternalMsg();
uint        WINAPI WM_MT4();

HANDLE      WINAPI GetWindowPropertyA(HWND hWnd, const char* name);
BOOL        WINAPI SetWindowPropertyA(HWND hWnd, const char* name, HANDLE value);
HANDLE      WINAPI RemoveWindowPropertyA(HWND hWnd, const char* name);

int         WINAPI GetWindowIntegerA(HWND hWnd, const char* name);
double      WINAPI GetWindowDoubleA(HWND hWnd, const char* name);
const char* WINAPI GetWindowStringA(HWND hWnd, const char* name);

BOOL        WINAPI SetWindowIntegerA(HWND hWnd, const char* name, int value);
BOOL        WINAPI SetWindowDoubleA(HWND hWnd, const char* name, double value);
BOOL        WINAPI SetWindowStringA(HWND hWnd, const char* name, const char* value);

int         WINAPI RemoveWindowIntegerA(HWND hWnd, const char* name);
double      WINAPI RemoveWindowDoubleA(HWND hWnd, const char* name);
const char* WINAPI RemoveWindowStringA(HWND hWnd, const char* name);

void        WINAPI ReleaseWindowProperties();
