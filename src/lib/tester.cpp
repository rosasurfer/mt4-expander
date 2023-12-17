#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/helper.h"
#include "lib/math.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/tester.h"

#include <ctime>
#include <fstream>
#include <windowsx.h>


/**
 * Find the window handle of the Stratetgy Tester's main window. The function returns NULL if the window doesn't yet exist
 * (i.e. before the window was openend the first time).
 *
 * @return HWND - handle or NULL (0) in case of errors
 */
HWND WINAPI FindTesterWindow() {
   static HWND hWndTester;

   if (!hWndTester) {
      // The window may be docked at the terminal main window or may float in an independent top-level window.
      // In both cases the handle is the same.

      // check for a tester window docked at the terminal main window
      HWND hWndMain = GetTerminalMainWindow();
      if (!hWndMain) return(NULL);

      HWND hWnd = GetDlgItem(hWndMain, IDC_DOCKABLES_CONTAINER);        // top-level container for docked terminal windows
      if (!hWnd) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  cannot find top-level container for docked terminal windows"));

      hWndTester = GetDlgItem(hWnd, IDC_TESTER);
      if (hWndTester) return(hWndTester);

      // check for a floating tester window
      SetLastError(NO_ERROR);                                           // reset ERROR_CONTROL_ID_NOT_FOUND
      DWORD processId, self = GetCurrentProcessId();
      HWND hWndNext = GetTopWindow(NULL);
      if (!hWndNext) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetTopWindow(NULL)"));
      int error = NULL;

      while (hWndNext) {                                                // iterate over all top-level windows owned by the current process
         GetWindowThreadProcessId(hWndNext, &processId);
         if (processId == self) {                                       // the window belongs to us
            hWnd = GetDlgItem(hWndNext, IDC_UNDOCKED_CONTAINER);        // we can't rely on the window title as it's subject to i18n
            if (hWnd) {
               hWndTester = GetDlgItem(hWnd, IDC_TESTER);
               if (hWndTester) break;
               error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  no tester window found in container for floating tester window");
            }
         }
         hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
      }

      if (!hWndTester) debug("floating tester window not found");       // the window doesn't yet exist
   }
   return(hWndTester);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the bar model currently selected in the tester. If the tester window wasn't yet opened the function returns EMPTY (-1).
 *
 * @return int - bar model id or EMPTY (-1) in case of errors
 */
int WINAPI Tester_GetBarModel() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return(EMPTY);

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return(_EMPTY(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  \"Settings\" tab not found")));

   HWND hWndBarModel = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_BARMODEL);
   if (!hWndBarModel) return(_EMPTY(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  control element \"Model\" not found")));

   int index = ComboBox_GetCurSel(hWndBarModel);
   if (index == CB_ERR) {                          // CB_ERR = EMPTY = -1
      error(ERR_RUNTIME_ERROR, "failed to retrieve selection of control element Tester->Settings->Model (hWnd=%p)", hWndBarModel);
   }
   return(index);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the start date selected in the tester. If the tester window wasn't yet opened by the user the function
 * returns NULL (0).
 *
 * @return time32 - start date as a Unix timestamp or NULL (0) in case of errors
 */
