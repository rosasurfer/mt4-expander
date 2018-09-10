#pragma once

#include "expander.h"
#include "struct/mt4/MqlStr.h"


const char* WINAPI GetString(const char* value);
uint        WINAPI GetStringAddress(const char* value);
uint        WINAPI GetStringsAddress(const MqlStr values[]);
BOOL        WINAPI StringCompare(const char* s1, const char* s2);
BOOL        WINAPI StringEndsWith(const char* str, const char* suffix);
BOOL        WINAPI StringIsNull(const char* value);
void        WINAPI StrToLower(char* str);
void        WINAPI StrToLower(std::basic_string<char>& str);
void        WINAPI StrToLower(std::basic_string<wchar_t>& str);
void        WINAPI StrToUpper(char* str);
void        WINAPI StrToUpper(std::basic_string<char>& str);
void        WINAPI StrToUpper(std::basic_string<wchar_t>& str);
