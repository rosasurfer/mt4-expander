/**
 * MQL-Interface zum Zugriff auf ein struct EXECUTION_CONTEXT.
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt die Programm-ID eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Programm-ID
 */
uint WINAPI ec_ProgramId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->launchType);
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
   if ((uint)ec  < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)sec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter sec = 0x%p (not a valid pointer)", sec));

   if (ec->superContext) {
      *sec = *ec->superContext;
      return(TRUE);
   }

   EXECUTION_CONTEXT zeroed = {};
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
   if ((uint)ec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->superContext);
   #pragma EXPORT
}


/**
 * Gibt die Init-Flags eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Init-Flags
 */
uint WINAPI ec_InitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initFlags);
   #pragma EXPORT
}


/**
 * Gibt die Deinit-Flags eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Deinit-Flags
 */
uint WINAPI ec_DeinitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->deinitFlags);
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
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->rootFunction);
   #pragma EXPORT
}


/**
 * Gibt den UninitializeReason eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI ec_UninitializeReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) {
      debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec);
      return(UNINITREASON_UNDEFINED);
   }
   return(ec->uninitializeReason);
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
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->timeframe);
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
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->hChartWindow);
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
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->hChart);
   #pragma EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherten Test-Flags zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Test-Flags
 */
uint WINAPI ec_TestFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->testFlags);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten letzten Fehler-Code zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_LastError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->lastError);
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
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
const char* WINAPI ec_LogFile(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->logFile);
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
uint WINAPI ec_setProgramId(EXECUTION_CONTEXT* ec, uint id) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (id <= 0)                      return(debug("ERROR:  invalid parameter id = %d (must be greater than zero)", id));

   ec->programId = id;
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
ProgramType WINAPI ec_setProgramType(EXECUTION_CONTEXT* ec, ProgramType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (type) {
      case PT_INDICATOR:
      case PT_EXPERT   :
      case PT_SCRIPT   : break;
      default:
         return((ProgramType)debug("ERROR:  invalid parameter type = %d (not a ProgramType)", type));
   }
   ec->programType = type;
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
const char* WINAPI ec_setProgramName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)                            return((char*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)                            return((char*)debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name));
   if (!strlen(name) || strlen(name) > sizeof(ec->programName)-1) return((char*)debug("ERROR:  invalid parameter name = \"%s\" (1 to %d characters)", name, sizeof(ec->programName)-1));

   return(strcpy(ec->programName, name));
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
ModuleType WINAPI ec_setModuleType(EXECUTION_CONTEXT* ec, ModuleType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (type) {
      case MT_INDICATOR:
      case MT_EXPERT   :
      case MT_SCRIPT   :
      case MT_LIBRARY  : break;
      default:
         return((ModuleType)debug("ERROR:  invalid parameter type = %d (not a ModuleType)", type));
   }
   ec->moduleType = type;
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
const char* WINAPI ec_setModuleName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)                           return((char*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)                           return((char*)debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name));
   if (!strlen(name) || strlen(name) > sizeof(ec->moduleName)-1) return((char*)debug("ERROR:  invalid parameter name = \"%s\" (1 - %d characters)", name, sizeof(ec->moduleName)-1));

   return(strcpy(ec->moduleName, name));
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
LaunchType WINAPI ec_setLaunchType(EXECUTION_CONTEXT* ec, LaunchType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (type) {
      case LT_TEMPLATE:
      case LT_PROGRAM:
      case LT_MANUAL: break;
      default:
         return((LaunchType)debug("ERROR:  invalid parameter type = %d (not a LaunchType)", type));
   }
   ec->launchType = type;
   return(type);
   #pragma EXPORT
}


/**
 * Setzt den SuperContext eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec  - zu modifizierender Context
 * @param  EXECUTION_CONTEXT* sec - zu setzender SuperContext
 *
 * @return EXECUTION_CONTEXT* - derselbe SuperContext
 */
const EXECUTION_CONTEXT* WINAPI ec_setSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if (       (uint)ec  < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)debug("ERROR:  invalid parameter sec = 0x%p (not a valid pointer)", sec));

   ec->superContext = sec;
   return(sec);
   #pragma EXPORT
}


/**
 * Setzt den SuperContext eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec    - zu modifizierender Context
 * @param  EXECUTION_CONTEXT* lpSec - zu setzender SuperContext
 *
 * @return EXECUTION_CONTEXT* - derselbe SuperContext
 */
const EXECUTION_CONTEXT* WINAPI ec_setLpSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* lpSec) {
   return(ec_setSuperContext(ec, lpSec));
   #pragma EXPORT
}


/**
 * Setzt die Init-Flags eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               flags
 *
 * @return uint - dieselben Init-Flags
 */
uint WINAPI ec_setInitFlags(EXECUTION_CONTEXT* ec, uint flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   ec->initFlags = flags;
   return(flags);
   #pragma EXPORT
}


