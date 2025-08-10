#pragma once
#include "expander.h"


#pragma pack(push, 1)
/**
 * MT4 struct HISTORY_BAR_401
 *
 * Bar format in history files of terminal builds > 509, and internal bar format of ArrayCopyRates().
 *
 * @see  https://docs.mql4.com/mql4changes
 */
struct HISTORY_BAR_401 {                        // -- offset --- size --- description -----------
   time32 time;                                 //         0        4     low int32 of int64
   DWORD  _reserved1;                           //         4        4     high int32 of int64
   double open;                                 //         8        8
   double high;                                 //        16        8
   double low;                                  //        24        8
   double close;                                //        32        8
   uint   ticks;                                //        40        4     low int32 of int64
   DWORD  _reserved2;                           //        44        4     high int32 of int64
   int    spread;                               //        48        4     (unused)
   uint64 volume;                               //        52        8     (unused)
};                                              // ----------------------------------------------
#pragma pack(pop)                               //               = 60

typedef HISTORY_BAR_401 HistoryBar401;
typedef HISTORY_BAR_401 MqlRates;               // MetaQuotes alias
