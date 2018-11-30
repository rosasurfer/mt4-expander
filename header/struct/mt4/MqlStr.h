#pragma once
#include "expander.h"


/**
 * MT4 struct MqlStr (MetaQuotes terminology). Internal representation of an MQL string.
 *
 *       New build-600 type definition:
 * @see  https://docs.mql4.com/basis/types/stringconst
 */
struct MqlStr {
   uint  bufsize;                      // size of allocated buffer; 0 (zero) if it's an internally stored C literal
   char* value;
};

// TODO: check alignment
