#pragma once
#include "expander.h"
#include "struct/mt4/MqlString.h"
#include <sstream>

#define mbslen       _mbslen           // length of a UTF-8 string (doesn't check for invalid UTF-8 chars)
#define mbstrlen     _mbstrlen         // length of a UTF-8 string (checks for invalid UTF-8 chars)
#define wstrlen      wcslen            // length of a UTF-16 string

#define sdup         _strdup           // duplicate a C string
#define mbsdup       _mbsdup           // duplicate a UTF-8 string
#define wsdup        _wcsdup           // duplicate a UTF-16 string

#define vscprintf    _vscprintf        // count C chars of the resulting string using a var-list of arguments
#define vwscprintf   _vscwprintf       // count UTF-16 chars of the resulting string using a var-list of arguments
#define snprintf     _snprintf         // write formatted data to a sized C string buffer
#define vwsprintf_s  vswprintf_s       // write formatted data to a UTF-16 string buffer using a var-list of arguments

#define strtoint     atoi              // convert a C string to an integer
#define wstrtoint    _wtoi             // convert a UTF-16 string to an integer

#define wstrcmp      wcscmp            // case-sensitive comparison of two UTF-16 strings
#define wstricmp     wcsicmp           // case-insensitive comparison of two UTF-16 strings


/**
 * C++11 to_string() replacement for VS 2008.
 *
 * @param  T value
 *
 * @return string
 *
 * Note: The returned string is a temporary object that will be destructed at the end of the expression.
 */
template <typename T>
string to_string(T value) {
   std::ostringstream os;
   os << value;
   return(os.str());
}


char*        WINAPI DoubleQuoteStr(const char* value);
wchar*       WINAPI DoubleQuoteStr(const wchar* value);
const char*  WINAPI GetStringA(const char* value);
const wchar* WINAPI GetStringW(const wchar* value);

BOOL         WINAPI SortMqlStringsA(MqlStringA strings[], int size);
BOOL         WINAPI SortMqlStringsW(MqlStringW strings[], int size);

BOOL         WINAPI StrCompare(const char* s1, const char* s2);
BOOL         WINAPI StrCompare(const wchar* s1, const wchar* s2);
BOOL         WINAPI StrIsNull(const char* value);
BOOL         WINAPI StrStartsWith(const char* str, const char* prefix);
BOOL         WINAPI StrStartsWith(const wchar* str, const wchar* prefix);
BOOL         WINAPI StrEndsWith(const char* str, const char* suffix);
BOOL         WINAPI StrEndsWith(const wchar* str, const wchar* suffix);
BOOL         WINAPI StrEndsWithI(const char* str, const char* suffix);
BOOL         WINAPI StrEndsWithI(const wchar* str, const wchar* suffix);
string&      WINAPI StrReplace(string &subject, const string &search, const string &replace, size_t count=INT_MAX);
char*        WINAPI StrToLower(char* const str);
string&      WINAPI StrToLower(string &str);
wstring&     WINAPI StrToLower(wstring &str);
char*        WINAPI StrToUpper(char* const str);
string&      WINAPI StrToUpper(string &str);
wstring&     WINAPI StrToUpper(wstring &str);

char*        WINAPI StrTrim(char* const str);
char*        WINAPI StrTrimLeft(char* const str);
char*        WINAPI StrTrimRight(char* const str);

string       WINAPI ansiToUtf8 (const  string &str);
wstring      WINAPI ansiToUtf16(const  string &str);
string       WINAPI utf8ToAnsi (const  string &str);
wstring      WINAPI utf8ToUtf16(const  string &str);
string       WINAPI utf16ToAnsi(const wstring &str);
string       WINAPI utf16ToUtf8(const wstring &str);

 char*      __cdecl asformat(const char* format, ...);                        // only __cdecl supports variadics
wchar*      __cdecl asformat(const wchar* format, ...);
 char*       WINAPI _asformat(const char* format, const va_list &args);
wchar*       WINAPI _asformat(const wchar* format, const va_list &args);

std::istream& getline(std::istream &is, string &line);
