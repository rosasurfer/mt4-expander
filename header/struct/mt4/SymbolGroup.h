#pragma once
#include "expander.h"


#pragma pack(push, 1)
/**
 * MT4 struct SYMBOL_GROUP (file format of "symgroups.raw")
 *
 * The file size is fix, a file always contains 32 groups. Unused group entries are empty (zeroed).
 */
struct SYMBOL_GROUP {                              // -- offset ---- size --- description -----------------------------------
   char name       [16];                           //         0        16     group name
   char description[60];                           //        16        60     group description (unused, original size 64)
   uint backgroundColor;                           //        76         4     rsf: group color in "Market Watch" window
};                                                 // -----------------------------------------------------------------------
#pragma pack(pop)                                  //                = 80


// getters
const char* WINAPI sg_Name            (const SYMBOL_GROUP* sg);
const char* WINAPI sg_Description     (const SYMBOL_GROUP* sg);
uint        WINAPI sg_BackgroundColor (const SYMBOL_GROUP* sg);

const char* WINAPI sgs_Name           (const SYMBOL_GROUP sgs[], int index);
const char* WINAPI sgs_Description    (const SYMBOL_GROUP sgs[], int index);
uint        WINAPI sgs_BackgroundColor(const SYMBOL_GROUP sgs[], int index);

// setters
const char* WINAPI sg_SetName            (SYMBOL_GROUP* sg, const char* name);
const char* WINAPI sg_SetDescription     (SYMBOL_GROUP* sg, const char* description);
uint        WINAPI sg_SetBackgroundColor (SYMBOL_GROUP* sg, uint color);

const char* WINAPI sgs_SetName           (SYMBOL_GROUP sgs[], int index, const char* name);
const char* WINAPI sgs_SetDescription    (SYMBOL_GROUP sgs[], int index, const char* description);
uint        WINAPI sgs_SetBackgroundColor(SYMBOL_GROUP sgs[], int index, uint color);
