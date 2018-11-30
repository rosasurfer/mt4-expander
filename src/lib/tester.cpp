#include "expander.h"
#include "lib/conversion.h"
#include "lib/file.h"
#include "lib/format.h"
#include "lib/math.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/tester.h"

#include <fstream>
#include <time.h>


/**
 * Find the window handle of the Stratetgy Tester's main window. The function returns NULL if the window doesn't yet exist
 * (i.e. before a floating tester window was openend by the user the first time).
 *
 * @return HWND - handle or NULL (0) in case of errors
 */
HWND WINAPI FindTesterWindow() {
   static HWND hWndTester;

   if (!hWndTester) {
      // The window may be (1) docked at the terminal main window or it may (2) float in an independent top-level window.
      // In both cases the handle is the same.
      // (last version using a dynamic classname for resolution: https://github.com/rosasurfer/mt4-mql CVS commit v1.498)

      // (1) check for a tester window docked at the terminal main window
      HWND hWndMain = GetTerminalMainWindow();
      if (!hWndMain) return(NULL);

      HWND hWnd = GetDlgItem(hWndMain, IDC_DOCKABLES_CONTAINER);        // top-level container for docked terminal windows
      if (!hWnd) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  cannot find top-level window for docked terminal windows"));

      hWndTester = GetDlgItem(hWnd, IDC_TESTER);
      if (hWndTester) return(hWndTester);
      SetLastError(NO_ERROR);                                           // reset ERROR_CONTROL_ID_NOT_FOUND


      // (2) check for a floating tester window owned by the current process
      DWORD processId, self = GetCurrentProcessId();
      uint bufSize = 12;                                                // big enough to hold the string "Tester"
      char* wndTitle = (char*) alloca(bufSize);                         // on the stack: buffer for window title
      HWND hWndNext = GetTopWindow(NULL);
      if (!hWndNext) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetTopWindow(NULL)"));

      while (hWndNext) {                                                // iterate over all top-level windows
         GetWindowThreadProcessId(hWndNext, &processId);
         if (processId == self) {                                       // the window belongs to us
            if (!GetWindowText(hWndNext, wndTitle, bufSize)) if (int error=GetLastError())
               return((HWND)error(ERR_WIN32_ERROR+error, "GetWindowText()"));

            if (StrStartsWith(wndTitle, "Tester")) {
               hWnd = GetDlgItem(hWndNext, IDC_UNDOCKED_CONTAINER);     // container for floating tester window
               if (!hWnd) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  container for floating tester window not found"));

               hWndTester = GetDlgItem(hWnd, IDC_TESTER);
               if (!hWndTester) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  no tester window found in container for floating tester window"));
               break;
            }
         }
         hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
      }

      if (!hWndTester) debug("Tester window not found");                // the window doesn't yet exist
   }
   return(hWndTester);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the bar model currently selected in the tester. If the tester window wasn't yet opened by the user the function
 * returns EMPTY (-1).
 *
 * @return int - bar model id or EMPTY (-1) in case of errors
 */
int WINAPI Tester_GetBarModel() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return(EMPTY);

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return(_EMPTY(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  tab \"Settings\" in tester window not found")));

   HWND hWndBarModel = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_BARMODEL);
   if (!hWndBarModel) return(_EMPTY(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  control \"Model\" in tab \"Settings\" of tester window not found")));

   uint bufSize = 20;                                                // big enough to hold the string "Open prices only"
   char* text = (char*) alloca(bufSize);                             // on the stack

   if (!GetWindowText(hWndBarModel, text, bufSize)) if (int error=GetLastError())
      return(_EMPTY(error(ERR_WIN32_ERROR+error, "GetWindowText()")));

   if (StrStartsWith(text, "Every tick"))       return(BARMODEL_EVERYTICK);
   if (StrStartsWith(text, "Control points"))   return(BARMODEL_CONTROLPOINTS);
   if (StrStartsWith(text, "Open prices only")) return(BARMODEL_BAROPEN);

   error(ERR_RUNTIME_ERROR, "unexpected window text of control Tester -> Settings -> Model: \"%s\"", text);
   return(EMPTY);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the start date currently selected in the tester. If the tester window wasn't yet opened by the user the function
 * returns NULL (0).
 *
 * @return datetime - start date as a Unix timestamp or NULL (0) in case of errors
 */
