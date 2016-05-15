#include "stdafx.h"


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende feste Werte zurückzugeben.
 * Alle Parameter werden ignoriert.
 */
BOOL _TRUE (...) { return(TRUE ); }
BOOL _FALSE(...) { return(FALSE); }


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende variable Werte zurückzugeben.
 * Außer dem ersten werden alle übergebenen Parameter ignoriert.
 */
BOOL _BOOL(BOOL value, ...) { return(value); }