time32 WINAPI Tester_GetStartDate() {
   // TODO: because of i18n we can't rely on the control's text

   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return(NULL);

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  \"Settings\" tab not found"));

   HWND hWndUseDate = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_USEDATE);
   if (!hWndUseDate) return(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  checkbox \"Use date\" in \"Settings\" tab not found"));

   uint bufSize = 24;                                       // big enough to hold the class name "SysDateTimePick32"
   char* className = (char*) alloca(bufSize);               // on the stack

   HWND hWndNext = GetWindow(hWndUseDate, GW_HWNDNEXT); if (!hWndNext)     return(error(ERR_WIN32_ERROR+GetLastError(), "GetWindow()  sibling of \"Use date\" checkbox in \"Settings\" tab not found"));

   char* wndTitle = GetInternalWindowTextA(hWndNext);       // FIX-ME
   if (!GetClassNameA(hWndNext, className, bufSize))                       return(error(ERR_WIN32_ERROR+GetLastError(), "GetClassNameA()"));
   if (!StrCompare(wndTitle, "From:") || !StrCompare(className, "Static")) return(error(ERR_RUNTIME_ERROR+GetLastError(), "unexpected sibling of \"Use date\" checkbox:  title=\"%s\"  class=\"%s\"", wndTitle, className));
   free(wndTitle);

   hWndNext = GetWindow(hWndNext, GW_HWNDNEXT); if (!hWndNext)             return(error(ERR_WIN32_ERROR+GetLastError(), "GetWindow()  sibling of \"From:\" label in \"Settings\" tab of tester window not found"));

   if (!GetClassNameA(hWndNext, className, bufSize))                       return(error(ERR_WIN32_ERROR+GetLastError(), "GetClassNameA()"));
   if (!StrCompare(className, "SysDateTimePick32"))                        return(error(ERR_RUNTIME_ERROR, "unexpected sibling of \"From:\" label:  title=\"%s\"  class=\"%s\"", wndTitle, className));
   wndTitle = GetInternalWindowTextA(hWndNext);
   if (!wndTitle || strlen(wndTitle) < 10)                                 return(error(ERR_WIN32_ERROR+GetLastError(), "GetInternalWindowTextA() unexpected text of \"Startdate\" control: \"%s\"", wndTitle));

   char* sDate = wndTitle;
   sDate[4] = sDate[7] = '\0';                              // string format: 2018.01.01
   TM time = {};
   time.tm_year  = atoi(&sDate[0]) - 1900;
   time.tm_mon   = atoi(&sDate[5]) - 1;
   time.tm_mday  = atoi(&sDate[8]);
   time.tm_isdst = 0;
   free(wndTitle);

   return(TmToUnixTime32(time));
   #pragma EXPANDER_EXPORT
}


/**
 * Get the end date selected in the tester. If the tester window wasn't yet opened by the user the function
 * returns NULL (0).
 *
 * @return time32 - end date as a Unix timestamp or NULL (0) in case of errors
 */
time32 WINAPI Tester_GetEndDate() {
   // TODO: because of i18n we can't rely on the control's text

   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return(NULL);

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  \"Settings\" tab not found in tester window"));

   HWND hWndOptimize = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_OPTIMIZATION);
   if (!hWndOptimize) return(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  \"Optimization\" checkbox in \"Settings\" tab of tester window not found"));

   uint bufSize = 24;                                       // big enough to hold the class name "SysDateTimePick32"
   char* className = (char*)alloca(bufSize);                // on the stack

   HWND hWndNext = GetWindow(hWndOptimize, GW_HWNDNEXT); if (!hWndNext)  return(error(ERR_WIN32_ERROR+GetLastError(), "GetWindow()  sibling of \"Optimization\" checkbox in \"Settings\" tab of tester window not found"));

   char* wndTitle = GetInternalWindowTextA(hWndNext);       // FIX-ME
   if (!GetClassName(hWndNext, className, bufSize))                      return(error(ERR_WIN32_ERROR+GetLastError(), "GetClassName()"));
   if (!StrCompare(wndTitle, "To:") || !StrCompare(className, "Static")) return(error(ERR_RUNTIME_ERROR+GetLastError(), "unexpected sibling of \"Optimization\" checkbox:  title=\"%s\"  class=\"%s\"", wndTitle, className));
   free(wndTitle);

   hWndNext = GetWindow(hWndNext, GW_HWNDNEXT); if (!hWndNext)           return(error(ERR_WIN32_ERROR+GetLastError(), "GetWindow()  sibling of \"To:\" label in \"Settings\" tab of tester window not found"));

   if (!GetClassName(hWndNext, className, bufSize))                      return(error(ERR_WIN32_ERROR+GetLastError(), "GetClassName()"));
   if (!StrCompare(className, "SysDateTimePick32"))                      return(error(ERR_RUNTIME_ERROR, "unexpected sibling of \"To:\" label:  title=\"%s\"  class=\"%s\"", wndTitle, className));
   wndTitle = GetInternalWindowTextA(hWndNext);
   if (!wndTitle || strlen(wndTitle) < 10)                               return(error(ERR_WIN32_ERROR+GetLastError(), "GetInternalWindowTextA() unexpected text of \"Enddate\" control: \"%s\"", wndTitle));

   char* sDate = wndTitle;
   sDate[4] = sDate[7] = '\0';                              // string format: 2018.01.01
   TM time = {};
   time.tm_year  = atoi(&sDate[0]) - 1900;
   time.tm_mon   = atoi(&sDate[5]) - 1;
   time.tm_mday  = atoi(&sDate[8]);
   time.tm_isdst = 0;
   free(wndTitle);

   return(TmToUnixTime32(time));
   #pragma EXPANDER_EXPORT
}


