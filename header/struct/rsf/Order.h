#pragma once
#include "expander.h"
#include <vector>


// forward declaration instead of an #include to break circular struct references
struct TEST;


#pragma pack(push, 1)
/**
 * Framework struct ORDER
 *
 * Represents an order ticket, i.e. a pending order or an openened or closed position.
 */
struct ORDER {                                     // -- offset --- size --- description --------------------------------------------------------------------------------------
   uint     id;                                    //         0        4     unique id greater 0 (zero)
   TEST*    test;                                  //         4        4     the test the order belongs to
   int      ticket;                                //         8        4
   int      type;                                  //        12        4
   double   lots;                                  //        16        8
   char     symbol[MAX_SYMBOL_LENGTH+1];           //        24       12
   time32   openTime;                              //        36        4
   double   openPrice;                             //        40        8
   double   stopLoss;                              //        48        8
   double   takeProfit;                            //        56        8
   time32   closeTime;                             //        64        4
   BYTE     _alignment[4];                         //        68        4     (alignment to the next double)
   double   closePrice;                            //        72        8
   double   commission;                            //        80        8     values in money terms
   double   swap;                                  //        88        8     ...
   double   profit;                                //        96        8     ...
   int      magicNumber;                           //       104        4
   char     comment[MAX_ORDER_COMMENT_LENGTH+1];   //       108       28
   double   highPrice;                             //       136        8     high of the open position
   double   lowPrice;                              //       144        8     low of the open position
   double   runup;                                 //       152        8     max. PL runup in pip
   double   drawdown;                              //       160        8     max. PL drawdown in pip
   double   realized;                              //       168        8     realized PL result in pip
};                                                 // -------------------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //              = 176

typedef std::vector<ORDER*> OrderList;


// helpers
char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug = FALSE);
