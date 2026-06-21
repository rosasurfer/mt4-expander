#pragma once
#include "expander.h"

BOOL           WINAPI   SetupUiIntegration();
static BOOL    WINAPI   RegisterWindowEventHook();
static LRESULT CALLBACK WindowEventHook(int type, WPARAM wParam, LPARAM lParam);

static BOOL    WINAPI   NotifyUiThread();
static LRESULT CALLBACK UiThreadHook(int code, WPARAM wParam, LPARAM lParam);

static BOOL    WINAPI   SubclassMainWindow();
static LRESULT CALLBACK MainWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam,LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data);

static BOOL    WINAPI   SubclassChartWindows();
static LRESULT CALLBACK ChartWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam,LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data);

static BOOL    WINAPI   CustomizeTerminal();
