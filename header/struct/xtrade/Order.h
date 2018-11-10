#pragma once
#include "expander.h"
#include <vector>


/**
 * Framework struct ORDER
 *
 * Represents a MetaTrader order.
 */
#pragma pack(push, 1)
struct ORDER {                                        // -- offset --- size --- description -----------------------------------------
   uint     id;                                       //         0        4     unique order id (positive, primary key)
   int      ticket;                                   //         4        4
   int      type;                                     //         8        4
   BYTE     _alignment1;                              //        12        4     (alignment to the next double)
   double   lots;                                     //        16        8
   char     symbol[MAX_SYMBOL_LENGTH+1];              //        24       12
   BYTE     _alignment2;                              //        36        4     (alignment to the next double)
   double   openPrice;                                //        40        8
   datetime openTime;                                 //        48        4
   BYTE     _alignment3;                              //        52        4     (alignment to the next double)
   double   stopLoss;                                 //        56        8
   double   takeProfit;                               //        64        8
   double   closePrice;                               //        72        8
   datetime closeTime;                                //        80        4
   BYTE     _alignment4;                              //        84        4     (alignment to the next double)
   double   commission;                               //        88        8
   double   swap;                                     //        96        8
   double   profit;                                   //       104        8
   int      magicNumber;                              //       112        4
   char     comment[MAX_ORDER_COMMENT_LENGTH+1];      //       116       28
};                                                    // ----------------------------------------------------------------------------
#pragma pack(pop)                                     //              = 144


typedef std::vector<ORDER> OrderHistory;


const char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug = FALSE);
