#pragma once
#include "expander.h"


time32     WINAPI GetGmtTime();
time32     WINAPI GetLocalTime();
SYSTEMTIME WINAPI getLocalTime();
FILETIME   WINAPI getLocalTimeAsFileTime();
SYSTEMTIME WINAPI getSystemTime();
FILETIME   WINAPI getSystemTimeAsFileTime();
BOOL       WINAPI GetTimeZoneInformationByNameA(TIME_ZONE_INFORMATION* tzi, const char* name);


// timezone conversions
time32     WINAPI GmtToLocalTime(time32 gmtTime);
time32     WINAPI LocalToGmtTime(time32 localTime);


// time to string conversions
string     WINAPI gmtTimeFormat (time32 gmtTime, const char*  format);
char*      WINAPI GmtTimeFormatA(time32 gmtTime, const char*  format);
wchar*     WINAPI GmtTimeFormatW(time64 gmtTime, const wchar* format);

string     WINAPI localTimeFormat (time32 gmtTime, const char*  format);
char*      WINAPI LocalTimeFormatA(time32 gmtTime, const char*  format);
wchar*     WINAPI LocalTimeFormatW(time64 gmtTime, const wchar* format);


// type conversions and helpers
SYSTEMTIME WINAPI FileTimeToSystemTime(const FILETIME &ft);
time32     WINAPI FileTimeToUnixTime  (const FILETIME &ft);

FILETIME   WINAPI SystemTimeToFileTime(const SYSTEMTIME &st);
string     WINAPI SystemTimeToStr     (const SYSTEMTIME &st);
TM         WINAPI SystemTimeToTm      (const SYSTEMTIME &st);
time32     WINAPI SystemTimeToUnixTime(const SYSTEMTIME &st);

string     WINAPI TmToStr     (const TM &time);
time32     WINAPI TmToUnixTime(const TM &time, BOOL isLocalTime = FALSE);

FILETIME   WINAPI UnixTimeToFileTime  (time32 time);
SYSTEMTIME WINAPI UnixTimeToSystemTime(time32 time);
TM         WINAPI UnixTimeToTm        (time32 gmtTime, BOOL toLocalTime = FALSE);
TM         WINAPI UnixTimeToTm        (time64 gmtTime, BOOL toLocalTime = FALSE);
