#pragma once
#include "expander.h"

#pragma pack(push, 1)


/**
 * MT4 struct TICK (Dateiformat "ticks.raw")
 */
struct TICK {                                      // -- offset ---- size --- description ----------------------------------------------------------------------------
   char     symbol[MAX_SYMBOL_LENGTH+1];           //         0        12     Symbol (szchar)
   datetime time;                                  //        12         4     Timestamp
   double   bid;                                   //        16         8     Bid
   double   ask;                                   //        24         8     Ask
   uint     counter;                               //        32         4     fortlaufender Zähler innerhalb der Datei
   BYTE     unknown[4];                            //        36         4     ?
};                                                 // ----------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //                = 40
