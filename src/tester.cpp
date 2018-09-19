#include "expander.h"
#include "struct/xtrade/ExecutionContext.h"
#include "util/format.h"
#include "util/math.h"
#include "util/terminal.h"
#include "util/toString.h"

#include <fstream>
#include <time.h>


// forward declaration
BOOL WINAPI SaveTest(TEST* test);


/**
 * TODO: documentation
 */
BOOL WINAPI CollectTestData(EXECUTION_CONTEXT* ec, datetime startTime, datetime endTime, double bid, double ask, uint bars, int reportingId, const char* reportingSymbol) {
   if ((uint)ec < MIN_VALID_POINTER)               return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec));
   if (!ec->programIndex)                          return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.programIndex=%d", ec->programIndex));
   if (ec->programType!=PT_EXPERT || !ec->testing) return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));

   TEST* test;

   if (ec->rootFunction == RF_START) {
      if (ec->test) return(error(ERR_RUNTIME_ERROR, "multiple TEST initializations in %s::start()", ec->programName));

      ec->test = test = new TEST();
      test_SetTime           (test, time(NULL)      );
      test_SetStrategy       (test, ec->programName );
      test_SetReportingId    (test, reportingId     );
      test_SetReportingSymbol(test, reportingSymbol );
      test_SetSymbol         (test, ec->symbol      );
      test_SetTimeframe      (test, ec->timeframe   );
      test_SetStartTime      (test, startTime       );
      //uint barModel;                                               // TODO
      test_SetSpread         (test, (ask-bid)/0.0001);               // TODO: statt 0.0001 Variable Pip
      test_SetBars           (test, bars            );
      //uint tradeDirections;                                        // TODO: aus Expert.ini auslesen
      test_SetVisualMode     (test, ec->visualMode  );
      test_SetDuration       (test, GetTickCount()  );
      test->orders = new OrderHistory(512);                          // reserve memory to speed-up testing
      test->orders->resize(0);
   }
   else if (ec->rootFunction == RF_DEINIT) {
      test = ec->test;
      if (!test) return(error(ERR_RUNTIME_ERROR, "missing TEST initialization in %s::deinit()", ec->programName));

      test_SetEndTime (test, endTime                        );
      test_SetBars    (test, bars - test->bars + 1          );
      test_SetTicks   (test, ec->ticks                      );
      test_SetDuration(test, GetTickCount() - test->duration);

      SaveTest(test);
   }
   else return(error(ERR_FUNC_NOT_ALLOWED, "function not allowed in %s::%s()", ec->programName, RootFunctionDescription(ec->rootFunction)));

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * TODO: validation
 */
BOOL WINAPI Test_OpenOrder(EXECUTION_CONTEXT* ec, int ticket, int type, double lots, const char* symbol, double openPrice, datetime openTime, double stopLoss, double takeProfit, double commission, int magicNumber, const char* comment) {
   if ((uint)ec < MIN_VALID_POINTER)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->testing)   return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));

   TEST*         test   = ec->test;     if (!test)   return(error(ERR_RUNTIME_ERROR, "invalid TEST initialization,  ec.test=0x%p", ec->test));
   OrderHistory* orders = test->orders; if (!orders) return(error(ERR_RUNTIME_ERROR, "invalid OrderHistory initialization,  test.orders=0x%p", test->orders));

   ORDER order = {};
      order.ticket      = ticket;
      order.type        = type;
      order.lots        = round(lots, 2);
      strcpy(order.symbol, symbol);
      order.openPrice   = round(openPrice, 5);
      order.openTime    = openTime;
      order.stopLoss    = round(stopLoss,   5);
      order.takeProfit  = round(takeProfit, 5);
      order.commission  = round(commission, 2);
      order.magicNumber = magicNumber;
      strcpy(order.comment, comment);
   orders->push_back(order);

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * TODO: validation
 *
 * @param  int      ticket
 * @param  double   closePrice
 * @param  datetime closeTime
 * @param  double   swap
 * @param  double   profit
 *
 * @return BOOL - success status
 */
BOOL WINAPI Test_CloseOrder(EXECUTION_CONTEXT* ec, int ticket, double closePrice, datetime closeTime, double swap, double profit) {
   if ((uint)ec < MIN_VALID_POINTER)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->testing)   return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));

   TEST*         test   = ec->test;     if (!test)   return(error(ERR_RUNTIME_ERROR, "invalid TEST initialization,  ec.test=0x%p", ec->test));
   OrderHistory* orders = test->orders; if (!orders) return(error(ERR_RUNTIME_ERROR, "invalid OrderHistory initialization,  test.orders=0x%p", test->orders));

   uint i = orders->size()-1;

   for (; i >= 0; --i) {                                             // iterate in reverse order to speed-up
      ORDER* order = &(*orders)[i];
      if (order->ticket == ticket) {
         order->closePrice = round(closePrice, 5);
         order->closeTime  = closeTime;
         order->swap       = round(swap,   2);
         order->profit     = round(profit, 2);
         break;
      }
   }
   if (i < 0) return(error(ERR_RUNTIME_ERROR, "ticket #%d not found, size(orders)=%d", ticket, orders->size()));

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Save the results of a TEST.
 *
 * @param  TEST* test
 *
 * @return BOOL - success status
 */
BOOL WINAPI SaveTest(TEST* test) {
   // save TEST to logfile
   string testLogfile = string(GetTerminalPathA()) +"/tester/files/testresults/"+ test->strategy +" #"+ to_string(test->reportingId) + localTimeFormat(test->time, "  %d.%m.%Y %H.%M.%S.log");
   std::ofstream fs;
   fs.open(testLogfile.c_str()); if (!fs.is_open()) return(error(ERR_RUNTIME_ERROR, "fs.open(\"%s\") failed", testLogfile.c_str()));
   fs << "test=" << TEST_toStr(test) << "\n";
   debug("test=%s", TEST_toStr(test));

   OrderHistory* orders = test->orders; if (!orders) return(error(ERR_RUNTIME_ERROR, "invalid OrderHistory  test.orders=0x%p", test->orders));
   int size = orders->size();

   for (int i=0; i < size; ++i) {
      ORDER* order = &(*orders)[i];
      fs << "order." << i << "=" << ORDER_toStr(order) << "\n";
   }
   fs.close();

   // backup input parameters
   // TODO: MetaTrader creates/updates the expert.ini file when the dialog "Expert properties" is confirmed.
   string paramSrcFile  = string(GetTerminalPathA()) +"/tester/"+ test->strategy +".ini";
   string paramDestFile = string(GetTerminalPathA()) +"/tester/files/testresults/"+ test->strategy +" #"+ to_string(test->reportingId) + localTimeFormat(test->time, "  %d.%m.%Y %H.%M.%S.ini");
   if (!CopyFile(paramSrcFile.c_str(), paramDestFile.c_str(), TRUE))
      return(error(ERR_WIN32_ERROR+GetLastError(), "CopyFile() failed"));
   return(TRUE);
}
