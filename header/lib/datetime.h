#pragma once
#include "expander.h"


// current time
time32     WINAPI GetGmtTime32();
time64     WINAPI GetGmtTime64();
time32     WINAPI GetLocalTime32();
time64     WINAPI GetLocalTime64();
SYSTEMTIME WINAPI getLocalTime();
FILETIME   WINAPI getLocalTimeAsFileTime();
SYSTEMTIME WINAPI getSystemTime();
FILETIME   WINAPI getSystemTimeAsFileTime();

// time to string conversion
char*      WINAPI GmtTimeFormatA(time32 time, const char*  format);
wchar*     WINAPI GmtTimeFormatW(time64 time, const wchar* format);
string     WINAPI gmtTimeFormat (time32 time, const char*  format);

char*      WINAPI LocalTimeFormatA(time32 time, const char*  format);
wchar*     WINAPI LocalTimeFormatW(time64 time, const wchar* format);
string     WINAPI localTimeFormat (time32 time, const char*  format);

// type conversion
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
TM         WINAPI UnixTimeToTm        (time32 time, BOOL toLocalTime = FALSE);
TM         WINAPI UnixTimeToTm        (time64 time, BOOL toLocalTime = FALSE);

// timezone conversion
time32     WINAPI GmtToLocalTime(time32 time);
time32     WINAPI LocalToGmtTime(time32 time);
