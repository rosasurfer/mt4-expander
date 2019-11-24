#pragma once
#include "expander.h"
#include "struct/mt4/MqlString.h"


char*         WINAPI DoubleQuoteStr(const char* value);
std::istream&        getline(std::istream &is, string &line);
const char*   WINAPI GetStringA(const char* value);
const wchar*  WINAPI GetStringW(const wchar* value);

const char*   WINAPI InputParamsDiff(const char* initial, const char* current);

BOOL          WINAPI SortMqlStringsA(MqlStringA strings[], int size);
BOOL          WINAPI SortMqlStringsW(MqlStringW strings[], int size);

BOOL          WINAPI StrCompare(const char* s1, const char* s2);
char*         WINAPI StrFormat(const char* format, ...);
BOOL          WINAPI StrIsNull(const char* value);
BOOL          WINAPI StrStartsWith(const char* str, const char* prefix);
BOOL          WINAPI StrStartsWith(const wchar* str, const wchar* prefix);
BOOL          WINAPI StrEndsWith(const char* str, const char* suffix);
char*         WINAPI StrToLower(char* const str);
string&       WINAPI StrToLower(string &str);
wstring&      WINAPI StrToLower(wstring &str);
char*         WINAPI StrToUpper(char* const str);
string&       WINAPI StrToUpper(string &str);
wstring&      WINAPI StrToUpper(wstring &str);

char*         WINAPI StrLTrim(char* const str);
char*         WINAPI StrRTrim(char* const str);
char*         WINAPI StrTrim(char* const str);

uint          WINAPI AnsiToWCharStr(const char* source, wchar* dest, uint destSize);
uint          WINAPI WCharToAnsiStr(const wchar* source, char* dest, uint destSize);


namespace rsf {
   inline wchar* WINAPI copywchars(const wchar* str) { return(wcscpy(new wchar[wcslen(str)+1], str)); };
   char*         WINAPI strformat(const char* format, ...);
   char*         WINAPI strformat(const char* format, const va_list &args);
   char*         WINAPI wchartombs(const wchar* str);
   char*         WINAPI wchartombs(const wchar* sequence, uint count);
   char*         WINAPI wchartombs(const wstring &str);
}
