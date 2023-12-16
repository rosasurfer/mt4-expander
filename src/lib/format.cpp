#include "expander.h"
#include "lib/string.h"


/**
 * Format a floating point value.
 *
 * @param  doube value
 * @param  char* format - format control string as used for printf()
 *
 * @return char* - formatted string or a NULL pointer in case of errors
 *
 * @see  https://alvinalexander.com/programming/printf-format-cheat-sheet
 * @see  http://www.cplusplus.com/reference/cstdio/printf/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm
 */
char* WINAPI NumberFormat(double value, const char* format) {
   if (format && (uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));
   return(asformat(format, value));
   #pragma EXPANDER_EXPORT
}
