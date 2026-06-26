#pragma once
#include "expander.h"

DWORD  WINAPI GetLastWin32Error();
DWORD  WINAPI GetPressedVirtualKeys(DWORD flags/*= F_VK_ALL*/);
DWORD  WINAPI GetUiThreadId();
BOOL   WINAPI IsCustomTimeframe(int timeframe);
BOOL   WINAPI IsDebugBuild();
BOOL   WINAPI IsStandardTimeframe(int timeframe);
BOOL   WINAPI IsProgramType(int type);
BOOL   WINAPI IsUiThread(DWORD threadId = NULL);
BOOL   WINAPI IsVirtualKeyDown(int key);
BOOL   WINAPI IsWindowAreaVisible(HWND hWnd);
string WINAPI MakeChartTitleA(const string &symbol, uint timeframe, bool custom = false);
uint   WINAPI MT4InternalMsg();
uint   WINAPI MT4ExpanderMsg();
uint   WINAPI WM_MT4();
uint   WINAPI WM_MT4X();