datetime WINAPI Tester_GetStartDate() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return(NULL);

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  \"Settings\" tab not found in tester window"));

   HWND hWndUseDate = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_USEDATE);
   if (!hWndUseDate) return(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  \"Use date\" checkbox in \"Settings\" tab of tester window not found"));

   uint bufSize = 24;                                       // big enough to hold the class name "SysDateTimePick32"
   char* wndTitle  = (char*)alloca(bufSize);
   char* className = (char*)alloca(bufSize);                // both on the stack

   HWND hWndNext = GetWindow(hWndUseDate, GW_HWNDNEXT); if (!hWndNext)            return(error(ERR_WIN32_ERROR+GetLastError(), "GetWindow()  sibling of \"Use date\" checkbox in \"Settings\" tab of tester window not found"));

   if (!GetWindowText(hWndNext, wndTitle, bufSize)) if (int error=GetLastError()) return(error(ERR_WIN32_ERROR+error, "GetWindowText()"));
   if (!GetClassName(hWndNext, className, bufSize))                               return(error(ERR_WIN32_ERROR+GetLastError(), "GetClassName()"));
   if (!StrCompare(wndTitle, "From:") || !StrCompare(className, "Static"))        return(error(ERR_RUNTIME_ERROR, "unexpected sibling of \"Use date\" checkbox:  title=\"%s\"  class=\"%s\"", wndTitle, className));

   hWndNext = GetWindow(hWndNext, GW_HWNDNEXT); if (!hWndNext)                    return(error(ERR_WIN32_ERROR+GetLastError(), "GetWindow()  sibling of \"From:\" label in \"Settings\" tab of tester window not found"));

   if (!GetWindowText(hWndNext, wndTitle, bufSize)) if (int error=GetLastError()) return(error(ERR_WIN32_ERROR+error, "GetWindowText()"));
   if (!GetClassName(hWndNext, className, bufSize))                               return(error(ERR_WIN32_ERROR+GetLastError(), "GetClassName()"));
   if (!StrCompare(className, "SysDateTimePick32"))                               return(error(ERR_RUNTIME_ERROR, "unexpected sibling of \"From:\" label:  title=\"%s\"  class=\"%s\"", wndTitle, className));

   char* date = wndTitle;
   date[4] = date[7] = '\0';                                // format: 2018.01.01
   tm tmdate = {};
   tmdate.tm_year  = atoi(&date[0]) - 1900;
   tmdate.tm_mon   = atoi(&date[5]) - 1;
   tmdate.tm_mday  = atoi(&date[8]);
   tmdate.tm_isdst = -1;

   //debug("startdate=%s.%s.%s  gmt=%s", &date[0], &date[5], &date[8], GmtTimeFormat(_mkgmtime(&tmdate), "%Y.%m.%d %H:%M:%S"));
   return(_mkgmtime(&tmdate));
   #pragma EXPANDER_EXPORT
}


/**
 * Get the end date currently selected in the tester. If the tester window wasn't yet opened by the user the function
 * returns NULL (0).
 *
 * @return datetime - end date as a Unix timestamp or NULL (0) in case of errors
 */
