#include "expander.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"


/**
 * Return a description "<symbol>,<timeframe>" for the chart title bar (e.g. "EURUSD,Daily").
 *
 * @param  string &symbol
 * @param  uint   timeframe
 * @param  bool   custom [optional] - whether to support custom timeframes (default: no)
 *
 * @return string - chart title or an empty string in case of errors; call GetLastError() for details
 */
string WINAPI ChartTitleA(const string &symbol, uint timeframe, bool custom/*= false*/) {
   size_t symbolLength = symbol.length();
   if (!symbolLength || symbolLength > MAX_SYMBOL_LENGTH) return _empty_str(error(ERR_INVALID_PARAMETER, "invalid parameter symbol: \"%s\"", symbol.c_str()));

   string description = "";

   switch (timeframe) {
      case PERIOD_M1 : description += "M1";      break;
      case PERIOD_M5 : description += "M5";      break;
      case PERIOD_M10: description += "M10";     break;
      case PERIOD_M15: description += "M15";     break;
      case PERIOD_M30: description += "M30";     break;
      case PERIOD_H1 : description += "H1";      break;
      case PERIOD_H4 : description += "H4";      break;
      case PERIOD_D1 : description += "Daily";   break;
      case PERIOD_W1 : description += "Weekly";  break;
      case PERIOD_MN1: description += "Monthly"; break;
      default:
         if (custom) {
            switch (timeframe) {
               case PERIOD_M2 : description += "M2";  break;
               case PERIOD_M3 : description += "M3";  break;
               case PERIOD_M4 : description += "M4";  break;
               case PERIOD_M6 : description += "M6";  break;
               case PERIOD_M12: description += "M12"; break;
               case PERIOD_M20: description += "M20"; break;
               case PERIOD_H2 : description += "H2";  break;
               case PERIOD_H3 : description += "H3";  break;
               case PERIOD_H6 : description += "H6";  break;
               case PERIOD_H8 : description += "H8";  break;
               case PERIOD_H12: description += "H12"; break;
            }
         }
         if (!description.length()) {
            description += "M";
            description += to_string(timeframe);      // for custom offline charts
         }
   }
   return string(symbol).append(",").append(description);
}


/**
 * Return the last Windows error of the current thread. Makes it accessible to MQL.
 *
 * @return DWORD - error code
 */
DWORD WINAPI GetLastWin32Error() {
   return GetLastError();
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the build of the DLL is a debug build.
 *
 * @return BOOL
 */
BOOL WINAPI IsDebugBuild() {
   #ifdef _DEBUG
      return TRUE;
   #else
      return FALSE;
   #endif
}


/**
 * Whether the specified value is a MetaTrader standard timeframe identifier.
 *
 * @param  int timeframe
 *
 * @return BOOL
 */
BOOL WINAPI IsStandardTimeframe(int timeframe) {
   switch (timeframe) {
      case PERIOD_M1 : return TRUE;
      case PERIOD_M5 : return TRUE;
      case PERIOD_M15: return TRUE;
      case PERIOD_M30: return TRUE;
      case PERIOD_H1 : return TRUE;
      case PERIOD_H4 : return TRUE;
      case PERIOD_D1 : return TRUE;
      case PERIOD_W1 : return TRUE;
      case PERIOD_MN1: return TRUE;
   }
   return FALSE;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified value is a custom timeframe identifier.
 *
 * @param  int timeframe
 *
 * @return BOOL
 */
BOOL WINAPI IsCustomTimeframe(int timeframe) {
   if (timeframe <= 0) {
      return FALSE;
   }
   return !IsStandardTimeframe(timeframe);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified value is a MQL program type.
 *
 * @param  int type
 *
 * @return BOOL
 */
BOOL WINAPI IsProgramType(int type) {
   switch (type) {
      case PT_INDICATOR: return TRUE;
      case PT_EXPERT:    return TRUE;
      case PT_SCRIPT:    return TRUE;
   }
   return FALSE;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a virtual key is up or down at the time the function is called.
 *
 * @param  int key - one of 256 possible virtual-key codes
 *
 * @return BOOL
 *
 * @see  https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
 */
BOOL WINAPI IsVirtualKeyDown(int key) {
   return (GetAsyncKeyState(key) & 0x8000);     // check the most significant bit of the SHORT result
   #pragma EXPANDER_EXPORT
}


/**
 * Return the flags of currently pressed virtual keys.
 *
 * @param  DWORD flags [optional] - virtual keys to test for (default: all keys defined in F_VK_ALL)
 *
 * @return DWORD
 */
DWORD WINAPI GetPressedVirtualKeys(DWORD flags = F_VK_ALL) {
   DWORD pressed = 0;

   if (flags & F_VK_ESCAPE && IsVirtualKeyDown(VK_ESCAPE)) {
      pressed |= F_VK_ESCAPE;
   }
   if (flags & F_VK_TAB && IsVirtualKeyDown(VK_TAB)) {
      pressed |= F_VK_TAB;
   }
   if (flags & F_VK_CAPITAL && IsVirtualKeyDown(VK_CAPITAL)) {
      pressed |= F_VK_CAPITAL;
   }
   if (flags & F_VK_SHIFT && IsVirtualKeyDown(VK_SHIFT)) {
      pressed |= F_VK_SHIFT;
   }
   if (flags & F_VK_CONTROL && IsVirtualKeyDown(VK_CONTROL)) {
      pressed |= F_VK_CONTROL;
   }
   if (flags & F_VK_MENU && IsVirtualKeyDown(VK_MENU)) {
      pressed |= F_VK_MENU;
   }
   if (flags & F_VK_LWIN && IsVirtualKeyDown(VK_LWIN)) {
      pressed |= F_VK_LWIN;
   }
   if (flags & F_VK_RWIN && IsVirtualKeyDown(VK_RWIN)) {
      pressed |= F_VK_RWIN;
   }

   return pressed;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether any part of the specified window's client area is currently visible. The visible area is defined by the current
 * clipping region or clip path, as well as any overlapping windows.
 *
 * @param  HWND hWnd - window handle
 *
 * @return BOOL
 */
BOOL WINAPI IsWindowAreaVisible(HWND hWnd) {
   if (!hWnd) return !error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: 0x%p (not a window)", hWnd);

   HDC hDC = GetDC(hWnd);
   if (!hDC) return !error(ERR_WIN32_ERROR + GetLastError(), "GetDC(hWnd=%p)", hWnd);

   RECT rect;
   int region = GetClipBox(hDC, &rect);
   ReleaseDC(hWnd, hDC);

   if (region == RGN_ERROR) return !error(ERR_WIN32_ERROR + GetLastError(), "GetClipBox(hDC=%p) => RGN_ERROR", hDC);

   return (region != NULLREGION);
   #pragma EXPANDER_EXPORT
}
