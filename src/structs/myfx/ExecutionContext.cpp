#include "header/Expander.h"


/**
 * Gibt die Programm-ID eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Programm-ID
 */
uint WINAPI ec_ProgramId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programId);
   #pragma EXPORT
}


/**
 * Gibt den Programm-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ProgramType
 */
ProgramType WINAPI ec_ProgramType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programType);
   #pragma EXPORT
}


/**
 * Gibt den Programmnamen eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Programmname
 */
const char* WINAPI ec_ProgramName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programName);
   #pragma EXPORT
}


/**
 * Gibt den Modul-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ModuleType
 */
ModuleType WINAPI ec_ModuleType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->moduleType);
   #pragma EXPORT
}


/**
 * Gibt den Modulnamen eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Modulname
 */
const char* WINAPI ec_ModuleName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->moduleName);
   #pragma EXPORT
}


/**
 * Gibt den Launch-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return LaunchType
 */
LaunchType WINAPI ec_LaunchType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->launchType);
   #pragma EXPORT
}


/**
 * Gibt die RootFunction eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 */
RootFunction WINAPI ec_RootFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->rootFunction);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten InitCycle-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_InitCycle(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initCycle);
   #pragma EXPORT
}


/**
 * Gibt den InitializeReason eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return InitializeReason
 */
InitializeReason WINAPI ec_InitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initReason);
   #pragma EXPORT
}


/**
 * Gibt den UninitializeReason eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI ec_UninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->uninitReason);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Testing-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Testing(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->testing);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten VisualMode-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_VisualMode(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->visualMode);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Optimization-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Optimization(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->optimization);
   #pragma EXPORT
}


/**
 * Gibt die Init-Konfiguration eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - Init-Flags
 */
DWORD WINAPI ec_InitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initFlags);
   #pragma EXPORT
}


/**
 * Gibt die Deinit-Konfiguration eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - Deinit-Flags
 */
DWORD WINAPI ec_DeinitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->deinitFlags);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Logging-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Logging(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->logging);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Logdateinamen zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Dateiname
 */
const char* WINAPI ec_CustomLogFile(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->customLogFile);
   #pragma EXPORT
}


/**
 * Gibt das Chartsymbol eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Symbol
 */
const char* WINAPI ec_Symbol(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->symbol);
   #pragma EXPORT
}


/**
 * Gibt die Chartperiode eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Chartperiode
 */
uint WINAPI ec_Timeframe(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->timeframe);
   #pragma EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Handle eines Chartframes zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - Handle, entspricht dem Rückgabewert der MQL-Funktion WindowHandle()
 */
HWND WINAPI ec_hChart(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->hChart);
   #pragma EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Handle eines Chartfensters zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - Handle
 */
HWND WINAPI ec_hChartWindow(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->hChartWindow);
   #pragma EXPORT
}


/**
 * Kopiert den SuperContext eines EXECUTION_CONTEXT in die übergebene Variable.
 *
 * @param  EXECUTION_CONTEXT* ec  - ExecutionContext
 * @param  EXECUTION_CONTEXT* sec - Variable zur Aufnahme des SuperContexts
 *
 * @return BOOL - TRUE,  wenn ein SuperContext kopiert wurde;
 *                FALSE, wenn der ExecutionContext keinen SuperContext enthielt oder ein Fehler auftrat
 */
BOOL WINAPI ec_SuperContext(const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if ((uint)ec  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)sec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));

   if (ec->superContext) {
      *sec = *ec->superContext;
      return(TRUE);
   }

   static const EXECUTION_CONTEXT zeroed = {};
   *sec = zeroed;
   return(FALSE);
   #pragma EXPORT
}


/**
 * Gibt den Zeiger auf den SuperContext eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return EXECUTION_CONTEXT*
 */
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->superContext);
   #pragma EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's current thread id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - thread id
 */
uint WINAPI ec_ThreadId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->threadId);
   #pragma EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's number of times start() was called.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint
 */
