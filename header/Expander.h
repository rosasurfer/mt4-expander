#pragma once

#include <stdlib.h>
#include <iostream>


#define WIN32_LEAN_AND_MEAN             
#include <windows.h>

#define EXPORT_FUNCTION comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)


#pragma pack(1)


// Timeframe-Identifier, siehe Period()
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


/**
 * Schickt einen String an den System-Debugger.
 *
 * @param  char* fileName - Name der Datei, in der der Aufruf erfolgt
 * @param  char* funcName - Name der Funktion, in der der Aufruf erfolgt
 * @param  int   line     - Zeile, in der der Aufruf erfolgt
 * @param  char* msg      - Message
 *
 * @return void
 * 
 * 
 * NOTE: Statt dieser Funktion sollte immer das Makro debug(s) benutzt werden, das die ersten 
 *       drei Parameter automatisch ermittelt und setzt.
 */
void _debug(char* fileName, char* funcName, int line, char* msg) {
   // TODO: Parameter validieren

   // Parameter file zerlegen: nur der Basisname wird angezeigt, kein Pfad
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // dynamischen Buffer auf dem Stack alloziieren
   char* format = "MetaTrader::%s%s::%s(%d)   %s";                    // maxLen(int) = 11: -2147483648  <NUL>
   int bufSize  = strlen(format) + strlen(baseName) + strlen(ext) + strlen(funcName) + 11 + strlen(msg) + 1;
   char* buffer = (char*)_alloca(bufSize);                            // wird wegen der Platzhalter immer ein paar Bytes zu groﬂ
   sprintf_s(buffer, bufSize, format, baseName, ext, funcName, line, msg);

   OutputDebugString(buffer);
}
#define debug(msg)  _debug(__FILE__, __FUNCTION__, __LINE__, msg)
