#pragma once

#include "header/common.h"
#include "header/stdafx.h"
#include "header/shared/defines.h"                                               // shared between DLL and MQL
#include "header/shared/errors.h"                                                // ...
#include <vector>


#ifdef EXPANDER_EXPORTS
 //#define EXPANDER_API extern "C" __declspec(dllexport)
   #define EXPANDER_API                                                          // empty define
   #define EXPORT       comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)  // export unmangled names without DEF file
#else
   #define EXPANDER_API extern "C" __declspec(dllimport)
#endif


#define MAX_CHART_DESCRIPTION_LENGTH   ((MAX_SYMBOL_LENGTH) + 1 + 7)             // "SYMBOL,Monthly"


#pragma pack(push, 1)


// MQL program types
enum ProgramType {
   PT_INDICATOR = PROGRAMTYPE_INDICATOR,
   PT_EXPERT    = PROGRAMTYPE_EXPERT,
   PT_SCRIPT    = PROGRAMTYPE_SCRIPT
};


// MQL module types                          // implemented as flags
enum ModuleType {
   MT_INDICATOR = MODULETYPE_INDICATOR,      // 1
   MT_EXPERT    = MODULETYPE_EXPERT,         // 2
   MT_SCRIPT    = MODULETYPE_SCRIPT,         // 4
   MT_LIBRARY   = MODULETYPE_LIBRARY         // 8
};


// MQL program launch types
enum LaunchType {
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,       // via template
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,        // via call of iCustom()
   LT_MANUAL    = LAUNCHTYPE_MANUAL          // manually (by hand)
};


// MQL program root functions
enum RootFunction {
   RF_INIT      = ROOTFUNCTION_INIT,
   RF_START     = ROOTFUNCTION_START,
   RF_DEINIT    = ROOTFUNCTION_DEINIT
};


// MQL program initialize reasons
enum InitializeReason {
   IR_USER              = INITREASON_USER,
   IR_TEMPLATE          = INITREASON_TEMPLATE,
   IR_PROGRAM           = INITREASON_PROGRAM,
   IR_PROGRAM_AFTERTEST = INITREASON_PROGRAM_AFTERTEST,
   IR_PARAMETERS        = INITREASON_PARAMETERS,
   IR_TIMEFRAMECHANGE   = INITREASON_TIMEFRAMECHANGE,
   IR_SYMBOLCHANGE      = INITREASON_SYMBOLCHANGE,
   IR_RECOMPILE         = INITREASON_RECOMPILE
};


// MQL program uninitialize reasons
enum UninitializeReason {
   UR_UNDEFINED         = UNINITREASON_UNDEFINED,
   UR_REMOVE            = UNINITREASON_REMOVE,
   UR_RECOMPILE         = UNINITREASON_RECOMPILE,
   UR_CHARTCHANGE       = UNINITREASON_CHARTCHANGE,
   UR_CHARTCLOSE        = UNINITREASON_CHARTCLOSE,
   UR_PARAMETERS        = UNINITREASON_PARAMETERS,
   UR_ACCOUNT           = UNINITREASON_ACCOUNT,
   UR_TEMPLATE          = UNINITREASON_TEMPLATE,       // since terminal build > 509
   UR_INITFAILED        = UNINITREASON_INITFAILED,     // ...
   UR_CLOSE             = UNINITREASON_CLOSE           // ...
};


// structs
#include "header/structs/mt4/FxtHeader.h"
#include "header/structs/mt4/HistoryBar400.h"
#include "header/structs/mt4/HistoryBar401.h"
#include "header/structs/mt4/HistoryHeader.h"
#include "header/structs/mt4/MqlStr.h"
#include "header/structs/mt4/Symbol.h"
#include "header/structs/mt4/SymbolGroup.h"
#include "header/structs/mt4/SymbolSelected.h"
#include "header/structs/mt4/Tick.h"

//#include "header/structs/myfx/LogMessage.h"
#include "header/structs/myfx/ExecutionContext.h"
#include "header/structs/myfx/Test.h"

#include "header/structs/win32/FileTime.h"
#include "header/structs/win32/ProcessInformation.h"
#include "header/structs/win32/SecurityAttributes.h"
#include "header/structs/win32/StartupInfo.h"
#include "header/structs/win32/SystemTime.h"
#include "header/structs/win32/TimeZoneInformation.h"
#include "header/structs/win32/Win32FindData.h"


// app functionality
#include "header/ContextManagement.h"
#include "header/String.h"


// type definitions
typedef std::vector<EXECUTION_CONTEXT*> pec_vector;


// external var declarations for context management
extern std::vector<pec_vector> contextChains;                        // alle Context-Chains (Index = ProgramID)
extern std::vector<DWORD>      threads;                              // ID's aller bekannten Threads
extern std::vector<uint>       threadsPrograms;                      // ID's des vom Thread zuletzt ausgeführten MQL-Programms
extern uint                    lastUIThreadProgram;                  // ID des vom UI-Thread zuletzt ausgeführten MQL-Programm
extern CRITICAL_SECTION        terminalLock;                         // Terminal-weites Lock


// function declarations
#define debug(...)   _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define  warn(...)    _warn(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define error(...)   _error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)


