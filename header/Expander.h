#pragma once

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

