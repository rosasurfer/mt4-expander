#include "expander.h"
#include "lib/conversion.h"
#include "lib/format.h"
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
const char* WINAPI ORDER_toStr(const ORDER* order, BOOL outputDebug/*=FALSE*/) {
   if (!order) return("NULL");

   char* result = "{}";
   const ORDER empty = {};

   if (memcmp(order, &empty, sizeof(ORDER))) {
      std::stringstream ss; ss
         <<  "{id="          <<                order->id
         << ", ticket="      <<                order->ticket
         << ", type="        << OrderTypeToStr(order->type)
         << ", lots="        <<               (order->lots       ? NumberFormat(order->lots, "%.2f") : "0")
         << ", symbol="      << DoubleQuoteStr(order->symbol)
         << ", openPrice="   <<               (order->openPrice  ? NumberFormat(order->openPrice, "%.5f") : "0")
         << ", openTime="    <<               (order->openTime   ? GmtTimeFormat(order->openTime, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", stopLoss="    <<               (order->stopLoss   ? NumberFormat(order->stopLoss, "%.5f") : "0")
         << ", takeProfit="  <<               (order->takeProfit ? NumberFormat(order->takeProfit, "%.5f") : "0")
         << ", closePrice="  <<               (order->closePrice ? NumberFormat(order->closePrice, "%.5f") : "0")
         << ", closeTime="   <<               (order->closeTime  ? GmtTimeFormat(order->closeTime, "\"%a, %d-%b-%Y %H:%M:%S\"") : "0")
         << ", commission="  <<   NumberFormat(order->commission, "%.2f")
         << ", swap="        <<   NumberFormat(order->swap, "%.2f")
         << ", profit="      <<   NumberFormat(order->profit, "%.2f")
         << ", magicNumber=" <<                order->magicNumber
         << ", comment="     << DoubleQuoteStr(order->comment)

         << ", maxRunup="    <<   NumberFormat(order->maxRunup, "%.5f")
         << ", maxDrawdown=" <<   NumberFormat(order->maxDrawdown, "%.5f")
         << "}";
      result = strdup(ss.str().c_str());                             // TODO: close memory leak
   }

   if (outputDebug) debug(result);
   return(result);
}
