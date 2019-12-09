#pragma once
#include "expander.h"

#pragma pack(push, 1)


/**
 * MT4 struct HISTORY_BAR_401
 *
 * Default bar format in history files since builds > 509, and internal bar format of ArrayCopyRates().
 *
 * @see  https://docs.mql4.com/mql4changes
 */
struct HISTORY_BAR_401 {                           // -- offset --- size --- description --------------------
   datetime time;                                  //         0        8     MetaQuotes: low part of int64
   DWORD    _reserved1;                            //         4        4     MetaQuotes: high part of int64
   double   open;                                  //         8        8
   double   high;                                  //        16        8
   double   low;                                   //        24        8
   double   close;                                 //        32        8
   uint     ticks;                                 //        40        4     MetaQuotes: low part of int64
   DWORD    _reserved2;                            //        44        4     MetaQuotes: high part of int64
   int      spread;                                //        48        4     (unused)
   uint64   volume;                                //        52        8     (unused)
};                                                 // -------------------------------------------------------
#pragma pack(pop)                                  //               = 60

typedef HISTORY_BAR_401 HistoryBar401;             //
//typedef HISTORY_BAR_401 MqlRates;                // MetaQuotes alias
