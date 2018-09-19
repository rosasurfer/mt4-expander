#pragma once

#include "expander.h"
#include "struct/mt4/MqlStr.h"


const char* WINAPI DoubleQuoteStr(const char* value);
string      WINAPI doubleQuoteStr(const char* value);
string      WINAPI doubleQuoteStr(const string& value);

const char* WINAPI GetString(const char* value);
uint        WINAPI GetStringAddress(const char* value);
uint        WINAPI GetStringsAddress(const MqlStr values[]);
BOOL        WINAPI StringIsNull(const char* value);

BOOL        WINAPI StringCompare(const char* s1, const char* s2);
BOOL        WINAPI StringEndsWith(const char* str, const char* suffix);
BOOL        WINAPI StringStartsWith(const char* str, const char* prefix);

char*       WINAPI StrToLower(char* str);
string&     WINAPI StrToLower(string& str);
wstring&    WINAPI StrToLower(wstring& str);
char*       WINAPI StrToUpper(char* str);
string&     WINAPI StrToUpper(string& str);
wstring&    WINAPI StrToUpper(wstring& str);

uint        WINAPI AnsiToWCharStr(const char* source, wchar_t* dest, size_t destSize);
uint        WINAPI WCharToAnsiStr(const wchar_t* source, char* dest, size_t destSize);
char*              wchartombs(const wchar_t* str);
char*              wchartombs(const wchar_t* sequence, size_t count);
char*              wchartombs(const wstring& str);