datetime WINAPI Tester_GetEndDate() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return(NULL);

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  \"Settings\" tab not found in tester window"));

   HWND hWndOptimize = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_OPTIMIZATION);
   if (!hWndOptimize) return(error(ERR_WIN32_ERROR+GetLastError(), "GetDlgItem()  \"Optimization\" checkbox in \"Settings\" tab of tester window not found"));

   uint bufSize = 24;                                       // big enough to hold the class name "SysDateTimePick32"
   char* wndTitle  = (char*)alloca(bufSize);
   char* className = (char*)alloca(bufSize);                // both on the stack

   HWND hWndNext = GetWindow(hWndOptimize, GW_HWNDNEXT); if (!hWndNext)           return(error(ERR_WIN32_ERROR+GetLastError(), "GetWindow()  sibling of \"Optimization\" checkbox in \"Settings\" tab of tester window not found"));

   if (!GetWindowText(hWndNext, wndTitle, bufSize)) if (int error=GetLastError()) return(error(ERR_WIN32_ERROR+error, "GetWindowText()"));
   if (!GetClassName(hWndNext, className, bufSize))                               return(error(ERR_WIN32_ERROR+GetLastError(), "GetClassName()"));
   if (!StrCompare(wndTitle, "To:") || !StrCompare(className, "Static"))          return(error(ERR_RUNTIME_ERROR, "unexpected sibling of \"Optimization\" checkbox:  title=\"%s\"  class=\"%s\"", wndTitle, className));

   hWndNext = GetWindow(hWndNext, GW_HWNDNEXT); if (!hWndNext)                    return(error(ERR_WIN32_ERROR+GetLastError(), "GetWindow()  sibling of \"To:\" label in \"Settings\" tab of tester window not found"));

   if (!GetWindowText(hWndNext, wndTitle, bufSize)) if (int error=GetLastError()) return(error(ERR_WIN32_ERROR+error, "GetWindowText()"));
   if (!GetClassName(hWndNext, className, bufSize))                               return(error(ERR_WIN32_ERROR+GetLastError(), "GetClassName()"));
   if (!StrCompare(className, "SysDateTimePick32"))                               return(error(ERR_RUNTIME_ERROR, "unexpected sibling of \"To:\" label:  title=\"%s\"  class=\"%s\"", wndTitle, className));

   char* date = wndTitle;
   date[4] = date[7] = '\0';                                // format: 2018.01.01
   tm tmdate = {};
   tmdate.tm_year  = atoi(&date[0]) - 1900;
   tmdate.tm_mon   = atoi(&date[5]) - 1;
   tmdate.tm_mday  = atoi(&date[8]);
   tmdate.tm_isdst = -1;

   //debug("enddate=%s.%s.%s  gmt=%s", &date[0], &date[5], &date[8], GmtTimeFormat(_mkgmtime(&tmdate), "%Y.%m.%d %H:%M:%S"));
   return(_mkgmtime(&tmdate));
   #pragma EXPANDER_EXPORT
}


/**
 * Read and return the header of the test history file for the specified symbol, timeframe and bar model.
 *
 * @param  char*  symbol    - tested symbol
 * @param  uint   timeframe - test timeframe
 * @param  uint   barModel  - test bar model: BARMODEL_EVERYTICK | BARMODEL_CONTROLPOINTS | BARMODEL_BAROPEN
 *
 * @return FXT_HEADER* - FXT header or NULL (0) in case of errors (e.g. the file does not exist)
 *
 *
 * Note: The memory for the returned FXT_HEADER was allocated with "new" and should be released after usage (with "delete").
 */
