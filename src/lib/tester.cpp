#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/helper.h"
#include "lib/math.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/tester.h"

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
      // The window may be docked at the terminal main window or float in an independent top-level window.
      // In both cases the handle is the same.

      // check for a docked tester window
      HWND hWndMain = GetTerminalMainWindow();
      if (!hWndMain) return(NULL);

      HWND hWnd = GetDlgItem(hWndMain, IDC_DOCKED_CONTAINER);           // container for docked terminal windows
      if (!hWnd) return((HWND)!error(ERR_WIN32_ERROR+GetLastError(), "no TerminalMainWindow->IDC_DOCKED_CONTAINER found"));

      hWndTester = GetDlgItem(hWnd, IDC_TESTER);
      if (hWndTester) return(hWndTester);
      SetLastError(NO_ERROR);                                           // reset ERROR_CONTROL_ID_NOT_FOUND

      // check for a floating tester window
      HWND hWndNext = GetTopWindow(NULL);
      if (!hWndNext) return((HWND)!error(ERR_WIN32_ERROR+GetLastError(), "no top-level windows found"));

      DWORD processId, self=GetCurrentProcessId();
      while (hWndNext) {                                                // iterate over all top-level windows owned by the current process
         GetWindowThreadProcessId(hWndNext, &processId);
         if (processId == self) {                                       // the window belongs to us
            hWnd = GetDlgItem(hWndNext, IDC_FLOATING_CONTAINER);        // due to i18n we can't interprete the window titles
            if (hWnd) {
               hWndTester = GetDlgItem(hWnd, IDC_TESTER);
               if (hWndTester) break;
            }
         }
         hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
      }

      if (!hWndTester) debug("tester window doesn't yet exist");
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
 * Get the start date selected in the tester. If the tester window wasn't yet opened the function returns NULL (0).
 *
 * @return time32 - start date as a Unix timestamp or NULL (0) in case of errors
 */
time32 WINAPI Tester_GetStartDate() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return(NULL);

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return(!error(ERR_WIN32_ERROR+GetLastError(), "Tester->IDC_TESTER_SETTINGS (\"Settings\" tab) not found"));

   HWND hWndUseDate = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_USEDATE);
   if (!hWndUseDate) return(!error(ERR_WIN32_ERROR+GetLastError(), "TesterSettings->IDC_TESTER_SETTINGS_USEDATE (\"Use date\" checkbox) not found"));

   HWND hWndNext = GetWindow(hWndUseDate, GW_HWNDNEXT);
   if (hWndNext) hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
   if (!hWndNext) return(!error(ERR_WIN32_ERROR+GetLastError(), "TesterSettings: no 2 siblings of IDC_TESTER_SETTINGS_USEDATE (\"Use date\" checkbox) found"));

   uint bufSize = 32;                                 // big enough to hold class name "SysDateTimePick32"
   wchar* className = (wchar*)alloca(bufSize*2);      // on the stack
   GetClassNameW(hWndNext, className, bufSize);
   if (!StrCompare(className, L"SysDateTimePick32")) return(!error(ERR_ILLEGAL_STATE, "TesterSettings: unexpected sibling of \"Use date\" checkbox: %p  title=%S  class=%S", hWndNext, DoubleQuoteStr(GetInternalWindowTextW(hWndNext)), DoubleQuoteStr(className)));

   wchar* wndTitle = GetWindowTextW(hWndNext);        // class "SysDateTimePick32" can't be read using GetInternalWindowText()
   if (!wndTitle || wstrlen(wndTitle) < 10)          return(!error(ERR_WIN32_ERROR+GetLastError(), "TesterSettings: unexpected text of \"From date\" control: %S", DoubleQuoteStr(wndTitle)));

   wchar* sDate = wndTitle;
   sDate[4] = sDate[7] = '\0';                        // string format: 2018.01.01
   TM time = {};
   time.tm_year  = wstrtoint(&sDate[0]) - 1900;
   time.tm_mon   = wstrtoint(&sDate[5]) - 1;
   time.tm_mday  = wstrtoint(&sDate[8]);
   time.tm_isdst = 0;
   free(wndTitle);

   return(TmToUnixTime32(time));
   #pragma EXPANDER_EXPORT
}


/**
 * Get the end date selected in the tester. If the tester window wasn't yet opened the function returns NULL (0).
 *
 * @return time32 - end date as a Unix timestamp or NULL (0) in case of errors
 */
