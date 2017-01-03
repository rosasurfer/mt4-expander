#include "header/Expander.h"
#include "header/time.h"


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

   char* buffer;
   uint size = 32;                                                   // initial buffer size of 64 (32<<1)
   for (;;) {
      size <<= 1;
      buffer = new char[size];                                       // TODO: close memory leak
      if (strftime(buffer, size, format, localtime(&timestamp)))
         break;
      delete[] buffer;
   }
   return(buffer);
   #pragma EXPORT
}


/**
 * Format a timestamp as a std::string representing local time.
 *
 * @param  datetime timestamp
 * @param  char*    format    - strftime() format control string
 *
 * @return std::string - local time string
 */
std::string WINAPI localTimeFormat(datetime timestamp, const char* format) {
   if (timestamp < 0)                    return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp)));
   if ((uint)format < MIN_VALID_POINTER) return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format)));

   char* buffer;
   uint size = 32;                                                   // initial buffer size of 64 (32<<1)
   for (;;) {
      size <<= 1;
      buffer = (char*)alloca(size);                                  // on the stack
      if (strftime(buffer, size, format, localtime(&timestamp)))
         break;
   }
   return(std::string(buffer));
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

   char* buffer;
   uint size = 32;                                                   // initial buffer size of 64 (32<<1)
   for (;;) {
      size <<= 1;
      buffer = new char[size];                                       // TODO: close memory leak
      if (strftime(buffer, size, format, gmtime(&timestamp)))
         break;
      delete[] buffer;
   }
   return(buffer);
   #pragma EXPORT
}


/**
 * Format a timestamp as a std::string representing GMT time.
 *
 * @param  datetime timestamp
 * @param  char*    format    - strftime() format control string
 *
 * @return std::string - GMT time string
 */
std::string WINAPI gmTimeFormat(datetime timestamp, const char* format) {
   if (timestamp < 0)                    return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp)));
   if ((uint)format < MIN_VALID_POINTER) return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format)));

   char* buffer;
   uint size = 32;                                                   // initial buffer size of 64 (32<<1)
   for (;;) {
      size <<= 1;
      buffer = (char*)alloca(size);                                  // on the stack
      if (strftime(buffer, size, format, gmtime(&timestamp)))
         break;
   }
   return(std::string(buffer));
}


/**
 * Format a numeric value as a C string.
 *
 * @param  doube value
 * @param  char* format - printf() format control string
 *
 * @return char* - formatted string or NULL pointer if an error occurred
 */
const char* WINAPI NumberFormat(double value, const char* format) {
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   std::string str = numberFormat(value, format);

   size_t size = str.length() + 1;                                   // +1 for the terminating '\0'
   char* buffer = new char[size];                                    // TODO: close memory leak
   if (!strcpy(buffer, str.c_str()))
      return(NULL);
   return(buffer);
   #pragma EXPORT
}


/**
 * Format a numeric value as a std::string.
 *
 * @param  doube value
 * @param  char* format - printf() format control string
 *
 * @return std::string - formatted string or empty string if an error occurred
 */
std::string WINAPI numberFormat(double value, const char* format) {
   if ((uint)format < MIN_VALID_POINTER) return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format)));

   size_t size = _scprintf(format, value) + 1;                       // +1 for the terminating '\0'
   char* buffer = (char*) alloca(size);                              // on the stack
   sprintf_s(buffer, size, format, value);

   return(std::string(buffer));
}
