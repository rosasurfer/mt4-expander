#pragma once


const char* WINAPI GetString        (const char* value);
uint        WINAPI GetStringAddress (const char* value);
uint        WINAPI GetStringsAddress(const MqlStr values[]);
BOOL        WINAPI StringCompare    (const char* s1, const char* s2);
BOOL        WINAPI StringEndsWith   (const char* str, const char* suffix);
BOOL        WINAPI StringIsNull     (const char* value);
