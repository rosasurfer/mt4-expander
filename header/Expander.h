#pragma once

#include "common.h"
#include "stdafx.h"
#include "shared/defines.h"                                                      // shared between DLL and MQL
#include "shared/errors.h"                                                       // ...
#include <vector>


#ifdef EXPANDER_EXPORTS
 //#define EXPANDER_API extern "C" __declspec(dllexport)
   #define EXPANDER_API                                                          // empty define
   #define EXPORT       comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)  // export unmangled names without DEF file
#else
   #define EXPANDER_API extern "C" __declspec(dllimport)
#endif


#pragma pack(push, 1)


// MQL-Programmtypen
enum ProgramType {
   PT_INDICATOR = PROGRAMTYPE_INDICATOR,
   PT_EXPERT    = PROGRAMTYPE_EXPERT,
   PT_SCRIPT    = PROGRAMTYPE_SCRIPT
};


// MQL-Modultypen                            // als Flag implementiert
enum ModuleType {
   MT_INDICATOR = MODULETYPE_INDICATOR,      // 1
   MT_EXPERT    = MODULETYPE_EXPERT,         // 2
   MT_SCRIPT    = MODULETYPE_SCRIPT,         // 4
   MT_LIBRARY   = MODULETYPE_LIBRARY         // 8 - Libraries sind keine eigenständigen Programme
};


// Launchtypen eines MQL-Programms: via Template, via iCustom() oder von Hand
enum LaunchType {
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,
   LT_MANUAL    = LAUNCHTYPE_MANUAL
};


// MQL-Rootfunktionen
enum RootFunction {
   RF_INIT      = ROOTFUNCTION_INIT,
   RF_START     = ROOTFUNCTION_START,
   RF_DEINIT    = ROOTFUNCTION_DEINIT
};


// InitializeReasons
enum InitializeReason {
   INITREASON_USER              = INIT_REASON_USER,
   INITREASON_TEMPLATE          = INIT_REASON_TEMPLATE,
   INITREASON_PROGRAM           = INIT_REASON_PROGRAM,
   INITREASON_PROGRAM_AFTERTEST = INIT_REASON_PROGRAM_AFTERTEST,
   INITREASON_PARAMETERS        = INIT_REASON_PARAMETERS,
   INITREASON_TIMEFRAMECHANGE   = INIT_REASON_TIMEFRAMECHANGE,
   INITREASON_SYMBOLCHANGE      = INIT_REASON_SYMBOLCHANGE,
   INITREASON_RECOMPILE         = INIT_REASON_RECOMPILE
};


// MQL-UninitializeReasons
enum UninitializeReason {
   UNINITREASON_UNDEFINED       = REASON_UNDEFINED,
   UNINITREASON_REMOVE          = REASON_REMOVE,
   UNINITREASON_RECOMPILE       = REASON_RECOMPILE,
   UNINITREASON_CHARTCHANGE     = REASON_CHARTCHANGE,
   UNINITREASON_CHARTCLOSE      = REASON_CHARTCLOSE,
   UNINITREASON_PARAMETERS      = REASON_PARAMETERS,
   UNINITREASON_ACCOUNT         = REASON_ACCOUNT,
   UNINITREASON_TEMPLATE        = REASON_TEMPLATE,       // ab Build > 509
   UNINITREASON_INITFAILED      = REASON_INITFAILED,     // ...
   UNINITREASON_CLOSE           = REASON_CLOSE           // ...
};


#include "mql/structs/mt4/FxtHeader.h"
#include "mql/structs/mt4/HistoryBar400.h"
#include "mql/structs/mt4/HistoryBar401.h"
#include "mql/structs/mt4/HistoryHeader.h"
#include "mql/structs/mt4/MqlStr.h"
#include "mql/structs/mt4/Symbol.h"
#include "mql/structs/mt4/SymbolGroup.h"
#include "mql/structs/mt4/SymbolSelected.h"
#include "mql/structs/mt4/Tick.h"

//#include "mql/structs/myfx/LogMessage.h"
#include "mql/structs/myfx/ExecutionContext.h"

#include "mql/structs/win32/FileTime.h"
#include "mql/structs/win32/ProcessInformation.h"
#include "mql/structs/win32/SecurityAttributes.h"
#include "mql/structs/win32/StartupInfo.h"
#include "mql/structs/win32/SystemTime.h"
#include "mql/structs/win32/TimeZoneInformation.h"
#include "mql/structs/win32/Win32FindData.h"