int   _debug(const char* fileName, const char* funcName, int line, const char*        format, ...);
int   _debug(const char* fileName, const char* funcName, int line, const std::string &format, ...);
void __debug(const char* fileName, const char* funcName, int line, const char*        format, const va_list &args);

int   _warn (const char* fileName, const char* funcName, int line, int code, const char*        msgFormat, ...);
int   _warn (const char* fileName, const char* funcName, int line, int code, const std::string &msgFormat, ...);
void __warn (const char* fileName, const char* funcName, int line, int code, const char*        msgFormat, const va_list &args);

int   _error(const char* fileName, const char* funcName, int line, int code, const char*        msgFormat, ...);
int   _error(const char* fileName, const char* funcName, int line, int code, const std::string &msgFormat, ...);
void __error(const char* fileName, const char* funcName, int line, int code, const char*        msgFormat, const va_list &args);


BOOL              WINAPI onProcessAttach();
BOOL              WINAPI onProcessDetach();

const char*       WINAPI BoolToStr                (BOOL value);
const char*       WINAPI DeinitFlagsToStr         (uint flags);
const char*       WINAPI DoubleQuoteStr           (const char* value);
const char*       WINAPI ErrorToStr               (int error);
HWND              WINAPI GetApplicationWindow();
uint              WINAPI GetBoolsAddress          (const BOOL values[]);
uint              WINAPI GetChartDescription      (const char* symbol, uint timeframe, char* buffer, uint bufferSize);
uint              WINAPI GetDoublesAddress        (const double values[]);
uint              WINAPI GetGmtTime();
uint              WINAPI GetIntsAddress           (const int values[]);
int               WINAPI GetLastWin32Error();
uint              WINAPI GetLocalTime();
uint              WINAPI GetTerminalBuild();
const char*       WINAPI GetTerminalVersion();
BOOL              WINAPI GetTerminalVersions      (uint* major, uint* minor, uint* hotfix, uint* build);
DWORD             WINAPI GetUIThreadId();
HANDLE            WINAPI GetWindowProperty        (HWND hWnd, const char* lpName);
const char*       WINAPI InitFlagsToStr           (uint flags);
const char*       WINAPI InitReasonToStr          (InitializeReason reason);
const char*       WINAPI InitializeReasonToStr    (InitializeReason reason);          // Alias
const char*       WINAPI IntToHexStr              (int value);
const char*       WINAPI IntToHexStr              (const void* value);
BOOL              WINAPI IsCustomTimeframe        (int timeframe);
BOOL              WINAPI IsStdTimeframe           (int timeframe);
BOOL              WINAPI IsUIThread();
const char*       WINAPI ModuleTypeDescription    (ModuleType type);
const char*       WINAPI ModuleTypeToStr          (ModuleType type);
uint              WINAPI MT4InternalMsg();
const char*       WINAPI PeriodDescription        (uint period);
const char*       WINAPI PeriodToStr              (uint period);
const char*       WINAPI ProgramTypeDescription   (ProgramType type);
const char*       WINAPI ProgramTypeToStr         (ProgramType type);
BOOL              WINAPI RemoveTickTimer          (int timerId);
void              WINAPI RemoveTickTimers();
HANDLE            WINAPI RemoveWindowProperty     (HWND hWnd, const char* lpName);
const char*       WINAPI RootFunctionDescription  (RootFunction id);
const char*       WINAPI RootFunctionToStr        (RootFunction id);
uint              WINAPI SetupTickTimer           (HWND hWnd, int millis, DWORD flags=NULL);
BOOL              WINAPI SetWindowProperty        (HWND hWnd, const char* lpName, HANDLE value);
BOOL              WINAPI ShiftIndicatorBuffer     (double buffer[], int bufferSize, int bars, double emptyValue);
const char*       WINAPI ShowWindowCmdToStr       (int cmdShow);
const char*       WINAPI StringToStr              (const char* value);
const char*       WINAPI TimeframeDescription     (uint timeframe);                   // Alias
const char*       WINAPI TimeframeToStr           (uint timeframe);                   // Alias
VOID            CALLBACK TimerCallback            (HWND hWnd, UINT msg, UINT_PTR timerId, DWORD time);
const char*       WINAPI UninitReasonToStr        (UninitializeReason reason);
const char*       WINAPI UninitializeReasonToStr  (UninitializeReason reason);        // Alias
char*             WINAPI WCharsToStr              (const WCHAR* wcstr, size_t count);


/**
 * Pseudo functions return fixed values according to their name. All parameters are ignored.
 */
int  WINAPI _CLR_NONE    (...);
int  WINAPI _EMPTY       (...);
HWND WINAPI _INVALID_HWND(...);
int  WINAPI _NULL        (...);
bool WINAPI _true        (...);
bool WINAPI _false       (...);
BOOL WINAPI _TRUE        (...);
BOOL WINAPI _FALSE       (...);


/**
 * Pseudo functions return variable values according to their name. Except the first all parameters are ignored.
 */
bool   WINAPI _bool  (bool   value, ...);
char   WINAPI _char  (char   value, ...);
int    WINAPI _int   (int    value, ...);
float  WINAPI _float (float  value, ...);
double WINAPI _double(double value, ...);
BOOL   WINAPI _BOOL  (BOOL   value, ...);
