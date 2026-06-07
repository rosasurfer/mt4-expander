#pragma once
#include "expander.h"

static BOOL    WINAPI   onProcessAttach();
static BOOL    WINAPI   onProcessDetach(BOOL isTerminating);
static DWORD   WINAPI   ExpanderStartThread(void* lpParam);
static DWORD   WINAPI   SessionMonitorThread(void* lpParam);
static LRESULT CALLBACK SessionMonitorWndProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);

BOOL WINAPI PinDllToMemory();
