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
   if (ec->programType != PT_EXPERT || !StrCompare(ec->programName, MQL_PROGRAM_NAME)) {
      return (HWND)!error(ERR_ILLEGAL_STATE, "illegal caller: %s \"%s\"", ProgramTypeDescription(ec->programType), ec->programName);
   }

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
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_SIZEBOX,  // regular styles     | WS_CAPTION | WS_SYSMENU
            300, 200, 206, 70,                        // position(x, y) + size(w, h)
            args->hWndParent,                         // parent window
            0,                                        // control id
            HMODULE_EXPANDER,                         // module instance
            args->viewData                            // user data
         );
         if (!hWnd) error(args->error = ERR_WIN32_ERROR + GetLastError(), "CreateWindowExW()");
         return (LRESULT)hWnd;
      }
   };

   VIEW_DATA* data = new VIEW_DATA();
   data->pid              = pid;
   data->textColor        = textColor;
   data->bgColorUpTrend   = upTrendColor;
   data->bgColorDownTrend = downTrendColor;

   struct ARGS {
      __in  HWND         hWndParent;
      __in  const wchar* className;
      __in  VIEW_DATA*   viewData;
      __out DWORD        error;
   } args = {
      ec->chart,
      wc.lpszClassName,
      data,
      NO_ERROR,
   };

   // create the panel
   SetLastError(NO_ERROR);
   hWndPanel = (HWND) InvokeUiThread(local::CreatePanel, (LPARAM)&args);
   if (!hWndPanel || args.error) return (HWND)!error(orElse(args.error, GetLastError()), "CreatePanel()");

   // move it to the top
   SetWindowPos(hWndPanel, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

   // update the EXECUTION_CONTEXT
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
   if (ec->programType != PT_EXPERT || !StrCompare(ec->programName, MQL_PROGRAM_NAME)) {
      return !error(ERR_ILLEGAL_STATE, "illegal caller: %s \"%s\"", ProgramTypeDescription(ec->programType), ec->programName);
   }

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
 * @param  uint   pid       - indicator pid
 * @param  double price     - current price
 * @param  int    trend     - current trend
 * @param  double dcWidth   - current Donchian Channel width
 * @param  double dcLegsize - current Donchian Channel leg size
 *
 * @return BOOL - success status
 */
BOOL WINAPI RulesMonitor_UpdateStatusPanel(uint pid, double price, int trend, double dcWidth, double dcLegsize) {
   // get the EXECUTION_CONTEXT
   if ((int)pid <= 0) return !error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid);
   if (!ec) return FALSE;
   if (ec->programType != PT_EXPERT || !StrCompare(ec->programName, MQL_PROGRAM_NAME)) {
      return !error(ERR_ILLEGAL_STATE, "illegal caller: %s \"%s\"", ProgramTypeDescription(ec->programType), ec->programName);
   }

   // get the panel
   HWND hWndPanel = (HWND)ec->userData[USERDATA_HWND_PANEL];
   if (!hWndPanel) return !error(ERR_ILLEGAL_STATE, "status panel not found");

   // create a view model update
   VIEW_UPDATE* update = new VIEW_UPDATE();
   update->price           = price;
   update->trend           = trend;
   update->donchianWidth   = dcWidth;
   update->donchianLegsize = dcLegsize;

   // post it to the panel
   if (!PostMessageW(hWndPanel, WM_MT4EXPANDER(), ID_UPDATE_VIEW, (LPARAM)update)) {
      delete update;
      return !error(SetLastErrorEx(ERR_WIN32_ERROR + GetLastError()), "PostMessageW()");
   }
   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize the view model of the status panel. Called when the panel receives the first WM_PAINT message.
 *
 * @param  VIEW_DATA* data - view data
 * @param  HDC        hDC  - device context of the panel
 *
 * @return BOOL - success status
 */
BOOL WINAPI StatusPanel_InitView(VIEW_DATA* data, HDC hDC) {
   if (!data->bgBrushUpTrend) {
      data->bgBrushUpTrend = CreateSolidBrush(colorRef(data->bgColorUpTrend));
      if (!data->bgBrushUpTrend) return !error(ERR_WIN32_ERROR + GetLastError(), "CreateSolidBrush()");
   }

   if (!data->bgBrushDownTrend) {
      data->bgBrushDownTrend = CreateSolidBrush(colorRef(data->bgColorDownTrend));
      if (!data->bgBrushDownTrend) return !error(ERR_WIN32_ERROR + GetLastError(), "CreateSolidBrush()");
   }

   // create a font by hand
   if (!data->hFont) {
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
   }

   // get the system font for dialogs/message boxes
   if (!data->hSystemFont) {
      NONCLIENTMETRICSW ncm = {};
      ncm.cbSize = sizeof(ncm);
      if (!SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0)) return !error(ERR_WIN32_ERROR + GetLastError(), "SystemParametersInfoW()");
      data->hSystemFont = CreateFontIndirectW(&ncm.lfMessageFont);
      if (!data->hSystemFont) return !error(ERR_WIN32_ERROR + GetLastError(), "CreateFontIndirectW()");
   }

   return data->initialized = TRUE;
}


