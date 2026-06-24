#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/helper.h"
#include "lib/math.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/tester.h"
#include "lib/win32.h"

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
      if (!hWndMain) return NULL;

      HWND hWnd = GetDlgItem(hWndMain, IDC_DOCKED_CONTAINER);           // container for docked terminal windows
      if (!hWnd) return (HWND)!error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(MainWindow, IDC_DOCKED_CONTAINER)");

      HWND hWndSuccess = GetDlgItem(hWnd, IDC_TESTER);                  // use non-static var for intermediate results (thread concurrency)
      if (!hWndSuccess) {
         SetLastError(NO_ERROR);                                        // reset the last ERROR_CONTROL_ID_NOT_FOUND

         // check for a floating tester window
         HWND hWndNext = GetTopWindow(NULL);
         if (!hWndNext) return (HWND)!error(ERR_WIN32_ERROR + GetLastError(), "no top-level windows found");

         DWORD processId, self=GetCurrentProcessId();
         while (hWndNext) {                                             // iterate over all top-level windows owned by the current process
            GetWindowThreadProcessId(hWndNext, &processId);
            if (processId == self) {                                    // the window belongs to us
               hWnd = GetDlgItem(hWndNext, IDC_FLOATING_CONTAINER);     // due to i18n we can't interprete the window titles
               if (hWnd) {
                  hWndSuccess = GetDlgItem(hWnd, IDC_TESTER);
                  if (hWndSuccess) break;
               }
            }
            hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
         }
         if (!hWndSuccess) debug("tester window doesn't yet exist");
      }
      if (hWndSuccess) hWndTester = hWndSuccess;
   }
   return hWndTester;
   #pragma EXPANDER_EXPORT
}


/**
 * Get the bar model currently selected in the tester. If the tester window wasn't yet opened the function returns EMPTY (-1).
 *
 * @return int - bar model id or EMPTY (-1) in case of errors
 */
int WINAPI Tester_GetBarModel() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return EMPTY;

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return _EMPTY(error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(TesterWindow, IDC_TESTER_SETTINGS)"));

   HWND hWndBarModel = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_BARMODEL);
   if (!hWndBarModel) return _EMPTY(error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(Settings, IDC_TESTER_SETTINGS_BARMODEL)"));

   int index = ComboBox_GetCurSel(hWndBarModel);      // CB_ERR = EMPTY = -1
   if (index == CB_ERR) error(ERR_RUNTIME_ERROR, "failed to retrieve selection of control element Tester->Settings->Model (hWnd=%p)", hWndBarModel);

   return index;
   #pragma EXPANDER_EXPORT
}


/**
 * Get the start date selected in the tester. If the tester window wasn't yet opened the function returns NULL (0).
 *
 * @return time32 - start date as a Unix timestamp or NULL (0) in case of errors
 */
