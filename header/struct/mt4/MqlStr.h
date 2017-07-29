#pragma once


/**
 * MT4 struct MqlStr (MetaQuotes terminology). Internal representation of an MQL string.
 *
 * New build-600 type definition:
 * @see  https://docs.mql4.com/basis/types/stringconst
 */
struct MqlStr {
   int   sizeAlloc;                                // size of allocated memory; 0 if an internal C literal (in string pool)
   char* string;
};                                                 // TODO: check struct alignment
