#pragma once

#include "common.h"
#include "stdafx.h"
#include <vector>
#include "shared/defines.h"
#include "shared/errors.h"


#ifdef EXPANDER_EXPORTS
 //#define EXPANDER_API extern "C" __declspec(dllexport)
   #define EXPANDER_API                                                             // empty define
   #define EXPORT       comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)     // export unmangled names without DEF file
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


// MQL-UninitializeReasons
enum UninitializeReason {
   UNINITREASON_UNDEFINED   = REASON_UNDEFINED,
   UNINITREASON_REMOVE      = REASON_REMOVE,
   UNINITREASON_RECOMPILE   = REASON_RECOMPILE,
   UNINITREASON_CHARTCHANGE = REASON_CHARTCHANGE,
   UNINITREASON_CHARTCLOSE  = REASON_CHARTCLOSE,
   UNINITREASON_PARAMETERS  = REASON_PARAMETERS,
   UNINITREASON_ACCOUNT     = REASON_ACCOUNT,
   UNINITREASON_TEMPLATE    = REASON_TEMPLATE,     // ab Build > 509
   UNINITREASON_INITFAILED  = REASON_INITFAILED,   // ...
   UNINITREASON_CLOSE       = REASON_CLOSE         // ...
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
extern std::vector<DWORD>      threadIds;
extern std::vector<uint>       threadIdsProgramIds;
extern std::vector<pec_vector> contextChains;


// Funktionsdeklarationen
BOOL onProcessAttach();
BOOL onProcessDetach();
void RemoveTickTimers();

BOOL         WINAPI SyncMainExecutionContext(EXECUTION_CONTEXT* ec, ProgramType type, const char* name, RootFunction functionId, UninitializeReason reason, const char* symbol, int period);
BOOL         WINAPI SyncLibExecutionContext (EXECUTION_CONTEXT* ec,                   const char* name, RootFunction functionId,                            const char* symbol, int period);

void         WINAPI SetLogLevel(int level);
HWND         WINAPI GetApplicationWindow();
DWORD        WINAPI GetUIThreadId();
BOOL         WINAPI IsUIThread();
uint         WINAPI GetBoolsAddress  (BOOL   values[]);
uint         WINAPI GetIntsAddress   (int    values[]);   int WINAPI GetBufferAddress(int values[]);   // Alias
uint         WINAPI GetDoublesAddress(double values[]);
uint         WINAPI GetStringsAddress(MqlStr values[]);
uint         WINAPI GetStringAddress (char*  value   );
const char*  WINAPI GetString        (char*  value   );
int          WINAPI GetLastWin32Error();
BOOL         WINAPI IsCustomTimeframe(int timeframe);
BOOL         WINAPI IsStdTimeframe(int timeframe);
const char*  WINAPI IntToHexStr(int value);
uint         WINAPI MT4InternalMsg();

const char*  WINAPI ModuleTypeDescription  (ModuleType  type);
const char*  WINAPI ModuleTypeToStr        (ModuleType  type);
const char*  WINAPI PeriodDescription      (int period);        const char* WINAPI TimeframeDescription(int timeframe);   // Alias
const char*  WINAPI PeriodToStr            (int period);        const char* WINAPI TimeframeToStr      (int timeframe);   // Alias
const char*  WINAPI ProgramTypeDescription (ProgramType type);
const char*  WINAPI ProgramTypeToStr       (ProgramType type);
const char*  WINAPI RootFunctionName       (RootFunction id);
const char*  WINAPI RootFunctionToStr      (RootFunction id);
const char*  WINAPI UninitializeReasonToStr(UninitializeReason reason);

BOOL         WINAPI GetTerminalVersionNumbers(uint* major, uint* minor, uint* hotfix, uint* build);
const char*  WINAPI GetTerminalVersion();
uint         WINAPI GetTerminalBuild();


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende feste Werte zurückzugeben.
 * Alle Parameter werden ignoriert.
 */
int _CLR_NONE(...);
