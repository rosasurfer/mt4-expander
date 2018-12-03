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
         << ", lots="        <<                order->lots
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

         << ", maxRunup="    <<                order->maxRunupPip
         << ", maxDrawdown=" <<                order->maxDrawdownPip
         << "}";
      result = strdup(ss.str().c_str());                             // TODO: close memory leak
   }

   if (outputDebug) debug(result);
   return(result);
}
