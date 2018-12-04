#pragma once
#include "expander.h"
#include <vector>


// forward declaration instead of #include to break circular struct references
struct TEST;


/**
 * Framework struct ORDER
 *
 * Represents an order ticket, i.e. a pending order or an openened or closed position.
 */
struct ORDER {
   uint     id;                                    // unique id greater 0 (zero)
   TEST*    test;                                  // test the order belongs to
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
   double   commission;                            // values in money terms
   double   swap;                                  // ...
   double   profit;                                // ...
   int      magicNumber;
   char     comment[MAX_ORDER_COMMENT_LENGTH+1];

   double   high;                                  // high/low of the open position
   double   low;
   double   runupPip;                              // values in pip
   double   drawdownPip;                           // ...
   double   plPip;                                 // ...
};

typedef std::vector<ORDER*> OrderList;


// helpers
char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug = FALSE);