uint WINAPI ec_Ticks(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->ticks);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten MQL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_MqlError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   return(ec->mqlError);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten DLL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_DllError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   return(ec->dllError);
   #pragma EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherte DLL-Warnung zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_DllWarning(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   return(ec->dllWarning);
   #pragma EXPORT
}


/**
 * Setzt die Programm-ID eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               id - ID größer 0
 *
 * @return uint - dieselbe ID
 */
uint WINAPI ec_SetProgramId(EXECUTION_CONTEXT* ec, uint id) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (id <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter id = %d (must be greater than zero)", id));

   ec->programId = id;                                               // synchronize main and master context
   if (contextChains.size() > id && ec==contextChains[id][1] && contextChains[id][0])
      return(ec_SetProgramId(contextChains[id][0], id));

   return(id);
   #pragma EXPORT
}


/**
 * Setzt den Programm-Typ eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  ProgramType        type
 *
 * @return ProgramType - derselbe ProgramType
 */
ProgramType WINAPI ec_SetProgramType(EXECUTION_CONTEXT* ec, ProgramType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   switch (type) {
      case PT_INDICATOR:
      case PT_EXPERT   :
      case PT_SCRIPT   : break;
      default:
         return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter type = %d (not a ProgramType)", type));
   }

   ec->programType = type;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetProgramType(contextChains[pid][0], type));

   return(type);
   #pragma EXPORT
}


/**
 * Setzt den Programmnamen eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - derselbe Name
 */
const char* WINAPI ec_SetProgramName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)          return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)          return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(ec->programName)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name = \"%s\" (must be 1 to %d characters)", name, sizeof(ec->programName)-1));

   if (!strcpy(ec->programName, name))
      return(NULL);

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetProgramName(contextChains[pid][0], name));

   return(name);
   #pragma EXPORT
}


/**
 * Setzt den Modul-Typ eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  ModuleType         type
 *
 * @return ModuleType - derselbe ModuleType
 */
ModuleType WINAPI ec_SetModuleType(EXECUTION_CONTEXT* ec, ModuleType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   switch (type) {
      case MT_INDICATOR:
      case MT_EXPERT   :
      case MT_SCRIPT   :
      case MT_LIBRARY  : break;
      default:
         return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter type = %d (not a ModuleType)", type));
   }

   ec->moduleType = type;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetModuleType(contextChains[pid][0], type));

   return(type);
   #pragma EXPORT
}


/**
 * Setzt den Modulnamen eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - derselbe Name
 */
const char* WINAPI ec_SetModuleName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(ec->moduleName)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name = \"%s\" (must be 1 to %d characters)", name, sizeof(ec->moduleName)-1));

   if (!strcpy(ec->moduleName, name))
      return(NULL);

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetModuleName(contextChains[pid][0], name));

   return(name);
   #pragma EXPORT
}


/**
 * Setzt den Launch-Typ eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  LaunchType         type
 *
 * @return LaunchType - derselbe Launch-Typ
 */
LaunchType WINAPI ec_SetLaunchType(EXECUTION_CONTEXT* ec, LaunchType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   switch (type) {
      case LT_TEMPLATE:
      case LT_PROGRAM :
      case LT_MANUAL  : break;
      default:
         return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter type = %d (not a LaunchType)", type));
   }

   ec->launchType = type;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetLaunchType(contextChains[pid][0], type));

   return(type);
   #pragma EXPORT
}


/**
 * Setzt die RootFunction-ID eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  RootFunction       id
 *
 * @return RootFunction - dieselbe RootFunction-ID
 */
RootFunction WINAPI ec_SetRootFunction(EXECUTION_CONTEXT* ec, RootFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   switch (id) {
      case RF_INIT  :
      case RF_START :
      case RF_DEINIT:
      case NULL     :
         break;

      default:
         return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter id = %d (not a RootFunction)", id));
   }

   ec->rootFunction = id;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetRootFunction(contextChains[pid][0], id));

   return(id);
   #pragma EXPORT
}


