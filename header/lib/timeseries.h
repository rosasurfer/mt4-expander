#pragma once
#include "expander.h"
#include "struct/mt4/PriceBar400.h"
#include "struct/mt4/PriceBar401.h"


extern uint g_terminalBuild;              // terminal build number


/**
 * Return the open price of a bar.
 *
 * @param  void* rates - bar timeseries with youngest prices at the end
 * @param  uint  bars  - number of bars in the series
 * @param  uint  bar   - bar offset to return
 *
 * @return double
 */
inline double WINAPI iOpen(const void* rates, uint bars, uint bar) {
   if (bar < 0 || bar >= bars) return(NULL);

   double open;
   uint shift = bars-1-bar;
   if (g_terminalBuild <= 509) open = ((RateInfo*) rates)[shift].open;
   else                        open = ((MqlRates*) rates)[shift].open;
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
   if (bar < 0 || bar >= bars) return(NULL);

   double high;
   uint shift = bars-1-bar;
   if (g_terminalBuild <= 509) high = ((RateInfo*) rates)[shift].high;
   else                        high = ((MqlRates*) rates)[shift].high;
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
   if (bar < 0 || bar >= bars) return(NULL);

   double low;
   uint shift = bars-1-bar;
   if (g_terminalBuild <= 509) low = ((RateInfo*) rates)[shift].low;
   else                        low = ((MqlRates*) rates)[shift].low;
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
   if (bar < 0 || bar >= bars) return(NULL);

   double close;
   uint shift = bars-1-bar;
   if (g_terminalBuild <= 509) close = ((RateInfo*) rates)[shift].close;
   else                        close = ((MqlRates*) rates)[shift].close;
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
   if (bar < 0 || bar >= bars) return(NULL);

   uint volume;
   uint shift = bars-1-bar;
   if (g_terminalBuild <= 509) volume = (uint)((RateInfo*) rates)[shift].ticks;
   else                        volume = (uint)((MqlRates*) rates)[shift].ticks;
   return(volume);
}


/**
 * Return the open time of a bar.
 *
 * @param  void* rates - bar timeseries with youngest prices at the end
 * @param  uint  bars  - number of bars in the series
 * @param  uint  bar   - bar offset to return
 *
 * @return datetime
 */
inline datetime WINAPI iTime(const void* rates, int bars, int bar) {
   if (bar < 0 || bar >= bars) return(NULL);

   datetime time;
   uint shift = bars-1-bar;
   if (g_terminalBuild <= 509) time = ((RateInfo*) rates)[shift].time;
   else                        time = ((MqlRates*) rates)[shift].time;
   return(time);
}
