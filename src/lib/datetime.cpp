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
