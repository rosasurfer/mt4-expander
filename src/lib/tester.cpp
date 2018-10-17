#include "expander.h"
#include "lib/conversion.h"
#include "lib/file.h"
#include "lib/format.h"
#include "lib/math.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/tester.h"
#include "struct/mt4/FxtHeader.h"

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
            int len = GetWindowText(hWndNext, wndTitle, bufSize);
            if (!len) if (int error=GetLastError()) return((HWND)error(ERR_WIN32_ERROR+error, "GetWindowText()"));

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

   int len = GetWindowText(hWndBarModel, text, bufSize);
   if (!len) return(_EMPTY(error(ERR_WIN32_ERROR+GetLastError(), "GetWindowText()  0 chars copied")));

   if (StrStartsWith(text, "Every tick"))       return(BARMODEL_EVERYTICK);
   if (StrStartsWith(text, "Control points"))   return(BARMODEL_CONTROLPOINTS);
   if (StrStartsWith(text, "Open prices only")) return(BARMODEL_BAROPEN);

   error(ERR_RUNTIME_ERROR, "unexpected window text of control Tester -> Settings -> Model: \"%s\"", text);
   return(EMPTY);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the commission value for the given lotsize as defined for the specified symbol under test. Opposite to life trading in
 * the Strategy Tester commission can be set to a custom value per test (in the tester's history file). In order for this
 * function to work a tester history file for the specified symbol, timeframe and bar model must exist.
 *
 * @param  char*  symbol          - symbol under test
 * @param  uint   timeframe       - test timeframe
 * @param  uint   barModel        - test bar model: BARMODEL_EVERYTICK | BARMODEL_CONTROLPOINTS | BARMODEL_BAROPEN
 * @param  double lots [optional] - lotsize to calculate commission for (default: 1 lot)
 *
 * @return double - commission value or EMPTY (-1) in case of errors
 */
double WINAPI Tester_GetCommissionValue(const char* symbol, uint timeframe, uint barModel, double lots/*=1.0*/) {
   if ((uint)symbol < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol)));
   if ((int)timeframe <= 0)              return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", (int)timeframe)));
   using namespace std;

   // e.g. string(GetTerminalDataPathA()).append("\\tester\\history\\GBPJPY15_2.fxt");
   string fxtFile = string(GetTerminalDataPathA()).append("\\tester\\history\\")
                                                  .append(symbol)
                                                  .append(to_string(timeframe))
                                                  .append("_")
                                                  .append(to_string(barModel))
                                                  .append(".fxt");
   ifstream file(fxtFile.c_str(), ios::binary);
   if (!file) return(_EMPTY(error(ERR_WIN32_ERROR+GetLastError(), "ifstream() cannot open file \"%s\"", fxtFile.c_str())));

   FXT_HEADER fxt = {};
   file.read((char*)&fxt, sizeof(FXT_HEADER));
   file.close(); if (file.fail()) return(_EMPTY(error(ERR_WIN32_ERROR+GetLastError(), "ifstream.read() cannot read %d bytes from file \"%s\"", sizeof(FXT_HEADER), fxtFile.c_str())));

   if (lots == 1)
      return(fxt.commissionValue);
   return(fxt.commissionValue * lots);
   #pragma EXPANDER_EXPORT
}


/**
 * TODO: documentation
 */
BOOL WINAPI CollectTestData(EXECUTION_CONTEXT* ec, datetime startTime, datetime endTime, int barModel, double bid, double ask, uint bars, int reportingId, const char* reportingSymbol) {
   if ((uint)ec < MIN_VALID_POINTER)               return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (!ec->programIndex)                          return(error(ERR_INVALID_PARAMETER, "invalid execution context, ec.programIndex: %d", ec->programIndex));
   if (ec->programType!=PT_EXPERT || !ec->testing) return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));

   TEST* test;

   if (ec->rootFunction == RF_START) {
      if (ec->test) return(error(ERR_RUNTIME_ERROR, "multiple TEST initializations in %s::start()", ec->programName));

      ec->test = test = new TEST();
      test_SetTime           (test, time(NULL)      );
      test_SetStrategy       (test, ec->programName );
      test_SetReportingId    (test, reportingId     );
      test_SetReportingSymbol(test, reportingSymbol );
      test_SetSymbol         (test, ec->symbol      );
      test_SetTimeframe      (test, ec->timeframe   );
      test_SetStartTime      (test, startTime       );
      test_SetBarModel       (test, barModel        );
      test_SetSpread         (test, (ask-bid)/0.0001);               // TODO: statt 0.0001 Variable Pip
      test_SetBars           (test, bars            );
      //uint tradeDirections;                                        // TODO: aus Expert.ini auslesen
      test_SetVisualMode     (test, ec->visualMode  );
      test_SetDuration       (test, GetTickCount()  );
      test->orders = new OrderHistory(512);                          // reserve memory to speed-up testing
      test->orders->resize(0);
   }
   else if (ec->rootFunction == RF_DEINIT) {
      test = ec->test;
      if (!test) return(error(ERR_RUNTIME_ERROR, "missing TEST initialization in %s::deinit()", ec->programName));

      test_SetEndTime (test, endTime                        );
      test_SetBars    (test, bars - test->bars + 1          );
      test_SetTicks   (test, ec->ticks                      );
      test_SetDuration(test, GetTickCount() - test->duration);

      SaveTest(test);                                                // TODO: close memory leak => TEST, OrderHistory
   }
   else return(error(ERR_FUNC_NOT_ALLOWED, "function not allowed in %s::%s()", ec->programName, RootFunctionDescription(ec->rootFunction)));

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Save the results of a test.
 *
 * @param  TEST* test
 *
 * @return BOOL - success status
 */
