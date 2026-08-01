#pragma once
#include "expander.h"

string WINAPI ChartTitleA(const string &symbol, uint timeframe, bool custom = false);
DWORD  WINAPI GetLastWin32Error();
DWORD  WINAPI GetPressedVirtualKeys(DWORD flags/*= F_VK_ALL*/);
BOOL   WINAPI IsCustomTimeframe(int timeframe);
BOOL   WINAPI IsDebugBuild();
BOOL   WINAPI IsStandardTimeframe(int timeframe);
BOOL   WINAPI IsProgramType(int type);
BOOL   WINAPI IsVirtualKeyDown(int key);
BOOL   WINAPI IsWindowAreaVisible(HWND hWnd);
