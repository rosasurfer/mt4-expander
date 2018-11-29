#pragma once
#include "struct/mt4/FxtHeader.h"
#include "struct/rsf/Order.h"


/**
 * Framework struct TEST
 *
 * A structure holding test data (i.e. tester settings, strategy settings, trade history).
 */
struct TEST {
   int               id;                                 // unique test id (positive, primary key)
   datetime          created;                            // creation time of the test
   char              strategy[MAX_PATH];                 // strategy name
   char              symbol[MAX_SYMBOL_LENGTH+1];        // tested symbol
   uint              timeframe;                          // tested timeframe
   datetime          startTime;                          // time of the first tick of testing
   datetime          endTime;                            // time of the last tick of testing
   uint              barModel;                           // used bar model: 0=EveryTick|1=ControlPoints|2=BarOpen
   uint              bars;                               // number of tested bars
   uint              ticks;                              // number of ticks
   double            spread;                             // spread in pips
   DWORD             tradeDirections;                    // enabled trade directions: Long|Short|Both
   int               reportId;                           // reporting id (for composition of reportSymbol)
   char              reportSymbol[MAX_SYMBOL_LENGTH+1];  // reporting symbol (terminal symbol for charted reports)s
   const FXT_HEADER* fxtHeader;                          // FXT header of the price history used for the test

   OrderList*        positions;                          // all open positions
   OrderList*        longPositions;                      // open long positions
   OrderList*        shortPositions;                     // open short positions

   OrderList*        trades;                             // all closed positions (completed trades)
   OrderList*        longTrades;                         // long trades
   OrderList*        shortTrades;                        // short trades
};


// getters: access from MQL is possible via the program's execution context
// @see  "header/struct/rsf/ExecutionContext.h"


// validating setters
int         WINAPI test_SetId             (TEST* test, int         id       );
datetime    WINAPI test_SetCreated        (TEST* test, datetime    time     );
const char* WINAPI test_SetStrategy       (TEST* test, const char* name     );
const char* WINAPI test_SetSymbol         (TEST* test, const char* symbol   );
uint        WINAPI test_SetTimeframe      (TEST* test, uint        timeframe);
datetime    WINAPI test_SetStartTime      (TEST* test, datetime    time     );
datetime    WINAPI test_SetEndTime        (TEST* test, datetime    time     );
int         WINAPI test_SetBarModel       (TEST* test, int         type     );
uint        WINAPI test_SetBars           (TEST* test, uint        bars     );
uint        WINAPI test_SetTicks          (TEST* test, uint        ticks    );
double      WINAPI test_SetSpread         (TEST* test, double      spread   );
//DWORD     WINAPI test_SetTradeDirections(TEST* test, uint        types    );   // TODO
int         WINAPI test_SetReportId       (TEST* test, int         id       );
const char* WINAPI test_SetReportSymbol   (TEST* test, const char* symbol   );


// helpers
const char* WINAPI TEST_toStr(const TEST* test, BOOL outputDebug = FALSE);
