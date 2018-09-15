#pragma once

#include "expander.h"


uint WINAPI SetupTickTimer(HWND hWnd, int millis, DWORD flags=NULL);
BOOL WINAPI RemoveTickTimer(int timerId);
void WINAPI RemoveTickTimers();