/**
 * Setzt den InitCycle-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe InitCycle-Status
 */
BOOL WINAPI ec_SetInitCycle(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->initCycle = status;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetInitCycle(contextChains[pid][0], status));

   return(status);
   #pragma EXPORT
}


/**
 * Setzt den InitializeReason eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  InitializeReason reason
 *
 * @return InitializeReason - derselbe InitializeReason
 */
InitializeReason WINAPI ec_SetInitReason(EXECUTION_CONTEXT* ec, InitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (reason) {
      case IR_USER:
      case IR_TEMPLATE:
      case IR_PROGRAM:
      case IR_PROGRAM_AFTERTEST:
      case IR_PARAMETERS:
      case IR_TIMEFRAMECHANGE:
      case IR_SYMBOLCHANGE:
      case IR_RECOMPILE:
         break;

      case NULL:
         if (ec->moduleType==MT_EXPERT ) break;
         if (ec->moduleType==MT_LIBRARY) break;

      default:
         return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason = %d (not an InitializeReason)", reason));
   }

   ec->initReason = reason;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetInitReason(contextChains[pid][0], reason));

   return(reason);
   #pragma EXPORT
}


/**
 * Setzt den UninitializeReason eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  UninitializeReason reason
 *
 * @return UninitializeReason - derselbe UninitializeReason
 */
UninitializeReason WINAPI ec_SetUninitReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (reason) {
      case UR_UNDEFINED  :
      case UR_REMOVE     :
      case UR_RECOMPILE  :
      case UR_CHARTCHANGE:
      case UR_CHARTCLOSE :
      case UR_PARAMETERS :
      case UR_ACCOUNT    :
      // build > 509
      case UR_TEMPLATE   :
      case UR_INITFAILED :
      case UR_CLOSE      : break;
      default:
         return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason = %d (not an UninitializeReason)", reason));
   }

   ec->uninitReason = reason;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetUninitReason(contextChains[pid][0], reason));

   return(reason);
   #pragma EXPORT
}


/**
 * Setzt den Testing-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe Status
 */
BOOL WINAPI ec_SetTesting(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->testing = status;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetTesting(contextChains[pid][0], status));

   return(status);
   #pragma EXPORT
}


/**
 * Setzt den VisualMode-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe Status
 */
BOOL WINAPI ec_SetVisualMode(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->visualMode = status;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetVisualMode(contextChains[pid][0], status));

   return(status);
   #pragma EXPORT
}


/**
 * Setzt den Optimization-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe Status
 */
BOOL WINAPI ec_SetOptimization(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->optimization = status;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetOptimization(contextChains[pid][0], status));

   return(status);
   #pragma EXPORT
}


/**
 * Setzt die Init-Konfiguration eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - dieselben Init-Flags
 */
DWORD WINAPI ec_SetInitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->initFlags = flags;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetInitFlags(contextChains[pid][0], flags));

   return(flags);
   #pragma EXPORT
}


/**
 * Setzt die Deinit-Konfiguration eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - dieselben Deinit-Flags
 */
DWORD WINAPI ec_SetDeinitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->deinitFlags = flags;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetDeinitFlags(contextChains[pid][0], flags));

   return(flags);
   #pragma EXPORT
}


/**
 * Setzt den Logging-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe Logging-Status
 */
BOOL WINAPI ec_SetLogging(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->logging = status;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetLogging(contextChains[pid][0], status));

   return(status);
   #pragma EXPORT
}


/**
 * Setzt den Namen der Logdatei eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              fileName - statt eines NULL-Pointers kann auch ein Leerstring angegeben werden
 *
 * @return char* - derselbe Dateiname
 */
