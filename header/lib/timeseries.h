#pragma once
#include "expander.h"
#include "struct/mt4/HistoryBar400.h"
#include "struct/mt4/HistoryBar401.h"


/**
 * Return the open price of a bar.
 *
 * @param  void* rates - bar timeseries with youngest prices at the end
 * @param  uint  bars  - number of bars in the series
 * @param  uint  bar   - bar offset to return
 *
 * @return double
 */
inline double iOpen(const void* rates, uint bars, uint bar) {
   static uint build = GetTerminalBuild();               // TODO: defeats inlining
   if (!build || bar < 0 || bar >= bars) return(NULL);

   uint shift = bars-1-bar;

   double open;
   if (build <= 509) open = ((HistoryBar400*) rates)[shift].open;
   else              open = ((HistoryBar401*) rates)[shift].open;
   return(open);
}


/**
 * Return the high price of a bar.
 *
 * @param  void* rates - bar timeseries with youngest prices at the end
 * @param  uint  bars  - number of bars in the series
 * @param  uint  bar   - bar offset to return
 *
 * @return double
 */
inline double WINAPI iHigh(const void* rates, uint bars, uint bar) {
   static uint build = GetTerminalBuild();               // TODO: defeats inlining
   if (!build || bar < 0 || bar >= bars) return(NULL);

   double high;
   uint shift = bars-1-bar;
   if (build <= 509) high = ((HistoryBar400*) rates)[shift].high;
   else              high = ((HistoryBar401*) rates)[shift].high;
   return(high);
}


/**
 * Return the low price of a bar.
 *
 * @param  void* rates - bar timeseries with youngest prices at the end
 * @param  uint  bars  - number of bars in the series
 * @param  uint  bar   - bar offset to return
 *
 * @return double
 */
inline double WINAPI iLow(const void* rates, uint bars, uint bar) {
   static uint build = GetTerminalBuild();               // TODO: defeats inlining
   if (!build || bar < 0 || bar >= bars) return(NULL);

   double low;
   uint shift = bars-1-bar;
   if (build <= 509) low = ((HistoryBar400*) rates)[shift].low;
   else              low = ((HistoryBar401*) rates)[shift].low;
   return(low);
}


/**
 * Return the close price of a bar.
 *
 * @param  void* rates - bar timeseries with youngest prices at the end
 * @param  uint  bars  - number of bars in the series
 * @param  uint  bar   - bar offset to return
 *
 * @return double
 */
inline double WINAPI iClose(const void* rates, uint bars, uint bar) {
   static uint build = GetTerminalBuild();               // TODO: defeats inlining
   if (!build || bar < 0 || bar >= bars) return(NULL);

   double close;
   uint shift = bars-1-bar;
   if (build <= 509) close = ((HistoryBar400*) rates)[shift].close;
   else              close = ((HistoryBar401*) rates)[shift].close;
   return(close);
}


/**
 * Return the volume of a bar.
 *
 * @param  void* rates - bar timeseries with youngest prices at the end
 * @param  uint  bars  - number of bars in the series
 * @param  uint  bar   - bar offset to return
 *
 * @return uint
 */
inline uint WINAPI iVolume(const void* rates, uint bars, uint bar) {
   static uint build = GetTerminalBuild();               // TODO: defeats inlining
   if (!build || bar < 0 || bar >= bars) return(NULL);

   uint volume;
   uint shift = bars-1-bar;
   if (build <= 509) volume = (uint)((HistoryBar400*) rates)[shift].ticks;
   else              volume = (uint)((HistoryBar401*) rates)[shift].ticks;
   return(volume);
}


/**
 * Return the open time of a bar.
 *
 * @param  void* rates - bar timeseries with youngest prices at the end
 * @param  uint  bars  - number of bars in the series
 * @param  uint  bar   - bar offset to return
 *
 * @return time32
 */
inline time32 WINAPI iTime(const void* rates, int bars, int bar) {
   static uint build = GetTerminalBuild();               // TODO: defeats inlining
   if (!build || bar < 0 || bar >= bars) return(NULL);

   time32 time;
   uint shift = bars-1-bar;
   if (build <= 509) time = ((HistoryBar400*) rates)[shift].time;
   else              time = ((HistoryBar401*) rates)[shift].time;
   return(time);
}
