#pragma once
#include "struct/mt4/FxtHeader.h"
#include "struct/rsf/Order.h"


// forward declaration instead of #include to break circular struct references
struct EXECUTION_CONTEXT;


#pragma pack(push, 1)
/**
 * Framework struct TEST
 *
 * A structure holding test data (i.e. tester settings, strategy settings, trade history).
 */
struct TEST {
   int                id;                                // unique id greater 0 (zero)
   time32             created;                           // creation time of the test
   EXECUTION_CONTEXT* ec;                                // master context of the tested expert
   time32             startTime;                         // time of the first tick of testing
   time32             endTime;                           // time of the last tick of testing
   uint               barModel;                          // used bar model: 0=EveryTick | 1=ControlPoints | 2=BarOpen
   uint               bars;                              // number of tested bars
   uint               ticks;                             // number of tested ticks
   double             spread;                            // spread in pip
   const FXT_HEADER*  fxtHeader;                         // FXT header of the test's price history
   DWORD              tradeDirections;                   // enabled trade directions: Long|Short|Both

   OrderList*         openPositions;
   OrderList*         openLongPositions;
   OrderList*         openShortPositions;

   OrderList*         closedPositions;
   OrderList*         closedLongPositions;
   OrderList*         closedShortPositions;

   double             stat_avgRunup;                     // average runup of all trades in pip
   double             stat_avgLongRunup;                 // average long runup in pip
   double             stat_avgShortRunup;                // average short runup in pip

   double             stat_avgDrawdown;                  // average drawdown of all trades in pip
   double             stat_avgLongDrawdown;              // average long drawdown in pip
   double             stat_avgShortDrawdown;             // average short drawdown in pip

   double             stat_avgRealized;                  // average realized PL of all trades in pip
   double             stat_avgLongRealized;              // average realized PL of long trades in pip
   double             stat_avgShortRealized;             // average realized PL of short trades in pip
};
#pragma pack(pop)


// getters: access from MQL is possible via the EXECUTION_CONTEXT getters ec_Test*()
// @see  "header/struct/rsf/ExecutionContext.h"


// validating setters
int         WINAPI test_SetId             (TEST* test, int    id    );
time32      WINAPI test_SetStartTime      (TEST* test, time32 time  );
time32      WINAPI test_SetEndTime        (TEST* test, time32 time  );
uint        WINAPI test_SetBars           (TEST* test, uint   bars  );
uint        WINAPI test_SetTicks          (TEST* test, uint   ticks );
double      WINAPI test_SetSpread         (TEST* test, double spread);
//DWORD     WINAPI test_SetTradeDirections(TEST* test, uint   types );   // TODO


// helpers
char*       WINAPI TEST_toStr(const TEST* test, BOOL outputDebug = FALSE);