const char* WINAPI ec_SetCustomLogFile(EXECUTION_CONTEXT* ec, const char* fileName) {
   if ((uint)ec < MIN_VALID_POINTER)                      return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   if (fileName) {
      // fileName ist kein NULL-Pointer
      if ((uint)fileName < MIN_VALID_POINTER)             return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter fileName = 0x%p (not a valid pointer)", fileName));
      if (strlen(fileName) > sizeof(ec->customLogFile)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter fileName = \"%s\" (max %d characters)", fileName, sizeof(ec->customLogFile)-1));

      if (!strcpy(ec->customLogFile, fileName))
         return(NULL);
   }
   else {
      // fileName ist NULL-Pointer
      ec->customLogFile[0] = '\0';
   }

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetCustomLogFile(contextChains[pid][0], fileName));

   return(fileName);
   #pragma EXPORT
}


/**
 * Setzt das Symbol eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              symbol
 *
 * @return char* - dasselbe Symbol
 */
const char* WINAPI ec_SetSymbol(EXECUTION_CONTEXT* ec, const char* symbol) {
   if ((uint)ec     < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   int len = strlen(symbol);
   if (!len || len > sizeof(ec->symbol)) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol = \"%s\" (must be 1 to %d characters)", symbol, sizeof(ec->symbol)-1));

   if (!strcpy(ec->symbol, symbol))
      return(NULL);

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetSymbol(contextChains[pid][0], symbol));

   return(symbol);
   #pragma EXPORT
}


/**
 * Setzt den Chart-Timeframe eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               timeframe
 *
 * @return uint - derselbe Timeframe
 */
uint WINAPI ec_SetTimeframe(EXECUTION_CONTEXT* ec, uint timeframe) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (timeframe <= 0)               return(error(ERR_INVALID_PARAMETER, "invalid parameter timeframe = %d (must be greater than zero)", timeframe));

   ec->timeframe = timeframe;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetTimeframe(contextChains[pid][0], timeframe));

   return(timeframe);
   #pragma EXPORT
}


/**
 * Setzt das Handle des Chart-Frames eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  HWND               hWnd - entspricht dem Rückgabewert von WindowHandle()
 *
 * @return HWND - dasselbe Handle
 */
HWND WINAPI ec_SetHChart(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->hChart = hWnd;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetHChart(contextChains[pid][0], hWnd));

   return(hWnd);
   #pragma EXPORT
}


/**
 * Setzt das Handle des Chart-Fensters eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  HWND               hWnd
 *
 * @return HWND - dasselbe Handle
 */
HWND WINAPI ec_SetHChartWindow(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->hChartWindow = hWnd;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetHChartWindow(contextChains[pid][0], hWnd));

   return(hWnd);
   #pragma EXPORT
}


/**
 * Setzt den SuperContext eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec  - zu modifizierender Context
 * @param  EXECUTION_CONTEXT* sec - zu setzender SuperContext
 *
 * @return EXECUTION_CONTEXT* - der gesetzte SuperContext
 */
EXECUTION_CONTEXT* WINAPI ec_SetSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if (       (uint)ec  < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));

   ec->superContext = sec;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetSuperContext(contextChains[pid][0], sec));

   return(sec);
   #pragma EXPORT
}


/**
 * Alias (imported differently in MQL)
 */
EXECUTION_CONTEXT* WINAPI ec_SetLpSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* lpSec) {
   return(ec_SetSuperContext(ec, lpSec));
   #pragma EXPORT
}


/**
 * Set an EXECUTION_CONTEXT's current thread id.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               id - thread id
 *
 * @return uint - the same thread id
 */
uint WINAPI ec_SetThreadId(EXECUTION_CONTEXT* ec, uint id) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (id <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter id = %d (must be greater than zero)", id));

   ec->threadId = id;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetThreadId(contextChains[pid][0], id));

   return(id);
   #pragma EXPORT
}


/**
 * Set an EXECUTION_CONTEXT's number of times start() was called.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               count
 *
 * @return uint - dieselbe Anzahl
 */
