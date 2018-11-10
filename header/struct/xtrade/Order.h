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
   double   lots;                                     //        12        8     (mis-aligned)
   char     symbol[MAX_SYMBOL_LENGTH+1];              //        20       12
   double   openPrice;                                //        32        8
   datetime openTime;                                 //        40        4
   double   stopLoss;                                 //        44        8     (mis-aligned)
   double   takeProfit;                               //        52        8     (mis-aligned)
   double   closePrice;                               //        60        8     (mis-aligned)
   datetime closeTime;                                //        68        4
   double   commission;                               //        72        8
   double   swap;                                     //        80        8
   double   profit;                                   //        88        8
   int      magicNumber;                              //        96        4
   char     comment[MAX_ORDER_COMMENT_LENGTH+1];      //       100       28
};                                                    // ----------------------------------------------------------------------------
#pragma pack(pop)                                     //              = 128


typedef std::vector<ORDER> OrderHistory;


const char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug = FALSE);