/**
 * Read the header of the test history file for the specified symbol, timeframe and bar model.
 *
 * @param  _In_  char*       symbol    - tested symbol
 * @param  _In_  uint        timeframe - test timeframe
 * @param  _In_  uint        barModel  - test bar model: MODE_EVERYTICK | MODE_CONTROLPOINTS | MODE_BAROPEN
 * @param  _Out_ FXT_HEADER* fxtHeader - pointer to a FXT_HEADER structure receiving the data
 *
 * @return BOOL - success status (e.g. FALSE on i/o errors or if the file does not exist)
 */
BOOL WINAPI Tester_ReadFxtHeader(const char* symbol, uint timeframe, uint barModel, FXT_HEADER* fxtHeader) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   if ((int)timeframe <= 0)              return(error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", (int)timeframe));
   using namespace std;

   // e.g. string(GetTerminalDataPathA()).append("\\tester\\history\\GBPJPY15_2.fxt");
   string fxtFile = string(GetTerminalDataPathA()).append("\\tester\\history\\")
                                                  .append(symbol)
                                                  .append(to_string(timeframe))
                                                  .append("_")
                                                  .append(to_string(barModel))
                                                  .append(".fxt");
   ifstream file(fxtFile.c_str(), ios::binary);
   if (!file) return(!warn(ERR_WIN32_ERROR+GetLastError(), "cannot open file \"%s\"", fxtFile.c_str()));

   file.read((char*)fxtHeader, sizeof(FXT_HEADER));
   file.close(); if (file.fail()) return(error(ERR_WIN32_ERROR+GetLastError(), "cannot read %d bytes from file \"%s\"", sizeof(FXT_HEADER), fxtFile.c_str()));

   return(TRUE);
}


/**
 * Get the commission value for the specified lotsize.
 *
 * @param  EXECUTION_CONTEXT* ec              - execution context of the expert under test
 * @param  double             lots [optional] - lotsize to calculate commission for (default: 1 lot)
 *
 * @return double - commission value or EMPTY (-1) in case of errors
 */