time32 WINAPI Tester_GetStartDate() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return NULL;

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(TesterWindow, IDC_TESTER_SETTINGS)");

   HWND hWndUseDate = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_USEDATE);
   if (!hWndUseDate) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(Settings, IDC_TESTER_SETTINGS_USEDATE");

   HWND hWndNext = GetWindow(hWndUseDate, GW_HWNDNEXT);
   if (hWndNext) hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
   if (!hWndNext) return !error(ERR_WIN32_ERROR + GetLastError(), "TesterSettings: no 2 siblings of IDC_TESTER_SETTINGS_USEDATE (\"Use date\" checkbox) found");

   wstring className = getClassNameW(hWndNext);
   if (className != L"SysDateTimePick32") return !error(ERR_ILLEGAL_STATE, "TesterSettings: unexpected sibling of \"Use date\" checkbox: %p %S \"%S\"", hWndNext, className.c_str(), getInternalWindowTextW(hWndNext).c_str());

   wchar* wndTitle = GetWindowTextW(hWndNext);        // class SysDateTimePick32 can't be read using GetInternalWindowText()
   if (!wndTitle || wstrlen(wndTitle) < 10) {
      error(ERR_WIN32_ERROR + GetLastError(), "TesterSettings: unexpected text of \"From date\" control: \"%S\"", wndTitle);
      return _NULL(bfree(wndTitle));
   }

   wchar* sDate = wndTitle;
   sDate[4] = sDate[7] = '\0';                        // string format: 2018.01.01
   TM time = {};
   time.tm_year  = wstrtoint(&sDate[0]) - 1900;
   time.tm_mon   = wstrtoint(&sDate[5]) - 1;
   time.tm_mday  = wstrtoint(&sDate[8]);
   time.tm_isdst = 0;
   free(wndTitle);

   return TmToUnixTime32(time);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the end date selected in the tester. If the tester window wasn't yet opened the function returns NULL (0).
 *
 * @return time32 - end date as a Unix timestamp or NULL (0) in case of errors
 */
time32 WINAPI Tester_GetEndDate() {
   HWND hWndTester = FindTesterWindow();
   if (!hWndTester) return NULL;

   HWND hWndSettings = GetDlgItem(hWndTester, IDC_TESTER_SETTINGS);
   if (!hWndSettings) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(TesterWindow, IDC_TESTER_SETTINGS)");

   HWND hWndOptimize = GetDlgItem(hWndSettings, IDC_TESTER_SETTINGS_OPTIMIZATION);
   if (!hWndOptimize) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDlgItem(TesterSettings, IDC_TESTER_SETTINGS_OPTIMIZATION)");

   HWND hWndNext = GetWindow(hWndOptimize, GW_HWNDNEXT);
   if (hWndNext) hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
   if (!hWndNext) return !error(ERR_WIN32_ERROR + GetLastError(), "TesterSettings: no 2 siblings of IDC_TESTER_SETTINGS_OPTIMIZATION (\"Optimization\" checkbox) found");

   wstring className = getClassNameW(hWndNext);
   if (className != L"SysDateTimePick32") return !error(ERR_ILLEGAL_STATE, "TesterSettings: unexpected sibling of \"Optimization:\" checkbox: %p %S \"%S\"", hWndNext, className.c_str(), getInternalWindowTextW(hWndNext).c_str());

   wchar* wndTitle = GetWindowTextW(hWndNext);        // class SysDateTimePick32 can't be read using GetInternalWindowText()
   if (!wndTitle || wstrlen(wndTitle) < 10) {
      error(ERR_WIN32_ERROR + GetLastError(), "TesterSettings: unexpected text of \"To date\" control: \"%S\"", wndTitle);
      return _NULL(bfree(wndTitle));
   }

   wchar* sDate = wndTitle;
   sDate[4] = sDate[7] = '\0';                        // string format: 2018.01.01
   TM time = {};
   time.tm_year  = wstrtoint(&sDate[0]) - 1900;
   time.tm_mon   = wstrtoint(&sDate[5]) - 1;
   time.tm_mday  = wstrtoint(&sDate[8]);
   time.tm_isdst = 0;
   free(wndTitle);

   return TmToUnixTime32(time);
   #pragma EXPANDER_EXPORT
}


/**
 * Read the header of the test history file for the specified symbol, timeframe and bar model.
 *
 * @param  _In_  char*       symbol    - tested symbol
 * @param  _In_  uint        timeframe - test timeframe
 * @param  _In_  uint        barModel  - test bar model: MODE_EVERYTICK | MODE_CONTROLPOINTS | MODE_BAROPEN
 * @param  _Out_ FXT_HEADER &fxtHeader - struct FXT_HEADER receiving the data
 *
 * @return BOOL - success status (e.g. FALSE on I/O errors or if the file does not exist)
 */
BOOL WINAPI Tester_ReadFxtHeader(const char* symbol, uint timeframe, uint barModel, FXT_HEADER &fxtHeader) {
   if ((uint)symbol < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol);
   if ((int)timeframe <= 0)              return !error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", (int)timeframe);
   using namespace std;

   // e.g. string(GetTerminalDataPathA()).append("\\tester\\history\\GBPJPY15_2.fxt");
   string fxtFile = string(GetTerminalDataPathA()).append("\\tester\\history\\")
                                                  .append(symbol)
                                                  .append(to_string(timeframe))
                                                  .append("_")
                                                  .append(to_string(barModel))
                                                  .append(".fxt");
   ifstream file(fxtFile.c_str(), ios::binary);
   if (!file) return !warn(ERR_WIN32_ERROR + GetLastError(), "cannot open file \"%s\"", fxtFile.c_str());

   file.read((char*)&fxtHeader, sizeof(fxtHeader));
   file.close(); if (file.fail()) return !error(ERR_WIN32_ERROR + GetLastError(), "cannot read %d bytes from file \"%s\"", sizeof(fxtHeader), fxtFile.c_str());
   return TRUE;
}


/**
 * Get the commission value for a test.
 *
 * @param  EXECUTION_CONTEXT* ec - execution context of the expert under test
 *
 * @return double - commission value or EMPTY (-1) in case of errors
 */
double WINAPI Test_GetCommission(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER)               return _EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->programType!=PT_EXPERT || !ec->testing) return _EMPTY(error(ERR_FUNC_NOT_ALLOWED, "function allowed only in experts under test"));

   int barModel = Tester_GetBarModel();
   FXT_HEADER fxtHeader = {};

   if (!Tester_ReadFxtHeader(ec->symbol, ec->timeframe, barModel, fxtHeader)) {
      return _EMPTY(error(ERR_RUNTIME_ERROR, "cannot read FXT header for %s,%s (bar model: %s)", ec->symbol, PeriodDescriptionA(ec->timeframe), BarModelDescription(barModel)));
   }
   return fxtHeader.commissionValue;
   #pragma EXPANDER_EXPORT
}
