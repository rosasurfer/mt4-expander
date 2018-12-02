#pragma once
#include "expander.h"


/**
 * MT4 struct PRICE_BAR_401
 *
 * Bar format in history files and internal bar format of ArrayCopyRates().
 *
 * version id: 401 - default in terminals since build 600+
 *
 * @see  https://docs.mql4.com/mql4changes
 */
#pragma pack(push, 1)
struct PRICE_BAR_401 {                             // -- offset --- size --- description --------------------
   datetime time;                                  //         0        8     (MetaQuotes: low part of int64)
   DWORD    _reserved1;                            //         4        4     (MetaQuotes: high part of int64)
   double   open;                                  //         8        8
   double   high;                                  //        16        8
   double   low;                                   //        24        8
   double   close;                                 //        32        8
   uint     ticks;                                 //        40        4     (MetaQuotes: low part of int64)
   DWORD    _reserved2;                            //        44        4     (MetaQuotes: high part of int64)
   int      spread;                                //        48        4     (unused)
   uint64   volume;                                //        52        8     (unused)
};                                                 // -------------------------------------------------------
#pragma pack(pop)                                  //               = 60

typedef PRICE_BAR_401 MqlRates;                    // MetaQuotes alias
