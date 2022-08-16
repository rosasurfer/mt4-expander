#include "expander.h"
#include "lib/datetime.h"
#include "lib/memory.h"

#include <ctime>
#include <sstream>


/**
 * Return the current system time as a Unix timestamp (seconds since 01.01.1970 00:00 GMT). In tester this time is not
 * modelled. Use the MQL framework function TimeGmt() to get the modelled GMT time in tester.
 *
 * @return datetime
 */
datetime WINAPI GetGmtTime() {
   return(time(NULL));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the system's local time as a Unix timestamp (seconds since 01.01.1970 00:00 local time). In tester this time is not
 * modelled. Use the MQL framework function TimeLocal() to get the modelled local time in tester.
 *
 * @return datetime
 */
datetime WINAPI GetLocalTime() {
   return(SystemTimeToUnixTime(getLocalTime()));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current local time as a SYSTEMTIME struct.
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI getLocalTime() {
   SYSTEMTIME st = {};
   GetLocalTime(&st);
   return(st);
}


/**
 * Return the current system time as a SYSTEMTIME struct.
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI getSystemTime() {
   SYSTEMTIME st = {};
   GetSystemTime(&st);
   return(st);
}


/**
 * Get timezone settings for a specific timezone.
 *
 * @param  _Out_ TIME_ZONE_INFORMATION* tzi  - struct receiving the timezone settings
 * @param  _In_  char*                  name - timezone name
 *
 * @return BOOL - success status; FALSE if no timezone with the specified name was found
 */
BOOL WINAPI GetTimeZoneInformationByNameA(TIME_ZONE_INFORMATION* tzi, const char* name) {
   // open the registry key
   string key = string("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\").append(name);
   HKEY hKey;
   if (int error = RegOpenKey(HKEY_LOCAL_MACHINE, key.c_str(), &hKey)) {
      return(error(ERR_WIN32_ERROR+error, "failed to open key: \"HKEY_LOCAL_MACHINE\\%s\"", key.c_str()));
   }

   // timezone info format in the registry
   struct REG_TIME_ZONE_INFORMATION {
      long       Bias;
      long       StandardBias;
      long       DaylightBias;
      SYSTEMTIME StandardDate;
      SYSTEMTIME DaylightDate;
   };

   // a local function as C++98 substitute for a lambda function
   struct local {
      static BOOL ReadRegistryValue(HKEY hKey, string &key, wchar* value, DWORD type, void* buffer, DWORD size) {
         int error = RegGetValueW(hKey, NULL, value, type, NULL, buffer, &size);
         if (error) error(ERR_WIN32_ERROR+error, "failed to read value: \"HKEY_LOCAL_MACHINE\\%s\\%S\"", key.c_str(), value);
         return(!error);
      }
   };
   REG_TIME_ZONE_INFORMATION regtzi = {};

   // read timezone settings from the registry
   BOOL success = 1;
   success = success && local::ReadRegistryValue(hKey, key, L"TZI", RRF_RT_REG_BINARY, &regtzi,            sizeof(regtzi));
   success = success && local::ReadRegistryValue(hKey, key, L"Std", RRF_RT_REG_SZ,     &tzi->StandardName, sizeof(tzi->StandardName));
   success = success && local::ReadRegistryValue(hKey, key, L"Dlt", RRF_RT_REG_SZ,     &tzi->DaylightName, sizeof(tzi->DaylightName));
   if (success) {
      tzi->Bias         = regtzi.Bias;
      tzi->DaylightBias = regtzi.DaylightBias;
      tzi->DaylightDate = regtzi.DaylightDate;
      tzi->StandardBias = regtzi.StandardBias;
      tzi->StandardDate = regtzi.StandardDate;
   }

   RegCloseKey(hKey);
   return(success);
}


/**
 * Format a Unix timestamp as a string representing GMT time.
 *
 * @param  datetime gmtTime - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*    format  - format control string as supported by strftime()
 *
 * @return string - formatted string or an empty string in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
string WINAPI gmtTimeFormat(datetime gmtTime, const char* format) {
   TM time = UnixTimeToTm(gmtTime);
   char* buffer = NULL;
   size_t bufSize = strlen(format) << 1;                    // times 2 (final initial size is 4 times strlen(format)

   for (;;) {
      bufSize <<= 1;                                        // times 2
      buffer = (char*)alloca(bufSize);                      // on the stack
      if (strftime(buffer, bufSize, format, &time)) break;
   }
   return(string(buffer));
}


/**
 * Format a 32-bit Unix timestamp as a string representing GMT time.
 *
 * @param  datetime gmtTime - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*    format  - format control string supported by strftime()
 *
 * @return char* - time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
char* WINAPI GmtTimeFormatA(datetime gmtTime, const char* format) {
   if (gmtTime == NaT)                   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter gmtTime: Not-a-Time"));
   if (gmtTime < 0)                      return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter gmtTime: %d (must be non-negative)", gmtTime));
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string s = gmtTimeFormat(gmtTime, format);
   return(strdup(s.c_str()));                               // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a 64-bit Unix timestamp as a string representing GMT time.
 *
 * @param  datetime64 gmtTime - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  wchar*     format  - format control string supported by strftime()
 *
 * @return wchar* - time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
wchar* WINAPI GmtTimeFormatW(datetime64 gmtTime, const wchar* format) {
   if (gmtTime == NaT)                   return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter gmtTime: Not-a-Time"));
   if (gmtTime < 0)                      return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter gmtTime: %d (must be non-negative)", gmtTime));
   if ((uint)format < MIN_VALID_POINTER) return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   TM time = UnixTimeToTm(gmtTime);
   wchar* buffer = NULL;
   uint bufSize = 32;                                       // initial buffer size is 64 (32 << 1)

   for (;;) {
      bufSize <<= 1;
      buffer = (wchar*)alloca(bufSize);                     // on the stack
      if (wcsftime(buffer, bufSize, format, &time)) break;
   }

   return(wcsdup(buffer));                                  // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a Unix timestamp as a string representing local time.
 *
 * @param  datetime gmtTime - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*    format  - format control string as supported by strftime()
 *
 * @return string - formatted string or an empty string in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
string WINAPI localTimeFormat(datetime gmtTime, const char* format) {
   TM time = UnixTimeToTm(gmtTime, TRUE);
   char* buffer = NULL;
   size_t bufSize = strlen(format) << 1;                    // times 2 (final initial size is 4 times strlen(format)

   for (;;) {
      bufSize <<= 1;                                        // times 2
      buffer = (char*)alloca(bufSize);                      // on the stack
      if (strftime(buffer, bufSize, format, &time)) break;
   }
   return(string(buffer));
}


/**
 * Format a 32-bit Unix timestamp as a string representing local time.
 *
 * @param  datetime gmtTime - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*    format  - format control string supported by strftime()
 *
 * @return char* - time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
char* WINAPI LocalTimeFormatA(datetime gmtTime, const char* format) {
   if (gmtTime == NaT)                   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter gmtTime: Not-a-Time"));
   if (gmtTime < 0)                      return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter gmtTime: %d (must be non-negative)", gmtTime));
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string s = localTimeFormat(gmtTime, format);
   return(strdup(s.c_str()));                               // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a 64-bit Unix timestamp as a string representing local time.
 *
 * @param  datetime64 gmtTime - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  wchar*     format  - format control string supported by strftime()
 *
 * @return wchar* - time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
wchar* WINAPI LocalTimeFormatW(datetime64 gmtTime, const wchar* format) {
   if (gmtTime == NaT)                   return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter gmtTime: Not-a-Time"));
   if (gmtTime < 0)                      return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter gmtTime: %d (must be non-negative)", gmtTime));
   if ((uint)format < MIN_VALID_POINTER) return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   TM time = UnixTimeToTm(gmtTime, TRUE);
   wchar* buffer = NULL;
   uint bufSize = 32;                                                // initial buffer size is 64 (32<<1)

   for (;;) {
      bufSize <<= 1;
      buffer = (wchar*)alloca(bufSize);                              // on the stack
      if (wcsftime(buffer, bufSize, format, &time)) break;
   }

   return(wcsdup(buffer));                                           // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Convert a SYSTEMTIME struct from local to GMT time.
 *
 * @param  SYSTEMTIME &localTime
 *
 * @return SYSTEMTIME - GMT time
 */
SYSTEMTIME WINAPI LocalTimeToSystemTime(const SYSTEMTIME &localTime) {
   SYSTEMTIME st = {};
   TzSpecificLocalTimeToSystemTime(NULL, &localTime, &st);
   return(st);
}


/**
 * Convert a FILETIME struct to a SYSTEMTIME struct.
 *
 * @param  FILETIME &ft
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI FileTimeToSystemTime(const FILETIME &ft) {
   SYSTEMTIME st = {};
   FileTimeToSystemTime(&ft, &st);
   return(st);
}


/**
 * Convert a FILETIME struct to a Unix timestamp.
 *
 * @param  FILETIME &ft
 *
 * @return datetime
 */
datetime WINAPI FileTimeToUnixTime(const FILETIME &ft) {
   // @see  https://stackoverflow.com/questions/20370920/convert-current-time-from-windows-to-unix-timestamp-in-c-or-c

   LARGE_INTEGER li = {};
   li.LowPart  = ft.dwLowDateTime;
   li.HighPart = ft.dwHighDateTime;

   const int64 UNIX_TIME_START  = 0x019DB1DED53E8000;                // 01.01.1970 (start of Unix epoch) in "ticks"
   const int64 TICKS_PER_SECOND = 10000000;                          // a tick is 100ns

   int64 seconds = (li.QuadPart-UNIX_TIME_START) / TICKS_PER_SECOND;
   return((datetime)seconds);
}


/**
 * Convert a SYSTEMTIME struct to a FILETIME struct.
 *
 * @param  SYSTEMTIME &st
 *
 * @return FILETIME
 */
FILETIME WINAPI SystemTimeToFileTime(const SYSTEMTIME &st) {
   FILETIME ft = {};
   SystemTimeToFileTime(&st, &ft);
   return(ft);
}


/**
 * Return a readable representation of a SYSTEMTIME struct.
 *
 * @param  SYSTEMTIME &st
 *
 * @return string
 */
string WINAPI SystemTimeToStr(const SYSTEMTIME &st) {
   std::ostringstream ss;
   SYSTEMTIME empty = {};

   if (MemCompare(&st, &empty, sizeof(SYSTEMTIME))) {
      ss << "{}";
   }
   else {
      ss <<  "{wYear="         << st.wYear            // year                          - [1601-30827]
         << ", wMonth="        << st.wMonth           // month of the year             - [1..12]
         << ", wDay="          << st.wDay             // day of the month              - [1..31]
         << ", wDayOfWeek="    << st.wDayOfWeek       // days since Sunday             - [0..6]
         << ", wHour="         << st.wHour            // hours since midnight          - [0..23]
         << ", wMinute="       << st.wMinute          // minutes after the hour        - [0..59]
         << ", wSecond="       << st.wSecond          // seconds after the minute      - [0..59]
         << ", wMilliseconds=" << st.wMilliseconds    // milliseconds after the second - [0..999]
         << "}";
   }
   return(ss.str());
}


/**
 * Convert a SYSTEMTIME struct to a C time struct.
 *
 * @param  SYSTEMTIME &st
 *
 * @return TM - C time struct (field TM.tm_yday will be incomplete)
 */
TM WINAPI SystemTimeToTm(const SYSTEMTIME &st) {
   TM time = {};
   time.tm_year  = st.wYear - 1900;
   time.tm_mon   = st.wMonth - 1;
   time.tm_mday  = st.wDay;
   time.tm_wday  = st.wDayOfWeek;
   time.tm_hour  = st.wHour;
   time.tm_min   = st.wMinute;
   time.tm_sec   = st.wSecond;
   time.tm_isdst = -1;                    // let the CRT compute whether DST is in effect
   return(time);
}


/**
 * Convert a SYSTEMTIME struct to a Unix timestamp.
 *
 * @param  SYSTEMTIME &st
 *
 * @return datetime
 */
datetime WINAPI SystemTimeToUnixTime(const SYSTEMTIME &st) {
   return(FileTimeToUnixTime(SystemTimeToFileTime(st)));
}


/**
 * Return a readable representation of a C time struct.
 *
 * @param  TM &time
 *
 * @return string
 */
string WINAPI TmToStr(const TM &time) {
   std::ostringstream ss;
   TM empty = {};

   if (MemCompare(&time, &empty, sizeof(TM))) {
      ss << "{}";
   }
   else {
      ss <<  "{year="  << time.tm_year                // years since 1900
         << ", mon="   << time.tm_mon                 // months since January       - [0..11]
         << ", mday="  << time.tm_mday                // day of the month           - [1..31]
         << ", yday="  << time.tm_yday                // days since January 1st     - [0..365]
         << ", wday="  << time.tm_wday                // days since Sunday          - [0..6]
         << ", hour="  << time.tm_hour                // hours since midnight       - [0..23]
         << ", min="   << time.tm_min                 // minutes after the hour     - [0..59]
         << ", sec="   << time.tm_sec                 // seconds after the minute   - [0..59]
         << ", isdst=" << time.tm_isdst               // daylight savings time flag - [1,0,-1] on/off/evaluate
         << "}";
   }
   return(ss.str());
}


/**
 * Convert a C time struct to a Unix timestamp.
 *
 * @param  TM   &time
 * @param  BOOL isUTC - whether the C time struct is UTC or local time
 *
 * @return datetime
 */
datetime WINAPI TmToUnixTime(const TM &time, BOOL isUTC) {
   TM tm = time;
   if (isUTC) return(mkgmtime(&tm));                  // convert a UTC time to a UTC timestamp
   else       return(mktime(&tm));                    // convert a local time to a UTC timestamp
}


/**
 * Convert a 32-bit Unix timestamp to a C time struct.
 *
 * @param  datetime gmtTime                - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  BOOL     toLocalTime [optional] - whether the resulting C time struct holds local or GMT time (default: GMT)
 *
 * @return TM
 */
TM WINAPI UnixTimeToTm(datetime gmtTime, BOOL toLocalTime/*=FALSE*/) {
   if (toLocalTime) return(*localtime(&gmtTime));
   else             return(*gmtime(&gmtTime));
}


/**
 * Convert a 64-bit Unix timestamp to a C time struct.
 *
 * @param  datetime64 gmtTime              - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  BOOL     toLocalTime [optional] - whether the resulting C time struct holds local or GMT time (default: GMT)
 *
 * @return TM
 */
TM WINAPI UnixTimeToTm(datetime64 gmtTime, BOOL toLocalTime/*=FALSE*/) {
   if (toLocalTime) return(*localtime64(&gmtTime));
   else             return(*gmtime64(&gmtTime));
}



// ---------------------------------------------------------------------------------------------------------------------------


/**
 * @return int
 */
int WINAPI Time_test(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

   TIME_ZONE_INFORMATION tzi = {};
   BOOL result = GetTimeZoneInformationByNameA(&tzi, name);
   //debug("timezone \"%s\": %d", name, result);

   SYSTEMTIME st = getLocalTime();
   //debug("SYSTEMTIME localTime = %s", SystemTimeToStr(st).c_str());

   /*
   datetime now = time(NULL);
   debug("now = %d", now);

   TM gmtTime = *gmtime(&now);
   debug("TM gmtTime   = %s", TmToStr(gmtTime).c_str());
   debug("timestamp    = %d", TmToUnixTime(gmtTime, TRUE));

   gmtTime.tm_isdst = 1;
   debug("TM gmtTime   = %s", TmToStr(gmtTime).c_str());
   debug("timestamp    = %d", TmToUnixTime(gmtTime, TRUE));

   TM localTime = *localtime(&now);
   debug("TM localTime = %s", TmToStr(localTime).c_str());
   debug("timestamp    = %d", TmToUnixTime(localTime, FALSE));

   localTime.tm_isdst = -1;
   debug("TM localTime = %s", TmToStr(localTime).c_str());
   debug("timestamp    = %d", TmToUnixTime(localTime, FALSE));

   localTime.tm_isdst = 0;
   debug("TM localTime = %s", TmToStr(localTime).c_str());
   debug("timestamp    = %d", TmToUnixTime(localTime, FALSE));
   */

   return(NULL);
   #pragma EXPANDER_EXPORT
}
