#pragma once
#include "expander.h"

#pragma pack(push,1)


/**
 * Internal MT4 representation of an MQL4 string. MQL4 strings are stored as ANSI strings. If an MQL4 string variable is
 * passed to a C function, in fact the field MqlStringA.value is passed. If an MQL4 string array is passed to a C function,
 * in fact a pointer to an array of struct MqlStringA is passed.
 *
 * @see  type definition in MetaTrader4 DLL example project
 */
struct MqlStringA {
   uint  size;                      // size of the buffer allocated for the string; 0 if the string is a C literal (CRT managed)
   char* value;                     // ANSI string
};
//typedef MqlStringA MqlStr;        // MetaQuotes alias


/**
 * Internal MT4 representation of an MQL5 string. MQL5 strings are stored as Unicode strings.
 *
 * TODO: document behaviour when passing variables and arrays to C functions
 *
 * @see  https://docs.mql4.com/basis/types/stringconst (type definition)
 * @see  https://www.mql5.com/en/forum/149943
 * @see  https://www.mql5.com/en/forum/149412          (cross-solutions)
 */
struct MqlStringW {
   uint   size;                     // size of the buffer allocated for the string
   wchar* value;                    // Unicode string
   int    reserved;                 // reserved
};
//typedef MqlStringW MqlString;     // MetaQuotes alias

#pragma pack(pop,1)
