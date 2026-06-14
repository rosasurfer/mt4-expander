#pragma once
#include "expander.h"

void    WINAPI   SetupUiIntegration();
void    WINAPI   CustomizeTerminal();
BOOL    WINAPI   SubclassMainWindow(HWND hWndMain, BOOL onUiThread);
BOOL    WINAPI   SubclassChartWindows(HWND hWndMain, BOOL onUiThread);
LRESULT CALLBACK MainWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam,LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data);
LRESULT CALLBACK ChartWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam,LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data);
LRESULT CALLBACK UiHookProc(int code, WPARAM wParam, LPARAM lParam);
