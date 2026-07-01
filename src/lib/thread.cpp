#include "expander.h"
#include "lib/terminal.h"
#include "lib/thread.h"
#include "lib/window.h"


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
 * @param  UiInvokeProc func            - callback function to execute
 * @param  LPARAM       args            - callback function arguments
 * @param  bool         wait [optional] - whether to wait and return the function result (default: fire-and-forget)
 *
 * @return LRESULT - function return value if parameter `wait` is true;
 *                   NULL (0) if parameter `wait` is false or in case of errors
 */
LRESULT WINAPI UiInvoke(UiInvokeProc func, LPARAM args, bool wait/*=false*/) {
   if (!func) return !error(ERR_INVALID_PARAMETER, "invalid parameter func: 0x%p (not a valid pointer)", func);

   // execute directly if already in the UI thread
   if (IsUiThread()) {
      LRESULT result = func(args);
      return wait ? result : NULL;
   }

   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return NULL;

   // dispatch function to the UI-thread
   if (!wait) {                              // fire-and-forget
      JOB* job = new JOB();                  // on the heap
      job->func  = func;
      job->args  = args;
      job->owner = true;                     // runner will free the job

      if (!PostMessageW(hWndMain, WM_MT4EXPANDER(), ID_UI_CALLBACK, (LPARAM)job)) {
         delete job;
         return !error(ERR_WIN32_ERROR + GetLastError(), "PostMessageW()");
      }
      return NULL;
   }

   // wait and return the result
   JOB job = {};
   job.func = func;
   job.args = args;
   job.done = CreateEventW(NULL, TRUE, FALSE, NULL);
   if (!job.done) return !error(ERR_WIN32_ERROR + GetLastError(), "CreateEventW()");

   if (!PostMessageW(hWndMain, WM_MT4EXPANDER(), ID_UI_CALLBACK, (LPARAM)&job)) {
      CloseHandle(job.done);
      return !error(ERR_WIN32_ERROR + GetLastError(), "PostMessageW()");
   }

   switch (WaitForSingleObject(job.done, INFINITE)) {
      case WAIT_ABANDONED:
         error(job.error = ERR_RUNTIME_ERROR, "UI thread terminated");
         break;
      case WAIT_FAILED:
         error(job.error = ERR_WIN32_ERROR + GetLastError(), "WaitForSingleObject()");
         break;
   }
   CloseHandle(job.done);
   return job.error ? NULL : job.result;
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