const FXT_HEADER* WINAPI Tester_ReadFxtHeader(const char* symbol, uint timeframe, uint barModel) {
   if ((uint)symbol < MIN_VALID_POINTER) return((FXT_HEADER*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   if ((int)timeframe <= 0)              return((FXT_HEADER*)error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", (int)timeframe));
   using namespace std;

   // e.g. string(GetTerminalDataPathA()).append("\\tester\\history\\GBPJPY15_2.fxt");
   string fxtFile = string(GetTerminalDataPathA()).append("\\tester\\history\\")
                                                  .append(symbol)
                                                  .append(to_string(timeframe))
                                                  .append("_")
                                                  .append(to_string(barModel))
                                                  .append(".fxt");
   ifstream file(fxtFile.c_str(), ios::binary);
   if (!file) return((FXT_HEADER*)warn(ERR_WIN32_ERROR+GetLastError(), "ifstream() cannot open file \"%s\"", fxtFile.c_str()));

   FXT_HEADER* fxt = new FXT_HEADER();
   file.read((char*)fxt, sizeof(FXT_HEADER));
   file.close(); if (file.fail()) return((FXT_HEADER*)error(ERR_WIN32_ERROR+GetLastError(), "ifstream.read() cannot read %d bytes from file \"%s\"", sizeof(FXT_HEADER), fxtFile.c_str()));

   return(fxt);
}


/**
 * Get the commission value for the specified lotsize.
 *
 * @param  EXECUTION_CONTEXT* ec              - execution context of the tested expert
 * @param  double             lots [optional] - lotsize to calculate commission for (default: 1 lot)
 *
 * @return double - commission value or EMPTY (-1) in case of errors
 */
double WINAPI Test_GetCommission(const EXECUTION_CONTEXT* ec, double lots/*=1.0*/) {
   if ((uint)ec < MIN_VALID_POINTER)            return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (ec->programType!=PT_EXPERT || !ec->test) return(_EMPTY(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test")));

   TEST* test = ec->test;
   if (!test->fxtHeader) test->fxtHeader = Tester_ReadFxtHeader(ec->symbol, ec->timeframe, test->barModel);

   const FXT_HEADER* fxt = test->fxtHeader;
   if (!fxt) return(EMPTY);

   if (lots == 1)
      return(fxt->commissionValue);
   return(fxt->commissionValue * lots);
   #pragma EXPANDER_EXPORT
}


/**
 * TODO: validation
 */
BOOL WINAPI Test_onPositionOpen(const EXECUTION_CONTEXT* ec, int ticket, int type, double lots, const char* symbol, double openPrice, datetime openTime, double stopLoss, double takeProfit, double commission, int magicNumber, const char* comment) {
   if ((uint)ec        < MIN_VALID_POINTER)        return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->test)    return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));
   if ((uint)symbol    < MIN_VALID_POINTER)        return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   if (strlen(symbol)  > MAX_SYMBOL_LENGTH)        return(error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol: \"%s\" (max %d characters)", symbol, MAX_SYMBOL_LENGTH));
   if ((uint)comment   < MIN_VALID_POINTER)        return(error(ERR_INVALID_PARAMETER, "invalid parameter comment: 0x%p (not a valid pointer)", comment));
   if (strlen(comment) > MAX_ORDER_COMMENT_LENGTH) return(error(ERR_INVALID_PARAMETER, "illegal length of parameter comment: \"%s\" (max %d characters)", comment, MAX_ORDER_COMMENT_LENGTH));

   OrderList* positions      = ec->test->positions;      if (!positions)      return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.positions: 0x%p", ec->test->positions));
   OrderList* longPositions  = ec->test->longPositions;  if (!longPositions)  return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.longPositions: 0x%p", ec->test->longPositions));
   OrderList* shortPositions = ec->test->shortPositions; if (!shortPositions) return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.shortPositions: 0x%p", ec->test->shortPositions));

   ORDER order = {};
      order.ticket      = ticket;
      order.type        = type;
      order.lots        = lots;
      strcpy(order.symbol, symbol);
      order.openPrice   = openPrice;
      order.openTime    = openTime;
      order.stopLoss    = stopLoss;
      order.takeProfit  = takeProfit;
      order.commission  = commission;
      order.magicNumber = magicNumber;
      strcpy(order.comment, comment);
   positions->push_back(order);                    // TODO: avoid push_back() creating a copy

   if (order.type == OP_LONG)  longPositions->push_back(order);
   if (order.type == OP_SHORT) shortPositions->push_back(order);

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
BOOL WINAPI Test_onPositionClose(const EXECUTION_CONTEXT* ec, int ticket, double closePrice, datetime closeTime, double swap, double profit) {
   if ((uint)ec < MIN_VALID_POINTER)            return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->test) return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));
   if (!ec->test->positions)                    return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.positions: 0x%p", ec->test->positions));

   OrderList& positions = *ec->test->positions;

   int i = positions.size()-1;
   for (; i >= 0; --i) {                                          // iterate in reverse order to speed-up
      ORDER& order = positions[i];
      if (order.ticket == ticket) {
         order.closePrice = closePrice;
         order.closeTime  = closeTime;
         order.swap       = swap;
         order.profit     = profit;

         // copy open position to closed positions
         ec->test->trades->push_back(order);

         if (order.type == OP_LONG) {
            OrderList& longPositions = *ec->test->longPositions;
            int j = longPositions.size()-1;
            for (; j >= 0; --j) {                                 // move open long position to closed long positions
               if (longPositions[j].ticket == ticket) {
                  longPositions.erase(longPositions.begin() + j);
                  break;
               }
            }
            if (j < 0) return(error(ERR_RUNTIME_ERROR, "open long position #%d not found, size(longPositions)=%d", ticket, longPositions.size()));
            ec->test->longTrades->push_back(order);
         }
         else if (order.type == OP_SHORT) {
            OrderList& shortPositions = *ec->test->shortPositions;
            int j = shortPositions.size()-1;
            for (; j >= 0; --j) {                                 // move open short position to closed short positions
               if (shortPositions[j].ticket == ticket) {
                  shortPositions.erase(shortPositions.begin() + j);
                  break;
               }
            }
            if (j < 0) return(error(ERR_RUNTIME_ERROR, "open short position #%d not found, size(shortPositions)=%d", ticket, shortPositions.size()));
            ec->test->shortTrades->push_back(order);
         }

         // drop order from open positions
         positions.erase(positions.begin() + i);                  // calls "delete order"
         break;
      }
   }
   if (i < 0) return(error(ERR_RUNTIME_ERROR, "open position #%d not found, size(positions)=%d", ticket, positions.size()));

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
   if (!test->trades) return(error(ERR_RUNTIME_ERROR, "invalid OrderList initialization, test.trades: 0x%p", test->trades));

   // create logfile
   string logfile = string(GetTerminalPathA()).append("/tester/files/testresults/")
                                              .append(test->strategy)
                                              .append(" #")
                                              .append(to_string(test->reportId))
                                              .append(localTimeFormat(test->created, "  %d.%m.%Y %H.%M.%S.log"));
   std::ofstream file(logfile.c_str());
   if (!file.is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "ofstream()  cannot open file \"%s\"", logfile.c_str()));

   file << "test=" << TEST_toStr(test) << NL;
   debug("test=%s", TEST_toStr(test));

   // process the known closed positions (skip open positions closed automatically by the tester at test stop)
   OrderList& trades = *test->trades;
   int size = trades.size();

   for (int i=0; i < size; ++i) {
      ORDER& order = trades[i];
      file << "order." << i << "=" << ORDER_toStr(&order) << NL;
   }
   file.close();

   // backup input parameters
   // TODO: MetaTrader creates/updates the expert.ini file when the dialog "Expert properties" is confirmed.
   string source = string(GetTerminalPathA()) +"/tester/"+ test->strategy +".ini";
   string target = string(GetTerminalPathA()) +"/tester/files/testresults/"+ test->strategy +" #"+ to_string(test->reportId) + localTimeFormat(test->created, "  %d.%m.%Y %H.%M.%S.ini");
   if (!CopyFile(source.c_str(), target.c_str(), TRUE))
      return(error(ERR_WIN32_ERROR+GetLastError(), "CopyFile()"));
   return(TRUE);
}


