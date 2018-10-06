#pragma once
#include "expander.h"


/**
 * MT4 struct HISTORY_BAR_400
 *
 * HistoryFile Barformat v400 (bis MetaTrader Build 509)
 *
 * @see  MetaQuotes::ExpertSample.cpp
 */
#pragma pack(push, 1)
struct HISTORY_BAR_400 {                           // -- offset ---- size --- description --------
   datetime time;                                  //         0         4     OpenTime (timestamp)
   double   open;                                  //         4         8
   double   low;                                   //        12         8
   double   high;                                  //        20         8
   double   close;                                 //        28         8
   double   ticks;                                 //        36         8     immer Ganzzahl
};                                                 // --------------------------------------------
#pragma pack(pop)                                  //                = 44

typedef HISTORY_BAR_400 RateInfo;                  // MetaQuotes-Terminologie
