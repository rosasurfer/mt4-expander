#include "expander.h"

#include <time.h>


/**
 * Return the system's current GMT time (also in Strategy Tester).
 *
 * @return datetime - Unix timestamp as seconds since 01.01.1970 00:00:00 GMT
 */
datetime WINAPI GetGmtTime() {
   return(time(NULL));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the system's current local time (also in Strategy Tester).
 *
 * @return datetime - local timestamp as seconds since 01.01.1970 00:00:00 local time
 */
datetime WINAPI GetLocalTime() {
   datetime gmtTime = time(NULL);
   localtime(&gmtTime);
   return(gmtTime - timezone);
   #pragma EXPANDER_EXPORT
}


/**
 * Format a timestamp as a string representing GMT time.
 *
 * @param  datetime timestamp - Unix timestamp (GMT)
 * @param  char*    format    - format control string supported by strftime()
 *
 * @return char* - GMT time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
const char* WINAPI GmtTimeFormatA(datetime timestamp, const char* format) {
   if (timestamp == NaT)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: Not-a-Time"));
   if (timestamp < 0)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (negative)", timestamp));
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   char* buffer = NULL;
   uint size = 32;                                                   // initial buffer size is 64 (32 << 1)
   for (;;) {
      size <<= 1;
      buffer = (char*)alloca(size);                                  // on the stack
      if (strftime(buffer, size, format, gmtime(&timestamp)))
         break;
   }
   return(strdup(buffer));                                           // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a timestamp as a string representing local time.
 *
 * @param  datetime timestamp - Unix timestamp (GMT)
 * @param  char*    format    - format control string supported by strftime()
 *
 * @return char* - local time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
const char* WINAPI LocalTimeFormatA(datetime timestamp, const char* format) {
   if (timestamp == NaT)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: Not-a-Time"));
   if (timestamp < 0)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (negative)", timestamp));
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   char* buffer = NULL;
   uint size = 32;                                                   // initial buffer size is 64 (32<<1)
   for (;;) {
      size <<= 1;
      buffer = (char*)alloca(size);                                  // on the stack
      if (strftime(buffer, size, format, localtime(&timestamp)))
         break;
   }
   return(strdup(buffer));                                           // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}
