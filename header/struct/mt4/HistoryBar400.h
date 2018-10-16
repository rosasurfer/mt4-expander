#pragma once
#include "expander.h"


/**
 * MT4 struct HISTORY_BAR_400
 *
 * Bar format in history files and internal bar format of ArrayCopyRates().
 *
 * version id: 400 - standard in terminals up to build 509
 *
 * @see  https://docs.mql4.com/mql4changes
 */
#pragma pack(push, 1)

struct HISTORY_BAR_400 {                           // -- offset --- size --- description --------
   datetime time;                                  //         0        4     opentime
   double   open;                                  //         4        8
   double   low;                                   //        12        8
   double   high;                                  //        20        8
   double   close;                                 //        28        8
   double   ticks;                                 //        36        8     always an integer
};                                                 // -------------------------------------------
#pragma pack(pop)                                  //               = 44

typedef HISTORY_BAR_400 RateInfo;                  // MetaQuotes alias
