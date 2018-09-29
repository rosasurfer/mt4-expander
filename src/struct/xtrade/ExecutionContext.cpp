#include "expander.h"
#include "lib/conversion.h"
#include "lib/format.h"
#include "lib/string.h"
#include "struct/xtrade/ExecutionContext.h"


extern std::vector<ContextChain> g_contextChains;                    // all context chains (i.e. MQL programs, index = program id)


/**
 * Return the MQL program index of an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - program index (starting from 1)
 */
uint WINAPI ec_ProgramIndex(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programIndex);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Programm-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ProgramType
 */
ProgramType WINAPI ec_ProgramType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programType);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Programmnamen eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Programmname
 */
const char* WINAPI ec_ProgramName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programName);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Modul-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ModuleType
 */
ModuleType WINAPI ec_ModuleType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->moduleType);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Modulnamen eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Modulname
 */
const char* WINAPI ec_ModuleName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->moduleName);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Launch-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return LaunchType
 */
LaunchType WINAPI ec_LaunchType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->launchType);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die RootFunction eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 */
RootFunction WINAPI ec_RootFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->rootFunction);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten InitCycle-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_InitCycle(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->initCycle);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den InitializeReason eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return InitializeReason
 */
InitializeReason WINAPI ec_InitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->initReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den UninitializeReason eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI ec_UninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->uninitReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Testing-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Testing(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->testing);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten VisualMode-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_VisualMode(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->visualMode);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Optimization-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Optimization(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->optimization);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Init-Konfiguration eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - Init-Flags
 */
DWORD WINAPI ec_InitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->initFlags);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Deinit-Konfiguration eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - Deinit-Flags
 */
DWORD WINAPI ec_DeinitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->deinitFlags);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Logging-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Logging(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->logging);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Logdateinamen zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Dateiname
 */
const char* WINAPI ec_CustomLogFile(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->customLogFile);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das Chartsymbol eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Symbol
 */
const char* WINAPI ec_Symbol(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->symbol);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Chartperiode eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Chartperiode
 */
uint WINAPI ec_Timeframe(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->timeframe);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Handle eines Chartframes zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - Handle, entspricht dem Rückgabewert der MQL-Funktion WindowHandle()
 */
HWND WINAPI ec_hChart(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->hChart);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Handle eines Chartfensters zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - Handle
 */
HWND WINAPI ec_hChartWindow(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->hChartWindow);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)sec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec));

   if (ec->superContext) {
      *sec = *ec->superContext;
      return(TRUE);
   }

   static const EXECUTION_CONTEXT empty = {};
   *sec = empty;
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Zeiger auf den SuperContext eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return EXECUTION_CONTEXT*
 */
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->superContext);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's current thread id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - thread id
 */
uint WINAPI ec_ThreadId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->threadId);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the number of times an EXECUTION_CONTEXT's start() function was called.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint
 */
uint WINAPI ec_Ticks(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->ticks);
   #pragma EXPANDER_EXPORT
}



/**
 * Return an EXECUTION_CONTEXT's current tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - server time
 */
datetime WINAPI ec_CurrentTickTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->currentTickTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's previous tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - server time
 */
datetime WINAPI ec_PreviousTickTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->previousTickTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten MQL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_MqlError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->mqlError);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten DLL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_DllError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->dllError);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherte DLL-Warnung zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_DllWarning(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->dllWarning);
   #pragma EXPANDER_EXPORT
}


/**
 * Set the MQL program index of an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               index - MQL program index (must be greater than zero)
 *
 * @return uint - the same index
 */
