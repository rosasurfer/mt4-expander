#include "expander.h"
#include "lib/helper.h"
#include "lib/timer.h"

#include <vector>


std::vector<TICK_TIMER_DATA> tickTimers;                 // all registered ticktimers


/**
 * Callback function for tick timer events.
 *
 * @param  TICK_TIMER_DATA* ttd        - tick timer configuration as passed to CreateTimerQueueTimer()
 * @param  BOOLEAN          timerFired - always TRUE for queued timer callbacks
 */
VOID CALLBACK TickTimerEvent(TICK_TIMER_DATA* ttd, BOOLEAN timerFired) {
   debug("tick timer event: id=%d  hWnd=%p  flags=%d", ttd->timerId, ttd->hWnd, ttd->flags);

   if (ttd->flags & TICK_IF_VISIBLE) {                // check if the chart is visible
      RECT rect;
      HDC hDC = GetDC(ttd->hWnd);
      int rgn = GetClipBox(hDC, &rect);
      ReleaseDC(ttd->hWnd, hDC);

      if (rgn == NULLREGION)                          // skip timer event if the chart is completely invisible
         return;
      if (rgn == RGN_ERROR) {
         warn(ERR_WIN32_ERROR+GetLastError(), "GetClipBox(hDC=%p) => RGN_ERROR", hDC);
         return;
      }
   }
   if (ttd->flags & TICK_PAUSE_ON_WEEKEND) {}         // skip timer event on weekends (not yet implemented)

   if      (ttd->flags & TICK_CHART_REFRESH) PostMessage(ttd->hWnd, WM_COMMAND, ID_CHART_REFRESH,     0);
   else if (ttd->flags & TICK_TESTER)        PostMessage(ttd->hWnd, WM_COMMAND, ID_CHART_STEPFORWARD, 0);
   else                                      PostMessage(ttd->hWnd, WM_MT4(), MT4_TICK, TICK_OFFLINE_EA);   // standard tick
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
   DWORD processId = NULL;
   DWORD threadId = GetWindowThreadProcessId(hWnd, &processId);
   if (!threadId)                                         return(error(ERR_INVALID_PARAMETER, "invalid parameter hWnd=%p (not a window)", hWnd));
   if (processId != GetCurrentProcessId())                return(error(ERR_INVALID_PARAMETER, "window hWnd=%p is not owned by the current process", hWnd));
   if ((int)millis <= 0)                                  return(error(ERR_INVALID_PARAMETER, "invalid parameter millis: %d", (int)millis));
   if (flags & TICK_CHART_REFRESH && flags & TICK_TESTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter flags: combination of TICK_CHART_REFRESH & TICK_TESTER"));
   if (flags & TICK_PAUSE_ON_WEEKEND)                     warn(ERR_NOT_IMPLEMENTED, "flag TICK_PAUSE_ON_WEEKEND not yet implemented");

   // generate a new timer id and store timer metadata
   static uint lastTimerId = 0;                             // simple counter
   uint timerId = ++lastTimerId;
   TICK_TIMER_DATA data = {timerId, NULL, hWnd, flags};
   tickTimers.push_back(data);                              // TODO: avoid push_back() creating a copy
   TICK_TIMER_DATA &ttd = tickTimers.back();

   // create the timer
   if (!CreateTimerQueueTimer(&ttd.hTimer, NULL, (WAITORTIMERCALLBACK)TickTimerEvent, (void*)&ttd, millis, millis, WT_EXECUTEINTIMERTHREAD))
      return(error(ERR_WIN32_ERROR+GetLastError(), "CreateTimerQueueTimer(interval=%d)", millis));

   debug("tick timer created: id=%d  hTimer=%p  interval=%d", ttd.timerId, ttd.hTimer, millis);
   return(ttd.timerId);
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

   uint size = tickTimers.size();

   for (uint i=0; i < size; i++) {
      TICK_TIMER_DATA &ttd = tickTimers[i];

      if (ttd.timerId == timerId) {
         if (HANDLE hTimer = ttd.hTimer) {
            ttd.hTimer = NULL;                           // reset handle to prevent multiple release errors
            DeleteTimerQueueTimer(NULL, hTimer, NULL) || error(ERR_WIN32_ERROR+GetLastError(), "DeleteTimerQueueTimer(timerId=%d, hTimer=%p)", timerId, hTimer);
            debug("tick timer removed: id=%d  hTimer=%p", timerId, hTimer);
         }
         else warn(ERR_ILLEGAL_STATE, "tick timer already released: id=%d", timerId);
         return(TRUE);
      }
   }

   return(warn(ERR_ILLEGAL_STATE, "tick timer not found: id=%d", timerId));
   #pragma EXPANDER_EXPORT
}


/**
 * Clean-up and release all unreleased tick timers. Called only in DLL::onProcessDetach().
 */
void WINAPI ReleaseTickTimers() {
   uint size = tickTimers.size();

   for (uint i=0; i < size; i++) {
      TICK_TIMER_DATA &ttd = tickTimers[i];

      if (ttd.hTimer) {
         warn(NO_ERROR, "releasing orphaned tick timer: id=%d", ttd.timerId);
         RemoveTickTimer(ttd.timerId);
      }
   }
}
