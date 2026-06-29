#pragma once
#include "expander.h"


char*       WINAPI GetClassNameA(HWND hWnd);
wchar*      WINAPI GetClassNameW(HWND hWnd);
string      WINAPI getClassNameA(HWND hWnd);
wstring     WINAPI getClassNameW(HWND hWnd);

DWORD       WINAPI GetWindowStyles(HWND hWnd);
DWORD       WINAPI GetWindowStylesEx(HWND hWnd);

char*       WINAPI GetWindowTextA(HWND hWnd);
wchar*      WINAPI GetWindowTextW(HWND hWnd);

char*       WINAPI GetInternalWindowTextA(HWND hWnd);
string      WINAPI getInternalWindowTextA(HWND hWnd);
wchar*      WINAPI GetInternalWindowTextW(HWND hWnd);
wstring     WINAPI getInternalWindowTextW(HWND hWnd);

HANDLE      WINAPI GetWindowPropertyA(HWND hWnd, const char* name);
BOOL        WINAPI SetWindowPropertyA(HWND hWnd, const char* name, HANDLE value);
HANDLE      WINAPI RemoveWindowPropertyA(HWND hWnd, const char* name);

int         WINAPI GetWindowIntegerA(HWND hWnd, const char* name);
double      WINAPI GetWindowDoubleA(HWND hWnd, const char* name);
const char* WINAPI GetWindowStringA(HWND hWnd, const char* name);

BOOL        WINAPI SetWindowIntegerA(HWND hWnd, const char* name, int value);
BOOL        WINAPI SetWindowDoubleA(HWND hWnd, const char* name, double value);
BOOL        WINAPI SetWindowStringA(HWND hWnd, const char* name, const char* value);

int         WINAPI RemoveWindowIntegerA(HWND hWnd, const char* name);
double      WINAPI RemoveWindowDoubleA(HWND hWnd, const char* name);
char*       WINAPI RemoveWindowStringA(HWND hWnd, const char* name);
void        WINAPI ReleaseWindowProperties();

int         WINAPI EnumChildWindowsToDebug(HWND hWnd, BOOL recursive = FALSE);
int         WINAPI EnumWindowPropertiesA(HWND hWnd, const char* prefix);
int         WINAPI EnumWindowPropertiesW(HWND hWnd, const wchar* prefix);

uint        WINAPI MT4InternalMsg();
uint        WINAPI MT4ExpanderMsg();
uint        WINAPI WM_MT4();
uint        WINAPI WM_MT4EXPANDER();


// transport of callback details for MT4Expander ID_CALLBACK messages (see UI-thread dispatcher)
struct JOB {
   LRESULT (*func)(LPARAM);               // function pointer
   LPARAM  args;                          // argument pointer
   LRESULT result;                        // return value
   HANDLE  done;                          // optional completion event

   LRESULT run() {                        // excutes the job
      result = func ? func(args) : 0;
      if (done) SetEvent(done);
      return result;
   }
};
