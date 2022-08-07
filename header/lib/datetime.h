#pragma once
#include "expander.h"


datetime WINAPI GetLocalTimeAsUnixTime();
datetime WINAPI GetSystemTimeAsUnixTime();
size_t   WINAPI gmtimeFormat(char* buffer, size_t bufSize, datetime timestamp, const char* format);
char*    WINAPI GmtTimeFormatA(datetime timestamp, const char* format);
wchar*   WINAPI GmtTimeFormatW(datetime64 timestamp, const wchar* format);
size_t   WINAPI localtimeFormat(char* buffer, size_t bufSize, datetime timestamp, const char* format);
size_t   WINAPI localtimeFormat(char* buffer, size_t bufSize, SYSTEMTIME st, const char* format);
char*    WINAPI LocalTimeFormatA(datetime timestamp, const char* format);
wchar*   WINAPI LocalTimeFormatW(datetime64 timestamp, const wchar* format);

datetime WINAPI win32FileTimeToUnixTime(const FILETIME &ft);
FILETIME WINAPI win32SystemTimeToFileTime(const SYSTEMTIME &st);
