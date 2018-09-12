#pragma once

#include "expander.h"
#include "struct/mt4/MqlStr.h"


const char* WINAPI GetString(const char* value);
uint        WINAPI GetStringAddress(const char* value);
uint        WINAPI GetStringsAddress(const MqlStr values[]);
BOOL        WINAPI StringCompare(const char* s1, const char* s2);
BOOL        WINAPI StringEndsWith(const char* str, const char* suffix);
BOOL        WINAPI StringIsNull(const char* value);
char*       WINAPI StrToLower(char* str);
string&     WINAPI StrToLower(string& str);
wstring&    WINAPI StrToLower(wstring& str);
char*       WINAPI StrToUpper(char* str);
string&     WINAPI StrToUpper(string& str);
wstring&    WINAPI StrToUpper(wstring& str);
