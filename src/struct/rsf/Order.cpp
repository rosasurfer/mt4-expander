#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "struct/rsf/ExecutionContext.h"
#include "struct/rsf/Order.h"
#include "struct/rsf/Test.h"


/**
 * Return a human-readable version of an ORDER.
 *
 * @param  ORDER* order
 * @param  BOOL   outputDebug - whether to duplicate the result to OutputDebugString()
 *                              (default: no)
 * @return char*
 */
char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug/*=FALSE*/) {
   if ((uint)order < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter order: 0x%p (not a valid pointer)", order));

   std::ostringstream ss;
   ORDER empty = {};

   if (MemCompare(order, &empty, sizeof(ORDER))) {
      ss << "{}";
   }
   else {
      uint digits = order->test->ec->digits;
      ss << std::fixed
         <<  "{id="          <<                         order->id
         << ", ticket="      <<                         order->ticket
         << ", type="        <<    OrderTypeDescription(order->type)
         << ", lots="        <<       std::setprecision(order->lots ? 2 : 0) << order->lots
         << ", symbol="      <<                         order->symbol
         << ", openTime="    <<                        (order->openTime   ? GmtTimeFormatA(order->openTime, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", openPrice="   <<       std::setprecision(order->openPrice  ? digits : 0) << order->openPrice
         << ", stopLoss="    <<       std::setprecision(order->stopLoss   ? digits : 0) << order->stopLoss
         << ", takeProfit="  <<       std::setprecision(order->takeProfit ? digits : 0) << order->takeProfit
         << ", closeTime="   <<                        (order->closeTime  ? GmtTimeFormatA(order->closeTime, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", closePrice="  <<       std::setprecision(order->closePrice ? digits : 0) << order->closePrice
         << ", commission="  << std::setprecision(2) << order->commission
         << ", swap="        << std::setprecision(2) << order->swap
         << ", profit="      << std::setprecision(2) << order->profit
         << ", magicNumber=" <<                         order->magicNumber
         << ", comment="     <<          DoubleQuoteStr(order->comment)
         << ", highPrice="   <<       std::setprecision(order->highPrice ? digits : 0) << order->highPrice
         << ", lowPrice="    <<       std::setprecision(order->lowPrice  ? digits : 0) << order->lowPrice
         << ", runup="       << std::setprecision(1) << order->runup
         << ", drawdown="    << std::setprecision(1) << order->drawdown
         << ", realized="    << std::setprecision(1) << order->realized
         << ", test="        <<                        (order->test ? strformat("0x%p", order->test) : "NULL")
         << "}";
   }
   char* result = strdup(ss.str().c_str());                          // TODO: add to GC (close memory leak)

   if (outputDebug) debug(result);
   return(result);
}
