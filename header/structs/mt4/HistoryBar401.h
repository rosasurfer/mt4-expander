#pragma once

/**
 * MT4 struct HISTORY_BAR_401
 *
 * HistoryFile Barformat v401 (ab MetaTrader Build 510)
 *
 * @see  https://docs.mql4.com/mql4changes
 */
struct HISTORY_BAR_401 {                           // -- offset ---- size --- description ---------
   int64  time;                                    //         0         8     Open-Time (timestamp)
   double open;                                    //         8         8
   double high;                                    //        16         8
   double low;                                     //        24         8
   double close;                                   //        32         8
   uint64 ticks;                                   //        40         8
   int    spread;                                  //        48         4     (unbenutzt)
   uint64 volume;                                  //        52         8     (unbenutzt)
};                                                 // ---------------------------------------------
                                                   //                = 60

typedef HISTORY_BAR_401 MqlRates;                  // MetaQuotes-Terminologie
