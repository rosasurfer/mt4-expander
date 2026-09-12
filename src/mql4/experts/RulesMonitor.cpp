#include "expander.h"
#include "lib/conversion.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/thread.h"
#include "lib/ui/window.h"
#include "mql4/experts/RulesMonitor.h"
#include "struct/ExecutionContext.h"

extern "C" IMAGE_DOS_HEADER          __ImageBase;
#define HMODULE_EXPANDER ((HMODULE) &__ImageBase)     // this DLL's module handle

#define MQL_PROGRAM_NAME      "Rules Monitor"
#define USERDATA_HWND_PANEL   0


/**
 * Create the status panel.
 *
 * @param  uint  pid            - indicator pid
 * @param  color textColor      - text foreground color
 * @param  color upTrendColor   - background color for uptrends
 * @param  color downTrendColor - background color for downtrends
 *
 * @return HWND - window handle
 */
HWND WINAPI RulesMonitor_CreateStatusPanel(uint pid, color textColor, color upTrendColor, color downTrendColor) {
   // get the EXECUTION_CONTEXT
   if ((int)pid <= 0) return (HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid);
   if (!ec) return NULL;
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAME)) return (HWND)!error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

   // return an existing panel
   HWND hWndPanel = (HWND)ec->userData[USERDATA_HWND_PANEL];
   if (hWndPanel && IsWindow(hWndPanel)) {
      return hWndPanel;
   }

   // register the window class
   WNDCLASSW wc = {};
   wc.lpfnWndProc   = StatusPanelWindowProc;
   wc.hInstance     = HMODULE_EXPANDER;
   wc.lpszClassName = L"rsfMT4Expander.RulesMonitor";
   wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
   if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
      return (HWND)!error(ERR_WIN32_ERROR + GetLastError(), "RegisterClassW(\"%S\")", wc.lpszClassName);
   }

   // define creation callback and arguments
   struct local {
      static LRESULT CALLBACK CreatePanel(LPARAM lParam) {
         ARGS* args = (ARGS*)lParam;
         if (!args) return !error(ERR_INVALID_PARAMETER, "invalid arguments: NULL");

         HWND hWnd = CreateWindowExW(
            0,                                        // extended styles
            args->className,                          // class name
            L"Rules Monitor",                         // window text
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,  // regular styles |= WS_CAPTION | WS_SYSMENU
            300, 200, 180, 30,                        // position(x,y) + size(w,h)
            args->hWndParent,                         // parent window
            0,                                        // control id
            HMODULE_EXPANDER,                         // module instance
            args->viewData                            // user data
         );
         if (!hWnd) error(args->error = ERR_WIN32_ERROR + GetLastError(), "CreateWindowExW()");
         return (LRESULT)hWnd;
      }
   };

   VIEW_DATA data = {};
   data.pid              = pid;
   data.textColor        = textColor;
   data.bgColorUpTrend   = upTrendColor;
   data.bgColorDownTrend = downTrendColor;

   struct ARGS {
      __in  HWND         hWndParent;
      __in  const wchar* className;
      __in  VIEW_DATA*   viewData;
      __out DWORD        error;
   } args = {
      ec->chart,
      wc.lpszClassName,
      new VIEW_DATA(data),
      NO_ERROR,
   };

   // create the panel
   SetLastError(NO_ERROR);
   hWndPanel = (HWND) InvokeUiThread(local::CreatePanel, (LPARAM)&args);
   if (!hWndPanel || args.error) return (HWND)!error(orElse(args.error, GetLastError()), "CreatePanel()");

   // move it to the top
   SetWindowPos(hWndPanel, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

   // update EXECUTION_CONTEXT
   ec_SetUserData(ec, USERDATA_HWND_PANEL, (DWORD)hWndPanel);

   return hWndPanel;
   #pragma EXPANDER_EXPORT
}


/**
 * Destroy the status panel.
 *
 * @param  uint pid - indicator pid
 *
 * @return BOOL - success status
 */
BOOL WINAPI RulesMonitor_DestroyStatusPanel(uint pid) {
   // get the EXECUTION_CONTEXT
   if ((int)pid <= 0) return !error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid);
   if (!ec) return FALSE;
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAME)) return !error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

   // destroy the panel
   HWND hWndPanel = (HWND)ec->userData[USERDATA_HWND_PANEL];
   if (hWndPanel && IsWindow(hWndPanel)) {
      if (IsUiThread()) DestroyWindow(hWndPanel);
      else              PostMessageW(hWndPanel, WM_CLOSE, 0, 0);
   }

   // update the EXECUTION_CONTEXT
   ec_SetUserData(ec, USERDATA_HWND_PANEL, NULL);
   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Update the status panel.
 *
 * @param  uint pid   - indicator pid
 * @param  int  trend - current trend direction
 *
 * @return BOOL - success status
 */
