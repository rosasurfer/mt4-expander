#pragma once
#include "expander.h"


#pragma pack(push, 1)
/**
 * MT4 struct HISTORY_BAR_401
 *
 * Bar format in history files of terminal builds > 509, and internal bar format of ArrayCopyRates().
 *
 * @see  https://docs.mql4.com/mql4changes# [partially inaccurate]
 */
struct HISTORY_BAR_401 {                        // -- offset --- size --- description -----------
   union {                                      //         0        8     opentime
      struct {                                  //
         time32 time;                           //         0        4     32-bit datetime value
         DWORD  reserved1;                      //         4        4     high 32 bits
      };                                        //
      time64 time_ex;                           //         0        8     64-bit datetime value
   };                                           //
   double open;                                 //         8        8
   double high;                                 //        16        8
   double low;                                  //        24        8
   double close;                                //        32        8
   union {                                      //        40        8
      struct {                                  //
         uint  ticks;                           //        40        4      low 32 bits
         DWORD reserved2;                       //        44        4      high 32 bits
      };                                        //
      uint64 tickVolume;                        //        40        8      full 64-bit value
   };                                           //
   int    spread;                               //        48        4     (unused)
   uint64 realVolume;                           //        52        8     (unused)
};                                              // ----------------------------------------------
#pragma pack(pop)                               //               = 60

typedef HISTORY_BAR_401 HistoryBar401;
typedef HISTORY_BAR_401 MqlRates;               // MetaQuotes alias
