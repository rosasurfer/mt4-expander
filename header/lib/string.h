#pragma once
#include "expander.h"
#include "struct/mt4/MqlStr.h"


const char*   WINAPI DoubleQuoteStr(const char* value);
string        WINAPI doubleQuoteStr(const char* value);
string        WINAPI doubleQuoteStr(const string& value);

std::istream&        getline(std::istream& is, string& line);

const char*   WINAPI GetString(const char* value);
uint          WINAPI GetStringAddress(const char* value);
uint          WINAPI GetStringsAddress(const MqlStr values[]);

const char*   WINAPI InputParamsDiff(const char* initial, const char* current);

BOOL          WINAPI StrCompare(const char* s1, const char* s2);
BOOL          WINAPI StrIsNull(const char* value);
BOOL          WINAPI StrStartsWith(const char* str, const char* prefix);
BOOL          WINAPI StrStartsWith(const wchar_t* str, const wchar_t* prefix);
BOOL          WINAPI StrEndsWith(const char* str, const char* suffix);
char*         WINAPI StrToLower(char* const str);
string&       WINAPI StrToLower(string& str);
wstring&      WINAPI StrToLower(wstring& str);
char*         WINAPI StrToUpper(char* const str);
string&       WINAPI StrToUpper(string& str);
wstring&      WINAPI StrToUpper(wstring& str);

char*         WINAPI strLTrim(char* const str);
char*         WINAPI strRTrim(char* const str);
char*         WINAPI strTrim(char* const str);

uint          WINAPI AnsiToWCharStr(const char* source, wchar_t* dest, size_t destSize);
uint          WINAPI WCharToAnsiStr(const wchar_t* source, char* dest, size_t destSize);
char*                wchartombs(const wchar_t* str);
char*                wchartombs(const wchar_t* sequence, size_t count);
char*                wchartombs(const wstring& str);

inline WCHAR*        copywchars(const WCHAR* str) { return(wcscpy(new WCHAR[wcslen(str)+1], str)); };