double WINAPI Test_GetCommission(const EXECUTION_CONTEXT* ec, double lots/*=1.0*/) {
   if ((uint)ec < MIN_VALID_POINTER)            return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (ec->programType!=PT_EXPERT || !ec->test) return(_EMPTY(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test")));

   TEST* test = ec->test;

   if (!test->fxtHeader) {
      FXT_HEADER* fxtHeader = new FXT_HEADER();
      if (!Tester_ReadFxtHeader(ec->symbol, ec->timeframe, test->barModel, fxtHeader)) {
         delete fxtHeader;
         return(_EMPTY(error(ERR_RUNTIME_ERROR, "cannot read FXT header")));
      }
      test->fxtHeader = fxtHeader;
   }

   return(test->fxtHeader->commissionValue * lots);
   #pragma EXPANDER_EXPORT
}


/**
 * TODO: validation
 */
BOOL WINAPI Test_onPositionOpen(const EXECUTION_CONTEXT* ec, int ticket, int type, double lots, const char* symbol, time32 openTime, double openPrice, double stopLoss, double takeProfit, double commission, int magicNumber, const char* comment) {
   if ((uint)ec        < MIN_VALID_POINTER)        return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->test)    return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));
   if ((uint)symbol    < MIN_VALID_POINTER)        return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   if (strlen(symbol)  > MAX_SYMBOL_LENGTH)        return(error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol: \"%s\" (max %d characters)", symbol, MAX_SYMBOL_LENGTH));
   if ((uint)comment   < MIN_VALID_POINTER)        return(error(ERR_INVALID_PARAMETER, "invalid parameter comment: 0x%p (not a valid pointer)", comment));
   if (strlen(comment) > MAX_ORDER_COMMENT_LENGTH) return(error(ERR_INVALID_PARAMETER, "illegal length of parameter comment: \"%s\" (max %d characters)", comment, MAX_ORDER_COMMENT_LENGTH));

   OrderList* positions      = ec->test->openPositions;      if (!positions)      return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.openPositions: 0x%p", ec->test->openPositions));
   OrderList* longPositions  = ec->test->openLongPositions;  if (!longPositions)  return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.openLongPositions: 0x%p", ec->test->openLongPositions));
   OrderList* shortPositions = ec->test->openShortPositions; if (!shortPositions) return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.openShortPositions: 0x%p", ec->test->openShortPositions));

   ORDER* order = new ORDER();
      order->test          = ec->test;
      order->ticket        = ticket;
      order->type          = type;
      order->lots          = lots;
      strcpy(order->symbol,  symbol);
      order->openTime      = openTime;
      order->openPrice     = openPrice;
      order->stopLoss      = stopLoss;
      order->takeProfit    = takeProfit;
      order->commission    = commission;
      order->magicNumber   = magicNumber;
      strcpy(order->comment, comment);
      order->highPrice     = ec->bid;              // inner prices to prevent stats distortion by spread widening
      order->lowPrice      = ec->ask;
   positions->push_back(order);

   if (order->type == OP_LONG)  longPositions->push_back(order);
   if (order->type == OP_SHORT) shortPositions->push_back(order);

   debug("position opened:  %s", ORDER_toStr(order));
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * TODO: validation
 *
 * @param  int    ticket
 * @param  double closePrice
 * @param  time32 closeTime
 * @param  double swap
 * @param  double profit
 *
 * @return BOOL - success status
 */
BOOL WINAPI Test_onPositionClose(const EXECUTION_CONTEXT* ec, int ticket, time32 closeTime, double closePrice, double swap, double profit) {
   if ((uint)ec < MIN_VALID_POINTER)            return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->test) return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));
   if (!ec->test->openPositions)                return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.openPositions: NULL"));

   OrderList &openPositions = *ec->test->openPositions;
   uint size = openPositions.size();
   ORDER* order = NULL;
   BOOL positionFound = FALSE;

   for (uint i=0; i < size; ++i) {
      order = openPositions[i];
      if (order->ticket == ticket) {
         // update order data
         order->closeTime  = closeTime;
         order->closePrice = closePrice;
         order->swap       = swap;
         order->profit     = profit;

         // update/calculate metrics
         if (order->type == OP_LONG) {
            order->runup    = round((order->highPrice  - order->openPrice)/ec->pip, 1);
            order->drawdown = round((order->lowPrice   - order->openPrice)/ec->pip, 1);
            order->realized = round((order->closePrice - order->openPrice)/ec->pip, 1);
         }
         else {
            order->runup    = round((order->openPrice - order->lowPrice  )/ec->pip, 1);
            order->drawdown = round((order->openPrice - order->highPrice )/ec->pip, 1);
            order->realized = round((order->openPrice - order->closePrice)/ec->pip, 1);
         }
         if (order->realized > order->runup)    order->runup    = order->realized;  // fix for applyment of closing spread
         if (order->realized < order->drawdown) order->drawdown = order->realized;

         // move the order to closed positions
         openPositions.erase(openPositions.begin() + i);                            // drop open position
         ec->test->closedPositions->push_back(order);                               // add it to closed positions
         positionFound = TRUE;

         if (order->type == OP_LONG) {
            OrderList &openLongs = *ec->test->openLongPositions;
            size = openLongs.size();
            for (i=0; i < size; ++i) {
               if (openLongs[i]->ticket == ticket) {
                  openLongs.erase(openLongs.begin() + i);                           // drop open long position
                  break;
               }
            }
            if (i >= size) return(error(ERR_RUNTIME_ERROR, "open long position #%d not found (%d long positions)", ticket, size));
            ec->test->closedLongPositions->push_back(order);                        // add it to closed long positions
         }
         else {
            OrderList &openShorts = *ec->test->openShortPositions;
            size = openShorts.size();
            for (i=0; i < size; ++i) {
               if (openShorts[i]->ticket == ticket) {
                  openShorts.erase(openShorts.begin() + i);                         // drop open short position
                  break;
               }
            }
            if (i >= size) return(error(ERR_RUNTIME_ERROR, "open short position #%d not found (%d short positions)", ticket, size));
            ec->test->closedShortPositions->push_back(order);                       // add it to closed short positions
         }

         debug("position closed:  %s", ORDER_toStr(order));
         break;
      }
   }

   if (!positionFound) return(error(ERR_RUNTIME_ERROR, "open position #%d not found (%d open positions)", ticket, size));
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Save the results of a test to a logfile.
 *
 * @param  TEST* test
 *
 * @return BOOL - success status
 */
