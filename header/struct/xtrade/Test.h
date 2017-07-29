#pragma once

#include "struct/xtrade/Order.h"


/**
 * XTrade struct TEST
 *
 * Structure holding metadata of a test: Strategy Tester settings, strategy settings, order history
 */
#pragma pack(push, 1)
struct TEST {                                            // - size --- description --------------------------------------------
   int           id;                                     //      4     unique test id (positive, primary key)
   datetime      time;                                   //      4     time of the test
   char          strategy[MAX_PATH];                     //    260     strategy name
   int           reportingId;                            //      4     reporting id (for composition of reportingSymbol)
   char          reportingSymbol[MAX_SYMBOL_LENGTH+1];   //     12     test symbol for charted reports
   char          symbol         [MAX_SYMBOL_LENGTH+1];   //     12     tested symbol
   uint          timeframe;                              //      4     tested timeframe
   datetime      startTime;                              //      4     time of the first tick of testing
   datetime      endTime;                                //      4     time of the last tick of testing
   uint          barModel;                               //      4     used bar model: 0=EveryTick|1=ControlPoints|2=BarOpen
   double        spread;                                 //      8     spread in pips
   uint          bars;                                   //      4     number of tested bars
   uint          ticks;                                  //      4     number of tested ticks
   uint          tradeDirections;                        //      4     enabled trade directions: Long|Short|Both
   BOOL          visualMode;                             //      4     whether or not the test was run in visual mode
   uint          duration;                               //      4     test duration in milliseconds
   OrderHistory* orders;                                 //      4     array of orders
};                                                       // -------------------------------------------------------------------
#pragma pack(pop)


// Getters


// Setters
int         WINAPI test_SetId             (TEST* test, int         id       );
datetime    WINAPI test_SetTime           (TEST* test, datetime    time     );
const char* WINAPI test_SetStrategy       (TEST* test, const char* name     );
int         WINAPI test_SetReportingId    (TEST* test, int         id       );
const char* WINAPI test_SetReportingSymbol(TEST* test, const char* symbol   );
const char* WINAPI test_SetSymbol         (TEST* test, const char* symbol   );
uint        WINAPI test_SetTimeframe      (TEST* test, uint        timeframe);
datetime    WINAPI test_SetStartTime      (TEST* test, datetime    time     );
datetime    WINAPI test_SetEndTime        (TEST* test, datetime    time     );
//uint      WINAPI test_SetBarModel       (TEST* test, uint        type     );   // TODO
double      WINAPI test_SetSpread         (TEST* test, double      spread   );
uint        WINAPI test_SetBars           (TEST* test, uint        bars     );
uint        WINAPI test_SetTicks          (TEST* test, uint        ticks    );
//uint      WINAPI test_SetTradeDirections(TEST* test, uint        types    );   // TODO
BOOL        WINAPI test_SetVisualMode     (TEST* test, BOOL        status   );
uint        WINAPI test_SetDuration       (TEST* test, uint        duration );


// Helpers
const char* WINAPI TEST_toStr(const TEST* test, BOOL outputDebug=FALSE);
