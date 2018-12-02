#pragma once
#include "expander.h"
#include <vector>


/**
 * Framework struct ORDER
 *
 * Represents an order ticket, i.e. a pending order or an openened or closed position.
 */
struct ORDER {
   uint     id;                                    // unique id greater 0 (zero)
   int      ticket;
   int      type;
   double   lots;
   char     symbol[MAX_SYMBOL_LENGTH+1];
   double   openPrice;
   datetime openTime;
   double   stopLoss;
   double   takeProfit;
   double   closePrice;
   datetime closeTime;
   double   commission;
   double   swap;
   double   profit;
   int      magicNumber;
   char     comment[MAX_ORDER_COMMENT_LENGTH+1];

   double   high;                                  // open high/low tracking
   double   low;                                   //
   double   maxRunup;                              // max runup/drawdown tracking
   double   maxDrawdown;                           //
};

typedef std::vector<ORDER*> OrderList;


const char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug = FALSE);
