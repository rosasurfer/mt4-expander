#include "expander.h"
#include "lib/string.h"

#include <time.h>


/**
 * Format a timestamp as a std::string representing GMT time.
 *
 * @param  datetime timestamp - Unix timestamp (GMT)
 * @param  char*    format    - strftime() format control string
 *
 * @return string - GMT time string
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
string WINAPI gmtTimeFormat(datetime timestamp, const char* format) {
   if (timestamp < 0)                    return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp)));
   if ((uint)format < MIN_VALID_POINTER) return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format)));

   char* buffer;
   uint size = 32;                                                   // initial buffer size is 64 (32 << 1)
   for (;;) {
      size <<= 1;
      buffer = (char*)alloca(size);                                  // on the stack
      if (strftime(buffer, size, format, gmtime(&timestamp)))
         break;
   }
   return(string(buffer));
}


/**
 * Format a timestamp as a C string representing GMT time.
 *
 * @param  datetime timestamp - Unix timestamp (GMT)
 * @param  char*    format    - strftime() format control string
 *
 * @return char* - GMT time string
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
const char* WINAPI GmtTimeFormat(datetime timestamp, const char* format) {
   if (timestamp < 0)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp));
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string str = gmtTimeFormat(timestamp, format);
   if (!str.length())
      return(NULL);
   return(strdup(str.c_str()));                                      // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}


/**
 * Format a timestamp as a std::string representing local time.
 *
 * @param  datetime timestamp - Unix timestamp (GMT)
 * @param  char*    format    - strftime() format control string
 *
 * @return string - local time string
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
string WINAPI localTimeFormat(datetime timestamp, const char* format) {
   if (timestamp < 0)                    return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp)));
   if ((uint)format < MIN_VALID_POINTER) return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format)));

   char* buffer;
   uint size = 32;                                                   // initial buffer size is 64 (32<<1)
   for (;;) {
      size <<= 1;
      buffer = (char*)alloca(size);                                  // on the stack
      if (strftime(buffer, size, format, localtime(&timestamp)))
         break;
   }
   return(string(buffer));
}


/**
 * Format a timestamp as a C string representing local time.
 *
 * @param  datetime timestamp - Unix timestamp (GMT)
 * @param  char*    format    - strftime() format control string
 *
 * @return char* - local time string
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
const char* WINAPI LocalTimeFormat(datetime timestamp, const char* format) {
   if (timestamp < 0)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp));
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string str = localTimeFormat(timestamp, format);
   if (!str.length())
      return(NULL);
   return(strdup(str.c_str()));                                      // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}


/**
 * Format a signed integral value.
 *
 * @param  int   value
 * @param  char* format - format control string as used for printf()
 *
 * @return char* - formatted string or a NULL pointer in case of errors
 *
 * Number format codes:
 * @see  https://alvinalexander.com/programming/printf-format-cheat-sheet
 * @see  http://www.cplusplus.com/reference/cstdio/printf/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm
 */
char* WINAPI NumberFormat(int value, const char* format) {
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   uint size = _scprintf(format, value) + 1;                         // +1 for the terminating '\0'
   char* buffer = (char*) alloca(size);                              // on the stack
   sprintf_s(buffer, size, format, value);

   return(buffer);                                                   // TODO: close memory leak
}


/**
 * Format an unsigned integral value.
 *
 * @param  uint  value
 * @param  char* format - format control string as used for printf()
 *
 * @return char* - formatted string or a NULL pointer in case of errors
 *
 * Number format codes:
 * @see  https://alvinalexander.com/programming/printf-format-cheat-sheet
 * @see  http://www.cplusplus.com/reference/cstdio/printf/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm
 */
char* WINAPI NumberFormat(uint value, const char* format) {
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   uint size = _scprintf(format, value) + 1;                         // +1 for the terminating '\0'
   char* buffer = (char*) alloca(size);                              // on the stack
   sprintf_s(buffer, size, format, value);

   return(buffer);                                                   // TODO: close memory leak
}


/**
 * Format a floating point value.
 *
 * @param  doube value
 * @param  char* format - format control string as used for printf()
 *
 * @return char* - formatted string or a NULL pointer in case of errors
 *
 * Number format codes:
 * @see  https://alvinalexander.com/programming/printf-format-cheat-sheet
 * @see  http://www.cplusplus.com/reference/cstdio/printf/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm
 */
char* WINAPI NumberFormat(double value, const char* format) {
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   uint size = _scprintf(format, value) + 1;                         // +1 for the terminating '\0'
   char* buffer = (char*) alloca(size);                              // on the stack
   sprintf_s(buffer, size, format, value);

   return(buffer);                                                   // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}
