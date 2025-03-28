/**
 * Comparison and ranges of time types supported by Windows:
 *
 * @link  https://www.codeproject.com/Articles/144159/Time-Format-Conversion-Made-Easy
 */
#include "expander.h"
#include "lib/datetime.h"
#include "lib/memory.h"
#include "lib/string.h"

#include <ctime>
#include <sstream>

#define UNIX_TIME_START    116444736000000000LL                // 01.01.1970 (start of Unix epoch) in Windows "ticks"
#define TICKS_PER_SECOND   10000000LL                          // a tick is 100ns


/**
 * Return the current GMT time as a 32-bit Unix timestamp (seconds since 01.01.1970 00:00 GMT). In tester this time is not
 * modelled. Use the MQL framework function TimeGmt() to get the modelled GMT time in tester.
 *
 * @return time32
 */
time32 WINAPI GetGmtTime32() {
   return(_time32(NULL));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current GMT time as a 64-bit Unix timestamp (seconds since 01.01.1970 00:00 GMT). In tester this time is not
 * modelled. Use the MQL framework function TimeGmt() to get the modelled GMT time in tester.
 *
 * @return time64
 */
time64 WINAPI GetGmtTime64() {
   return(_time64(NULL));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current local time as a 32-bit Unix timestamp (seconds since 01.01.1970 00:00 local time). In tester this time
 * is not modelled. Use the MQL framework function TimeLocal() to get the modelled local time in tester.
 *
 * @return time32
 */
time32 WINAPI GetLocalTime32() {
   return(FileTimeToUnixTime32(getLocalTimeAsFileTime()));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current local time as a 64-bit Unix timestamp (seconds since 01.01.1970 00:00 local time). In tester this time
 * is not modelled. Use the MQL framework function TimeLocal() to get the modelled local time in tester.
 *
 * @return time64
 */
time64 WINAPI GetLocalTime64() {
   return(FileTimeToUnixTime64(getLocalTimeAsFileTime()));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current local time as a SYSTEMTIME.
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI getLocalTime() {
   SYSTEMTIME st = {};
   GetLocalTime(&st);
   return(st);
}


/**
 * Return the current local time as a FILETIME.
 *
 * @return FILETIME
 */
FILETIME WINAPI getLocalTimeAsFileTime() {
   FILETIME st=getSystemTimeAsFileTime(), lt={};
   FileTimeToLocalFileTime(&st, &lt);
   return(lt);
}


/**
 * Return the current system time as a SYSTEMTIME.
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI getSystemTime() {
   SYSTEMTIME st = {};
   GetSystemTime(&st);
   return(st);
}


/**
 * Return the current system time as a FILETIME.
 *
 * @return FILETIME
 */
FILETIME WINAPI getSystemTimeAsFileTime() {
   FILETIME ft = {};
   GetSystemTimeAsFileTime(&ft);
   return(ft);
}


/**
 * Get timezone infos for a Windows timezone.
 *
 * @param  _Out_ TIME_ZONE_INFORMATION* tzi  - struct receiving the timezone infos
 * @param  _In_  char*                  name - timezone name
 *
 * @return BOOL - success status; FALSE if no timezone with the specified name was found
 */
BOOL WINAPI GetTimeZoneInfoByNameA(TIME_ZONE_INFORMATION* tzi, const char* name) {
   // open the registry key
   string key = string("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\").append(name);
   HKEY hKey;
   if (int error = RegOpenKey(HKEY_LOCAL_MACHINE, key.c_str(), &hKey)) {
      return(!error(ERR_WIN32_ERROR+error, "failed to open key: \"HKEY_LOCAL_MACHINE\\%s\"", key.c_str()));
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
 * @param  time32 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*  format - format control string as supported by strftime()
 *
 * @return string - formatted string or an empty string in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
string WINAPI gmtTimeFormat(time32 time, const char* format) {
   TM tm = UnixTimeToTm(time);
   char* buffer = NULL;
   size_t bufSize = strlen(format) << 1;                    // times 2 (final initial size is 4 times strlen(format)

   for (;;) {
      bufSize <<= 1;                                        // times 2
      buffer = (char*)alloca(bufSize);                      // on the stack
      if (strftime(buffer, bufSize, format, &tm)) break;
   }
   return(string(buffer));
}


/**
 * Format a 32-bit Unix timestamp as a string representing GMT time.
 *
 * @param  time32 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*  format - format control string supported by strftime()
 *
 * @return char* - time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
char* WINAPI GmtTimeFormatA(time32 time, const char* format) {
   if (time == NaT)                      return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time"));
   if (time < 0)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));
   if ((uint)format < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string s = gmtTimeFormat(time, format);
   return sdup(s.c_str());                                  // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a 64-bit Unix timestamp as a string representing GMT time.
 *
 * @param  time64 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  wchar* format - format control string supported by strftime()
 *
 * @return wchar* - time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
wchar* WINAPI GmtTimeFormatW(time64 time, const wchar* format) {
   if (time == NaT)                      return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time"));
   if (time < 0)                         return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));
   if ((uint)format < MIN_VALID_POINTER) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   TM tm = UnixTimeToTm(time);
   wchar* buffer = NULL;
   uint bufSize = 32;                                       // initial buffer size is 64 (32 << 1)

   for (;;) {
      bufSize <<= 1;
      buffer = (wchar*)alloca(bufSize);                     // on the stack
      if (wcsftime(buffer, bufSize, format, &tm)) break;
   }

   return wsdup(buffer);                                    // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a Unix timestamp as a string representing local time.
 *
 * @param  time32 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*  format - format control string as supported by strftime()
 *
 * @return string - formatted string or an empty string in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
string WINAPI localTimeFormat(time32 time, const char* format) {
   TM tm = UnixTimeToTm(time, TRUE);
   char* buffer = NULL;
   size_t bufSize = strlen(format) << 1;                    // times 2 (final initial size is 4 times strlen(format)

   for (;;) {
      bufSize <<= 1;                                        // times 2
      buffer = (char*)alloca(bufSize);                      // on the stack
      if (strftime(buffer, bufSize, format, &tm)) break;
   }
   return string(buffer);
}


/**
 * Format a 32-bit Unix timestamp as a string representing local time.
 *
 * @param  time32 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*  format - format control string supported by strftime()
 *
 * @return char* - time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
char* WINAPI LocalTimeFormatA(time32 time, const char* format) {
   if (time == NaT)                      return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time"));
   if (time < 0)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));
   if ((uint)format < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string s = localTimeFormat(time, format);
   return sdup(s.c_str());                                  // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a 64-bit Unix timestamp as a string representing local time.
 *
 * @param  time64 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  wchar* format - format control string supported by strftime()
 *
 * @return wchar* - time string or a NULL pointer in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
wchar* WINAPI LocalTimeFormatW(time64 time, const wchar* format) {
   if (time == NaT)                      return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time"));
   if (time < 0)                         return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));
   if ((uint)format < MIN_VALID_POINTER) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   TM tm = UnixTimeToTm(time, TRUE);
   wchar* buffer = NULL;
   uint bufSize = 32;                                                // initial buffer size is 64 (32<<1)

   for (;;) {
      bufSize <<= 1;
      buffer = (wchar*)alloca(bufSize);                              // on the stack
      if (wcsftime(buffer, bufSize, format, &tm)) break;
   }

   return wsdup(buffer);                                             // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Convert a FILETIME to a SYSTEMTIME.
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
 * Convert a FILETIME to a 32-bit Unix timestamp.
 *
 * @param  FILETIME &ft
 *
 * @return time32
 */
time32 WINAPI FileTimeToUnixTime32(const FILETIME &ft) {
   // @see  https://stackoverflow.com/questions/20370920/convert-current-time-from-windows-to-unix-timestamp-in-c-or-c

   LARGE_INTEGER li = {};
   li.LowPart  = ft.dwLowDateTime;
   li.HighPart = ft.dwHighDateTime;

   int64 seconds = (li.QuadPart-UNIX_TIME_START) / TICKS_PER_SECOND;
   return((time32)seconds);
}


/**
 * Convert a FILETIME to a 64-bit Unix timestamp.
 *
 * @param  FILETIME &ft
 *
 * @return time64
 */
time64 WINAPI FileTimeToUnixTime64(const FILETIME &ft) {
   // @see  https://stackoverflow.com/questions/20370920/convert-current-time-from-windows-to-unix-timestamp-in-c-or-c

   LARGE_INTEGER li = {};
   li.LowPart  = ft.dwLowDateTime;
   li.HighPart = ft.dwHighDateTime;

   int64 seconds = (li.QuadPart-UNIX_TIME_START) / TICKS_PER_SECOND;
   return(seconds);
}


/**
 * Convert a SYSTEMTIME to a FILETIME.
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
 * Return a readable representation of a SYSTEMTIME.
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
 * Convert a SYSTEMTIME to a C time.
 *
 * @param  SYSTEMTIME &st
 *
 * @return TM - C time (field TM.tm_yday will be incomplete)
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
 * Convert a SYSTEMTIME to a 32-bit Unix timestamp.
 *
 * @param  SYSTEMTIME &st
 *
 * @return time32
 */
time32 WINAPI SystemTimeToUnixTime32(const SYSTEMTIME &st) {
   return(FileTimeToUnixTime32(SystemTimeToFileTime(st)));
}


/**
 * Convert a SYSTEMTIME to a 64-bit Unix timestamp.
 *
 * @param  SYSTEMTIME &st
 *
 * @return time64
 */
time64 WINAPI SystemTimeToUnixTime64(const SYSTEMTIME &st) {
   return(FileTimeToUnixTime64(SystemTimeToFileTime(st)));
}


/**
 * Return a readable representation of a C time.
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
 * Convert a C time to a 32-bit Unix timestamp.
 *
 * @param  TM   &time
 * @param  BOOL isLocalTime [optional] - whether the C time holds local or GMT time (default: GMT)
 *
 * @return time32
 */
time32 WINAPI TmToUnixTime32(const TM &time, BOOL isLocalTime/*=FALSE*/) {
   TM tm = time;
   if (isLocalTime) return(_mktime32(&tm));           // convert a local time to a GMT Unix timestamp
   else             return(_mkgmtime32(&tm));         // convert a GMT time to a GMT Unix timestamp
}


/**
 * Convert a C time to a 64-bit Unix timestamp.
 *
 * @param  TM   &time
 * @param  BOOL isLocalTime [optional] - whether the C time holds local or GMT time (default: GMT)
 *
 * @return time64
 */
time64 WINAPI TmToUnixTime64(const TM &time, BOOL isLocalTime/*=FALSE*/) {
   TM tm = time;
   if (isLocalTime) return(_mktime64(&tm));           // convert a local time to a UTC timestamp
   else             return(_mkgmtime64(&tm));         // convert a UTC time to a UTC timestamp
}


/**
 * Convert a Unix timestamp to a FILETIME.
 *
 * @param  time32 time - Unix timestamp (seconds since 01.01.1970 00:00)
 *
 * @return FILETIME
 */
FILETIME WINAPI UnixTimeToFileTime(time32 time) {
   // @see  https://docs.microsoft.com/en-us/windows/win32/sysinfo/converting-a-time-t-value-to-a-file-time

   ULARGE_INTEGER li = {};
   li.QuadPart = (time * TICKS_PER_SECOND) + UNIX_TIME_START;

   FILETIME ft = {};
   ft.dwLowDateTime  = li.LowPart;
   ft.dwHighDateTime = li.HighPart;
   return(ft);
}


/**
 * Convert a Unix timestamp to a SYSTEMTIME.
 *
 * @param  time32 time - Unix timestamp (seconds since 01.01.1970 00:00)
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI UnixTimeToSystemTime(time32 time) {
   return(FileTimeToSystemTime(UnixTimeToFileTime(time)));
}


/**
 * Convert a 32-bit Unix timestamp to a C time.
 *
 * @param  time32 time                   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  BOOL   toLocalTime [optional] - whether the resulting C time holds local or GMT time (default: GMT)
 *
 * @return TM
 */
TM WINAPI UnixTimeToTm(time32 time, BOOL toLocalTime/*=FALSE*/) {
   if (toLocalTime) return(*_localtime32(&time));
   else             return(*_gmtime32(&time));
}


/**
 * Convert a 64-bit Unix timestamp to a C time.
 *
 * @param  time64 time                   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  BOOL   toLocalTime [optional] - whether the resulting C struct holds local or GMT time (default: GMT)
 *
 * @return TM
 */
TM WINAPI UnixTimeToTm(time64 time, BOOL toLocalTime/*=FALSE*/) {
   if (toLocalTime) return(*_localtime64(&time));
   else             return(*_gmtime64(&time));
}


// ---------------------------------------------------------------------------------------------------------------------------


/**
 * @todo: finish unit tests
 *
 * Convert a GMT time to local time.
 *
 * @param  time32 time - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 *
 * @return time32 - Unix timestamp (seconds since 01.01.1970 00:00 local time) or NaT (Not-a-Time) in case of errors
 */
time32 WINAPI GmtToLocalTime(time32 time) {
   if (time == NaT) return(_NaT32(error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time")));
   if (time < 0)    return(_NaT32(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time)));

   SYSTEMTIME st = UnixTimeToSystemTime(time), lt = {};

   if (!SystemTimeToTzSpecificLocalTime(NULL, &st, &lt)) {
      return(_NaT32(error(ERR_WIN32_ERROR+GetLastError(), "SystemTimeToTzSpecificLocalTime()")));
   }
   return(SystemTimeToUnixTime32(lt));
   #pragma EXPANDER_EXPORT
}


/**
 * @todo: finish unit tests
 *
 * Convert a local time to GMT.
 *
 * @param  time32 time - Unix timestamp (seconds since 01.01.1970 00:00 local time)
 *
 * @return time32 - Unix timestamp (seconds since 01.01.1970 00:00 GMT) or NaT (Not-a-Time) in case of errors
 */
time32 WINAPI LocalToGmtTime(time32 time) {
   if (time == NaT) return(_NaT32(error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time")));
   if (time < 0)    return(_NaT32(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time)));

   SYSTEMTIME lt = UnixTimeToSystemTime(time), st = {};

   if (!TzSpecificLocalTimeToSystemTime(NULL, &lt, &st)) {
      return(_NaT32(error(ERR_WIN32_ERROR+GetLastError(), "TzSpecificLocalTimeToSystemTime()")));
   }
   return(SystemTimeToUnixTime32(st));
   #pragma EXPANDER_EXPORT
}


/**
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI D(const char* datetime) {
   SYSTEMTIME st = {};

   if (!datetime || strlen(datetime) < 19) {
      error(ERR_INVALID_PARAMETER, "invalid parameter datetime: %s", datetime);
      return st;
   }

   char* s = sdupa(datetime);
   s[4] = s[7] = s[10] = s[13] = s[16] = s[19] = '\0';   // format: 1981.09.30 20:00:00

   st.wYear   = (WORD) atoi(&s[ 0]);
   st.wMonth  = (WORD) atoi(&s[ 5]);
   st.wDay    = (WORD) atoi(&s[ 8]);
   st.wHour   = (WORD) atoi(&s[11]);
   st.wMinute = (WORD) atoi(&s[14]);
   st.wSecond = (WORD) atoi(&s[17]);

   return st;
}



/**
 * @return int
 */
int WINAPI test_Time(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

   TIME_ZONE_INFORMATION tzi = {};
   BOOL result = GetTimeZoneInfoByNameA(&tzi, name);
   debug("timezone \"%s\": %d", name, result);

   SYSTEMTIME st = D("1981.09.30 20:12:13");
   debug("SYSTEMTIME = %s", SystemTimeToStr(st).c_str());
   debug("time: %s", GmtTimeFormatA(SystemTimeToUnixTime32(st), "%a, %d.%m.%Y %H:%M:%S"));

   return(NULL);
   #pragma EXPANDER_EXPORT
}
