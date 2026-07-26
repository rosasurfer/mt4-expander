#include "expander.h"
#include "lib/terminal.h"
#include "lib/thread.h"
#include "lib/window.h"
#include "lib/ui/integration.h"


/**
 * Return the id of the UI thread.
 *
 * @return DWORD - thread id (not thread handle) or NULL in case of errors
 */
DWORD WINAPI GetUiThreadId() {
   static DWORD uiThreadId;

   if (!uiThreadId) {
      if (HWND hWnd = GetTerminalMainWindow()) {
         DWORD threadId = GetWindowThreadProcessId(hWnd, NULL);
         if (!uiThreadId) uiThreadId = threadId;      // another thread may have been faster
      }
   }
   return uiThreadId;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified thread is the application's UI thread.
 *
 * @param  DWORD threadId [optional] - thread id (default: the current thread id)
 *
 * @return BOOL
 */
BOOL WINAPI IsUiThread(DWORD threadId/*= NULL*/) {
   if (!threadId) {
      threadId = GetCurrentThreadId();
   }
   return (threadId == GetUiThreadId());
   #pragma EXPANDER_EXPORT
}


/**
 * Executes a function in the UI thread and returns the result.
 *
 * @param  UiThreadCallback func - callback function to execute
 * @param  void*            args - callback function arguments
 *
 * @return LRESULT - function return value, or NULL (0) in case of errors
 */
LRESULT WINAPI InvokeUiThread(UiThreadCallback func, void* args) {
   if (!func) return !error(SetLastErrorEx(ERR_INVALID_PARAMETER), "invalid parameter func: 0x%p (not a valid pointer)", func);

   SetLastError(NO_ERROR);

   // call directly if already in the UI thread
   if (IsUiThread()) {
      return func(args);
   }

   // make sure the UI thread dispatcher is installed
   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return NULL;
   if (!GetPropW(hWndMain, PROP_WINDOW_SUBCLASSED)) return !error(SetLastErrorEx(ERR_ILLEGAL_STATE), "terminal main window not subclassed");

   // dispatch the function call to the UI thread
   JOB job = {};
   job.func = func;
   job.args = args;
   job.event = CreateEventW(NULL, TRUE, FALSE, NULL);
   if (!job.event) return !error(SetLastErrorEx(ERR_WIN32_ERROR + GetLastError()), "CreateEventW()");

   if (!PostMessageW(hWndMain, WM_MT4EXPANDER(), ID_UI_CALLBACK, (LPARAM)&job)) {
      CloseHandle(job.event);
      return !error(SetLastErrorEx(ERR_WIN32_ERROR + GetLastError()), "PostMessageW()");
   }
   if (WaitForSingleObject(job.event, INFINITE) == WAIT_FAILED) {
      job.last_error = error(ERR_WIN32_ERROR + GetLastError(), "WaitForSingleObject()");
   }
   CloseHandle(job.event);

   if (job.last_error) {
      SetLastError(job.last_error);
      return NULL;
   }
   return job.result;
}


/**
 * Sets the last-error code for the calling thread. Helper to use kernel32::SetLastError() as an expression.
 *
 * @param  DWORD - error code
 *
 * @return DWORD - the same error code
 */
DWORD WINAPI SetLastErrorEx(DWORD error) {
   SetLastError(error);
   return error;
}
