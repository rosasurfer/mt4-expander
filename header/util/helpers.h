#pragma once

#include "expander.h"
#include <istream>

uint          WINAPI GetBoolsAddress(const BOOL values[]);
uint          WINAPI GetIntsAddress(const int values[]);
uint          WINAPI GetDoublesAddress(const double values[]);
int           WINAPI GetLastWin32Error();
BOOL          WINAPI IsStdTimeframe(int timeframe);
BOOL          WINAPI IsCustomTimeframe(int timeframe);
HWND          WINAPI GetApplicationWindow();
DWORD         WINAPI GetUIThreadId();
BOOL          WINAPI IsUIThread();
HANDLE        WINAPI GetWindowProperty(HWND hWnd, const char* lpName);
HANDLE        WINAPI RemoveWindowProperty(HWND hWnd, const char* lpName);
BOOL          WINAPI SetWindowProperty(HWND hWnd, const char* lpName, HANDLE value);
BOOL          WINAPI ShiftIndicatorBuffer(double buffer[], int bufferSize, int bars, double emptyValue);
BOOL          WINAPI GetTerminalVersion(uint* major, uint* minor, uint* hotfix, uint* build);
const char*   WINAPI GetTerminalVersion();
uint          WINAPI GetTerminalBuild();
uint          WINAPI GetChartDescription(const char* symbol, uint timeframe, char* buffer, uint bufferSize);
const string& WINAPI getTerminalPath();
uint          WINAPI MT4InternalMsg();
std::istream&        getLine(std::istream& is, string& line);
