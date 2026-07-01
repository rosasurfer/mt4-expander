#include "expander.h"
#include "dev/dev.h"
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

   // UI creation callback
   struct local {
      static LRESULT CALLBACK CreateChildControl(LPARAM lParam) {
         ARGS* args = (ARGS*)lParam;
         if (!args) return !error(ERR_INVALID_POINTER, "invalid arguments: NULL");

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
      __in  HWND  hWndParent;
      __out DWORD error;
   } args = { ec->chart, NO_ERROR };

   // create child control
   HWND hWndChild = (HWND) UiInvoke(local::CreateChildControl, (LPARAM)&args, true);
   if (args.error) return (HWND)!error(args.error, "CreateChildControl()");

   SetWindowPos(hWndChild, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   debug("child control created: %p", hWndChild);

   return hWndChild;
   #pragma EXPANDER_EXPORT
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

   // UI creation callback
   struct local {
      static LRESULT CALLBACK CreateChildWindow(LPARAM lParam) {
         ARGS* args = (ARGS*)lParam;
         if (!args) return !error(ERR_INVALID_POINTER, "invalid arguments: NULL");

         const wchar* className = L"rsfMT4Expander.chart.childwindow";

         // register the window class
         WNDCLASSW wc = {};
         wc.lpfnWndProc   = ChildWindowProc;
         wc.hInstance     = HMODULE_EXPANDER;
         wc.lpszClassName = className;
         wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
         wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
         if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
            return !error(ERR_WIN32_ERROR + GetLastError(), "RegisterClassW(\"%S\")", className);
         }

         // create the window
         DWORD styles = WS_CHILD | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS;
         HWND hWndChild = CreateWindowExW(
            0,                                        // extended styles
            className,                                // class name
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
      __in  HWND  hWndParent;
      __out DWORD error;
   } args = { ec->chart, NO_ERROR };

   // create child window
   HWND hWndChild = (HWND) UiInvoke(local::CreateChildWindow, (LPARAM)&args, true);
   if (args.error) return (HWND)!error(args.error, "CreateChildWindow()");

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
   switch (msg) {
      case WM_ERASEBKGND: {
         return 1;                        // don't paint the bg in a separate message (causes flicker)
      }

      case WM_PAINT: {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint(hWnd, &ps);
         RECT rc;
         GetClientRect(hWnd, &rc);

         // draw background
         static HBRUSH classBg = GetClassBackground(hWnd);
         FillRect(hdc, &rc, classBg);

         // draw text
         SetBkMode(hdc, TRANSPARENT);
         SetTextColor(hdc, Blue);
         DrawTextW(hdc, L"Margin: 142.5%", -1, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

         EndPaint(hWnd, &ps);
         return 0;
      }
   }
   return DefWindowProcW(hWnd, msg, wParam, lParam);
}
