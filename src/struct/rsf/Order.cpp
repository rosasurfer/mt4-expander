#include "expander.h"
#include "lib/conversion.h"
#include "lib/format.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "struct/rsf/Order.h"


/**
 * Return a human-readable version of an ORDER.
 *
 * @param  ORDER* order
 * @param  BOOL   outputDebug - whether or not to duplicate the result to OutputDebugString()
 *                              (default: no)
 * @return char*
 */
char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug/*=FALSE*/) {
   if ((uint)order < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter order: 0x%p (not a valid pointer)", order));

   std::stringstream ss;
   ORDER empty = {};

   if (MemCompare(order, &empty, sizeof(ORDER))) {
      ss << "{}";
   }
   else {
      ss <<  "{id="          <<                order->id
         << ", ticket="      <<                order->ticket
         << ", type="        << OrderTypeToStr(order->type)
         << ", lots="        <<   NumberFormat(order->lots, "%.2f")
         << ", symbol="      <<                order->symbol
         << ", openPrice="   <<                order->openPrice
         << ", openTime="    <<               (order->openTime   ? GmtTimeFormat(order->openTime, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", stopLoss="    <<                order->stopLoss
         << ", takeProfit="  <<                order->takeProfit
         << ", closePrice="  <<                order->closePrice
         << ", closeTime="   <<               (order->closeTime  ? GmtTimeFormat(order->closeTime, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", commission="  <<   NumberFormat(order->commission, "%.2f")
         << ", swap="        <<   NumberFormat(order->swap, "%.2f")
         << ", profit="      <<   NumberFormat(order->profit, "%.2f")
         << ", magicNumber=" <<                order->magicNumber
         << ", comment="     << DoubleQuoteStr(order->comment)
         << ", runup="       <<   NumberFormat(order->runupPip, "%.1f")
         << ", drawdown="    <<   NumberFormat(order->drawdownPip, "%.1f")
         << ", result="      <<   NumberFormat(order->plPip, "%.1f")
         << "}";
   }
   char* result = strdup(ss.str().c_str());                          // TODO: close memory leak

   if (outputDebug) debug(result);
   return(result);
}
