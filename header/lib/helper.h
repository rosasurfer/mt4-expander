#pragma once
#include "expander.h"


uint   WINAPI GetBoolsAddress(const BOOL values[]);
uint   WINAPI GetChartDescription(const char* symbol, uint timeframe, char* buffer, uint bufferSize);
uint   WINAPI GetDoublesAddress(const double values[]);
uint   WINAPI GetIntsAddress(const int values[]);
int    WINAPI GetLastWin32Error();
DWORD  WINAPI GetUIThreadId();
HANDLE WINAPI GetWindowProperty(HWND hWnd, const char* lpName);
BOOL   WINAPI IsCustomTimeframe(int timeframe);
BOOL   WINAPI IsStdTimeframe(int timeframe);
BOOL   WINAPI IsUIThread(DWORD threadId = NULL);
char*  WINAPI MD5Hash(const void* input, uint length);
char*  WINAPI MD5HashA(const char* input);
uint   WINAPI MT4InternalMsg();
HANDLE WINAPI RemoveWindowProperty(HWND hWnd, const char* lpName);
BOOL   WINAPI SetWindowProperty(HWND hWnd, const char* lpName, HANDLE value);
BOOL   WINAPI ShiftIndicatorBuffer(double buffer[], int bufferSize, int bars, double emptyValue);
uint   WINAPI WM_MT4();
