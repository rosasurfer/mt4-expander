#pragma once
#include "expander.h"

#define PROP_WINDOW_SUBCLASSED L"rsfMT4Expander.subclassed"


void           WINAPI   IntegrateExpander();
static BOOL    WINAPI   CustomizeTerminal();

static BOOL    WINAPI   HookUiThread();
static LRESULT CALLBACK UiThreadHookProc(int code, WPARAM wParam, LPARAM lParam);

static BOOL    WINAPI   HookWindowEvents();
static LRESULT CALLBACK WindowEventsHookProc(int type, WPARAM wParam, LPARAM lParam);

static BOOL    WINAPI   SubclassMainWindow();
static LRESULT CALLBACK MainWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam,LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data);

static BOOL    WINAPI   SubclassChartWindows();
static BOOL    WINAPI   SubclassChartWindow(HWND hWnd);
static LRESULT CALLBACK ChartWindowSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data);

static BOOL    WINAPI   SubclassChartFrame(HWND hWndChart, HWND hWndChartFrame);
static LRESULT CALLBACK ChartFrameSubclassProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR data);