/**
 * Release the view model of the status panel.
 *
 * @param  VIEW_DATA* data
 */
void WINAPI StatusPanel_ReleaseView(VIEW_DATA* data) {
   if (data->bgBrushUpTrend)   DeleteObject(data->bgBrushUpTrend);
   if (data->bgBrushDownTrend) DeleteObject(data->bgBrushDownTrend);
   if (data->hFont)            DeleteObject(data->hFont);
   if (data->hSystemFont)      DeleteObject(data->hSystemFont);
   delete data;
}


/**
 * Status panel window procedure. Processes all messages for the window. Executed in the UI thread.
 *
 * @param  HWND   hWnd   - window receiving the message
 * @param  uint   msg    - received message
 * @param  WPARAM wParam - additional message info
 * @param  LPARAM lParam - additional message info
 *
 * @return LRESULT - depends on the message sent
 */
LRESULT CALLBACK StatusPanelWindowProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam) {
   static uint WM_MT4EXPANDER = MT4ExpanderMsg();

   switch (msg) {
      // link the view model to the window
      case WM_NCCREATE: {
         CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
         VIEW_DATA* view = (VIEW_DATA*)cs->lpCreateParams;
         view->hWnd = hWnd;
         SetWindowUserData(hWnd, (LONG_PTR)view);
         break;
      }

      // release the view data
      case WM_NCDESTROY: {
         StatusPanel_ReleaseView((VIEW_DATA*)GetWindowUserData(hWnd));
         SetWindowUserData(hWnd, NULL);
         break;
      }

      // make the whole client area draggable
      case WM_NCHITTEST: {
         LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
         return (hit == HTCLIENT) ? HTCAPTION : hit;  // mouse handling must listen to NC message variants
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

      case WM_COMMAND:    return StatusPanel_onCommand(hWnd, msg, wParam, lParam);
      case WM_PAINT:      return StatusPanel_onPaint  (hWnd, msg, wParam, lParam);
      case WM_ERASEBKGND: return 1;                   // don't spread painting over multiple messages (causes flicker)

      default:
         if (msg == WM_MT4EXPANDER) return StatusPanel_onMT4ExpanderMsg(hWnd, msg, wParam, lParam);
   }
   return DefWindowProcW(hWnd, msg, wParam, lParam);
}


/**
 * Process a WM_COMMAND message.
 *
 * @param  HWND   hWnd   - window receiving the message
 * @param  uint   msg    - received message
 * @param  WPARAM wParam - additional message info
 * @param  LPARAM lParam - additional message info
 *
 * @return LRESULT
 */
LRESULT WINAPI StatusPanel_onCommand(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam) {
   uint id   = LOWORD(wParam);
   uint code = HIWORD(wParam);
   HWND hCtl = (HWND)lParam;

   static DWORD debugFeatures = GetDebugFeatures();
   if (debugFeatures & DEBUG_FEATURE_WM_COMMAND) debug("WM_COMMAND  id=%d  code=%d  hCtl=%p", id, code, hCtl);

   return DefWindowProcW(hWnd, msg, wParam, lParam);
}


/**
 * Process an internal MT4Expander message.
 *
 * @param  HWND   hWnd   - window receiving the message
 * @param  uint   msg    - received message
 * @param  WPARAM wParam - additional message info
 * @param  LPARAM lParam - additional message info
 *
 * @return LRESULT
 */
LRESULT WINAPI StatusPanel_onMT4ExpanderMsg(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam) {
   uint id = wParam;

   switch (id) {
      // update the panel's view model
      case ID_UPDATE_VIEW: {
         VIEW_UPDATE* update = (VIEW_UPDATE*)lParam;
         if (!update) return !error(ERR_ILLEGAL_STATE, "id=ID_VIEW_UPDATE  invalid parameter VIEW_UPDATE: NULL");

         VIEW_DATA* view = (VIEW_DATA*)GetWindowUserData(hWnd);
         if (!view) return !error(ERR_ILLEGAL_STATE, "id=ID_VIEW_UPDATE  linked VIEW_DATA not found: NULL");

         view->price           = update->price;
         view->trend           = update->trend;
         view->donchianWidth   = update->donchianWidth;
         view->donchianLegsize = update->donchianLegsize;
         delete update;

         if (view->initialized) {               // the view is initialized in the first WM_PAINT which may be deferred by the system
            InvalidateRect(hWnd, NULL, FALSE);
            UpdateWindow(hWnd);                 // immediately send WM_PAINT bypassing the message queue
         }
         return 0;
      }
   }
   return DefWindowProcW(hWnd, msg, wParam, lParam);
}


/**
 * Process a WM_PAINT message.
 *
 * @param  HWND   hWnd   - window receiving the message
 * @param  uint   msg    - received message
 * @param  WPARAM wParam - not used
 * @param  LPARAM lParam - not used
 *
 * @return LRESULT
 */
LRESULT WINAPI StatusPanel_onPaint(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam) {
   PAINTSTRUCT ps;
   HDC hDC = BeginPaint(hWnd, &ps);

   VIEW_DATA* view = (VIEW_DATA*)GetWindowUserData(hWnd);
   if (!view->initialized && !StatusPanel_InitView(view, hDC)) return _NULL(EndPaint(hWnd, &ps));

   // use custom colors/styles
   //FillRect(hDC, &rc, view->trend > 0 ? view->bgBrushUpTrend : view->bgBrushDownTrend);
   //DrawEdge(hDC, &rc, BDR_RAISEDINNER, BF_RECT);
   //SetTextColor(hDC, colorRef(view->textColor));
   //SelectObject(hDC, view->hFont);
   //DrawTextW(hDC, L"text", -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_NOCLIP);

   // use system colors/styles
   RECT rc;
   GetClientRect(hWnd, &rc);
   FillRect(hDC, &rc, GetSysColorBrush(COLOR_BTNFACE));     // background color
   SetTextColor(hDC, GetSysColor(COLOR_WINDOWTEXT));        // foreground color
   SetBkMode(hDC, TRANSPARENT);                             // no text background color
   SelectObject(hDC, view->hSystemFont);

   wchar* text = asformat(L"Price: %.2f\nTrend: %d\nDonchian Channel width: %.2f\nDonchian Channel leg size: %.2f", view->price, view->trend, view->donchianWidth, view->donchianLegsize);
   DrawTextW(hDC, text, -1, &rc, DT_LEFT|DT_NOPREFIX|DT_NOCLIP);  // DT_CENTER|DT_VCENTER|DT_SINGLELINE|
   free(text);

   EndPaint(hWnd, &ps);
   return 0;
}
