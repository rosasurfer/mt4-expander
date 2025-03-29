#pragma once
#include "expander.h"
#include "struct/mt4/MqlString.h"
#include <cctype>
#include <sstream>

#define mbslen       _mbslen           // length of a UTF-8 string (doesn't check for invalid UTF-8 chars)
#define mbstrlen     _mbstrlen         // length of a UTF-8 string (checks for invalid UTF-8 chars)
#define wstrlen      wcslen            // length of a UTF-16 string

#define sdup         _strdup           // duplicate a C string on the heap
#define mbsdup       _mbsdup           // duplicate a UTF-8 string on the heap
#define wsdup        _wcsdup           // duplicate a UTF-16 string on the heap

#define vscprintf    _vscprintf        // count C chars of the resulting string using a var-list of arguments
#define vwscprintf   _vscwprintf       // count UTF-16 chars of the resulting string using a var-list of arguments
#define snprintf     _snprintf         // write formatted data to a sized C string buffer
#define vwsprintf_s  vswprintf_s       // write formatted data to a UTF-16 string buffer using a var-list of arguments

#define strtoint     atoi              // convert a C string to an integer
#define wstrtoint    _wtoi             // convert a UTF-16 string to an integer

#define wstrcmp      wcscmp            // case-sensitive comparison of two UTF-16 strings
#define wstricmp     wcsicmp           // case-insensitive comparison of two UTF-16 strings


/**
 * Duplicate a C string on the stack.
 *
 * @param  char* s
 *
 * @return char*
 */
__forceinline char* WINAPI sdupa(const char* s) {
   size_t size = strlen(s) + 1;
   char* copy = (char*) alloca(size);
   return (char*)memcpy(copy, s, size);
}


/**
 * Duplicate a UTF-16 string on the stack.
 *
 * @param  wchar* s
 *
 * @return wchar*
 */
__forceinline wchar* WINAPI wsdupa(const wchar* s) {
   size_t size = (wstrlen(s) + 1) * 2;
   wchar* copy = (wchar*) alloca(size);
   return (wchar*)memcpy(copy, s, size);
}


/**
 * VS2008: C++03 replacement for the C++11 to_string() function.
 *
 * @param  T value
 *
 * @return string
 *
 * Note: The returned string is a temporary object that will be destroyed at the end of the expression.
 */
template <typename T>
string WINAPI to_string(T value) {
   std::ostringstream os;
   os << value;
   return os.str();
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

char*        WINAPI strToLower(char* str);
wchar*       WINAPI strToLower(wchar* str);
string&      WINAPI strToLower(string &str);
wstring&     WINAPI strToLower(wstring &str);

char*        WINAPI strToUpper(char* str);
wchar*       WINAPI strToUpper(wchar* str);
string&      WINAPI strToUpper(string &str);
wstring&     WINAPI strToUpper(wstring &str);

char*        WINAPI strim(char* str);
string&      WINAPI strim(string &str);
char*        WINAPI strim_left(char* str);
string&      WINAPI strim_left(string &str);
char*        WINAPI strim_right(char* str);
string&      WINAPI strim_right(string &str);

wchar*       WINAPI wstrim(wchar* str);
wstring&     WINAPI wstrim(wstring &str);
wchar*       WINAPI wstrim_left(wchar* str);
wstring&     WINAPI wstrim_left(wstring &str);
wchar*       WINAPI wstrim_right(wchar* str);
wstring&     WINAPI wstrim_right(wstring &str);

char*        WINAPI ansiToUtf8(const char* str);
string       WINAPI ansiToUtf8(const string &str);

wchar*       WINAPI ansiToUtf16(const char* str);
wstring      WINAPI ansiToUtf16(const string &str);

char*        WINAPI utf8ToAnsi(const char* str);
string       WINAPI utf8ToAnsi(const string &str);

wchar*       WINAPI utf8ToUtf16(const char* str);
wstring      WINAPI utf8ToUtf16(const string &str);

char*        WINAPI utf16ToAnsi(const wchar* wstr);
string       WINAPI utf16ToAnsi(const wstring &str);

char*        WINAPI utf16ToUtf8(const wchar* wstr);
string       WINAPI utf16ToUtf8(const wstring &str);

char*       __cdecl asformat(const char* format, ...);                        // only __cdecl supports variadics
wchar*      __cdecl asformat(const wchar* format, ...);
char*        WINAPI _asformat(const char* format, const va_list &args);
wchar*       WINAPI _asformat(const wchar* format, const va_list &args);

std::istream& getline(std::istream &is, string &line);
