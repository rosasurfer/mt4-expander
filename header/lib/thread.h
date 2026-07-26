#pragma once
#include "expander.h"

// callback function signature used by InvokeUiThread()
typedef LRESULT (CALLBACK *UiThreadCallback)(void* args);

// transport of callback details for the UI-thread dispatcher
struct JOB {
   UiThreadCallback func;        // function pointer
   void*            args;        // argument pointer
   LRESULT          result;      // return value
   HANDLE           event;       // completion event
   int              last_error;  // last job execution error (if any)

   LRESULT run() {               // executes the job
      if (!func) {
         last_error = error(ERR_INVALID_PARAMETER, "invalid job function: (null)");
         return result = 0;
      }
      if (!event) {
         last_error = error(ERR_INVALID_PARAMETER, "invalid completion event: (null)");
         return result = 0;
      }

      SetLastError(NO_ERROR);
      result = func(args);
      last_error = GetLastError();

      SetEvent(event);
      return result;
   }
};


DWORD   WINAPI GetUiThreadId();
BOOL    WINAPI IsUiThread(DWORD threadId = NULL);
LRESULT WINAPI InvokeUiThread(UiThreadCallback func, void* args);
DWORD   WINAPI SetLastErrorEx(DWORD error);
