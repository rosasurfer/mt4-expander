#pragma once

/**
 * MT4 struct MqlStr (MetaQuotes-Terminologie)
 *
 * Interne Darstellung eines MQL-Strings.
 */
struct MqlStr {
   int   sizeAlloc;                                // Größe des alloziierten Speicherblocks (0, wenn es ein intern verwaltetes C-Literal ist)
   char* string;
};


// @see  neues Format: https://docs.mql4.com/basis/types/stringconst