/**
 * Setzt die Deinit-Flags eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               flags
 *
 * @return uint - dieselben Deinit-Flags
 */
uint WINAPI ec_setDeinitFlags(EXECUTION_CONTEXT* ec, uint flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   ec->deinitFlags = flags;
   return(flags);
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
RootFunction WINAPI ec_setRootFunction(EXECUTION_CONTEXT* ec, RootFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (id) {
      case RF_INIT  :
      case RF_START :
      case RF_DEINIT: break;
      default:
         return((RootFunction)debug("ERROR:  invalid parameter id = %d (not a RootFunction)", id));
   }
   ec->rootFunction = id;
   return(id);
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
UninitializeReason WINAPI ec_setUninitializeReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) {
      debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec);
      return(UNINITREASON_UNDEFINED);
   }

   switch (reason) {
      case REASON_UNDEFINED  :
      case REASON_REMOVE     :
      case REASON_RECOMPILE  :
      case REASON_CHARTCHANGE:
      case REASON_CHARTCLOSE :
      case REASON_PARAMETERS :
      case REASON_ACCOUNT    :
      // builds > 509
      case REASON_TEMPLATE   :
      case REASON_INITFAILED :
      case REASON_CLOSE      : break;
      default:
         debug("ERROR:  invalid parameter reason = %d (not an UninitializeReason)", reason);
         return(UNINITREASON_UNDEFINED);
   }
   ec->uninitializeReason = reason;

   return(reason);
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
const char* WINAPI ec_setSymbol(EXECUTION_CONTEXT* ec, const char* symbol) {
   if ((uint)ec     < MIN_VALID_POINTER)                       return((char*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)symbol < MIN_VALID_POINTER)                       return((char*)debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (!strlen(symbol) || strlen(symbol) > sizeof(ec->symbol)) return((char*)debug("ERROR:  invalid parameter symbol = \"%s\" (1 - %d characters)", symbol, sizeof(ec->symbol)));

   return(strcpy(ec->symbol, symbol));
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
uint WINAPI ec_setTimeframe(EXECUTION_CONTEXT* ec, uint timeframe) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (timeframe <= 0)               return(debug("ERROR:  invalid parameter timeframe = %d (must be greater than zero)", timeframe));

   ec->timeframe = timeframe;
   return(timeframe);
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
HWND WINAPI ec_setHChartWindow(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   ec->hChartWindow = hWnd;
   return(hWnd);
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
HWND WINAPI ec_setHChart(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   ec->hChart = hWnd;
   return(hWnd);
   #pragma EXPORT
}


/**
 * Setzt die Test-Flags eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               flags
 *
 * @return uint - dieselben Test-Flags
 */
uint WINAPI ec_setTestFlags(EXECUTION_CONTEXT* ec, uint flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   ec->testFlags = flags;
   return(flags);
   #pragma EXPORT
}


/**
 * Setzt den letzten MQL-Fehler eines EXECUTION_CONTEXT.
 *
 * • Zusätzlich wird der letzte MQL-Fehler des jeweiligen Hauptkontexts gesetzt (Fehlerpropagation von Libraries zum aufrufenden
 *   Hauptmodul). Ist kein Hauptkontext verfügbar, z.B. während des init()-Cycles von Libraries, wird der letzte MQL-Fehler des
 *   Master-Kontexts gesetzt.
 *
 * • Hat der Kontext einen SuperContext, wird auch dessen letzter MQL-Fehler gesetzt (Fehlerpropagation zum aufrufenden Programm).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - derselbe Fehler-Code
 */
int WINAPI ec_setLastError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->lastError = error;

   if (ec->programId) {
      pec_vector &chain = contextChains[ec->programId];
      if (chain[1]) chain[1]->lastError = error;            // Fehler des Hauptkontexts oder
      else          chain[0]->lastError = error;            // Fehler des Master-Kontexts setzen
   }

   if (ec->superContext)
      ec->superContext->error = error;                      // Fehler im SuperContext setzen

   return(error);
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
BOOL WINAPI ec_setLogging(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->logging = status;
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
const char* WINAPI ec_setLogFile(EXECUTION_CONTEXT* ec, const char* fileName) {
   if ((uint)ec < MIN_VALID_POINTER)                return((char*)debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));

   if (fileName) {
      if ((uint)fileName < MIN_VALID_POINTER)       return((char*)debug("ERROR:  invalid parameter fileName = 0x%p (not a valid pointer)", fileName));
      if (strlen(fileName) > sizeof(ec->logFile)-1) return((char*)debug("ERROR:  invalid parameter fileName = \"%s\" (1 - %d characters)", fileName, sizeof(ec->logFile)-1));
      strcpy(ec->logFile, fileName);
   }
   else {
      ec->logFile[0] = '\0';
   }
   return(ec->logFile);
   #pragma EXPORT
}
