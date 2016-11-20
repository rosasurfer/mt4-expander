/**
 * MyFX struct EXECUTION_CONTEXT
 *
 * Ausführungskontext von MQL-Programmen zur Kommunikation zwischen MQL und DLL
 */
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
   if ((uint)ec  < MIN_VALID_POINTER) return(_FALSE(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   if ((uint)sec < MIN_VALID_POINTER) return(_FALSE(debug("ERROR:  invalid parameter sec = 0x%p (not a valid pointer)", sec)));

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
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten MQL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_MqlError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->mqlError);
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
uint WINAPI ec_SetProgramId(EXECUTION_CONTEXT* ec, uint id) {
   if ((uint)ec < MIN_VALID_POINTER) return(_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   if (id <= 0)                      return(_NULL(debug("ERROR:  invalid parameter id = %d (must be greater than zero)", id)));

   return(ec->programId = id);
   #pragma EXPORT
}


/**
 * Setzt die Programm-ID aller Elemente eines EXECUTION_CONTEXT*-Vectors
 *
 * @param pec_vector chain - Vector, typischerweise die Context-Chain eines MQL-Programms
 * @param uint       id    - zu setzende Programm-ID
 *
 * @return uint - dieselbe ID
 */
uint WINAPI ecc_SetProgramId(const pec_vector &chain, uint id) {
   int size = chain.size();
   for (int i=0; i < size; i++) {
      ec_SetProgramId(chain[i], id);
   }
   return(id);
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
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   switch (type) {
      case PT_INDICATOR:
      case PT_EXPERT   :
      case PT_SCRIPT   : break;
      default:
         return((ProgramType)_NULL(debug("ERROR:  invalid parameter type = %d (not a ProgramType)", type)));
   }

   return(ec->programType = type);
   #pragma EXPORT
}


/**
 * Setzt den Programmnamen eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - der gesetzte Name
 */
const char* WINAPI ec_SetProgramName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)          return((char*)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   if ((uint)name < MIN_VALID_POINTER)          return((char*)_NULL(debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name)));
   int len = strlen(name);
   if (!len || len > sizeof(ec->programName)-1) return((char*)_NULL(debug("ERROR:  illegal length of parameter name = \"%s\" (must be 1 to %d characters)", name, sizeof(ec->programName)-1)));

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
ModuleType WINAPI ec_SetModuleType(EXECUTION_CONTEXT* ec, ModuleType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   switch (type) {
      case MT_INDICATOR:
      case MT_EXPERT   :
      case MT_SCRIPT   :
      case MT_LIBRARY  : break;
      default:
         return((ModuleType)_NULL(debug("ERROR:  invalid parameter type = %d (not a ModuleType)", type)));
   }

   return(ec->moduleType = type);
   #pragma EXPORT
}


/**
 * Setzt den Modulnamen eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - der gesetzte Name
 */
const char* WINAPI ec_SetModuleName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)         return((char*)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   if ((uint)name < MIN_VALID_POINTER)         return((char*)_NULL(debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name)));
   int len = strlen(name);
   if (!len || len > sizeof(ec->moduleName)-1) return((char*)_NULL(debug("ERROR:  illegal length of parameter name = \"%s\" (must be 1 to %d characters)", name, sizeof(ec->moduleName)-1)));

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
LaunchType WINAPI ec_SetLaunchType(EXECUTION_CONTEXT* ec, LaunchType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   switch (type) {
      case LT_TEMPLATE:
      case LT_PROGRAM:
      case LT_MANUAL: break;
      default:
         return((LaunchType)_NULL(debug("ERROR:  invalid parameter type = %d (not a LaunchType)", type)));
   }

   return(ec->launchType = type);
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
   if (       (uint)ec  < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)_NULL(debug("ERROR:  invalid parameter sec = 0x%p (not a valid pointer)", sec)));

   return(ec->superContext = sec);
   #pragma EXPORT
}


/**
 * Setzt den SuperContext eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec    - zu modifizierender Context
 * @param  EXECUTION_CONTEXT* lpSec - zu setzender SuperContext
 *
 * @return EXECUTION_CONTEXT* - der gesetzte SuperContext
 */
