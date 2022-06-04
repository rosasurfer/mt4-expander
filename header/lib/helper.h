#pragma once
#include "expander.h"


uint         WINAPI GetChartDescription(const char* symbol, uint timeframe, char* buffer, uint bufferSize);
char*        WINAPI GetInternalWindowTextA(HWND hWnd);
wchar*       WINAPI GetInternalWindowTextW(HWND hWnd);
char*        WINAPI GetWindowTextA(HWND hWnd);
wchar*       WINAPI GetWindowTextW(HWND hWnd);
int          WINAPI GetLastWin32Error();
DWORD        WINAPI GetUIThreadId();
BOOL         WINAPI IsAsyncKeyDown(int vKey);
BOOL         WINAPI IsCustomTimeframe(int timeframe);
BOOL         WINAPI IsStdTimeframe(int timeframe);
BOOL         WINAPI IsProgramType(int type);
BOOL         WINAPI IsUIThread(DWORD threadId = NULL);
const char*  WINAPI MD5Hash(const void* input, uint length);
const char*  WINAPI MD5HashA(const char* input);
uint         WINAPI MT4InternalMsg();
uint         WINAPI WM_MT4();

BOOL         WINAPI SetWindowIntegerA(HWND hWnd, const char* name, int value);
BOOL         WINAPI SetWindowDoubleA(HWND hWnd, const char* name, double value);
BOOL         WINAPI SetWindowStringA(HWND hWnd, const char* name, const char* value);

int          WINAPI GetWindowIntegerA(HWND hWnd, const char* name);
double       WINAPI GetWindowDoubleA(HWND hWnd, const char* name);
const char*  WINAPI GetWindowStringA(HWND hWnd, const char* name);

int          WINAPI RemoveWindowIntegerA(HWND hWnd, const char* name);
double       WINAPI RemoveWindowDoubleA(HWND hWnd, const char* name);
const char*  WINAPI RemoveWindowStringA(HWND hWnd, const char* name);

void         WINAPI ReleaseWindowProperties();
