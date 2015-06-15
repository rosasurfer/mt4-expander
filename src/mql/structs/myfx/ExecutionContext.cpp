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
 * @author  Peter Walther
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherte Programm-ID zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Programm-ID
 *
 *
 * @mql    int ec_ProgramId(int ec[]);
 */
uint WINAPI ec_ProgramId(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programId);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Programm-Typ zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ProgramType
 *
 *
 * @mql    int ec_ProgramType(int ec[]);
 */
ProgramType WINAPI ec_ProgramType(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programType);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Programmnamen zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Programmname
 *
 *
 * @mql    string ec_ProgramName(int ec[]);
 */
char* WINAPI ec_ProgramName(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programName);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Launch-Typ zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return LaunchType
 *
 *
 * @mql    int ec_LaunchType(int ec[]);
 */
LaunchType WINAPI ec_LaunchType(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->launchType);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Zeiger auf einen SuperContext zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return EXECUTION_CONTEXT* - Zeiger
 *
 *
 * @mql    int ec_lpSuperContext(int ec[]);
 */
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->superContext);
   #pragma EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherten Init-Flags zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Init-Flags
 *
 *
 * @mql    int ec_InitFlags(int ec[]);
 */
uint WINAPI ec_InitFlags(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initFlags);
   #pragma EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherten Deinit-Flags zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Deinit-Flags
 *
 *
 * @mql    int ec_DeinitFlags(int ec[]);
 */
uint WINAPI ec_DeinitFlags(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->deinitFlags);
   #pragma EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherte RootFunction zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 *
 *
 * @mql    int ec_RootFunction(int ec[]);
 */
RootFunction WINAPI ec_RootFunction(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->rootFunction);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten UninitializeReason zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - UninitializeReason
 *
 *
 * @mql    int ec_ProgramId(int ec[]);
 */
int WINAPI ec_UninitializeReason(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->uninitializeReason);
   #pragma EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Chartsymbol zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Symbol
 *
 *
 * @mql    string ec_Symbol(int ec[]);
 */
char* WINAPI ec_Symbol(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->symbol);
   #pragma EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherte Chartperiode zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Chartperiode
 *
 *
 * @mql    int ec_Timeframe(int ec[]);
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
 *
 *
 * @mql    int ec_hChartWindow(int ec[]);
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
 *
 *
 * @mql    int ec_hChart(int ec[]);
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
 *
 *
 * @mql    int ec_TestFlags(int ec[]);
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
 *
 *
 * @mql    int ec_LastError(int ec[]);
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
 *
 *
 * @mql    bool ec_Logging(int ec[]);
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
 *
 *
 * @mql    string ec_ProgramName(int ec[]);
 */
char* WINAPI ec_LogFile(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->logFile);
   #pragma EXPORT
}


/*
int    ec.SuperContext         (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if (ArrayDimension(sec) != 1)        return(catch("ec.SuperContext(1)  too many dimensions of parameter sec = "+ ArrayDimension(sec), ERR_INCOMPATIBLE_ARRAYS));
   if (ArraySize(sec) != EXECUTION_CONTEXT.intSize)
      ArrayResize(sec, EXECUTION_CONTEXT.intSize);
   int lpSuperContext = ec.lpSuperContext(ec);
   if (!lpSuperContext) ArrayInitialize(sec, 0);
   else                 CopyMemory(GetBufferAddress(sec), lpSuperContext, EXECUTION_CONTEXT.size);
   return(catch("ec.SuperContext(2)"));
}
*/