time32 WINAPI Tester_GetEndDate() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return(NULL);

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return(!error(ERR_WIN32_ERROR+GetLastError(), "Tester->IDC_TESTER_SETTINGS (\"Settings\" tab) not found"));

   HWND hWndOptimize = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_OPTIMIZATION);
   if (!hWndOptimize) return(!error(ERR_WIN32_ERROR+GetLastError(), "TesterSettings->IDC_TESTER_SETTINGS_OPTIMIZATION (\"Optimization\" checkbox) not found"));

   HWND hWndNext = GetWindow(hWndOptimize, GW_HWNDNEXT);
   if (hWndNext) hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
   if (!hWndNext) return(!error(ERR_WIN32_ERROR+GetLastError(), "TesterSettings: no 2 siblings of IDC_TESTER_SETTINGS_OPTIMIZATION (\"Optimization\" checkbox) found"));

   uint bufSize = 32;                                 // big enough to hold class name "SysDateTimePick32"
   wchar* className = (wchar*)alloca(bufSize*2);      // on the stack
   GetClassNameW(hWndNext, className, bufSize);
   if (!StrCompare(className, L"SysDateTimePick32")) return(!error(ERR_ILLEGAL_STATE, "TesterSettings: unexpected sibling of \"Optimization:\" checkbox: %p  title=%S  class=%S", hWndNext, DoubleQuoteStr(GetInternalWindowTextW(hWndNext)), DoubleQuoteStr(className)));

   wchar* wndTitle = GetWindowTextW(hWndNext);        // class "SysDateTimePick32" can't be read using GetInternalWindowText()
   if (!wndTitle || wstrlen(wndTitle) < 10)          return(!error(ERR_WIN32_ERROR+GetLastError(), "TesterSettings: unexpected text of \"To date\" control: %S", DoubleQuoteStr(wndTitle)));

   wchar* sDate = wndTitle;
   sDate[4] = sDate[7] = '\0';                        // string format: 2018.01.01
   TM time = {};
   time.tm_year  = wstrtoint(&sDate[0]) - 1900;
   time.tm_mon   = wstrtoint(&sDate[5]) - 1;
   time.tm_mday  = wstrtoint(&sDate[8]);
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
 * @return BOOL - success status (e.g. FALSE on I/O errors or if the file does not exist)
 */
BOOL WINAPI Tester_ReadFxtHeader(const char* symbol, uint timeframe, uint barModel, FXT_HEADER* fxtHeader) {
   if ((uint)symbol < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   if ((int)timeframe <= 0)              return(!error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", (int)timeframe));
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
   file.close(); if (file.fail()) return(!error(ERR_WIN32_ERROR+GetLastError(), "cannot read %d bytes from file \"%s\"", sizeof(FXT_HEADER), fxtFile.c_str()));
   return(TRUE);
}


/**
 * Get the commission value for a test.
 *
 * @param  EXECUTION_CONTEXT* ec - execution context of the expert under test
 *
 * @return double - commission value or EMPTY (-1) in case of errors
 */
double WINAPI Test_GetCommission(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER)               return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (ec->programType!=PT_EXPERT || !ec->testing) return(_EMPTY(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test")));

   FXT_HEADER* fxtHeader = new FXT_HEADER();
   if (!Tester_ReadFxtHeader(ec->symbol, ec->timeframe, Tester_GetBarModel(), fxtHeader)) {
      delete fxtHeader;
      return(_EMPTY(error(ERR_RUNTIME_ERROR, "cannot read FXT header")));
   }
   return(fxtHeader->commissionValue);
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
 * @param  HWND  hWnd - window handle
 *
 * @return int
 */
int WINAPI Tester_Test(HWND hWnd) {
   RECT rect;
   if (!GetWindowRect(hWnd, &rect)) return(!error(ERR_WIN32_ERROR+GetLastError(), "GetWindowRect(hWnd=%p) failed", hWnd));

   debug("left=%d  top=%d  right=%d  bottom=%d", rect.left, rect.top, rect.right, rect.bottom);
   return(NULL);


   time32 startdate = Tester_GetStartDate();
   string sStartdate = gmtTimeFormat(startdate, "%a, %Y.%m.%d %H:%M:%S");
   debug("start date: %s", sStartdate.c_str());

   time32 enddate = Tester_GetEndDate();
   string sEnddate = gmtTimeFormat(enddate, "%a, %Y.%m.%d %H:%M:%S");
   debug("end date:   %s", sEnddate.c_str());

   return(startdate);
   //#pragma EXPANDER_EXPORT
}