// Deklaration Thread- und EXECUTION_CONTEXT-Verwaltung (Initialisierung in Expander.cpp)
extern std::vector<pec_vector> contextChains;                        // alle Context-Chains (Index = ProgramID)
extern std::vector<DWORD>      threads;                              // ID's aller bekannten Threads
extern std::vector<uint>       threadsPrograms;                      // ID's des vom Thread zuletzt ausgeführten MQL-Programms
extern uint                    lastUIThreadProgram;                  // ID des vom UI-Thread zuletzt ausgeführten MQL-Programm
extern CRITICAL_SECTION        terminalLock;                         // Terminal-weites Lock


// Funktionsdeklarationen
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

const char*       WINAPI BoolToStr        (BOOL value);
const char*       WINAPI DeinitFlagsToStr (uint flags);
const char*       WINAPI DoubleQuoteStr   (const char* value);
const char*       WINAPI ErrorToStr       (int error);
int               WINAPI FindFirstIndicatorInLimbo(HWND hChart, const char* name, UninitializeReason reason);
HWND              WINAPI GetApplicationWindow();
DWORD             WINAPI GetUIThreadId();
uint              WINAPI GetBoolsAddress  (const BOOL   values[]);
uint              WINAPI GetIntsAddress   (const int    values[]);
uint              WINAPI GetDoublesAddress(const double values[]);
uint              WINAPI GetStringsAddress(const MqlStr values[]);
uint              WINAPI GetStringAddress (const char*  value   );
const char*       WINAPI GetString        (const char*  value   );
int               WINAPI GetLastWin32Error();
uint              WINAPI GetTerminalBuild();
const char*       WINAPI GetTerminalVersion();
BOOL              WINAPI GetTerminalVersionNumbers(uint* major, uint* minor, uint* hotfix, uint* build);
const char*       WINAPI InitFlagsToStr         (uint flags );
InitializeReason  WINAPI InitReason             (const char* programName, ProgramType programType, UninitializeReason uninitializeReason, const char* symbol, EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec, BOOL testing, BOOL visualMode, HWND hChart, int subChartDropped);
const char*       WINAPI InitReasonToStr        (InitializeReason reason);
const char*       WINAPI InitializeReasonToStr  (InitializeReason reason);    // Alias
const char*       WINAPI IntToHexStr            (int   value);
const char*       WINAPI IntToHexStr            (void* value);
BOOL              WINAPI IsCustomTimeframe      (int timeframe);
BOOL              WINAPI IsStdTimeframe         (int timeframe);
BOOL              WINAPI IsUIThread();
const char*       WINAPI ModuleTypeDescription  (ModuleType  type);
const char*       WINAPI ModuleTypeToStr        (ModuleType  type);
uint              WINAPI MT4InternalMsg();
const char*       WINAPI PeriodDescription      (int period); const char* WINAPI TimeframeDescription(int timeframe);   // Alias
const char*       WINAPI PeriodToStr            (int period); const char* WINAPI TimeframeToStr      (int timeframe);   // Alias
const char*       WINAPI ProgramTypeDescription (ProgramType type);
const char*       WINAPI ProgramTypeToStr       (ProgramType type);
void              WINAPI RemoveTickTimers();
const char*       WINAPI RootFunctionDescription(RootFunction id);
const char*       WINAPI RootFunctionToStr      (RootFunction id);
const char*       WINAPI TestFlagsToStr         (uint flags);
const char*       WINAPI UninitReasonToStr      (UninitializeReason reason);
const char*       WINAPI UninitializeReasonToStr(UninitializeReason reason);  // Alias


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende feste Werte zurückzugeben.
 * Alle Parameter werden ignoriert.
 */
int  WINAPI _CLR_NONE(...);
int  WINAPI _EMPTY   (...);
int  WINAPI _NULL    (...);
bool WINAPI _true    (...);
bool WINAPI _false   (...);
BOOL WINAPI _TRUE    (...);
BOOL WINAPI _FALSE   (...);


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende variable Werte zurückzugeben.
 * Außer dem ersten werden alle übergebenen Parameter ignoriert.
 */
bool   WINAPI _bool  (bool   value, ...);
char   WINAPI _char  (char   value, ...);
int    WINAPI _int   (int    value, ...);
float  WINAPI _float (float  value, ...);
double WINAPI _double(double value, ...);
BOOL   WINAPI _BOOL  (BOOL   value, ...);
