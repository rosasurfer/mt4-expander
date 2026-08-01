#pragma once
#include "expander.h"

// callback function signature used by InvokeUiThread()
typedef LRESULT (CALLBACK *UiThreadCallback)(LPARAM args);

// transport of callback details for the UI-thread dispatcher
struct JOB {
   UiThreadCallback func;           // function pointer
   LPARAM           args;           // function arguments
   LRESULT          result;         // return value
   HANDLE           completion;     // completion event
   int              last_error;     // last job execution error (if any)

   LRESULT run() {                  // executes the job
      if (!func) {
         last_error = error(ERR_INVALID_PARAMETER, "invalid job function: (null)");
         return result = 0;
      }
      if (!completion) {
         last_error = error(ERR_INVALID_PARAMETER, "invalid completion event: (null)");
         return result = 0;
      }

      SetLastError(NO_ERROR);
      result = func(args);
      last_error = GetLastError();

      SetEvent(completion);
      return result;
   }
};


DWORD   WINAPI GetUiThreadId();
BOOL    WINAPI IsUiThread(DWORD threadId = NULL);
LRESULT WINAPI InvokeUiThread(UiThreadCallback func, LPARAM args);
DWORD   WINAPI SetLastErrorEx(DWORD error);
