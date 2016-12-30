#pragma once

/**
 * MT4 struct SYMBOL_SELECTED (Dateiformat "symbols.sel")
 */
struct SYMBOL_SELECTED {                           // -- offset ---- size --- description ----------------------------------------------------------------------------
   char   symbol[MAX_SYMBOL_LENGTH+1];             //         0        12     Symbol (szchar)
   uint   digits;                                  //        12         4     Digits

   uint   arrayKey;                                //        16         4     Array-Key des Symbols in "symbols.raw"
   DWORD  unknown_1;                               //        20         4     immer 0x0001

   uint   group;                                   //        24         4     Index der Symbolgruppe in "symgroups.raw"
   DWORD  unknown_2;                               //        28         4

   double pointSize;                               //        32         8     Point size
   uint   spread;                                  //        40         4     Spread: 0=current spread (variable)
   DWORD  unknown_3;                               //        44         4

   uint   tickType;                                //        48         4     Tick-Type: 0=Uptick, 1=Downtick, 2=n/a
   WORD   unknown_4;                               //        52         2     immer 0x0100
   WORD   unknown_5;                               //        54         2     unterschiedlich (oft NULL)

   uint   time;                                    //        56         4     Time
   DWORD  unknown_6;                               //        60         4
   double bid;                                     //        64         8     Bid
   double ask;                                     //        72         8     Ask
   double sessionHigh;                             //        80         8     Session High
   double sessionLow;                              //        88         8     Session Low

   BYTE   unknown_7[16];                           //        96        16     unterschiedlich (oft alles NULL)
   double bid_2;                                   //       112         8     Bid (Wiederholung)
   double ask_2;                                   //       120         8     Ask (Wiederholung)
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //               = 128