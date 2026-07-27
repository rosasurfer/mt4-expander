#include "expander.h"
#include "dev/dev.h"
#include "lib/terminal.h"
#include "lib/thread.h"
#include "lib/window.h"
#include "struct/ExecutionContext.h"

extern "C" IMAGE_DOS_HEADER          __ImageBase;     // this DLL's module handle
#define HMODULE_EXPANDER ((HMODULE) &__ImageBase)


/**
 * Create a "Static" child control.
 *
 * @param  uint pid - pid of the calling MQL program
 *
 * @return HWND - created window handle
 */
HWND WINAPI Test_CreateStatic(uint pid) {
   // get the EXECUTION_CONTEXT of the caller
   if ((int)pid <= 0) return (HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid); if (!ec) return NULL;

   // creation callback and arguments
   struct local {
      static LRESULT CALLBACK CreateChildControl(LPARAM lParam) {
         ARGS* args = (ARGS*)lParam;
         if (!args) return !error(ERR_INVALID_PARAMETER, "invalid arguments: (null)");

         DWORD styles = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS| SS_LEFT | SS_NOPREFIX;
         HWND hWndChild = CreateWindowExW(
            0,                                        // extended styles
            L"Static",                                // class name
            L"EURUSD   Bid 1.23456   Ask 1.23478",    // window text
            styles,                                   // regular styles
            200, 200, 300, 50,                        // position/size
            args->hWndParent,                         // parent window
            0,                                        // control id
            HMODULE_EXPANDER,                         // module instance
            NULL                                      // additional CREATESTRUCT
         );
         if (!hWndChild) error(args->error = ERR_WIN32_ERROR + GetLastError(), "CreateWindowExW()");
         return (LRESULT)hWndChild;
      }
   };
   struct ARGS {
      __in  HWND hWndParent;
      __out int  error;
   } args = { ec->chart, NO_ERROR };

   // create the child control
   SetLastError(NO_ERROR);
   HWND hWndChild = (HWND) InvokeUiThread(local::CreateChildControl, (LPARAM)&args);
   if (!hWndChild || args.error) return (HWND)!error(orElse(args.error, (int)GetLastError()), "CreateChildControl()");

   SetWindowPos(hWndChild, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   debug("child control created: %p", hWndChild);

   return hWndChild;
   //#pragma EXPANDER_EXPORT
}


/**
* Create a regular child window.
*
 * @param  uint pid - pid of the calling MQL program
 *
 * @return HWND - created window handle
 */
HWND WINAPI Test_CreateWindow(uint pid) {
   // get the EXECUTION_CONTEXT of the caller
   if ((int)pid <= 0) return (HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid); if (!ec) return NULL;

   const wchar* className = L"rsfMT4Expander.chart.childwindow";

   // register the window class
   WNDCLASSW wc = {};
   wc.lpfnWndProc   = ChildWindowProc;
   wc.hInstance     = HMODULE_EXPANDER;
   wc.lpszClassName = className;
   wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
   if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
      return (HWND)!error(ERR_WIN32_ERROR + GetLastError(), "RegisterClassW(\"%S\")", className);
   }

   // creation callback and arguments
   struct local {
      static LRESULT CALLBACK CreateChildWindow(LPARAM lParam) {
         ARGS* args = (ARGS*)lParam;
         if (!args) return !error(ERR_INVALID_PARAMETER, "invalid arguments: (null)");

         DWORD styles = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
         HWND hWndChild = CreateWindowExW(
            0,                                        // extended styles
            args->className,                          // class name
            L"My window",                             // window text
            styles,                                   // regular styles
            300, 200, 300, 150,                       // position/size
            args->hWndParent,                         // parent window
            0,                                        // control id
            HMODULE_EXPANDER,                         // module instance
            NULL                                      // additional CREATESTRUCT
         );
         if (!hWndChild) error(args->error = ERR_WIN32_ERROR + GetLastError(), "CreateWindowExW()");
         return (LRESULT)hWndChild;
      }
   };
   struct ARGS {
      __in  HWND         hWndParent;
      __in  const wchar* className;
      __out int          error;
   } args = { ec->chart, className, NO_ERROR };

   // create the child window
   SetLastError(NO_ERROR);
   HWND hWndChild = (HWND) InvokeUiThread(local::CreateChildWindow, (LPARAM)&args);
   if (!hWndChild || args.error) return (HWND)!error(orElse(args.error, (int)GetLastError()), "CreateChildWindow()");

   SetWindowPos(hWndChild, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   debug("child window created: %p", hWndChild);

   return hWndChild;
   #pragma EXPANDER_EXPORT
}


/**
 * ChartFrame child window procedure. Processes all messages for the window. Executed in the UI thread.
 *
 * @param  HWND   hWnd   - window receiving the message
 * @param  uint   msg    - sent message
 * @param  WPARAM wParam - additional message info
 * @param  LPARAM lParam - additional message info
 *
 * @return LRESULT - depends on the message sent
 */
LRESULT CALLBACK ChildWindowProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam) {
   static DWORD debugFeatures = GetDebugFeatures();

   switch (msg) {
      // make the whole client area draggable
      case WM_NCHITTEST: {
         LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
         return (hit == HTCLIENT) ? HTCAPTION : hit;  // because of HTCAPTION mouse messages will be NC variants
      }

      // load the context menu
      case WM_NCRBUTTONDOWN: {
         static HMENU hMenu = LoadMenuW(HMODULE_EXPANDER, MAKEINTRESOURCEW(IDR_CHART_STATUSPANEL_MENU));
         if (!hMenu) return !error(ERR_WIN32_ERROR + GetLastError(), "LoadMenuW()");

         POINTS pt = MAKEPOINTS(lParam);
         if (!TrackPopupMenu(GetSubMenu(hMenu, 0), TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, NULL, hWnd, NULL) && GetLastError()) {
            error(ERR_WIN32_ERROR + GetLastError(), "TrackPopupMenu()");
         }
         return 0;
      }

      case WM_COMMAND: {
         if (debugFeatures & DEBUG_FEATURE_WM_COMMAND) debug("WM_COMMAND  id=%d  lParam=0x%p", LOWORD(wParam), lParam);
         break;
      }

      case WM_ERASEBKGND: {                           // don't spread painting over multiple messages (causes flicker)
         return 1;
      }

      case WM_PAINT: {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint(hWnd, &ps);
         RECT rc;
         GetClientRect(hWnd, &rc);

         FillRect(hdc, &rc, GetSysColorBrush(COLOR_BTNFACE));
         DrawEdge(hdc, &rc, BDR_RAISEDINNER, BF_RECT);
         SetBkMode(hdc, TRANSPARENT);
         SetTextColor(hdc, Blue);
         DrawTextW(hdc, L"Margin: 142.5%", -1, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

         EndPaint(hWnd, &ps);
         return 0;
      }
   }
   return DefWindowProcW(hWnd, msg, wParam, lParam);
}