EXECUTION_CONTEXT* WINAPI ec_SetLpSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* lpSec) {
   return(ec_SetSuperContext(ec, lpSec));
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
uint WINAPI ec_SetInitFlags(EXECUTION_CONTEXT* ec, uint flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   return(ec->initFlags = flags);
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
uint WINAPI ec_SetDeinitFlags(EXECUTION_CONTEXT* ec, uint flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   return(ec->deinitFlags = flags);
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
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   switch (id) {
      case RF_INIT  :
      case RF_START :
      case RF_DEINIT: break;
      default:
         return((RootFunction)_NULL(debug("ERROR:  invalid parameter id = %d (not a RootFunction)", id)));
   }

   return(ec->rootFunction = id);
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
UninitializeReason WINAPI ec_SetUninitializeReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

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
         return((UninitializeReason)_NULL(debug("ERROR:  invalid parameter reason = %d (not an UninitializeReason)", reason)));
   }

   return(ec->uninitializeReason = reason);
   #pragma EXPORT
}


/**
 * Setzt das Symbol eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              symbol
 *
 * @return char* - das gesetzte Symbol
 */
const char* WINAPI ec_SetSymbol(EXECUTION_CONTEXT* ec, const char* symbol) {
   if ((uint)ec     < MIN_VALID_POINTER) return((char*)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)_NULL(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol)));
   int len = strlen(symbol);
   if (!len || len > sizeof(ec->symbol)) return((char*)_NULL(debug("ERROR:  illegal length of parameter symbol = \"%s\" (must be 1 to %d characters)", symbol, sizeof(ec->symbol)-1)));

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
uint WINAPI ec_SetTimeframe(EXECUTION_CONTEXT* ec, uint timeframe) {
   if ((uint)ec < MIN_VALID_POINTER) return(_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   if (timeframe <= 0)               return(_NULL(debug("ERROR:  invalid parameter timeframe = %d (must be greater than zero)", timeframe)));

   return(ec->timeframe = timeframe);
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
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   return(ec->hChartWindow = hWnd);
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
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   return(ec->hChart = hWnd);
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
uint WINAPI ec_SetTestFlags(EXECUTION_CONTEXT* ec, uint flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   return(ec->testFlags = flags);
   #pragma EXPORT
}


/**
 * Setzt den MQL-Fehler eines EXECUTION_CONTEXT. Diese Funktion wird nur von MQL::SetLastError() aufgerufen.
 *
 * • Zusätzlich wird der MQL-Fehler des jeweiligen Hauptkontexts gesetzt (Fehlerpropagation zum aufrufenden Hauptmodul).
 *   Ist kein Hauptkontext verfügbar, z.B. während des init()-Cycles von Libraries, wird der MQL-Fehler des Master-Kontexts
 *   gesetzt.
 *
 * • Hat der Kontext einen SuperContext, wird auch dessen MQL-Fehler gesetzt (Fehlerpropagation zum aufrufenden Programm).
 *
 * • Fehlerpropagation findet nur beim Setzen eines Fehlers, nicht jedoch beim Zurücksetzen eines gesetzten Fehlers statt.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - derselbe Fehler-Code
 */
int WINAPI ec_SetMqlError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   ec->mqlError = error;

   if (!error)
      return(error);                                                       // keine Fehlerpropagation beim Zurücksetzen

   if (ec->programId) {
      pec_vector &chain = contextChains[ec->programId];
      if (chain[1]) (chain[1]!=ec) && ec_SetMqlError(chain[1], error);     // Fehler im Hauptkontext...
      else          (chain[0]!=ec) && ec_SetMqlError(chain[0], error);     // ...oder im Master-Kontext setzen
   }

   if (ec->superContext) {
      ec_SetMqlError(ec->superContext, error);                             // Fehler im SuperContext setzen
   }

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
BOOL WINAPI ec_SetLogging(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(_FALSE(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   return(ec->logging = status);
   #pragma EXPORT
}


/**
 * Setzt den Namen der Logdatei eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              fileName - statt eines NULL-Pointers kann auch ein Leerstring angegeben werden
 *
 * @return char* - der gesetzte Dateiname
 */
const char* WINAPI ec_SetLogFile(EXECUTION_CONTEXT* ec, const char* fileName) {
   if ((uint)ec < MIN_VALID_POINTER)                return((char*)_NULL(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   if (fileName) {
      // fileName ist kein NULL-Pointer
      if ((uint)fileName < MIN_VALID_POINTER)       return((char*)_NULL(debug("ERROR:  invalid parameter fileName = 0x%p (not a valid pointer)", fileName)));
      if (strlen(fileName) > sizeof(ec->logFile)-1) return((char*)_NULL(debug("ERROR:  illegal length of parameter fileName = \"%s\" (max %d characters)", fileName, sizeof(ec->logFile)-1)));
      return(strcpy(ec->logFile, fileName));
   }

   // fileName ist NULL-Pointer
   ec->logFile[0] = '\0';
   return(ec->logFile);
   #pragma EXPORT
}
