#pragma once
#include "expander.h"

#pragma pack(push, 1)


/**
 * MT4 struct TICK (file format of "ticks.raw")
 */
struct TICK {                                      // -- offset ---- size --- description ----------------------------------------------------------------------------
   char     symbol[MAX_SYMBOL_LENGTH+1];           //         0        12     symbol
   datetime time;                                  //        12         4     timestamp
   double   bid;                                   //        16         8     bid
   double   ask;                                   //        24         8     ask
   uint     counter;                               //        32         4     consecutive counter for each tick of the file
   BYTE     unknown[4];                            //        36         4     ?
};                                                 // ----------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //                = 40
