#pragma once

/**
 * MT4 struct HISTORY_BAR_400
 *
 * HistoryFile Barformat v400 (bis MetaTrader Build 509)
 *
 * @see  MetaQuotes::ExpertSample.cpp
 */
struct HISTORY_BAR_400 {                           // -- offset ---- size --- description --------
   uint   time;                                    //         0         4     OpenTime (timestamp)
   double open;                                    //         4         8
   double low;                                     //        12         8
   double high;                                    //        20         8
   double close;                                   //        28         8
   double ticks;                                   //        36         8     immer Ganzzahl
};                                                 // --------------------------------------------
                                                   //                = 44

typedef HISTORY_BAR_400 RateInfo;                  // MetaQuotes-Terminologie
