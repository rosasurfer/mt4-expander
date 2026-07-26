#pragma once
#include "expander.h"

// callback function signature used by InvokeUiThread()
typedef LRESULT (CALLBACK *UiThreadCallback)(LPARAM args);

// transport of callback details for the UI-thread dispatcher
struct JOB {
   UiThreadCallback func;        // function pointer
   LPARAM           args;        // argument pointer
   LRESULT          result;      // return value
   HANDLE           done;        // optional completion event, without it: fire-and-forget
   int              last_error;  // last job execution error (if any)
   bool             owner;       // if true, run() deletes itself (this job) after execution

   LRESULT run() {               // executes the job
      if (func) {
         result = func(args);
         last_error = GetLastError();
      }
      else {
         result = 0;
         last_error = error(ERR_INVALID_PARAMETER, "invalid job function: 0x%p");
      }
      if (done) SetEvent(done);

      LRESULT retValue = result; // copy before potential self-delete
      if (owner) delete this;
      return retValue;
   }
};


DWORD   WINAPI GetUiThreadId();
BOOL    WINAPI IsUiThread(DWORD threadId = NULL);
LRESULT WINAPI InvokeUiThread(UiThreadCallback func, LPARAM args, bool wait = false);
DWORD   WINAPI SetLastErrorEx(DWORD error);
