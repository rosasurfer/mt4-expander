#include "header/Expander.h"


/**
 *
 */
BOOL WINAPI CollectTestData(EXECUTION_CONTEXT* ec, datetime startTime, datetime endTime, double bid, double ask, uint bars, double accountBalance, const char* accountCurrency, const char* reportSymbol) {
   if ((uint)ec              < MIN_VALID_POINTER)  return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec));
   if ((uint)accountCurrency < MIN_VALID_POINTER)  return(error(ERR_INVALID_PARAMETER, "invalid parameter accountCurrency=0x%p (not a valid pointer)", accountCurrency));
   if ((uint)reportSymbol    < MIN_VALID_POINTER)  return(error(ERR_INVALID_PARAMETER, "invalid parameter reportSymbol=0x%p (not a valid pointer)", reportSymbol));
   if (!ec->programId)                             return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.programId=%d", ec->programId));
   if (ec->programType!=PT_EXPERT || !ec->testing) return(error(ERR_ILLEGAL_STATE, "function allowed only in experts under test"));

   TEST* test;

   if (ec->rootFunction == RF_START) {
      if (ec->test) return(error(ERR_RUNTIME_ERROR, "multiple TEST initializations in %s::start()", ec->programName));

      ec->test = test = new TEST();
      test_SetTime           (test, time(NULL)      );
      test_SetSymbol         (test, ec->symbol      );
      test_SetTimeframe      (test, ec->timeframe   );
      test_SetStartTime      (test, startTime       );
      //uint tickModel;                                              // TODO
      test_SetSpread         (test, (ask-bid)/0.0001);               // TODO: statt 0.0001 / Pip
      test_SetBars           (test, bars            );
      test_SetAccountDeposit (test, accountBalance  );               //  oder aus EA.ini auslesen
      test_SetAccountCurrency(test, accountCurrency );               //  oder aus EA.ini auslesen
      //uint tradeDirections;                                        // TODO: aus EA.ini auslesen
      test_SetStrategy       (test, ec->programName );
      test->orders = new OrderHistory(512);                          // reserve memory to speed-up testing
      test->orders->resize(0);
   }
   else if (ec->rootFunction == RF_DEINIT) {
      test = ec->test;
      if (!test) return(error(ERR_RUNTIME_ERROR, "missing TEST initialization in %s::deinit()", ec->programName));

      test_SetEndTime     (test, endTime              );
      test_SetBars        (test, bars - test->bars + 1);
      test_SetTicks       (test, ec->ticks            );
      test_SetReportSymbol(test, reportSymbol         );
   }
   else return(error(ERR_ILLEGAL_STATE, "function not allowed in %s::%s()", ec->programName, RootFunctionDescription(ec->rootFunction)));

   debug("%s::%s()  test=%s", ec->programName, RootFunctionDescription(ec->rootFunction), TEST_toStr(test));
   return(TRUE);
   #pragma EXPORT
}
