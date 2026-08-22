#include "expander.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/thread.h"
#include "lib/ui/window.h"
#include "mql4/indicators/RulesMonitor.h"
#include "struct/ExecutionContext.h"

extern "C" IMAGE_DOS_HEADER          __ImageBase;     // this DLL's module handle
#define HMODULE_EXPANDER ((HMODULE) &__ImageBase)

#define MQL_PROGRAM_NAME      "Rules Monitor"
#define USERDATA_HWND_PANEL   0

// container for view data of the status panel
struct VIEW_DATA {
   int trend;

   // --- old ------
   double profit;
   uint   trades;
   wchar  text[256];
};


/**
 * Create the status panel.
 *
 * @param  uint pid - indicator pid
 *
 * @return HWND - window handle
 */
HWND WINAPI RulesMonitor_CreateStatusPanel(uint pid) {
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
   wc.lpszClassName = L"rsfMT4Expander.RulesMonitor.status-panel";
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

         DWORD stdStyles = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
               stdStyles |= WS_CAPTION | WS_SYSMENU;
         DWORD extStyles = WS_EX_DLGMODALFRAME;
         wstring windowText = ansiToUtf16(string(MQL_PROGRAM_NAME));

         HWND hWnd = CreateWindowExW(
            extStyles,                             // extended styles
            args->className,                       // class name
            windowText.c_str(),                    // window text
            stdStyles,                             // regular styles
            300, 200, 300, 150,                    // position + size
            args->hWndParent,                      // parent window
            0,                                     // control id
            HMODULE_EXPANDER,                      // module instance
            args->viewData                         // user data
         );
         if (!hWnd) error(args->error = ERR_WIN32_ERROR + GetLastError(), "CreateWindowExW()");
         return (LRESULT)hWnd;
      }
   };
   struct ARGS {
      __in  HWND         hWndParent;
      __in  const wchar* className;
      __in  VIEW_DATA*   viewData;
      __out DWORD        error;
   } args = {
      ec->chart,
      wc.lpszClassName,
      (VIEW_DATA*)calloc(sizeof(VIEW_DATA), 1),
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
 * @return bool - success status
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
      else              PostMessageA(hWndPanel, WM_CLOSE, 0, 0);
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
 * @param  int  trend - trend direction
 *
 * @return bool - success status
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

   debug("tick=%d  trend=%d", ec->ticks, trend);

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
         SetWindowUserData(hWnd, (LONG_PTR)cs->lpCreateParams);
         break;
      }

      // free the view data
      case WM_NCDESTROY: {
         free((void*)GetWindowUserData(hWnd));
         break;
      }

      // make the whole client area draggable
      case WM_NCHITTEST: {
         LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
         return (hit == HTCLIENT) ? HTCAPTION : hit;  // mouse handling must process NC message variants
      }

      case WM_COMMAND: {
         if (debugFeatures & DEBUG_FEATURE_WM_COMMAND) debug("WM_COMMAND  id=%d  lParam=0x%p", LOWORD(wParam), lParam);
         break;
      }
   }
   return DefWindowProcW(hWnd, msg, wParam, lParam);
}
