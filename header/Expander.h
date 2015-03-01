#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "mql4/include/stderror.h"


#ifdef EXPANDER_EXPORTS
 //#define EXPANDER_API extern "C" __declspec(dllexport)
   #define EXPANDER_API                                                             // empty define
   #define EXPORT       comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)     // export unmangled names w/o DEF file
#else
   #define EXPANDER_API extern "C" __declspec(dllimport)
#endif


#define debug(...)   _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)


/**
 * Schickt einen String an den System-Debugger.  Sollte nur über das Makro debug(...) aufgerufen werden.
 *
 * @param  char* fileName  - Name der Datei, in der der Aufruf erfolgt
 * @param  char* funcName  - Name der Funktion, in der der Aufruf erfolgt
 * @param  int   line      - Zeile, in der der Aufruf erfolgt
 * @param  char* msgFormat - Formatstring mit Platzhaltern für alle weiteren Parameter
 * @param  ...             - beliebige weitere Parameter
 *
 * @return void
 */
void _debug(char* fileName, char* funcName, int line, char* msgFormat, ...) {
   if (!fileName) fileName = __FILE__;                                           // Falls doch jemand die Funktion direkt
   if (!funcName) funcName = __FUNCTION__;                                       // und falsch aufruft.
   if (!msgFormat) return;

   // (1) zuerst alle explizit angegebenen Argumente in einen String transformieren (ab msgFormat)
   va_list args;
   va_start(args, msgFormat);
   int size = _vscprintf(msgFormat, args) + 1;                                   // +1 für das terminierende '\0'
   char* msg = (char*) alloca(size);                                             // auf dem Stack
   vsprintf_s(msg, size, msgFormat, args);
   va_end(args);

   // Parameter fileName zerlegen: nur der einfache Dateiname {basename.ext} wird benötigt
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // (2) dann die impliziten Location-Infos vorn anfügen
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;     // +1 für das terminierende '\0'
   char* buffer = (char*) alloca(size);                                          // auf dem Stack
   sprintf_s(buffer, size, locationFormat, baseName, ext, funcName, line, msg);

   OutputDebugString(buffer);
}


#pragma pack(1)


// Special constants
#define MIN_VALID_POINTER  0x00010000           // kleinster möglicher Wert für einen gültigen Pointer (x86)


// Timeframe-Identifier
#define PERIOD_M1                   1           // 1 Minute
#define PERIOD_M5                   5           // 5 Minuten
#define PERIOD_M15                 15           // 15 Minuten
#define PERIOD_M30                 30           // 30 Minuten
#define PERIOD_H1                  60           // 1 Stunde
#define PERIOD_H4                 240           // 4 Stunden
#define PERIOD_D1                1440           // 1 Tag
#define PERIOD_W1               10080           // 1 Woche (7 Tage)
#define PERIOD_MN1              43200           // 1 Monat (30 Tage)
#define PERIOD_Q1              129600           // 1 Quartal (3 Monate)


// MQL-Programmtypen: Indicator, Expert oder Script (Library-Module sind keine eigenständigen Programme)
enum ProgramType {
   PT_INDICATOR = 1,
   PT_EXPERT    = 2,
   PT_SCRIPT    = 4
};


// Launchtypen eines MQL-Programms: per Template, per iCustom() oder von Hand
enum LaunchType {
   LT_TEMPLATE = 1,     // von Template geladen
   LT_PROGRAM,          // von iCustom() geladen
   LT_MANUAL            // von Hand geladen
};


// MQL-Rootfunktionen: init(), start() oder deinit()
enum RootFunction {
   RF_INIT = 1,
   RF_START,
   RF_DEINIT
};


// MT4-interne Darstellung eines MQL-Strings
struct MqlStr {
   int   length;                 // 0, wenn der String ein C-Literal ist;
   char* string;                 // andererseits die Größe des verfügbaren Speicherblocks
};


// MT4-interne Darstellung einer Preis-Bar
struct RateInfo {
   unsigned int time;            // BarOpen-Time
   double       open;
   double       low;
   double       high;
   double       close;
   double       volume;          // immer Ganzzahl
};


// in der DLL aufgetretener Fehler, wird an das aufrufende MQL-Programm weitergeleitet
struct DLL_ERROR {
   int   code;
   char* message;
};


// Ausführungskontext: Datenaustausch zwischen mehreren MQL-Modulen eines Programms und der DLL
struct EXECUTION_CONTEXT {
   int                id;
   ProgramType        programType;
   LPSTR              programName;
   int                initFlags;
   int                deinitFlags;
   EXECUTION_CONTEXT* superContext;

   LaunchType         launchType;
   int                uninitializeReason;
   RootFunction       whereami;

   LPSTR              symbol;
   int                timeframe;
   HWND               hWndChart;             // Win32-Chartfenster (mit Titelzeile)
   HWND               hWndChartFrame;        // MQL-Chartfenster (Child von hWndChart = AfxFrame)
   HANDLE             hThreadId;
   int                testFlags;

   int                logLevel;
   LPSTR              logFile;
   int                errorsSize;
   DLL_ERROR*         errors;                // Array von DLL-Fehlern
};
