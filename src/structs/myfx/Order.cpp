#include "expander.h"
#include "structs/myfx/Order.h"
#include "utils/format.h"
#include "utils/toString.h"


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

   char* result = "{(empty)}";
   static const ORDER s_empty = {};

   if (memcmp(order, &s_empty, sizeof(ORDER))) {
      std::stringstream ss; ss
         <<  "{id="          <<                order->id
         << ", ticket="      <<                order->ticket
         << ", type="        << OrderTypeToStr(order->type)
         << ", lots="        <<               (order->lots       ? numberFormat(order->lots, "%.2f") : "0")
         << ", symbol="      << doubleQuoteStr(order->symbol)
         << ", openPrice="   <<               (order->openPrice  ? numberFormat(order->openPrice, "%.5f") : "0")
         << ", openTime="    <<               (order->openTime   ? doubleQuoteStr(gmTimeFormat(order->openTime, "%a, %d-%b-%Y %H:%M:%S")) : "0")
         << ", stopLoss="    <<               (order->stopLoss   ? numberFormat(order->stopLoss, "%.5f") : "0")
         << ", takeProfit="  <<               (order->takeProfit ? numberFormat(order->takeProfit, "%.5f") : "0")
         << ", closePrice="  <<               (order->closePrice ? numberFormat(order->closePrice, "%.5f") : "0")
         << ", closeTime="   <<               (order->closeTime  ? doubleQuoteStr(gmTimeFormat(order->closeTime, "%a, %d-%b-%Y %H:%M:%S")) : "0")
         << ", commission="  <<   numberFormat(order->commission, "%.2f")
         << ", swap="        <<   numberFormat(order->swap, "%.2f")
         << ", profit="      <<   numberFormat(order->profit, "%.2f")
         << ", magicNumber=" <<                order->magicNumber
         << ", comment="     << doubleQuoteStr(order->comment)
         << "}";
      string str = ss.str();
      result = strcpy(new char[str.size()+1], str.c_str());          // TODO: close memory leak
   }
   if (outputDebug) debug(result);
   return(result);
}