BOOL WINAPI Test_SaveReport(const TEST* test) {
   if (!test->closedPositions) return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.closedPositions: NULL"));

   // define report directory and filename
   string path     = string(GetTerminalPathA()).append("/tester/files/testresults/");
   string filename = string(path).append(test->ec->programName).append(LocalTimeFormatA(test->created, "  %d.%m.%Y %H.%M.%S.log"));

   // make sure the directory exists
   int error = CreateDirectoryA(path, MODE_SYSTEM|MODE_MKPARENT);
   if (error) return(error(ERR_WIN32_ERROR+error, "cannot create directory \"%s\"", path.c_str()));

   // create the report file
   std::ofstream file(filename.c_str(), std::ios::binary);
   if (!file.is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "cannot open file \"%s\" (%s)", filename.c_str(), strerror(errno)));

   char* sTest = TEST_toStr(test);
   file << "test=" << sTest << NL;

   debug("test=%s", sTest);
   free(sTest);

   // process closed positions (skip open positions closed automatically by the tester at test end)
   OrderList &trades = *test->closedPositions;
   uint size = trades.size();

   for (uint i=0; i < size; ++i) {
      ORDER* order = trades[i];
      char* sOrder = ORDER_toStr(order);
      file << "order." << i << "=" << sOrder << NL;
      free(sOrder);
   }
   file.close();

   // backup input parameters
   // TODO: MetaTrader creates/updates the expert.ini file when the dialog "Expert properties" is confirmed.
   string source = string(GetTerminalPathA()) +"/tester/"+ test->ec->programName +".ini";
   string target = string(GetTerminalPathA()) +"/tester/files/testresults/"+ test->ec->programName + LocalTimeFormatA(test->created, "  %d.%m.%Y %H.%M.%S.ini");
   if (!CopyFile(source.c_str(), target.c_str(), TRUE))
      return(error(ERR_WIN32_ERROR+GetLastError(), "CopyFile()"));
   return(TRUE);
}


/**
 * TODO: documentation
 */
