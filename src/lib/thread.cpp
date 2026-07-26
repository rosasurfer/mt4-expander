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
 * Executes a function in the UI thread and optionally returns the result.
 *
 * @param  UiThreadCallback func            - callback function to execute
 * @param  LPARAM           args            - callback function arguments
 * @param  bool             wait [optional] - whether to wait and return the function result (default: fire-and-forget)
 *
 * @return LRESULT - function return value if parameter `wait` is true;
 *                   NULL (0) if parameter `wait` is false or in case of errors
 */
LRESULT WINAPI InvokeUiThread(UiThreadCallback func, LPARAM args, bool wait/*=false*/) {
   if (!func) return !error(SetLastErrorEx(ERR_INVALID_PARAMETER), "invalid parameter func: 0x%p (not a valid pointer)", func);

   SetLastError(NO_ERROR);

   // execute directly if already in the UI thread
   if (IsUiThread()) {
      LRESULT result = func(args);
      return wait ? result : NULL;
   }

   // make sure the terminal main window is subclassed
   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return NULL;

   if (!GetPropW(hWndMain, PROP_WINDOW_SUBCLASSED)) {
      return !error(SetLastErrorEx(ERR_ILLEGAL_STATE), "terminal main window not subclassed");
   }

   // prepare JOB definition
   JOB* job = new JOB();
   job->func = func;
   job->args = args;

   // dispatch the function call to the UI-thread
   if (!wait) {                              // fire-and-forget, runner will free the job
      job->owner = true;
      if (!PostMessageW(hWndMain, WM_MT4EXPANDER(), ID_UI_CALLBACK, (LPARAM)job)) {
         delete job;
         return !error(SetLastErrorEx(ERR_WIN32_ERROR + GetLastError()), "PostMessageW()");
      }
      return NULL;
   }

   // wait to return the result
   job->done = CreateEventW(NULL, TRUE, FALSE, NULL);
   if (!job->done) {
      delete job;
      return !error(SetLastErrorEx(ERR_WIN32_ERROR + GetLastError()), "CreateEventW()");
   }

   if (!PostMessageW(hWndMain, WM_MT4EXPANDER(), ID_UI_CALLBACK, (LPARAM)job)) {
      CloseHandle(job->done);
      delete job;
      return !error(SetLastErrorEx(ERR_WIN32_ERROR + GetLastError()), "PostMessageW()");
   }

   // get the result
   int last_error = NO_ERROR;
   LRESULT result = NULL;

   switch (WaitForSingleObject(job->done, 3000)) {
      case WAIT_OBJECT_0: {                  // success
         last_error = job->last_error;
         result     = job->result;
         CloseHandle(job->done);
         delete job;
         SetLastError(last_error);
         return result;
      }

      case WAIT_TIMEOUT:
         last_error = error(ERR_WIN32_ERROR + ERROR_TIMEOUT, "WaitForSingleObject()");
         break;

      case WAIT_FAILED:
      default:
         last_error = error(ERR_WIN32_ERROR + GetLastError(), "WaitForSingleObject()");
   }

   // deliberately orphan job and event
   SetLastError(last_error);
   return NULL;
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
