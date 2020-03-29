#pragma once
#include "expander.h"


// ticktimer data
struct TICK_TIMER_DATA {
   uint  id;                                          // timer id
   HWND  hWnd;                                        // chart window to receive ticks
   DWORD flags;                                       // timer configuration
   DWORD userdata1;                                   // storage for user data (e.g. cookies)
   DWORD userdata2;                                   // ...
   DWORD userdata3;                                   // ...
};


uint WINAPI SetupTickTimer(HWND hWnd, uint millis, DWORD flags = NULL);
BOOL WINAPI RemoveTickTimer(uint timerId);
void WINAPI RemoveTickTimers();