BOOL WINAPI Test_InitReporting(const EXECUTION_CONTEXT* ec, time32 startTime, uint bars) {
   if ((uint)ec < MIN_VALID_POINTER)               return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->pid)                                   return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (ec->programType!=PT_EXPERT || !ec->testing) return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only for experts in tester:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   TEST* test = ec->test;
   if (!test) return(error(ERR_ILLEGAL_STATE, "invalid execution context, ec.test=NULL:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   double spread = round((ec->ask - ec->bid)/ec->point/10, 1);

   test_SetStartTime   (test, startTime   );
   test_SetBars        (test, bars        );
   test_SetSpread      (test, spread      );
 //test_SetTradeDirections...                                  // TODO: read from "{expert-name}.ini"

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * TODO: documentation
 */
BOOL WINAPI Test_StopReporting(const EXECUTION_CONTEXT* ec, time32 endTime, uint bars) {
   if ((uint)ec < MIN_VALID_POINTER)               return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->pid)                                   return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (ec->programType!=PT_EXPERT || !ec->testing) return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only for experts in tester:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   TEST* test = ec->test;
   if (!test)            return(error(ERR_ILLEGAL_STATE, "invalid execution context, ec.test=NULL:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (!test->startTime) return(debug("skipping (reporting not yet started)"));
   if (test->endTime)    return(error(ERR_ILLEGAL_STATE, "reporting already stopped:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   test_SetBars   (test, bars - test->bars + 1);
   test_SetTicks  (test, ec->ticks            );
   test_SetEndTime(test, endTime              );

   // update test statistics
   OrderList &trades = *test->closedPositions;
   uint allTrades   = trades.size();
   uint longTrades  = test->closedLongPositions->size();
   uint shortTrades = test->closedShortPositions->size();

   double runup    = 0, longRunup    = 0, shortRunup    = 0;
   double drawdown = 0, longDrawdown = 0, shortDrawdown = 0;
   double realized = 0, longRealized = 0, shortRealized = 0;

   for (uint i=0; i < allTrades; ++i) {
      ORDER* order = trades[i];

      runup    += order->runup;
      drawdown += order->drawdown;
      realized += order->realized;

      if (order->type == OP_LONG) {
         longRunup    += order->runup;
         longDrawdown += order->drawdown;
         longRealized += order->realized;
      }
      else {
         shortRunup    += order->runup;
         shortDrawdown += order->drawdown;
         shortRealized += order->realized;
      }
   }

   test->stat_avgRunup         = allTrades ? round(runup   /allTrades, 1) : 0;
   test->stat_avgDrawdown      = allTrades ? round(drawdown/allTrades, 1) : 0;
   test->stat_avgRealized      = allTrades ? round(realized/allTrades, 1) : 0;

   test->stat_avgLongRunup     = longTrades ? round(longRunup   /longTrades, 1) : 0;
   test->stat_avgLongDrawdown  = longTrades ? round(longDrawdown/longTrades, 1) : 0;
   test->stat_avgLongRealized  = longTrades ? round(longRealized/longTrades, 1) : 0;

   test->stat_avgShortRunup    = shortTrades ? round(shortRunup   /shortTrades, 1) : 0;
   test->stat_avgShortDrawdown = shortTrades ? round(shortDrawdown/shortTrades, 1) : 0;
   test->stat_avgShortRealized = shortTrades ? round(shortRealized/shortTrades, 1) : 0;

   return(Test_SaveReport(test));
   #pragma EXPANDER_EXPORT
}


#include "lib/lock/Locker.h"


/**
 * @return int
 */
int WINAPI Test_synchronize() {
   {
      synchronize();
      debug("inside synchronized block");
   }
   return(0);

   char* s1 = "Hello";
   char* s2 = " world";
   char* result = strcat(strcat((char*)alloca(strlen(s1) + strlen(s2) + 2), s1), s2);
   debug("s1=\"%s\"  s2=\"%s\"  result=\"%s\"", s1, s2, result);                       // TODO: fix me

   return(0);
}


/**
 * @return int
 */
int WINAPI Tester_Test(const char* filename) {
   if ((uint)filename < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter filename: 0x%p (not a valid pointer)", filename));
   if (!*filename)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter filename: \"\" (empty)"));

   std::ofstream file = std::ofstream();

   file.open(filename, std::ios::app);

   if (!file.is_open()) {
      error(ERR_WIN32_ERROR+GetLastError(), "opening \"%s\" failed", filename);
      return(CreateDirectoryA(filename, MODE_SYSTEM));
   }
   else {
      file.close();
   }
   return(NULL);
   //#pragma EXPANDER_EXPORT
}
