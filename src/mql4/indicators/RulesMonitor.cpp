#include "expander.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/thread.h"
#include "lib/ui/window.h"
#include "mql4/indicators/RulesMonitor.h"
#include "struct/ExecutionContext.h"

extern "C" IMAGE_DOS_HEADER          __ImageBase;     // this DLL's module handle
#define HMODULE_EXPANDER ((HMODULE) &__ImageBase)

#define MQL_PROGRAM_NAMEA  "Rules Monitor"
#define MQL_PROGRAM_NAMEW L"Rules Monitor"


// container for view data of the status panel
struct VIEW_DATA {
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
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAMEA)) return (HWND)!error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

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
         if (!args) return !error(ERR_INVALID_PARAMETER, "invalid arguments: (null)");

         DWORD styles = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
         styles |= WS_CAPTION | WS_SYSMENU;
         HWND hWnd = CreateWindowExW(
            0,                                        // extended styles
            args->className,                          // class name
            MQL_PROGRAM_NAMEW,                        // window text
            styles,                                   // regular styles
            300, 200, 300, 150,                       // position/size
            args->hWndParent,                         // parent window
            0,                                        // control id
            HMODULE_EXPANDER,                         // module instance
            args->viewData                            // additional user data
         );
         if (!hWnd) error(args->error = ERR_WIN32_ERROR + GetLastError(), "CreateWindowExW()");
         return (LRESULT)hWnd;
      }
   };
   struct ARGS {
      __in  HWND         hWndParent;
      __in  const wchar* className;
      __in  void*        viewData;
      __out DWORD        error;
   } args = { ec->chart, wc.lpszClassName, malloc(sizeof(VIEW_DATA)), NO_ERROR };

   // create the panel
   SetLastError(NO_ERROR);
   HWND hWndPanel = (HWND) InvokeUiThread(local::CreatePanel, (LPARAM)&args);
   if (!hWndPanel || args.error) return (HWND)!error(orElse(args.error, GetLastError()), "CreatePanel()");

   // move to top of Z-order
   SetWindowPos(hWndPanel, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
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
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAMEA)) return !error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Update the status panel.
 *
 * @param  uint pid - indicator pid
 *
 * @return bool - success status
 */
BOOL WINAPI RulesMonitor_UpdateStatusPanel(uint pid) {
   // get the EXECUTION_CONTEXT
   if ((int)pid <= 0) return !error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d (not a program id)", (int)pid);
   EXECUTION_CONTEXT* ec = GetMasterContext(pid);
   if (!ec) return FALSE;
   if (!StrCompare(ec->programName, MQL_PROGRAM_NAMEA)) return !error(ERR_ILLEGAL_STATE, "illegal caller: \"%s\"", ec->programName);

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
         CREATESTRUCT* cs = (CREATESTRUCT*) lParam;
         SetWindowUserData(hWnd, (LONG_PTR) cs->lpCreateParams);
         return TRUE;
      }

      // free the view data
      case WM_NCDESTROY: {
         free((void*)GetWindowUserData(hWnd));
         break;
      }

      // make the whole client area draggable
      case WM_NCHITTEST: {
         LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
         return (hit == HTCLIENT) ? HTCAPTION : hit;  // all mouse messages become NC variants
      }

      case WM_COMMAND: {
         if (debugFeatures & DEBUG_FEATURE_WM_COMMAND) debug("WM_COMMAND  id=%d  lParam=0x%p", LOWORD(wParam), lParam);
         break;
      }

   }
   return DefWindowProcW(hWnd, msg, wParam, lParam);
}
