#include "expander.h"
#include "lib/helper.h"
#include "lib/timer.h"

#include <vector>


extern CRITICAL_SECTION       g_terminalMutex;           // mutex for application-wide locking
std::vector<TICK_TIMER_DATA*> g_tickTimers;              // all registered ticktimers


/**
 * Callback function for tick timer events.
 *
 * @param  TICK_TIMER_DATA* ttd        - tick timer configuration as passed to CreateTimerQueueTimer()
 * @param  BOOLEAN          timerFired - always TRUE for queued timer callbacks
 */
VOID CALLBACK onTickTimerEvent(TICK_TIMER_DATA* ttd, BOOLEAN timerFired) {
   if (!ttd->hTimer) return;                             // skip queued events of an already released timer

   if (IsWindow(ttd->hWnd)) {
      if (ttd->flags & TICK_IF_WINDOW_VISIBLE) {         // check if the chart is visible
         RECT rect;
         HDC hDC = GetDC(ttd->hWnd);
         int rgn = GetClipBox(hDC, &rect);
         ReleaseDC(ttd->hWnd, hDC);

         if (rgn == NULLREGION) {                        // skip timer event if the chart is completely invisible
            return;
         }
         if (rgn == RGN_ERROR) {
            warn(ERR_WIN32_ERROR+GetLastError(), "GetClipBox(hDC=%p) => RGN_ERROR", hDC);
            return;
         }
      }
      if (ttd->flags & TICK_PAUSE_ON_WEEKEND) {}         // skip timer event on weekends (not yet implemented)

      if      (ttd->flags & TICK_CHART_REFRESH) PostMessage(ttd->hWnd, WM_COMMAND, ID_CHART_REFRESH,     0);   // triggers indicators but not experts
      else if (ttd->flags & TICK_TESTER)        PostMessage(ttd->hWnd, WM_COMMAND, ID_CHART_STEPFORWARD, 0);
      else                                      PostMessage(ttd->hWnd, WM_MT4(), MT4_TICK, TICK_OFFLINE_EA);   // triggers indicators and experts in online/offline charts
   }
   else {
      // expected case if an MQL program crashes and fails to release its resources
      debug("releasing unreleased tick timer with id=%d (references non-existing window hWnd=%p, did the MQL program crash?)", ttd->timerId, ttd->hWnd);
      ReleaseTickTimer(ttd->timerId);
   }
}


/**
 * Register a timer to send virtual price ticks to the specified chart window.
 *
 * @param  HWND  hWnd   - handle of the window to receive virtual ticks
 * @param  uint  millis - time interval of the virtual ticks in millicesonds
 * @param  DWORD flags  - tick configuration flags (default: standard ticks for experts and indicators)
 *                        TICK_CHART_REFRESH:     send command ID_CHART_REFRESH instead of standard ticks (for synthetic and
 *                                                offline charts)
 *                        TICK_TESTER:            send command ID_CHART_STEPFORWARD instead of standard ticks (for tester)
 *                        TICK_IF_WINDOW_VISIBLE: send ticks only if the receiving window is visible (saving of resources)
 *                        TICK_PAUSE_ON_WEEKEND:  skip sending ticks during sessionbreaks (saving of resources, not yet implemented)
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
   if (flags & TICK_CHART_REFRESH && flags & TICK_TESTER) return(error(ERR_INVALID_PARAMETER, "invalid combination in parameter flags: TICK_CHART_REFRESH & TICK_TESTER"));
   if (flags & TICK_PAUSE_ON_WEEKEND)                     warn(ERR_NOT_IMPLEMENTED, "flag TICK_PAUSE_ON_WEEKEND not yet implemented");

   // generate a new timer id and timer metadata
   if (!TryEnterCriticalSection(&g_terminalMutex)) {
      debug("waiting to aquire lock on g_terminalMutex...");
      EnterCriticalSection(&g_terminalMutex);
   }
   static uint lastTimerId = 0;                                // a simple counter
   uint timerId = ++lastTimerId;

   TICK_TIMER_DATA* ttd = new TICK_TIMER_DATA();
   ttd->timerId  = timerId;
   ttd->hTimer   = NULL;
   ttd->interval = millis;
   ttd->hWnd     = hWnd;
   ttd->flags    = flags;
   g_tickTimers.push_back(ttd);                                // may re-allocate, thus needs to be synchronized
   LeaveCriticalSection(&g_terminalMutex);

   // create the timer
   if (!CreateTimerQueueTimer(&ttd->hTimer, NULL, (WAITORTIMERCALLBACK)onTickTimerEvent, (void*)ttd, millis, millis, WT_EXECUTEINTIMERTHREAD))
      return(error(ERR_WIN32_ERROR+GetLastError(), "CreateTimerQueueTimer(interval=%d)", millis));

   return(ttd->timerId);
   #pragma EXPANDER_EXPORT
}


/**
 * Release a single tick timer.
 *
 * @param  uint timerId - timer id as returned by SetupTickTimer()
 *
 * @return BOOL - success status
 */
BOOL WINAPI ReleaseTickTimer(uint timerId) {
   if ((int)timerId <= 0) return(error(ERR_INVALID_PARAMETER, "invalid parameter timerId: %d", timerId));

   // The timer is released and marked as invalid. The vector holding all timer entries is not modified.
   uint size = g_tickTimers.size();

   for (uint i=0; i < size; i++) {
      TICK_TIMER_DATA* ttd = g_tickTimers[i];

      if (ttd->timerId == timerId) {
         if (HANDLE hTimer = ttd->hTimer) {
            ttd->hTimer = NULL;                                // reset handle to prevent multiple release errors

            if (!DeleteTimerQueueTimer(NULL, hTimer, NULL)) {  // ERROR_IO_PENDING: "Overlapped I/O operation in progress" is not an error
               DWORD error = GetLastError();                   // but a status. It says that there is still an operation in progress.
               if (error != ERROR_IO_PENDING) error(ERR_WIN32_ERROR+error, "DeleteTimerQueueTimer(timerId=%d, hTimer=%p)", timerId, hTimer);
            }
         }
         else warn(ERR_ILLEGAL_STATE, "tick timer has already been released: id=%d", timerId);
         return(TRUE);
      }
   }
   return(!warn(ERR_ILLEGAL_STATE, "tick timer not found: id=%d", timerId));
   #pragma EXPANDER_EXPORT
}


/**
 * Clean-up and release all unreleased tick timers. Called only in DLL::onProcessDetach().
 */
void WINAPI ReleaseTickTimers() {
   uint size = g_tickTimers.size();

   for (uint i=0; i < size; i++) {
      TICK_TIMER_DATA* ttd = g_tickTimers[i];

      if (ttd->hTimer) {
         warn(NO_ERROR, "releasing unreleased tick timer: id=%d", ttd->timerId);
         ReleaseTickTimer(ttd->timerId);
      }
   }
}