uint WINAPI ec_SetProgramIndex(EXECUTION_CONTEXT* ec, uint index) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (index <= 0)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (must be greater than zero)", index));

   ec->programIndex = index;                                         // synchronize main and master context
   if (g_contextChains.size() > index && ec==g_contextChains[index][1] && g_contextChains[index][0])
      return(ec_SetProgramIndex(g_contextChains[index][0], index));
   return(index);
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
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (type) {
      case PT_INDICATOR:
      case PT_EXPERT   :
      case PT_SCRIPT   : break;
      default:
         return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ProgramType)", type));
   }

   ec->programType = type;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetProgramType(g_contextChains[pid][0], type));
   return(type);
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
   if ((uint)ec   < MIN_VALID_POINTER)          return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)          return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(ec->programName)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name: \"%s\" (must be 1 to %d characters)", name, sizeof(ec->programName)-1));

   if (!strcpy(ec->programName, name))
      return(NULL);

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetProgramName(g_contextChains[pid][0], name));
   return(name);
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
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (type) {
      case MT_INDICATOR:
      case MT_EXPERT   :
      case MT_SCRIPT   :
      case MT_LIBRARY  : break;
      default:
         return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ModuleType)", type));
   }

   ec->moduleType = type;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetModuleType(g_contextChains[pid][0], type));
   return(type);
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
   if ((uint)ec   < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(ec->moduleName)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name: \"%s\" (must be 1 to %d characters)", name, sizeof(ec->moduleName)-1));

   if (!strcpy(ec->moduleName, name))
      return(NULL);

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetModuleName(g_contextChains[pid][0], name));
   return(name);
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
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (type) {
      case LT_TEMPLATE:
      case LT_PROGRAM :
      case LT_MANUAL  : break;
      default:
         return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a LaunchType)", type));
   }

   ec->launchType = type;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetLaunchType(g_contextChains[pid][0], type));
   return(type);
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
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (id) {
      case RF_INIT  :
      case RF_START :
      case RF_DEINIT:
      case NULL     :
         break;

      default:
         return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not a RootFunction)", id));
   }

   ec->rootFunction = id;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetRootFunction(g_contextChains[pid][0], id));
   return(id);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->initCycle = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetInitCycle(g_contextChains[pid][0], status));
   return(status);
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
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

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
         return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an InitializeReason)", reason));
   }

   ec->initReason = reason;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetInitReason(g_contextChains[pid][0], reason));
   return(reason);
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
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

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
         return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an UninitializeReason)", reason));
   }

   ec->uninitReason = reason;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetUninitReason(g_contextChains[pid][0], reason));
   return(reason);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->testing = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetTesting(g_contextChains[pid][0], status));
   return(status);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->visualMode = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetVisualMode(g_contextChains[pid][0], status));
   return(status);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->optimization = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetOptimization(g_contextChains[pid][0], status));
   return(status);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->initFlags = flags;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetInitFlags(g_contextChains[pid][0], flags));
   return(flags);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->deinitFlags = flags;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetDeinitFlags(g_contextChains[pid][0], flags));
   return(flags);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->logging = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetLogging(g_contextChains[pid][0], status));
   return(status);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec < MIN_VALID_POINTER)                      return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   if (fileName) {
      // fileName ist kein NULL-Pointer
      if ((uint)fileName < MIN_VALID_POINTER)             return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
      if (strlen(fileName) > sizeof(ec->customLogFile)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter fileName: \"%s\" (max %d characters)", fileName, sizeof(ec->customLogFile)-1));

      if (!strcpy(ec->customLogFile, fileName))
         return(NULL);
   }
   else {
      // fileName ist NULL-Pointer
      ec->customLogFile[0] = '\0';
   }

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetCustomLogFile(g_contextChains[pid][0], fileName));
   return(fileName);
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
   if ((uint)ec     < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   int len = strlen(symbol);
   if (!len || len > sizeof(ec->symbol)) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol: \"%s\" (must be 1 to %d characters)", symbol, sizeof(ec->symbol)-1));

   if (!strcpy(ec->symbol, symbol))
      return(NULL);

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetSymbol(g_contextChains[pid][0], symbol));
   return(symbol);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)timeframe <= 0)          return(error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d (must be greater than zero)", timeframe));

   ec->timeframe = timeframe;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetTimeframe(g_contextChains[pid][0], timeframe));
   return(timeframe);
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
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->hChart = hWnd;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetHChart(g_contextChains[pid][0], hWnd));
   return(hWnd);
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
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->hChartWindow = hWnd;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetHChartWindow(g_contextChains[pid][0], hWnd));
   return(hWnd);
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
   if (       (uint)ec  < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec));

   ec->superContext = sec;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetSuperContext(g_contextChains[pid][0], sec));
   return(sec);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (id <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (must be greater than zero)", id));

   ec->threadId = id;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetThreadId(g_contextChains[pid][0], id));
   return(id);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (count < 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must be non-negative)", count));

   ec->ticks = count;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetTicks(g_contextChains[pid][0], count));
   return(count);
}