uint WINAPI ec_SetTicks(EXECUTION_CONTEXT* ec, uint count) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (count < 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter count = %d (must be non-negative)", count));

   ec->ticks = count;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetTicks(contextChains[pid][0], count));

   return(count);
   #pragma EXPORT
}


/**
 * Set the MQL error code of an EXECUTION_CONTEXT. Called by MQL::SetLastError().
 *
 * If called with a library context the error will bubble up to the library's main module. If called with an indicator context
 * loaded by iCustom() the error will bubble up to the loading program. The error code NO_ERROR will never bubble up.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - same error or EMPTY (-1) if an error occurred
 */
int WINAPI ec_SetMqlError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec)));

   ec->mqlError = error;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      ec_SetMqlError(contextChains[pid][0], error);

   if (error != NO_ERROR) {                                          // no propagation for NO_ERROR
      if (ec->moduleType==MT_LIBRARY && pid) {                       // propagation from library to main module
         EXECUTION_CONTEXT* master = contextChains[pid][0];
         EXECUTION_CONTEXT* main   = contextChains[pid][1];
         if (main) ec_SetMqlError(main,   error);                    // whichever is available
         else      ec_SetMqlError(master, error);
      }

      if (ec->superContext)
         ec_SetMqlError(ec->superContext, error);                    // propagation to parent program
   }
   return(error);
   #pragma EXPORT
}


/**
 * Setzt den DLL-Fehler eines EXECUTION_CONTEXT.
 *
 * Zusätzlich wird der DLL-Fehler in den jeweiligen Hauptkontext propagiert (Propagation zum aufrufenden Hauptmodul).
 * Fehler werden nur beim Setzen propagiert, nicht beim Zurücksetzen.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - derselbe Fehler oder -1, falls ein Fehler auftrat
 */
int WINAPI ec_SetDllError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   ec->dllError = error;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      ec_SetDllError(contextChains[pid][0], error);

   if (!error)                                                       // keine Propagation beim Zurücksetzen eines Fehlers
      return(error);

   if (pid && ec->moduleType==MT_LIBRARY) {                          // Fehler aus Libraries in den Hauptkontext propagieren
      EXECUTION_CONTEXT* master = contextChains[pid][0];             // (oder den Master-Context, wenn Hauptkontext nicht verfügbar)
      EXECUTION_CONTEXT* main   = contextChains[pid][1];
      if (main) ec_SetDllError(main,   error);
      else      ec_SetDllError(master, error);
   }
   return(error);
   #pragma EXPORT
}


/**
 * Setzt die DLL-Warnung eines EXECUTION_CONTEXT.
 *
 * Zusätzlich wird die DLL-Warnung in den jeweiligen Hauptkontext propagiert (Propagation zum aufrufenden Hauptmodul).
 * Warnungen werden nur beim Setzen propagiert, nicht beim Zurücksetzen.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - derselbe Fehler oder -1, falls ein Fehler auftrat
 */
int WINAPI ec_SetDllWarning(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   ec->dllWarning = error;

   uint pid = ec->programId;                                         // synchronize main and master context
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      ec_SetDllWarning(contextChains[pid][0], error);

   if (!error)                                                       // keine Propagation beim Zurücksetzen eines Fehlers
      return(error);

   if (pid && ec->moduleType==MT_LIBRARY) {                          // Warnung aus Libraries in den Hauptkontext propagieren
      EXECUTION_CONTEXT* master = contextChains[pid][0];             // (oder den Master-Context, wenn Hauptkontext nicht verfügbar)
      EXECUTION_CONTEXT* main   = contextChains[pid][1];
      if (main) ec_SetDllWarning(main,   error);
      else      ec_SetDllWarning(master, error);
   }
   return(error);
   #pragma EXPORT
}


/**
 * Return the RootFunction of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 */