BOOL WINAPI SaveTest(TEST* test) {
   // save TEST to logfile
   string logfile = string(GetTerminalPathA()).append("/tester/files/testresults/")
                                              .append(test->strategy)
                                              .append(" #")
                                              .append(to_string(test->reportingId))
                                              .append(localTimeFormat(test->time, "  %d.%m.%Y %H.%M.%S.log"));
   std::ofstream file(logfile.c_str());
   if (!file.is_open()) return(error(ERR_WIN32_ERROR+GetLastError(), "ofstream()  cannot open file \"%s\"", logfile.c_str()));

   file << "test=" << TEST_toStr(test) << "\n";
   debug("test=%s", TEST_toStr(test));

   OrderHistory* orders = test->orders; if (!orders) return(error(ERR_RUNTIME_ERROR, "invalid OrderHistory, test.orders: 0x%p", test->orders));
   int size = orders->size();

   for (int i=0; i < size; ++i) {
      ORDER* order = &(*orders)[i];
      file << "order." << i << "=" << ORDER_toStr(order) << "\n";
   }
   file.close();

   // backup input parameters
   // TODO: MetaTrader creates/updates the expert.ini file when the dialog "Expert properties" is confirmed.
   string source = string(GetTerminalPathA()) +"/tester/"+ test->strategy +".ini";
   string target = string(GetTerminalPathA()) +"/tester/files/testresults/"+ test->strategy +" #"+ to_string(test->reportingId) + localTimeFormat(test->time, "  %d.%m.%Y %H.%M.%S.ini");
   if (!CopyFile(source.c_str(), target.c_str(), TRUE))
      return(error(ERR_WIN32_ERROR+GetLastError(), "=> CopyFile()"));
   return(TRUE);
}


/**
 * TODO: validation
 */
BOOL WINAPI Test_onPositionOpen(EXECUTION_CONTEXT* ec, int ticket, int type, double lots, const char* symbol, double openPrice, datetime openTime, double stopLoss, double takeProfit, double commission, int magicNumber, const char* comment) {
   if ((uint)ec < MIN_VALID_POINTER)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->testing)   return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));

   TEST*         test   = ec->test;     if (!test)   return(error(ERR_RUNTIME_ERROR, "invalid TEST initialization, ec.test: 0x%p", ec->test));
   OrderHistory* orders = test->orders; if (!orders) return(error(ERR_RUNTIME_ERROR, "invalid OrderHistory initialization, test.orders: 0x%p", test->orders));

   ORDER order = {};
      order.ticket      = ticket;
      order.type        = type;
      order.lots        = round(lots, 2);
      strcpy(order.symbol, symbol);
      order.openPrice   = round(openPrice, 5);
      order.openTime    = openTime;
      order.stopLoss    = round(stopLoss,   5);
      order.takeProfit  = round(takeProfit, 5);
      order.commission  = round(commission, 2);
      order.magicNumber = magicNumber;
      strcpy(order.comment, comment);
   orders->push_back(order);

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
BOOL WINAPI Test_onPositionClose(EXECUTION_CONTEXT* ec, int ticket, double closePrice, datetime closeTime, double swap, double profit) {
   if ((uint)ec < MIN_VALID_POINTER)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->testing)   return(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));

   TEST*         test   = ec->test;     if (!test)   return(error(ERR_RUNTIME_ERROR, "invalid TEST initialization, ec.test: 0x%p", ec->test));
   OrderHistory* orders = test->orders; if (!orders) return(error(ERR_RUNTIME_ERROR, "invalid OrderHistory initialization, test.orders: 0x%p", test->orders));

   uint i = orders->size()-1;

   for (; i >= 0; --i) {                                             // iterate in reverse order to speed-up
      ORDER* order = &(*orders)[i];
      if (order->ticket == ticket) {
         order->closePrice = round(closePrice, 5);
         order->closeTime  = closeTime;
         order->swap       = round(swap,   2);
         order->profit     = round(profit, 2);
         break;
      }
   }
   if (i < 0) return(error(ERR_RUNTIME_ERROR, "ticket #%d not found, size(orders)=%d", ticket, orders->size()));

   return(TRUE);
   #pragma EXPANDER_EXPORT
}
