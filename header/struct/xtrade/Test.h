#pragma once
#include "struct/xtrade/Order.h"


/**
 * Framework struct TEST
 *
 * A structure holding test data (i.e. tester settings, strategy settings, trade history).
 */
#pragma pack(push, 1)
struct TEST {                                         // - size --- description ------------------------------------------
   int           id;                                  //      4     unique test id (positive, primary key)
   datetime      created;                             //      4     creation time of the test
   char          strategy[MAX_PATH];                  //    260     strategy name
   int           reportId;                            //      4     reporting id (for composition of reportSymbol)
   char          reportSymbol[MAX_SYMBOL_LENGTH+1];   //     12     test symbol for charted reports
   char          symbol      [MAX_SYMBOL_LENGTH+1];   //     12     tested symbol
   uint          timeframe;                           //      4     tested timeframe
   datetime      startTime;                           //      4     time of the first tick of testing
   datetime      endTime;                             //      4     time of the last tick of testing
   uint          barModel;                            //      4     used bar model: 0=EveryTick|1=ControlPoints|2=BarOpen
   double        spread;                              //      8     spread in pips
   uint          bars;                                //      4     number of tested bars
   uint          ticks;                               //      4     number of tested ticks
   DWORD         tradeDirections;                     //      4     enabled trade directions: Long|Short|Both
   BOOL          visualMode;                          //      4     whether or not the test was run in visual mode
   uint          duration;                            //      4     test duration in milliseconds
   OrderHistory* orders;                              //      4     trade history
};                                                    // -----------------------------------------------------------------
#pragma pack(pop)


// for getters (used by MQL4) see "header/struct/xtrade/ExecutionContext.h"


// validating setters
int         WINAPI test_SetId             (TEST* test, int         id       );
datetime    WINAPI test_SetCreated        (TEST* test, datetime    time     );
const char* WINAPI test_SetStrategy       (TEST* test, const char* name     );
int         WINAPI test_SetReportId       (TEST* test, int         id       );
const char* WINAPI test_SetReportSymbol   (TEST* test, const char* symbol   );
const char* WINAPI test_SetSymbol         (TEST* test, const char* symbol   );
uint        WINAPI test_SetTimeframe      (TEST* test, uint        timeframe);
datetime    WINAPI test_SetStartTime      (TEST* test, datetime    time     );
datetime    WINAPI test_SetEndTime        (TEST* test, datetime    time     );
int         WINAPI test_SetBarModel       (TEST* test, int         type     );
double      WINAPI test_SetSpread         (TEST* test, double      spread   );
uint        WINAPI test_SetBars           (TEST* test, uint        bars     );
uint        WINAPI test_SetTicks          (TEST* test, uint        ticks    );
//DWORD     WINAPI test_SetTradeDirections(TEST* test, uint        types    );   // TODO
BOOL        WINAPI test_SetVisualMode     (TEST* test, BOOL        status   );
uint        WINAPI test_SetDuration       (TEST* test, uint        duration );


// helpers
const char* WINAPI TEST_toStr(const TEST* test, BOOL outputDebug = FALSE);
