#pragma once
#include "expander.h"


datetime   WINAPI GetGmtTime();
datetime   WINAPI GetLocalTime();
SYSTEMTIME WINAPI getLocalTime();
SYSTEMTIME WINAPI getSystemTime();
BOOL       WINAPI GetTimeZoneInformationByNameA(TIME_ZONE_INFORMATION* tzi, const char* name);

string     WINAPI gmtTimeFormat (datetime   gmtTime, const char*  format);
char*      WINAPI GmtTimeFormatA(datetime   gmtTime, const char*  format);
wchar*     WINAPI GmtTimeFormatW(datetime64 gmtTime, const wchar* format);

string     WINAPI localTimeFormat (datetime   gmtTime, const char*  format);
char*      WINAPI LocalTimeFormatA(datetime   gmtTime, const char*  format);
wchar*     WINAPI LocalTimeFormatW(datetime64 gmtTime, const wchar* format);

// format conversions
SYSTEMTIME WINAPI LocalTimeToSystemTime(const SYSTEMTIME &lt);

SYSTEMTIME WINAPI FileTimeToSystemTime(const FILETIME &ft);
datetime   WINAPI FileTimeToUnixTime  (const FILETIME &ft);

FILETIME   WINAPI SystemTimeToFileTime(const SYSTEMTIME &st);
string     WINAPI SystemTimeToStr     (const SYSTEMTIME &st);
TM         WINAPI SystemTimeToTm      (const SYSTEMTIME &st);
datetime   WINAPI SystemTimeToUnixTime(const SYSTEMTIME &st);

string     WINAPI TmToStr     (const TM &time);
datetime   WINAPI TmToUnixTime(const TM &time, BOOL isUTC);

TM         WINAPI UnixTimeToTm(datetime   gmtTime, BOOL toLocalTime = FALSE);
TM         WINAPI UnixTimeToTm(datetime64 gmtTime, BOOL toLocalTime = FALSE);