BOOL WINAPI RulesMonitor_UpdateStatusPanel(uint pid, int trend) {
   // get the EXECUTION_CONTEXT
   if ((int)pid <= 0) return !error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid);
   if (!ec) return FALSE;
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAME)) return !error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

   // get the panel
   HWND hWndPanel = (HWND)ec->userData[USERDATA_HWND_PANEL];
   if (!hWndPanel) return !error(ERR_ILLEGAL_STATE, "status panel not found");

   //debug("tick=%d  trend=%d", ec->ticks, trend);

   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Status panel window procedure. Processes all messages for the window. Executed in the UI thread.
 *
 * @param  HWND   hWnd   - window receiving the message
 * @param  uint   msg    - sent message
 * @param  WPARAM wParam - additional message info
 * @param  LPARAM lParam - additional message info
 *
 * @return LRESULT - depends on the message sent
 */
LRESULT CALLBACK StatusPanelWindowProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam) {
   static DWORD debugFeatures = GetDebugFeatures();

   switch (msg) {
      // link the view data to the window
      case WM_NCCREATE: {
         CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
         VIEW_DATA* data = (VIEW_DATA*)cs->lpCreateParams;
         data->hWnd = hWnd;
         SetWindowUserData(hWnd, (LONG_PTR)data);
         break;
      }

      // release the view data
      case WM_NCDESTROY: {
         ReleaseViewData((VIEW_DATA*)GetWindowUserData(hWnd));
         break;
      }

      // make the whole client area draggable
      case WM_NCHITTEST: {
         LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
         return (hit == HTCLIENT) ? HTCAPTION : hit;     // mouse handling must process NC message variants
      }

      // on click move the window to the top
      case WM_NCLBUTTONDOWN:
      case WM_NCRBUTTONDOWN:
      case WM_NCMBUTTONDOWN:
      case WM_NCXBUTTONDOWN: {
         SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
         RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
         break;
      }

      case WM_COMMAND: {
         if (debugFeatures & DEBUG_FEATURE_WM_COMMAND) debug("WM_COMMAND  id=%d  lParam=0x%p", LOWORD(wParam), lParam);
         break;
      }

      case WM_ERASEBKGND: {                              // don't spread painting over multiple messages (causes flicker)
         return 1;
      }

      case WM_PAINT: {
         PAINTSTRUCT ps;
         HDC hDC = BeginPaint(hWnd, &ps);

         VIEW_DATA* data = (VIEW_DATA*)GetWindowUserData(hWnd);
         if (!data->initialized && !InitViewData(data, hDC)) return _NULL(EndPaint(hWnd, &ps));

         RECT rc;
         GetClientRect(hWnd, &rc);
         FillRect(hDC, &rc, data->trend > 0 ? data->bgBrushUpTrend : data->bgBrushDownTrend);
         DrawEdge(hDC, &rc, BDR_RAISEDINNER, BF_RECT);

         SetTextColor(hDC, colorRef(data->textColor));
         SetBkMode(hDC, TRANSPARENT);                    // no text background color
         HFONT hOldFont = (HFONT)SelectObject(hDC, data->hFont);
         wchar* text = (data->trend > 0 ? L"LONG ONLY" : L"SHORT ONLY");
         DrawTextW(hDC, text, -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_NOCLIP);
         SelectObject(hDC, hOldFont);

         EndPaint(hWnd, &ps);
         return 0;
      }
   }
   return DefWindowProcW(hWnd, msg, wParam, lParam);
}


/**
 * Initialize the view data of the status panel.
 *
 * @param  VIEW_DATA* data - view data
 * @param  HDC        hDC  - device context of the panel
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitViewData(VIEW_DATA* data, HDC hDC) {
   data->bgBrushUpTrend = CreateSolidBrush(colorRef(data->bgColorUpTrend));
   if (!data->bgBrushUpTrend) return !error(ERR_WIN32_ERROR + GetLastError(), "CreateSolidBrush()");

   data->bgBrushDownTrend = CreateSolidBrush(colorRef(data->bgColorDownTrend));
   if (!data->bgBrushDownTrend) return !error(ERR_WIN32_ERROR + GetLastError(), "CreateSolidBrush()");

   data->hFont = CreateFontW(
      -MulDiv(12, GetDeviceCaps(hDC, LOGPIXELSY), 72),   // 12 pt, matches fontsize in MQL::ObjectSetText()
      0, 0, 0,
      FW_BOLD,
      FALSE, FALSE, FALSE,
      DEFAULT_CHARSET,
      0,
      0,
      CLEARTYPE_QUALITY,                                 // apply the user's ClearType configuration
      0,
      L"Arial Black"
   );
   if (!data->hFont) return !error(ERR_WIN32_ERROR + GetLastError(), "CreateFontW()");

   return data->initialized = TRUE;
}


/**
 * Release the view data of the status panel.
 *
 * @param  VIEW_DATA* data
 */
void WINAPI ReleaseViewData(VIEW_DATA* data) {
   if (data->bgBrushUpTrend)   DeleteObject(data->bgBrushUpTrend);
   if (data->bgBrushDownTrend) DeleteObject(data->bgBrushDownTrend);
   if (data->hFont)            DeleteObject(data->hFont);

   delete data;
}
