#pragma once

#include "header/structs/myfx/Order.h"


/**
 * MyFX struct TEST
 *
 * Structure holding metadata of a test: Strategy Tester settings, strategy settings, order history
 */
struct TEST {                                            // - size --- description --------------------------------------------
   uint          id;                                     //      4     unique test id (positive, primary key)
   datetime      time;                                   //      4     time of the test
   uint          duration;                               //      4     duration of the test in milliseconds
   char          symbol[MAX_SYMBOL_LENGTH+1];            //     12     tested symbol
   uint          timeframe;                              //      4     tested timeframe
   datetime      startTime;                              //      4     time of the first tick of testing
   datetime      endTime;                                //      4     time of the last tick of testing
   uint          tickModel;                              // ???  4     used tick model: 0=EveryTick|1=ControlPoints|2=BarOpen
   double        spread;                                 //      8     spread in points
   uint          bars;                                   //      4     number of tested bars
   uint          ticks;                                  //      4     number of tested ticks
   double        accountDeposit;                         //      8     account starting balance
   char          accountCurrency[MAX_SYMBOL_LENGTH+1];   //     12     account currency
   uint          tradeDirections;                        //      4     allowed trade directions: Long|Short|Both
   char          strategy[MAX_PATH];                     //    260     strategy name
   char          reportSymbol[MAX_SYMBOL_LENGTH+1];      //     12     generated unique symbol for charted reports
   OrderHistory* orders;                                 //      4     array of orders
};                                                       // -------------------------------------------------------------------


// Getters


// Setters
uint        WINAPI test_SetId             (TEST* test, uint        id       );
datetime    WINAPI test_SetTime           (TEST* test, datetime    time     );
uint        WINAPI test_SetDuration       (TEST* test, uint        duration );
const char* WINAPI test_SetSymbol         (TEST* test, const char* symbol   );
uint        WINAPI test_SetTimeframe      (TEST* test, uint        timeframe);
datetime    WINAPI test_SetStartTime      (TEST* test, datetime    time     );
datetime    WINAPI test_SetEndTime        (TEST* test, datetime    time     );
//uint      WINAPI test_SetTickModel      (TEST* test, uint        type     );   // TODO
double      WINAPI test_SetSpread         (TEST* test, double      spread   );
uint        WINAPI test_SetBars           (TEST* test, uint        bars     );
uint        WINAPI test_SetTicks          (TEST* test, uint        ticks    );
double      WINAPI test_SetAccountDeposit (TEST* test, double      value    );
const char* WINAPI test_SetAccountCurrency(TEST* test, const char* currency );
//uint      WINAPI test_SetTradeDirections(TEST* test, uint        types    );   // TODO
const char* WINAPI test_SetStrategy       (TEST* test, const char* name     );
const char* WINAPI test_SetReportSymbol   (TEST* test, const char* symbol   );


// Helpers
const char* WINAPI TEST_toStr(const TEST* test, BOOL outputDebug=FALSE);