/**
 * Set an EXECUTION_CONTEXT's current tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  datetime           time - server time
 *
 * @return datetime - the same time
 */
datetime WINAPI ec_SetCurrentTickTime(EXECUTION_CONTEXT* ec, datetime time) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (time < 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));

   ec->currentTickTime = time;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetCurrentTickTime(g_contextChains[pid][0], time));
   return(time);
}


/**
 * Set an EXECUTION_CONTEXT's previous tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  datetime           time - server time
 *
 * @return datetime - the same time
 */
datetime WINAPI ec_SetPreviousTickTime(EXECUTION_CONTEXT* ec, datetime time) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (time < 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));

   ec->previousTickTime = time;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetPreviousTickTime(g_contextChains[pid][0], time));
   return(time);
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
 * @return int - same error or EMPTY (-1) in case of errors
 */
int WINAPI ec_SetMqlError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->mqlError = error;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      ec_SetMqlError(g_contextChains[pid][0], error);

   if (error) {                                                      // no propagation for NO_ERROR
      if (ec->moduleType==MT_LIBRARY && pid) {                       // propagation from library to main module
         EXECUTION_CONTEXT* master = g_contextChains[pid][0];
         EXECUTION_CONTEXT* main   = g_contextChains[pid][1];
         if (main) ec_SetMqlError(main,   error);                    // whichever is available
         else      ec_SetMqlError(master, error);
      }

      if (ec->superContext)
         ec_SetMqlError(ec->superContext, error);                    // propagation to parent program
   }
   return(error);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->dllError = error;

   uint pid = ec->programIndex;
   if (pid && g_contextChains.size() > pid) {
      EXECUTION_CONTEXT* master = g_contextChains[pid][0];
      EXECUTION_CONTEXT* main   = g_contextChains[pid][1];

      if (ec==main && master)                                        // synchronize main and master context
         ec_SetDllError(master, error);

      if (error) {                                                   // no propagation for NO_ERROR
         if (ec->moduleType==MT_LIBRARY) {
            if (main) ec_SetDllError(main,   error);                 // Fehler aus Libraries in den Main-Context propagieren
            else      ec_SetDllError(master, error);                 // oder den Master-Context, falls Main-Context nicht verfügbar
         }
      }
   }
   return(error);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->dllWarning = error;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      ec_SetDllWarning(g_contextChains[pid][0], error);

   if (!error)                                                       // keine Propagation beim Zurücksetzen eines Fehlers
      return(error);

   if (pid && ec->moduleType==MT_LIBRARY) {                          // Warnung aus Libraries in den Hauptkontext propagieren
      EXECUTION_CONTEXT* master = g_contextChains[pid][0];           // (oder den Master-Context, wenn Hauptkontext nicht verfügbar)
      EXECUTION_CONTEXT* main   = g_contextChains[pid][1];
      if (main) ec_SetDllWarning(main,   error);
      else      ec_SetDllWarning(master, error);
   }
   return(error);
}


/**
 * Return the RootFunction of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 */
