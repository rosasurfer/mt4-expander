#pragma once
#include "expander.h"


datetime   WINAPI GetGmtTime();
datetime   WINAPI GetLocalTime();
SYSTEMTIME WINAPI getLocalTime();
FILETIME   WINAPI getLocalTimeAsFileTime();
SYSTEMTIME WINAPI getSystemTime();
FILETIME   WINAPI getSystemTimeAsFileTime();
BOOL       WINAPI GetTimeZoneInformationByNameA(TIME_ZONE_INFORMATION* tzi, const char* name);


// timezone conversions
datetime   WINAPI GmtToLocalTime(datetime gmtTime);
datetime   WINAPI LocalToGmtTime(datetime localTime);


// time to string conversions
string     WINAPI gmtTimeFormat (datetime   gmtTime, const char*  format);
char*      WINAPI GmtTimeFormatA(datetime   gmtTime, const char*  format);
wchar*     WINAPI GmtTimeFormatW(datetime64 gmtTime, const wchar* format);

string     WINAPI localTimeFormat (datetime   gmtTime, const char*  format);
char*      WINAPI LocalTimeFormatA(datetime   gmtTime, const char*  format);
wchar*     WINAPI LocalTimeFormatW(datetime64 gmtTime, const wchar* format);


// type conversions and helpers
SYSTEMTIME WINAPI FileTimeToSystemTime(const FILETIME &ft);
datetime   WINAPI FileTimeToUnixTime  (const FILETIME &ft);

FILETIME   WINAPI SystemTimeToFileTime(const SYSTEMTIME &st);
string     WINAPI SystemTimeToStr     (const SYSTEMTIME &st);
TM         WINAPI SystemTimeToTm      (const SYSTEMTIME &st);
datetime   WINAPI SystemTimeToUnixTime(const SYSTEMTIME &st);

string     WINAPI TmToStr     (const TM &time);
datetime   WINAPI TmToUnixTime(const TM &time, BOOL isLocalTime = FALSE);

FILETIME   WINAPI UnixTimeToFileTime  (datetime   time);
SYSTEMTIME WINAPI UnixTimeToSystemTime(datetime   time);
TM         WINAPI UnixTimeToTm        (datetime   gmtTime, BOOL toLocalTime = FALSE);
TM         WINAPI UnixTimeToTm        (datetime64 gmtTime, BOOL toLocalTime = FALSE);
