#include "expander.h"
#include "lib/helper.h"


/**
 * Get the height of the specified chart.
 *
 * @param  HWND hChart     - chart handle as returned by MQL::WindowHandle()
 * @param  int  lastHeight -
 *
 * @return int - chart height or NULL (0) if the chart is not visible; also in case of errors
 */
int WINAPI Grid_GetChartHeight(HWND hChart, int lastHeight) {
   HWND hChartWnd = GetParent(hChart);
   if (!hChartWnd) return(!error(ERR_INVALID_PARAMETER, "invalid parameter hChart: 0x%p (not a chart)", hChart));

   if (!IsWindowVisible(hChartWnd)) {
      return(NULL);
   }
   if (lastHeight) {
      if (IsIconic(hChartWnd) || !IsWindowAreaVisible(hChartWnd)) {
         return(NULL);                                // quick return only if the chart was already initialized (performance)
      }
   }

   RECT rect;
   if (!GetWindowRect(hChart, &rect)) return(!error(ERR_WIN32_ERROR+GetLastError(), "->GetWindowRect()"));

   return(rect.bottom - rect.top);                    // on (height == 0) the view port was resized to zero height
   #pragma EXPANDER_EXPORT
}
