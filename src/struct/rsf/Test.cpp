#include "expander.h"
#include "lib/conversion.h"
#include "lib/format.h"
#include "lib/math.h"
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
 * Set the reporting id of a TEST. Used for composition of TEST.reportingSymbol.
 *
 * @param  TEST* test
 * @param  int   id
 *
 * @return int - the same id
 */
int WINAPI test_SetReportId(TEST* test, int id) {
   if ((uint)test < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));
   if (id < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not positive)", id));

   test->reportId = id;
   return(id);
}


/**
 * Set the reporting symbol of a TEST. Used as terminal symbol for charted reports.
 *
 * @param  TEST* test
 * @param  char* symbol
 *
 * @return char* - the same symbol
 */
const char* WINAPI test_SetReportSymbol(TEST* test, const char* symbol) {
   if ((uint)test   < MIN_VALID_POINTER)   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));
   if ((uint)symbol < MIN_VALID_POINTER)   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   if (strlen(symbol) > MAX_SYMBOL_LENGTH) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol \"%s\" (max %d characters)", symbol, MAX_SYMBOL_LENGTH));

   if (!strcpy(test->reportSymbol, symbol))
      return(NULL);
   return(symbol);
}


/**
 * Return a human-readable version of a TEST struct.
 *
 * @param  TEST* test
 * @param  BOOL  outputDebug - whether or not to duplicate the result to OutputDebugString()
 *                             (default: no)
 * @return char*
 */
const char* WINAPI TEST_toStr(const TEST* test, BOOL outputDebug/*=FALSE*/) {
   if ((uint)test < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter test: 0x%p (not a valid pointer)", test));

   std::stringstream ss;
   TEST empty = {};

   if (!memcmp(test, &empty, sizeof(TEST))) {
      ss << "{}";
   }
   else {
      ss <<  "{id="              <<                     test->id
         << ", created="         <<                    (test->created   ? LocalTimeFormat(test->created, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", strategy="        <<                    (test->ec        ? DoubleQuoteStr(test->ec->programName) :"NULL")
         << ", symbol="          <<                    (test->ec        ? DoubleQuoteStr(test->ec->symbol)      :"NULL")
         << ", timeframe="       <<                    (test->ec        ? TimeframeToStr(test->ec->timeframe) : "0")
         << ", startTime="       <<                    (test->startTime ? GmtTimeFormat(test->startTime, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", endTime="         <<                    (test->endTime   ? GmtTimeFormat(test->endTime,   "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", barModel="        << BarModelDescription(test->barModel)
         << ", bars="            <<                     test->bars
         << ", ticks="           <<                     test->ticks
         << ", spread="          <<        NumberFormat(test->spread, "%.1f")
         << ", tradeDirections=" <<                     test->tradeDirections    // TODO: Long|Short|Both
         << ", trades="          <<                    (test->closedPositions      ? NumberFormat(test->closedPositions     ->size(), "%d") : "NULL")
         << ", long="            <<                    (test->closedLongPositions  ? NumberFormat(test->closedLongPositions ->size(), "%d") : "NULL")
         << ", short="           <<                    (test->closedShortPositions ? NumberFormat(test->closedShortPositions->size(), "%d") : "NULL")
         << ", reportId="        <<                     test->reportId
         << ", reportSymbol="    <<      DoubleQuoteStr(test->reportSymbol)
         << "}";
   }
   ss << NumberFormat((uint)test, " (0x%p)");
   char* result = strdup(ss.str().c_str());                                      // TODO: close memory leak

   if (outputDebug) debug(result);
   return(result);
}