RootFunction WINAPI mec_RootFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint programIndex = ec->programIndex;
   if (!programIndex) return((RootFunction)error(ERR_ILLEGAL_STATE, "illegal programIndex %d in ec: %s", programIndex, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = g_contextChains[programIndex][0];
   return(master->rootFunction);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the UninitializeReason of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI mec_UninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint programIndex = ec->programIndex;
   if (!programIndex) return((UninitializeReason)error(ERR_ILLEGAL_STATE, "illegal programIndex %d in ec: %s", programIndex, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = g_contextChains[programIndex][0];
   return(master->uninitReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the init configuration flags of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - flags
 */
DWORD WINAPI mec_InitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint programIndex = ec->programIndex;
   if (!programIndex) return(error(ERR_ILLEGAL_STATE, "illegal programIndex %d in ec: %s", programIndex, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = g_contextChains[programIndex][0];
   return(master->initFlags);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   char* result = "{(empty)}";
   static const EXECUTION_CONTEXT empty = {};

   if (memcmp(ec, &empty, sizeof(EXECUTION_CONTEXT))) {
      std::stringstream ss; ss
         <<  "{programIndex="     <<                   ec->programIndex
         << ", programType="      <<  ProgramTypeToStr(ec->programType  )
         << ", programName="      <<    doubleQuoteStr(ec->programName  )
         << ", moduleType="       <<   ModuleTypeToStr(ec->moduleType   )
         << ", moduleName="       <<    doubleQuoteStr(ec->moduleName   )
         << ", launchType="       <<                   ec->launchType
         << ", rootFunction="     << RootFunctionToStr(ec->rootFunction )
         << ", initCycle="        <<         BoolToStr(ec->initCycle    )
         << ", initReason="       <<   InitReasonToStr(ec->initReason   )
         << ", uninitReason="     << UninitReasonToStr(ec->uninitReason )
         << ", testing="          <<         BoolToStr(ec->testing      )
         << ", visualMode="       <<         BoolToStr(ec->visualMode   )
         << ", optimization="     <<         BoolToStr(ec->optimization )
         << ", initFlags="        <<    InitFlagsToStr(ec->initFlags    )
         << ", deinitFlags="      <<  DeinitFlagsToStr(ec->deinitFlags  )
         << ", logging="          <<         BoolToStr(ec->logging      )
         << ", customLogFile="    <<    doubleQuoteStr(ec->customLogFile)
         << ", symbol="           <<    doubleQuoteStr(ec->symbol       )
         << ", timeframe="        <<       PeriodToStr(ec->timeframe    )
         << ", hChart="           <<             (uint)ec->hChart
         << ", hChartWindow="     <<             (uint)ec->hChartWindow
         << ", superContext="     <<             (uint)ec->superContext
         << ", threadId="         <<                   ec->threadId
         << ", ticks="            <<                   ec->ticks
         << ", currentTickTime="  <<                  (ec->currentTickTime  ? doubleQuoteStr(gmTimeFormat(ec->currentTickTime,  "%Y.%m.%d %H:%M:%S")) : "0")
         << ", previousTickTime=" <<                  (ec->previousTickTime ? doubleQuoteStr(gmTimeFormat(ec->previousTickTime, "%Y.%m.%d %H:%M:%S")) : "0")
         << ", mqlError="         <<                 (!ec->mqlError   ? "0" : ErrorToStr(ec->mqlError  ))
         << ", dllError="         <<                 (!ec->dllError   ? "0" : ErrorToStr(ec->dllError  ))
         << ", dllWarning="       <<                 (!ec->dllWarning ? "0" : ErrorToStr(ec->dllWarning))
         << "}";
      result = copychars(ss.str());                                  // TODO: close memory leak
   }

   if (outputDebug) debug(result);
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Alias (different MQL import)
 */
const char* WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug/*=FALSE*/) {
   return(EXECUTION_CONTEXT_toStr(ec, outputDebug));
   #pragma EXPANDER_EXPORT
}
