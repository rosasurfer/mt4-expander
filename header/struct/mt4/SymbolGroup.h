#pragma once
#include "expander.h"


/**
 * MT4 struct SYMBOL_GROUP (file format of "symgroups.raw")
 *
 * The file size is fixed and always contains 32 groups. Unused group entries are just empty (zeroed).
 */
#pragma pack(push, 1)

struct SYMBOL_GROUP {                              // -- offset ---- size --- description -----------------------------------
   char name       [16];                           //         0        16     group name
   char description[60];                           //        16        60     unused: group description, original size 64
   int  backgroundColor;          // custom (pewa) //        76         4     unused: group color in "Market Watch" window
};                                                 // -----------------------------------------------------------------------
#pragma pack(pop)                                  //                = 80


// getters (used by MQL4)
const char* WINAPI sg_Name            (const SYMBOL_GROUP* sg);
const char* WINAPI sg_Description     (const SYMBOL_GROUP* sg);
uint        WINAPI sg_BackgroundColor (const SYMBOL_GROUP* sg);

const char* WINAPI sgs_Name           (const SYMBOL_GROUP sgs[], int index);
const char* WINAPI sgs_Description    (const SYMBOL_GROUP sgs[], int index);
uint        WINAPI sgs_BackgroundColor(const SYMBOL_GROUP sgs[], int index);

// validating setters (also used by MQL4)
const char* WINAPI sg_SetName            (SYMBOL_GROUP* sg, const char* name       );
const char* WINAPI sg_SetDescription     (SYMBOL_GROUP* sg, const char* description);
int         WINAPI sg_SetBackgroundColor (SYMBOL_GROUP* sg, uint        color      );

const char* WINAPI sgs_SetName           (SYMBOL_GROUP sgs[], int index, const char* name       );
const char* WINAPI sgs_SetDescription    (SYMBOL_GROUP sgs[], int index, const char* description);
int         WINAPI sgs_SetBackgroundColor(SYMBOL_GROUP sgs[], int index, uint        color      );
