/**
 * Ausführungskontext eines MQL-Programms für Laufzeitinformationen und Datenaustausch zwischen MQL-Modulen und DLL
 *
 * Im Indikator gibt es während eines init()-Cycles in der Zeitspanne vom Verlassen von Indicator::deinit() bis zum Wiedereintritt in
 * Indicator::init() keinen gültigen Hauptmodulkontext. Der alte Speicherblock wird sofort freigegeben, später wird ein neuer alloziiert.
 * Während dieser Zeitspanne wird der init()-Cycle von bereits geladenen Libraries durchgeführt, also die Funktionen Library::deinit()
 * und Library::init() aufgerufen. In Indikatoren geladene Libraries dürfen also während ihres init()-Cycles nicht auf den alten, bereits
 * ungültigen Hauptmodulkontext zugreifen (weder lesend noch schreibend).
 *
 *
 * MQL-Importdeklarationen:
 * ------------------------
 * int    ec_ProgramId            (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_ProgramType          (/*EXECUTION_CONTEXT/int ec[]);
 * string ec_ProgramName          (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_LaunchType           (/*EXECUTION_CONTEXT/int ec[]);
 * bool   ec_SuperContext         (/*EXECUTION_CONTEXT/int ec[], /*EXECUTION_CONTEXT/int sec[]);
 * int    ec_lpSuperContext       (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_InitFlags            (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_DeinitFlags          (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_RootFunction         (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_UninitializeReason   (/*EXECUTION_CONTEXT/int ec[]);
 * string ec_Symbol               (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_Timeframe            (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_hChartWindow         (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_hChart               (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_TestFlags            (/*EXECUTION_CONTEXT/int ec[]);
 * int    ec_LastError            (/*EXECUTION_CONTEXT/int ec[]);
 *        ec_DllErrors            ...
 *        ec_DllErrorsSize        ...
 * bool   ec_Logging              (/*EXECUTION_CONTEXT/int ec[]);
 * string ec_LogFile              (/*EXECUTION_CONTEXT/int ec[]);
 *
 * int    ec_setRootFunction      (/*EXECUTION_CONTEXT/int ec[], int function);
 * int    ec_setUninitializeReason(/*EXECUTION_CONTEXT/int ec[], int reason  );
 * int    ec_setLastError         (/*EXECUTION_CONTEXT/int ec[], int error   );
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt die Programm-ID eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Programm-ID
 */
uint WINAPI ec_ProgramId(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programId);
   #pragma EXPORT
}


/**
 * Gibt den Programm-Typ eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ProgramType
 */
ProgramType WINAPI ec_ProgramType(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programType);
   #pragma EXPORT
}


/**
 * Gibt den Programmnamen eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Programmname
 */
char* WINAPI ec_ProgramName(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programName);
   #pragma EXPORT
}


/**
 * Gibt den Launch-Typ eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return LaunchType
 */
LaunchType WINAPI ec_LaunchType(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->launchType);
   #pragma EXPORT
}


/**
 * Kopiert den SuperContext eines EXECUTION_CONTEXTs in die übergebene Variable.
 *
 * @param  EXECUTION_CONTEXT* ec  - ExecutionContext
 * @param  EXECUTION_CONTEXT* sec - Variable zur Aufnahme des SuperContexts
 *
 * @return BOOL - TRUE,  wenn ein SuperContext kopiert wurde;
 *                FALSE, wenn der ExecutionContext keinen SuperContext enthielt oder ein Fehler auftrat
 */
BOOL WINAPI ec_SuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if ((uint)ec  < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)sec < MIN_VALID_POINTER) return(debug("invalid parameter sec = 0x%p (not a valid pointer)", sec));

   if (ec->superContext) {
      *sec = *ec->superContext;
      return(TRUE);
   }

   EXECUTION_CONTEXT empty = {0};
   *sec = empty;
   return(FALSE);
   #pragma EXPORT
}


/**
 * Gibt den Zeiger auf den SuperContext eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return EXECUTION_CONTEXT*
 */
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->superContext);
   #pragma EXPORT
}


/**
 * Gibt die Init-Flags eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Init-Flags
 */
uint WINAPI ec_InitFlags(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initFlags);
   #pragma EXPORT
}


/**
 * Gibt die Deinit-Flags eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Deinit-Flags
 */
uint WINAPI ec_DeinitFlags(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->deinitFlags);
   #pragma EXPORT
}


/**
 * Gibt die RootFunction eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 */
RootFunction WINAPI ec_RootFunction(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->rootFunction);
   #pragma EXPORT
}


/**
 * Gibt den UninitializeReason eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - UninitializeReason
 */
int WINAPI ec_UninitializeReason(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->uninitializeReason);
   #pragma EXPORT
}


/**
 * Gibt das Chartsymbol eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Symbol
 */
char* WINAPI ec_Symbol(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->symbol);
   #pragma EXPORT
}


/**
 * Gibt die Chartperiode eines EXECUTION_CONTEXTs zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Chartperiode
 */
uint WINAPI ec_Timeframe(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
HWND WINAPI ec_hChartWindow(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
HWND WINAPI ec_hChart(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
uint WINAPI ec_TestFlags(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
int WINAPI ec_LastError(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
BOOL WINAPI ec_Logging(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
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
char* WINAPI ec_LogFile(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->logFile);
   #pragma EXPORT
}


/**
 * Setzt die RootFunction-ID eines EXECUTION_CONTEXTs.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  RootFunction       id
 *
 * @return RootFunction - dieselbe RootFunction-ID
 */
RootFunction WINAPI ec_setRootFunction(EXECUTION_CONTEXT* ec, RootFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (id) {
      case RF_INIT  :
      case RF_START :
      case RF_DEINIT: break;

      default:
         return((RootFunction)debug("invalid parameter id = %d (not a RootFunction)", id));
   }

   ec->rootFunction = id;
   return(id);
   #pragma EXPORT
}


/**
 * Setzt den UninitializeReason eines EXECUTION_CONTEXTs.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                reason
 *
 * @return int - derselbe UninitializeReason
 */
int WINAPI ec_setUninitializeReason(EXECUTION_CONTEXT* ec, int reason) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));

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
         return(debug("invalid parameter reason = %d (not an UninitializeReason)", reason));
   }

   ec->uninitializeReason = reason;
   return(reason);
   #pragma EXPORT
}


/**
 * Setzt den letzten Fehler-Code eines EXECUTION_CONTEXTs.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - derselbe Fehler-Code
 */
int WINAPI ec_setLastError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->lastError = error;

   if (ec->superContext)
      ec_setLastError(ec->superContext, error);                          // Fehler rekursiv im SuperContext setzen

   return(error);
   #pragma EXPORT
}