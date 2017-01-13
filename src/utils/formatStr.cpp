#include "expander.h"


/**
 * Format a timestamp as a std::string representing GMT time.
 *
 * @param  datetime timestamp
 * @param  char*    format    - strftime() format control string
 *
 * @return string - GMT time string
 */
string WINAPI gmTimeFormat(datetime timestamp, const char* format) {
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
 * @param  datetime timestamp
 * @param  char*    format    - strftime() format control string
 *
 * @return char* - GMT time string
 */
const char* WINAPI GmTimeFormat(datetime timestamp, const char* format) {
   if (timestamp < 0)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp));
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string str  = gmTimeFormat(timestamp, format);
   uint length = str.length();
   if (!length)
      return(NULL);
   return(strcpy(new char[length+1], str.c_str()));                  // TODO: close memory leak
   #pragma EXPORT
}


/**
 * Format a timestamp as a std::string representing local time.
 *
 * @param  datetime timestamp
 * @param  char*    format    - strftime() format control string
 *
 * @return string - local time string
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
 * @param  datetime timestamp
 * @param  char*    format    - strftime() format control string
 *
 * @return char* - local time string
 */
const char* WINAPI LocalTimeFormat(datetime timestamp, const char* format) {
   if (timestamp < 0)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp));
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string str  = localTimeFormat(timestamp, format);
   uint length = str.length();
   if (!length)
      return(NULL);
   return(strcpy(new char[length+1], str.c_str()));                  // TODO: close memory leak
   #pragma EXPORT
}
