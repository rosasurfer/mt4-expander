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
 * @param  char*    buffer    - target buffer to receive the resulting string
 * @param  size_t   bufSize   - buffer size
 * @param  datetime timestamp - Unix timestamp (GMT)
 * @param  char*    format    - format control string as supported by strftime()
 *
 * @return size_t - number of characters copied to the target buffer or 0 in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
size_t WINAPI gmtimeFormat(char* buffer, size_t bufSize, datetime timestamp, const char* format) {
   if (timestamp == NaT) return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: Not-a-Time"));
   if (timestamp < 0)    return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (negative)", timestamp));

   return(strftime(buffer, bufSize, format, gmtime(&timestamp)));
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
char* WINAPI GmtTimeFormatA(datetime timestamp, const char* format) {
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
 * Format a timestamp as a string representing GMT time.
 *
 * @param  datetime64 timestamp - 64-bit Unix timestamp (GMT)
 * @param  wchar*     format    - format control string supported by strftime()
 *
 * @return wchar* - GMT time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
wchar* WINAPI GmtTimeFormatW(datetime64 timestamp, const wchar* format) {
   if (timestamp == NaT)                 return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: Not-a-Time"));
   if (timestamp < 0)                    return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (negative)", timestamp));
   if ((uint)format < MIN_VALID_POINTER) return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   wchar* buffer = NULL;
   uint size = 32;                                                   // initial buffer size is 64 (32 << 1)
   for (;;) {
      size <<= 1;
      buffer = (wchar*)alloca(size);                                 // on the stack
      if (wcsftime(buffer, size, format, _gmtime64(&timestamp)))
         break;
   }

   return(wcsdup(buffer));                                           // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT


}


/**
 * Format a timestamp as a string representing local time.
 *
 * @param  char*    buffer    - target buffer to receive the resulting string
 * @param  size_t   bufSize   - buffer size
 * @param  datetime timestamp - Unix timestamp (GMT)
 * @param  char*    format    - format control string as supported by strftime()
 *
 * @return size_t - number of characters copied to the target buffer or 0 in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
size_t WINAPI localtimeFormat(char* buffer, size_t bufSize, datetime timestamp, const char* format) {
   if (timestamp == NaT) return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: Not-a-Time"));
   if (timestamp < 0)    return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (negative)", timestamp));

   return(strftime(buffer, bufSize, format, localtime(&timestamp)));
}


/**
 * Format a SYSTEMTIME structure as a string representing local time.
 *
 * @param  char*      buffer  - target buffer to receive the resulting string
 * @param  size_t     bufSize - buffer size
 * @param  SYSTEMTIME st      - SYSTEMTIME structure as returned by GetLocalTime(SYSTEMTIME &st)
 * @param  char*      format  - format control string as supported by strftime()
 *
 * @return size_t - number of characters copied to the target buffer or 0 in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
size_t WINAPI localtimeFormat(char* buffer, size_t bufSize, SYSTEMTIME st, const char* format) {
   tm tt = {};
   tt.tm_year  = st.wYear - 1900;                     // years since 1900
   tt.tm_mon   = st.wMonth - 1;                       // months since January:   0..11
   tt.tm_mday  = st.wDay;                             // day of the month:       1..31
   tt.tm_hour  = st.wHour;                            // hours since midnight:   0..23
   tt.tm_min   = st.wMinute;                          // minutes of the hour:    0..59
   tt.tm_sec   = st.wSecond;                          // seconds of the minute:  0..59
   tt.tm_isdst = -1;                                  // let the CRT compute whether DST is in effect
   return(strftime(buffer, bufSize, format, &tt));
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
char* WINAPI LocalTimeFormatA(datetime timestamp, const char* format) {
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


/**
 * Format a timestamp as a string representing local time.
 *
 * @param  datetime64 timestamp - Unix timestamp (GMT)
 * @param  wchar*     format    - format control string supported by strftime()
 *
 * @return wchar* - local time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
wchar* WINAPI LocalTimeFormatW(datetime64 timestamp, const wchar* format) {
   if (timestamp == NaT)                 return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: Not-a-Time"));
   if (timestamp < 0)                    return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (negative)", timestamp));
   if ((uint)format < MIN_VALID_POINTER) return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   wchar* buffer = NULL;
   uint size = 32;                                                   // initial buffer size is 64 (32<<1)
   for (;;) {
      size <<= 1;
      buffer = (wchar*)alloca(size);                                 // on the stack
      if (wcsftime(buffer, size, format, _localtime64(&timestamp)))
         break;
   }
   return(wcsdup(buffer));                                           // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}