/**
 * TODO: documentation
 */
BOOL WINAPI Test_StartReporting(const EXECUTION_CONTEXT* ec, datetime startTime, uint bars, int reportId, const char* reportSymbol) {
   if ((uint)ec < MIN_VALID_POINTER)               return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->pid)                                   return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (ec->programType!=PT_EXPERT || !ec->testing) return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only for experts in tester:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   TEST* test = ec->test;
   if (!test) return(error(ERR_ILLEGAL_STATE, "invalid execution context, ec.test=NULL:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   double spread = round((ec->ask - ec->bid)/ec->point/10, 1);

   test_SetReportId    (test, reportId    );
   test_SetReportSymbol(test, reportSymbol);
   test_SetStartTime   (test, startTime   );
   test_SetSpread      (test, spread      );
   test_SetBars        (test, bars        );
 //test_SetTradeDirections...                                  // TODO: read from "{expert-name}.ini"

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * TODO: documentation
 */
BOOL WINAPI Test_StopReporting(const EXECUTION_CONTEXT* ec, datetime endTime, uint bars) {
   if ((uint)ec < MIN_VALID_POINTER)               return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->pid)                                   return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (ec->programType!=PT_EXPERT || !ec->testing) return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only for experts in tester:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   TEST* test = ec->test;
   if (!test)            return(error(ERR_ILLEGAL_STATE, "invalid execution context, ec.test=NULL:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
   if (!test->startTime) return( warn(ERR_ILLEGAL_STATE, "reporting not yet started (skipping execution)"));
   if (test->endTime)    return(error(ERR_ILLEGAL_STATE, "reporting already stopped:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   test_SetEndTime(test, endTime              );
   test_SetBars   (test, bars - test->bars + 1);
   test_SetTicks  (test, ec->ticks            );

   return(Test_SaveReport(test));
   #pragma EXPANDER_EXPORT
}


/**
 * @return int
 */
int WINAPI Test() {

   debug("sizeofMember(EXECUTION_CONTEXT.symbol) = %d", sizeofMember(EXECUTION_CONTEXT, symbol));

   return(NULL);
   #pragma EXPANDER_EXPORT
}
