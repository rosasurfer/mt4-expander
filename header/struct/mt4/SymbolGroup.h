#pragma once
#include "expander.h"


/**
 * MT4 struct SYMBOL_GROUP (Dateiformat "symgroups.raw")
 *
 * Die Größe der Datei ist fix und enthält Platz für exakt 32 Gruppen. Einzelne Gruppen können undefiniert sein.
 */
#pragma pack(push, 1)
struct SYMBOL_GROUP {                              // -- offset ---- size --- description ------------------
   char name       [16];                           //         0        16     Name         (szchar)
   char description[60];                           //        16        60     Beschreibung (szchar)
   int  backgroundColor;          // custom (pewa) //        76         4     Farbe im "Market Watch" Window
};                                                 // ------------------------------------------------------
#pragma pack(pop)                                  //                = 80


// Getter
const char* WINAPI sg_Name            (const SYMBOL_GROUP* sg);
const char* WINAPI sg_Description     (const SYMBOL_GROUP* sg);
uint        WINAPI sg_BackgroundColor (const SYMBOL_GROUP* sg);

const char* WINAPI sgs_Name           (const SYMBOL_GROUP sgs[], int index);
const char* WINAPI sgs_Description    (const SYMBOL_GROUP sgs[], int index);
uint        WINAPI sgs_BackgroundColor(const SYMBOL_GROUP sgs[], int index);

// Setter
const char* WINAPI sg_SetName            (SYMBOL_GROUP* sg, const char* name       );
const char* WINAPI sg_SetDescription     (SYMBOL_GROUP* sg, const char* description);
int         WINAPI sg_SetBackgroundColor (SYMBOL_GROUP* sg, uint        color      );

const char* WINAPI sgs_SetName           (SYMBOL_GROUP sgs[], int index, const char* name       );
const char* WINAPI sgs_SetDescription    (SYMBOL_GROUP sgs[], int index, const char* description);
int         WINAPI sgs_SetBackgroundColor(SYMBOL_GROUP sgs[], int index, uint        color      );
