#pragma once
#include "expander.h"


/**
 * MT4 struct HISTORY_BAR_401
 *
 * Bar format in history files and internal bar format of ArrayCopyRates().
 *
 * version id: 401 - default in terminals since build 600+
 *
 * @see  https://docs.mql4.com/mql4changes
 */
#pragma pack(push, 1)

struct HISTORY_BAR_401 {                           // -- offset --- size --- description ---------
   int64  time;                                    //         0        8     opentime (timestamp)
   double open;                                    //         8        8
   double high;                                    //        16        8
   double low;                                     //        24        8
   double close;                                   //        32        8
   uint64 ticks;                                   //        40        8     always an integer
   int    spread;                                  //        48        4     (reserved)
   uint64 volume;                                  //        52        8     (reserved)
};                                                 // --------------------------------------------
#pragma pack(pop)                                  //               = 60

typedef HISTORY_BAR_401 MqlRates;                  // MetaQuotes alias
