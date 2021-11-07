#pragma once
#include "expander.h"


#pragma pack(push, 1)
/**
 * MT4 struct SYMBOL_SELECTED (file format "symbols.sel")
 */
struct SYMBOL_SELECTED {                           // -- offset ---- size --- description -----------------------------------
   char     symbol[MAX_SYMBOL_LENGTH+1];           //         0        12     symbol (szchar)
   uint     digits;                                //        12         4     digits

   uint     symbolIndex;                           //        16         4     field SYMBOL.index (@see "symbols.raw")
   DWORD    unknown_1;                             //        20         4     always 0x0001

   uint     group;                                 //        24         4     the symbol's SYMBOL_GROUP index in "symgroups.raw"
   DWORD    unknown_2;                             //        28         4

   double   pointSize;                             //        32         8     point size
   uint     spread;                                //        40         4     spread: 0=floating (variable)
   DWORD    unknown_3;                             //        44         4

   uint     tickType;                              //        48         4     tick type: 0=Uptick, 1=Downtick, 2=n/a
   WORD     unknown_4;                             //        52         2     always 0x0100
   WORD     unknown_5;                             //        54         2     many times NULL

   datetime time;                                  //        56         4     time
   DWORD    unknown_6;                             //        60         4
   double   bid;                                   //        64         8     bid
   double   ask;                                   //        72         8     ask
   double   sessionHigh;                           //        80         8     session high
   double   sessionLow;                            //        88         8     session low

   BYTE     unknown_7[16];                         //        96        16     many times NULL
   double   bid_2;                                 //       112         8     bid (repetition)
   double   ask_2;                                 //       120         8     ask (repetition)
};                                                 // -----------------------------------------------------------------------
#pragma pack(pop)                                  //               = 128
