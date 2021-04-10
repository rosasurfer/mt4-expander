#pragma once
#include "expander.h"


#pragma pack(push, 1)
/**
 * MT4 struct HISTORY_BAR_400
 *
 * Default bar format in history files up to build 509, and internal bar format of ArrayCopyRates().
 *
 * @see  https://docs.mql4.com/mql4changes
 */
struct HISTORY_BAR_400 {                           // -- offset --- size --- description --------
   datetime time;                                  //         0        4     opentime
   double   open;                                  //         4        8
   double   low;                                   //        12        8
   double   high;                                  //        20        8
   double   close;                                 //        28        8
   double   ticks;                                 //        36        8     always an integer
};                                                 // -------------------------------------------
#pragma pack(pop)                                  //               = 44

typedef HISTORY_BAR_400 HistoryBar400;
typedef HISTORY_BAR_400 RateInfo;                  // MetaQuotes alias
