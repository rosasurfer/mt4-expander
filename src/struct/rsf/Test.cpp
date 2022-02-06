#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/math.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "struct/rsf/ExecutionContext.h"
#include "struct/rsf/Test.h"


/**
 * Set the id of a TEST.
 *
 * @param  TEST* test
 * @param  int   id - test id (positive)
 *
 * @return int - the same id
 */
int WINAPI test_SetId(TEST* test, int id) {
   if ((uint)test < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));
   if (id <= 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not positive)", id));

   test->id = id;
   return(id);
}


/**
 * Set the time of a TEST's first tick.
 *
 * @param  TEST*    test
 * @param  datetime time - Unix timestamp (server time)
 *
 * @return datetime - the same time
 */
datetime WINAPI test_SetStartTime(TEST* test, datetime time) {
   if ((uint)test < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));
   if (time <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (not positive)", time));

   test->startTime = time;
   return(time);
}


/**
 * Set the time of a TEST's last tick.
 *
 * @param  TEST*    test
 * @param  datetime time - Unix timestamp (server time)
 *
 * @return datetime - the same time
 */
datetime WINAPI test_SetEndTime(TEST* test, datetime time) {
   if ((uint)test < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));
   if (time <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (not positive)", time));

   test->endTime = time;
   return(time);
}


/**
 * Set the number of bars of a TEST.
 *
 * @param  TEST* test
 * @param  uint  bars
 *
 * @return uint - the same amount
 */
uint WINAPI test_SetBars(TEST* test, uint bars) {
   if ((uint)test < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));
   if (bars <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter bars: %d (not positive)", bars));

   test->bars = bars;
   return(bars);
}


/**
 * Set the number of ticks of a TEST.
 *
 * @param  TEST* test
 * @param  uint  ticks
 *
 * @return uint - the same amount
 */
uint WINAPI test_SetTicks(TEST* test, uint ticks) {
   if ((uint)test < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));
   if (ticks <= 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter ticks: %d (not positive)", ticks));

   test->ticks = ticks;
   return(ticks);
}


/**
 * Set the spread used in a TEST.
 *
 * @param  TEST*  test
 * @param  double spread - spread in pips
 *
 * @return double - the same spread
 */
double WINAPI test_SetSpread(TEST* test, double spread) {
   if ((uint)test < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));
   if (spread < 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter spread: %f (must be non-negative)", spread));

   test->spread = spread;
   return(test->spread);
}


/**
 * Return a human-readable version of a TEST struct.
 *
 * @param  TEST* test
 * @param  BOOL  outputDebug - whether to duplicate the result to OutputDebugString()
 *                             (default: no)
 * @return char*
 */
char* WINAPI TEST_toStr(const TEST* test, BOOL outputDebug/*=FALSE*/) {
   if ((uint)test < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));

   std::ostringstream ss;
   TEST empty = {};

   if (MemCompare(test, &empty, sizeof(TEST))) {
      ss << "{}";
   }
   else {
      uint digits=test->ec->digits, pipDigits=test->ec->pipDigits;
      ss << std::fixed
         <<  "{id="              <<                         test->id
         << ", created="         <<                        (test->created   ? LocalTimeFormatA(test->created, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", strategy="        <<                        (test->ec        ? DoubleQuoteStr(test->ec->programName) :"NULL")
         << ", symbol="          <<                        (test->ec        ? test->ec->symbol : "NULL")
         << ", timeframe="       <<                        (test->ec        ? TimeframeToStr(test->ec->timeframe) : "0")
         << ", startTime="       <<                        (test->startTime ? GmtTimeFormatA(test->startTime, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", endTime="         <<                        (test->endTime   ? GmtTimeFormatA(test->endTime,   "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", barModel="        <<     BarModelDescription(test->barModel)
         << ", bars="            <<                         test->bars
         << ", ticks="           <<                         test->ticks
         << ", spread="          << std::setprecision(1) << test->spread
         << ", tradeDirections=" << "?"                  // test->tradeDirections      // TODO: Long|Short|Both
         << ", trades="          <<                        (test->closedPositions      ? StrFormat("%d", test->closedPositions     ->size()) : "NULL")
         << " ("                 <<                        (test->closedLongPositions  ? StrFormat("%d", test->closedLongPositions ->size()) : "NULL")
         << "/"                  <<                        (test->closedShortPositions ? StrFormat("%d", test->closedShortPositions->size()) : "NULL") << ")"
         << ", avgRunup="        << std::setprecision(1) << test->stat_avgRunup
         << " ("                 <<                         test->stat_avgLongRunup    << "/" << test->stat_avgShortRunup    << ")"
         << ", avgDrawdown="     <<                         test->stat_avgDrawdown
         << " ("                 <<                         test->stat_avgLongDrawdown << "/" << test->stat_avgShortDrawdown << ")"
         << ", avgRealized="     <<                         test->stat_avgRealized
         << " ("                 <<                         test->stat_avgLongRealized << "/" << test->stat_avgShortRealized << ")"
         << "}";
   }
   char* result = strdup(ss.str().c_str());                                            // TODO: add to GC (close memory leak)

   if (outputDebug) debug(result);
   return(result);
}
