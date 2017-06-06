#pragma once

#include "expander.h"
#include <vector>


/**
 * Xtrade struct ORDER
 *
 * Represents a Metatrader order.
 */
#pragma pack(push, 1)
struct ORDER {
   uint     id;                                       // unique order id (positive, primary key)
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
};
#pragma pack(pop)


typedef std::vector<ORDER> OrderVector;
typedef OrderVector        OrderHistory;


const char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug=FALSE);