RootFunction WINAPI mec_RootFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   uint programId = ec->programId;
   if (!programId) return((RootFunction)error(ERR_ILLEGAL_STATE, "illegal programId=%d in ec=%s", programId, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = contextChains[programId][0];
   return(master->rootFunction);
   #pragma EXPORT
}


/**
 * Return the UninitializeReason of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI mec_UninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   uint programId = ec->programId;
   if (!programId) return((UninitializeReason)error(ERR_ILLEGAL_STATE, "illegal programId=%d in ec=%s", programId, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = contextChains[programId][0];
   return(master->uninitReason);
   #pragma EXPORT
}


/**
 * Return the init configuration flags of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - flags
 */
DWORD WINAPI mec_InitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   uint programId = ec->programId;
   if (!programId) return(error(ERR_ILLEGAL_STATE, "illegal programId=%d in ec=%s", programId, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = contextChains[programId][0];
   return(master->initFlags);
   #pragma EXPORT
}


/**
 * Return a human-readable version of an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               outputDebug - whether or not to duplicate the result to OutputDebugString()
 *                                          (default: no)
 * @return char*
 */
const char* WINAPI EXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug/*=FALSE*/) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   char* result = "{(empty)}";
   static const EXECUTION_CONTEXT empty = {};

   if (memcmp(ec, &empty, sizeof(EXECUTION_CONTEXT)) != 0) {
      std::stringstream ss; ss
         <<  "{programId="     <<                   ec->programId
         << ", programType="   <<  ProgramTypeToStr(ec->programType  )
         << ", programName="   <<    DoubleQuoteStr(ec->programName  )
         << ", moduleType="    <<   ModuleTypeToStr(ec->moduleType   )
         << ", moduleName="    <<    DoubleQuoteStr(ec->moduleName   )
         << ", launchType="    <<                   ec->launchType
         << ", rootFunction="  << RootFunctionToStr(ec->rootFunction )
         << ", initCycle="     <<         BoolToStr(ec->initCycle    )
         << ", initReason="    <<   InitReasonToStr(ec->initReason   )
         << ", uninitReason="  << UninitReasonToStr(ec->uninitReason )
         << ", testing="       <<         BoolToStr(ec->testing      )
         << ", visualMode="    <<         BoolToStr(ec->visualMode   )
         << ", optimization="  <<         BoolToStr(ec->optimization )
         << ", initFlags="     <<    InitFlagsToStr(ec->initFlags    )
         << ", deinitFlags="   <<  DeinitFlagsToStr(ec->deinitFlags  )
         << ", logging="       <<         BoolToStr(ec->logging      )
         << ", customLogFile=" <<    DoubleQuoteStr(ec->customLogFile)
         << ", symbol="        <<    DoubleQuoteStr(ec->symbol       )
         << ", timeframe="     <<       PeriodToStr(ec->timeframe    )
         << ", hChart="        <<             (uint)ec->hChart
         << ", hChartWindow="  <<             (uint)ec->hChartWindow
         << ", superContext="  <<             (uint)ec->superContext
         << ", threadId="      <<                   ec->threadId
         << ", ticks="         <<                   ec->ticks
         << ", mqlError="      <<                 (!ec->mqlError   ? "0" : ErrorToStr(ec->mqlError  ))
         << ", dllError="      <<                 (!ec->dllError   ? "0" : ErrorToStr(ec->dllError  ))
         << ", dllWarning="    <<                 (!ec->dllWarning ? "0" : ErrorToStr(ec->dllWarning))
         << "}";
      std::string str = ss.str();
      result = strcpy(new char[str.size()+1], str.c_str());          // TODO: close memory leak
   }

   if (outputDebug) debug(result);
   return(result);
   #pragma EXPORT
}


/**
 * Alias (different MQL import)
 */
const char* WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug/*=FALSE*/) {
   return(EXECUTION_CONTEXT_toStr(ec, outputDebug));
   #pragma EXPORT
}
