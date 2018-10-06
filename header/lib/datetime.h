#pragma once
#include "expander.h"


// ticktimer data
struct TICK_TIMER_DATA {
   uint  id;                                                         // timer id
   HWND  hWnd;                                                       // the chart window receiving ticks
   DWORD flags;                                                      // flags for timer configuration
   DWORD userdata1;                                                  // storage for custom user data (cookies etc.)
   DWORD userdata2;                                                  // ...
   DWORD userdata3;                                                  // ...
};


datetime WINAPI GetGmtTime();
datetime WINAPI GetLocalTime();

uint     WINAPI SetupTickTimer(HWND hWnd, int millis, DWORD flags = NULL);
BOOL     WINAPI RemoveTickTimer(int timerId);
void     WINAPI RemoveTickTimers();
