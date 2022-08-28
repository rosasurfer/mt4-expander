#pragma once
#include "expander.h"


time32     WINAPI GetGmtTime32();
time64     WINAPI GetGmtTime64();
time32     WINAPI GetLocalTime32();
time64     WINAPI GetLocalTime64();
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
time32     WINAPI FileTimeToUnixTime32(const FILETIME &ft);
time64     WINAPI FileTimeToUnixTime64(const FILETIME &ft);

FILETIME   WINAPI SystemTimeToFileTime  (const SYSTEMTIME &st);
string     WINAPI SystemTimeToStr       (const SYSTEMTIME &st);
TM         WINAPI SystemTimeToTm        (const SYSTEMTIME &st);
time32     WINAPI SystemTimeToUnixTime32(const SYSTEMTIME &st);
time64     WINAPI SystemTimeToUnixTime64(const SYSTEMTIME &st);

string     WINAPI TmToStr       (const TM &time);
time32     WINAPI TmToUnixTime32(const TM &time, BOOL isLocalTime = FALSE);
time64     WINAPI TmToUnixTime64(const TM &time, BOOL isLocalTime = FALSE);

FILETIME   WINAPI UnixTimeToFileTime  (time32 time);
SYSTEMTIME WINAPI UnixTimeToSystemTime(time32 time);
TM         WINAPI UnixTimeToTm        (time32 gmtTime, BOOL toLocalTime = FALSE);
TM         WINAPI UnixTimeToTm        (time64 gmtTime, BOOL toLocalTime = FALSE);
