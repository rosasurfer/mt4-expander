#pragma once
#include "expander.h"

// callback function signature used by UiInvoke()
typedef LRESULT (CALLBACK *UiInvokeProc)(LPARAM args);

// transport of callback details for the UI-thread dispatcher
struct JOB {
   UiInvokeProc func;            // function pointer
   LPARAM       args;            // argument pointer
   LRESULT      result;          // return value
   HANDLE       done;            // optional completion event, without it: fire-and-forget
   int          error;           // job execution error (if any)
   bool         owner;           // if true, run() deletes itself (this job) after execution

   LRESULT run() {               // executes the job
      if (func) {
         result = func(args);
      }
      else {
         error = error(ERR_INVALID_PARAMETER, "invalid job function: 0x%p");
         result = 0;
      }
      if (done) SetEvent(done);

      LRESULT retValue = result; // copy before potential self-delete
      if (owner) delete this;
      return retValue;
   }
};


DWORD   WINAPI GetUiThreadId();
BOOL    WINAPI IsUiThread(DWORD threadId = NULL);
LRESULT WINAPI UiInvoke(UiInvokeProc func, LPARAM args, bool wait = false);
DWORD   WINAPI SetLastErrorEx(DWORD error);
