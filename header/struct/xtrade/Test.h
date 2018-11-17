#pragma once
#include "struct/mt4/FxtHeader.h"
#include "struct/xtrade/Order.h"


/**
 * Framework struct TEST
 *
 * A structure holding test data (i.e. tester settings, strategy settings, trade history).
 */
#pragma pack(push, 1)
struct TEST {                                            // -- offset --- size --- description -----------------------------------------
   int               id;                                 //         0        4     unique test id (positive, primary key)
   datetime          created;                            //         4        4     creation time of the test
   char              strategy[MAX_PATH];                 //         8      260     strategy name
   char              symbol      [MAX_SYMBOL_LENGTH+1];  //       268       12     tested symbol
   uint              timeframe;                          //       280        4     tested timeframe
   datetime          startTime;                          //       284        4     time of the first tick of testing
   datetime          endTime;                            //       288        4     time of the last tick of testing
   uint              barModel;                           //       292        4     used bar model: 0=EveryTick|1=ControlPoints|2=BarOpen
   uint              bars;                               //       296        4     number of tested bars
   uint              ticks;                              //       300        4     number of tested ticks
   double            spread;                             //       304        8     spread in pips
   DWORD             tradeDirections;                    //       312        4     active trade directions: Long|Short|Both
   int               reportId;                           //       316        4     reporting id (for composition of reportSymbol)
   char              reportSymbol[MAX_SYMBOL_LENGTH+1];  //       320       12     test symbol for charted reports
   const FXT_HEADER* fxtHeader;                          //       332        4     FXT header data
                                                         //
   OrderList*        positions;                          //       336        4     all open positions
   OrderList*        longPositions;                      //       340        4     open long positions
   OrderList*        shortPositions;                     //       344        4     open short positions
                                                         //
   OrderList*        trades;                             //       348        4     all closed positions (trades)
   OrderList*        longTrades;                         //       352        4     long trades
   OrderList*        shortTrades;                        //       356        4     short trades
};                                                       // ----------------------------------------------------------------------------
#pragma pack(pop)                                        //              = 360


// for getters (used by MQL4) see "header/struct/xtrade/ExecutionContext.h"


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
