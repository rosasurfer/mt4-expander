#include "expander.h"
#include "lib/helper.h"
#include "lib/timer.h"

#include <vector>


std::vector<TICK_TIMER_DATA> tickTimers;                 // all registered ticktimers


/**
 * Callback function for custom WM_TIMER messages.
 *
 * @param  HWND     hWnd    - handle of the window associated with the timer
 * @param  UINT     msg     - WM_TIMER message
 * @param  UINT_PTR timerId - the timer's identifier
 * @param  DWORD    time    - timer event creation time in system ticks as returned by GetTickCount()
 */
VOID CALLBACK TimerCallback(HWND hWnd, UINT msg, UINT_PTR timerId, DWORD time) {
   int size = tickTimers.size();
   GetTickCount();

   for (int i=0; i < size; i++) {
      if (tickTimers[i].id == timerId) {
         TICK_TIMER_DATA &ttd = tickTimers[i];

         if (ttd.flags & TICK_IF_VISIBLE) {              // check if the chart is visible
            RECT rect;
            HDC hDC = GetDC(hWnd);
            int rgn = GetClipBox(hDC, &rect);
            ReleaseDC(hWnd, hDC);

            if (rgn == NULLREGION)                       // skip timer event if the chart is completely invisible
               return;
            if (rgn == RGN_ERROR) {
               warn(ERR_WIN32_ERROR+GetLastError(), "GetClipBox(hDC=%p) => RGN_ERROR", hDC);
               return;
            }
         }
         if (ttd.flags & TICK_PAUSE_ON_WEEKEND) {}       // skip timer event on weekends: not yet implemented

         if      (ttd.flags & TICK_CHART_REFRESH) PostMessage(hWnd, WM_COMMAND, ID_CHART_REFRESH, 0);
         else if (ttd.flags & TICK_TESTER)        PostMessage(hWnd, WM_COMMAND, ID_CHART_STEPFORWARD, 0);
         else                                     PostMessage(hWnd, WM_MT4(), MT4_TICK, TICK_OFFLINE_EA);   // a standard tick
         return;
      }
   }
   warn(ERR_RUNTIME_ERROR, "timer not found, id: %d", timerId);
}


/**
 * Register a timer to send virtual price ticks to the specified chart window.
 *
 * @param  HWND  hWnd   - handle of the window to receive virtual ticks
 * @param  uint  millis - time interval of the virtual ticks in millicesonds
 * @param  DWORD flags  - tick configuration flags (default: standard ticks for experts and indicators)
 *                        TICK_CHART_REFRESH:    send command ID_CHART_REFRESH instead of standard ticks (for synthetic and
 *                                               offline charts)
 *                        TICK_TESTER:           send command ID_CHART_STEPFORWARD instead of standard ticks (for Strategy Tester)
 *                        TICK_IF_VISIBLE:       send ticks only if the chart is visible (saving of resources)
 *                        TICK_PAUSE_ON_WEEKEND: skip sending ticks during sessionbreaks (saving of resources, not yet implemented)
 *
 * @return uint - identifier of the registered timer or 0 (zero) in case of errors
 */
uint WINAPI SetupTickTimer(HWND hWnd, uint millis, DWORD flags/*=NULL*/) {
   // validate parameters
   DWORD wndThreadId = GetWindowThreadProcessId(hWnd, NULL);
   if (wndThreadId != GetCurrentThreadId()) {
      if (!wndThreadId)                                   return(error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: %p (not a window)", hWnd));
                                                          return(error(ERR_INVALID_PARAMETER, "window hWnd=%p not owned by the current thread", hWnd));
   }
   if ((int)millis <= 0)                                  return(error(ERR_INVALID_PARAMETER, "invalid parameter millis: %d", (int)millis));
   if (flags & TICK_CHART_REFRESH && flags & TICK_TESTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter flags combination: TICK_CHART_REFRESH & TICK_TESTER"));
   if (flags & TICK_PAUSE_ON_WEEKEND)                     warn(ERR_NOT_IMPLEMENTED, "flag TICK_PAUSE_ON_WEEKEND not yet implemented");

   // generate a timer id (starting at 10000)
   static uint timerId = 10000;
   timerId++;

   // set the timer
   uint result = SetTimer(hWnd, timerId, millis, (TIMERPROC)TimerCallback);
   if (result != timerId)                             // equal if hWnd is set
      return(error(ERR_WIN32_ERROR+GetLastError(), "SetTimer(hWnd=%p, timerId=%d, millis=%d) => %d", hWnd, timerId, millis, result));
   //debug("SetTimer(hWnd=%d, timerId=%d, millis=%d) success", hWnd, timerId, millis);

   // store timer metadata
   TICK_TIMER_DATA ttd = {timerId, hWnd, flags};
   tickTimers.push_back(ttd);                         // TODO: avoid push_back() creating a copy

   return(timerId);
   #pragma EXPANDER_EXPORT
}


/**
 * Remove a tick timer.
 *
 * @param  uint timerId - timer id as returned by SetupTickTimer()
 *
 * @return BOOL - success status
 */
BOOL WINAPI RemoveTickTimer(uint timerId) {
   if ((int)timerId <= 0) return(error(ERR_INVALID_PARAMETER, "invalid parameter timerId: %d", timerId));

   uint timersSize = tickTimers.size();

   for (uint i=0; i < timersSize; i++) {
      if (tickTimers[i].id == timerId) {
         if (!KillTimer(tickTimers[i].hWnd, timerId))
            return(error(ERR_WIN32_ERROR+GetLastError(), "KillTimer(hWnd=%p, timerId=%d)", tickTimers[i].hWnd, timerId));
         tickTimers.erase(tickTimers.begin() + i);
         return(TRUE);
      }
   }

   return(error(ERR_RUNTIME_ERROR, "timer not found, id: %d", timerId));
   #pragma EXPANDER_EXPORT
}


/**
 * Clean-up and remove all remaining timers (forgotten to release). Called only in DLL::onProcessDetach().
 */
void WINAPI RemoveTickTimers() {
   uint timersSize = tickTimers.size();

   for (uint i=timersSize-1; i>=0; i--) {             // iterate backwards as RemoveTickTimer() modifies the vector
      uint id = tickTimers[i].id;
      warn(NO_ERROR, "removing orphaned tick timer (id: %d)", id);
      RemoveTickTimer(id);
   }
}
