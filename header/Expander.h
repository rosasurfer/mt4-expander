#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <string>
#define WIN32_LEAN_AND_MEAN             
#include <windows.h>


#ifdef EXPANDER_EXPORTS
//#define EXPANDER_API extern "C" __declspec(dllexport)
#define EXPANDER_API                                                             // empty
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
void _debug(char* fileName, char* funcName, int line, const char* msgFormat, ...) {
   // (1) zuerst alle explizit angegebenen Argumente in einen String transformieren (ab msgFormat)
   va_list args;
   va_start(args, msgFormat);
   int size = _vscprintf(msgFormat, args) + 1;                             // +1 für das terminierende '\0'
   char* msg = (char*) _alloca(size);                                      // auf dem Stack
   vsprintf_s(msg, size, msgFormat, args);
   va_end(args);
   

   // Parameter fileName zerlegen: nur der Dateiname {basename.ext} wird angezeigt, kein Pfad
   char baseName[_MAX_FNAME], ext[_MAX_EXT];                         
   _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);


   // (2) dann die impliziten Location-Infos vorn anfügen
   char* locFormat = "MetaTrader::%s%s::%s(%d)  %s";
   size = _scprintf(locFormat, baseName, ext, funcName, line, msg) + 1;    // +1 für das terminierende '\0'
   char* buffer = (char*) _alloca(size);                                   // auf dem Stack
   sprintf_s(buffer, size, locFormat, baseName, ext, funcName, line, msg);

   OutputDebugString(buffer);
}


#pragma pack(1)


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


// MT4-interne Darstellung eines MQL-Strings
struct MqlStr {
   int   length;
   char* string;
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