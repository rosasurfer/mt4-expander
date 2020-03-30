#pragma once
#include "expander.h"


// ticktimer metadata
struct TICK_TIMER_DATA {
   uint   timerId;                              // timer id
   HANDLE hTimer;                               // timer handle if the timer is a queued timer, NULL otherwise
   uint   interval;                             // timer interval in milliseconds
   HWND   hWnd;                                 // chart window to receive virtual ticks
   DWORD  flags;                                // tick configuration
};


uint WINAPI SetupTickTimer(HWND hWnd, uint millis, DWORD flags = NULL);
BOOL WINAPI RemoveTickTimer(uint timerId);
void WINAPI ReleaseTickTimers();
