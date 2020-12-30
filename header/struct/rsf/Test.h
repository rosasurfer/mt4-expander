#pragma once
#include "struct/mt4/FxtHeader.h"
#include "struct/rsf/Order.h"


// forward declaration instead of #include to break circular struct references
struct EXECUTION_CONTEXT;


/**
 * Framework struct TEST
 *
 * A structure holding test data (i.e. tester settings, strategy settings, trade history).
 *
 */
struct TEST {
   int                id;                                // unique id greater 0 (zero)
   datetime           created;                           // creation time of the test
   EXECUTION_CONTEXT* ec;                                // master context of the tested expert
   datetime           startTime;                         // time of the first tick of testing
   datetime           endTime;                           // time of the last tick of testing
   uint               barModel;                          // used bar model: 0=EveryTick | 1=ControlPoints | 2=BarOpen
   uint               bars;                              // number of tested bars
   uint               ticks;                             // number of tested ticks
   double             spread;                            // spread in pip
   const FXT_HEADER*  fxtHeader;                         // FXT header of the test's price history
   int                reportId;                          // reporting id (for composition of reportSymbol)
   char               reportSymbol[MAX_SYMBOL_LENGTH+1]; // reporting symbol (terminal symbol for charted reports)
   DWORD              tradeDirections;                   // enabled trade directions: Long|Short|Both

   OrderList*         openPositions;
   OrderList*         openLongPositions;
   OrderList*         openShortPositions;

   OrderList*         closedPositions;
   OrderList*         closedLongPositions;
   OrderList*         closedShortPositions;

   double             stat_avgRunupPip;                  // average runup of all trades in pip
   double             stat_avgLongRunupPip;              // average long runup in pip
   double             stat_avgShortRunupPip;             // average short runup in pip

   double             stat_avgDrawdownPip;               // average drawdown of all trades in pip
   double             stat_avgLongDrawdownPip;           // average long drawdown in pip
   double             stat_avgShortDrawdownPip;          // average short drawdown in pip

   double             stat_avgPlPip;                     // average PL of all trades in pip
   double             stat_avgLongPlPip;                 // average long PL in pip
   double             stat_avgShortPlPip;                // average short PL in pip
};


// getters: access from MQL is possible via the EXECUTION_CONTEXT getters ec_Test*()
// @see  "header/struct/rsf/ExecutionContext.h"


// validating setters
int         WINAPI test_SetId             (TEST* test, int         id       );
datetime    WINAPI test_SetStartTime      (TEST* test, datetime    time     );
datetime    WINAPI test_SetEndTime        (TEST* test, datetime    time     );
uint        WINAPI test_SetBars           (TEST* test, uint        bars     );
uint        WINAPI test_SetTicks          (TEST* test, uint        ticks    );
double      WINAPI test_SetSpread         (TEST* test, double      spread   );
//DWORD     WINAPI test_SetTradeDirections(TEST* test, uint        types    );   // TODO
int         WINAPI test_SetReportId       (TEST* test, int         id       );
const char* WINAPI test_SetReportSymbol   (TEST* test, const char* symbol   );


// helpers
char*       WINAPI TEST_toStr(const TEST* test, BOOL outputDebug = FALSE);
