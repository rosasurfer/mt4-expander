#include "expander.h"
#include "lib/string.h"
#include "struct/mt4/MqlString.h"

#include <cctype>


/**
 * Wrap a C string in double quote characters.
 *
 * @param  char* value
 *
 * @return char* - wrapped string or the string "(null)" if a NULL pointer was passed
 */
char* WINAPI DoubleQuoteStr(const char* value) {
   if (!value) return sdup("(null)");
   return asformat("\"%s\"", value);
}


/**
 * Wrap a UTF-16 string in double quote characters.
 *
 * @param  wchar* value
 *
 * @return wchar* - wrapped string or the string "(null)" if a NULL pointer was passed
 */
wchar* WINAPI DoubleQuoteStr(const wchar* value) {
   if (!value) return wsdup(L"(null)");
   return asformat(L"\"%s\"", value);
}


/**
 * Dropin-replacement for std::getline(). Read the next line from an input stream auto-detecting standard line endings.
 *
 * @param  istream &is   - input stream
 * @param  string  &line - string into which the next line is read
 *
 * @return istream& - the same input stream
 */
std::istream& getline(std::istream &is, std::string &line) {
   // CR     = 0D     = 13       = \r       Mac
   // LF     = 0A     = 10       = \n       Linux
   // CRLF   = 0D0A   = 13,10    = \r\n     Windows
   // CRCRLF = 0D0D0A = 13,13,10 = \r\r\n   TODO: Netscape, Windows XP Notepad bug
   //
   // The characters in the stream are read one-by-one using std::streambuf. This is faster than reading them one-by-one using
   // std::istream. Code that uses streambuf this way must be guarded by a sentry object. The sentry object performs various
   // tasks, such as thread synchronization and updating the stream state.
   //
   // @see  http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf/6089413#6089413
   //
   std::istream::sentry se(is, true);                    // without it even a single thread might run into an infinite loop
   std::streambuf* sb = is.rdbuf();
   line.clear();

   for (;;) {
      int ch = sb->sbumpc();

      if (ch == '\n')
         break;
      if (ch == '\r') {
         if (sb->sgetc() == '\n') sb->sbumpc();
         break;
      }
      if (ch == EOF) {                                   // handle the case when the last line has no line ending
         if (line.empty()) is.setstate(std::ios::eofbit);
         break;
      }
      line += (char)ch;
   }
   return(is);

   /*
   char* fileName = "terminal-config.ini";
   std::ifstream fs(fileName);
   if (fs) {
      string line;
      uint n = 0;

      debug("reading file \"%s\"...", fileName);
      while (!getline(fs, line).eof()) {
         ++n;
         debug("line %d: %s (%d)", n, line.c_str(), line.length());
      }
      fs.close();
      debug("file contains %d line(s)", n);
   }
   else {
      error(ERR_FILE_CANNOT_OPEN, "cannot open file \"%s\"", fileName);
   }
   */
}


/**
 * Return the pointer to a C string. Essentially this is a no-op used in MQL to convert a C string to it's MQL
 * representation. The terminal will allocate new memory and copy the passed string to the resulting MQL string.
 *
 * @param  char* value - string
 *
 * @return char* - the same string or NULL in case of errors
 */
const char* WINAPI GetStringA(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));
   return value;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the pointer to a UTF-16 string. Essentially this is a no-op used in MQL to convert a UTF-16 string to it's MQL
 * representation. The terminal will allocate new memory and copy the passed string to the resulting MQL string.
 *
 * @param  wchar* value - UTF-16 string
 *
 * @return wchar* - the same string or NULL in case of errors
 */
const wchar* WINAPI GetStringW(const wchar* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));
   return value;
   #pragma EXPANDER_EXPORT
}


/**
 * Compare two ANSI MqlStrings.
 *
 * @param  void* first
 * @param  void* second
 *
 * @return int - positive value if the first string is considered greater than the second;
 *               negative value if the first string is considered smaller than the second;
 *               0 if both strings are considered equal
 */
int __cdecl CompareMqlStringsA(const void* first, const void* second) {
   MqlStringA* s1 = (MqlStringA*) first;
   MqlStringA* s2 = (MqlStringA*) second;

   if (s1 == s2) return( 0);
   if (!s1)      return(-1);
   if (!s2)      return(+1);
   return(strcmp(s1->value, s2->value));
}


/**
 * Compare two UTF-16 MqlStrings.
 *
 * @param  void* first
 * @param  void* second
 *
 * @return int - positive value if the first string is considered greater than the second;
 *               negative value if the first string is considered lower than the second;
 *               0 if both strings are considered equal
 */
int __cdecl CompareMqlStringsW(const void* first, const void* second) {
   MqlStringW* s1 = (MqlStringW*) first;
   MqlStringW* s2 = (MqlStringW*) second;

   if (s1 == s2) return( 0);
   if (!s1)      return(-1);
   if (!s2)      return(+1);
   return(wstrcmp(s1->value, s2->value));
}


/**
 * Sort an MqlStringA array.
 *
 * @param  MqlStringA strings[] - array
 * @param  int        size      - size of array
 *
 * @return BOOL - success status
 */
BOOL WINAPI SortMqlStringsA(MqlStringA strings[], int size) {
   if ((uint)strings < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter strings: 0x%p (not a valid pointer)", strings));
   if (size <= 0)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (size == 1) return(TRUE);           // nothing to do

   qsort(strings, size, sizeof(MqlStringA), CompareMqlStringsA);
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Sort an MqlStringW array.
 *
 * @param  MqlStringW strings[] - array
 * @param  int        size      - size of array
 *
 * @return BOOL - success status
 */
BOOL WINAPI SortMqlStringsW(MqlStringW strings[], int size) {
   if ((uint)strings < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter strings: 0x%p (not a valid pointer)", strings));
   if (size <= 0)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (size == 1) return(TRUE);           // nothing to do

   qsort(strings, size, sizeof(MqlStringW), CompareMqlStringsW);
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether two C strings are considered equal. Convenient helper to hide the non-intuitive strcmp() syntax.
 *
 * @param  char* s1
 * @param  char* s2
 *
 * @return BOOL
 */
BOOL WINAPI StrCompare(const char* s1, const char* s2) {
   if (s1 == s2)   return(TRUE);          // if pointers are equal values are too
   if (!s1 || !s2) return(FALSE);         // if one is a NULL pointer the other can't
   return(strcmp(s1, s2) == 0);           // both are not NULL pointers
   #pragma EXPANDER_EXPORT
}


/**
 * Whether two UTF-16 strings are considered equal. Convenient helper to hide the non-intuitive strcmp() syntax.
 *
 * @param  wchar* s1
 * @param  wchar* s2
 *
 * @return BOOL
 */
BOOL WINAPI StrCompare(const wchar* s1, const wchar* s2) {
   if (s1 == s2)   return(TRUE);          // if pointers are equal values are too
   if (!s1 || !s2) return(FALSE);         // if one is a NULL pointer the other can't
   return(wstrcmp(s1, s2) == 0);          // both are not NULL pointers
}


/**
 * Whether a C string is initialized or a NULL pointer.
 *
 * @param  char* value - the string to check
 *
 * @return BOOL
 */
BOOL WINAPI StrIsNull(const char* value) {
   return !value;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a C string starts with the specified substring.
 *
 * @param  char* str
 * @param  char* prefix
 *
 * @return BOOL
 */
BOOL WINAPI StrStartsWith(const char* str, const char* prefix) {
   if (!str)          return(FALSE);
   if (!prefix)       return(!error(ERR_INVALID_PARAMETER, "invalid parameter prefix: %s", prefix));
   if (str == prefix) return(TRUE);       // if pointers are equal values are too

   uint strLen    = strlen(str);
   uint prefixLen = strlen(prefix);
   if (!prefixLen) return(!error(ERR_INVALID_PARAMETER, "invalid parameter prefix: \"\""));

   if (strLen >= prefixLen)
      return(strncmp(str, prefix, prefixLen) == 0);
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a UTF-16 string starts with the specified substring.
 *
 * @param  wchar* str
 * @param  wchar* prefix
 *
 * @return BOOL
 */
BOOL WINAPI StrStartsWith(const wchar* str, const wchar* prefix) {
   if (!str)          return(FALSE);
   if (!prefix)       return(!error(ERR_INVALID_PARAMETER, "invalid parameter prefix: %S", prefix));
   if (str == prefix) return(TRUE);       // if pointers are equal values are too

   uint strLen    = wstrlen(str);
   uint prefixLen = wstrlen(prefix);
   if (!prefixLen) return(!error(ERR_INVALID_PARAMETER, "invalid parameter prefix: \"\""));

   if (strLen >= prefixLen)
      return(wcsncmp(str, prefix, prefixLen) == 0);
   return(FALSE);
}


/**
 * Whether a C string ends with the specified substring (case-sensitive).
 *
 * @param  char* str
 * @param  char* suffix
 *
 * @return BOOL
 */
BOOL WINAPI StrEndsWith(const char* str, const char* suffix) {
   if (!str)          return(FALSE);
   if (!suffix)       return(!error(ERR_INVALID_PARAMETER, "invalid parameter suffix: %s", suffix));
   if (str == suffix) return(TRUE);       // if pointers are equal values are too

   uint strLen    = strlen(str);
   uint suffixLen = strlen(suffix);
   if (!suffixLen) return(!error(ERR_INVALID_PARAMETER, "invalid parameter suffix: \"\""));

   if (strLen >= suffixLen)
      return(strcmp(str + strLen - suffixLen, suffix) == 0);
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a C string ends with the specified substring (case-insensitive).
 *
 * @param  char* str
 * @param  char* suffix
 *
 * @return BOOL
 */
BOOL WINAPI StrEndsWithI(const char* str, const char* suffix) {
   if (!str)          return(FALSE);
   if (!suffix)       return(!error(ERR_INVALID_PARAMETER, "invalid parameter suffix: %s", suffix));
   if (str == suffix) return(TRUE);       // if pointers are equal values are too

   uint strLen    = strlen(str);
   uint suffixLen = strlen(suffix);
   if (!suffixLen) return(!error(ERR_INVALID_PARAMETER, "invalid parameter suffix: \"\""));

   if (strLen >= suffixLen)
      return(stricmp(str + strLen - suffixLen, suffix) == 0);
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a UTF-16 string ends with the specified substring (case-sensitive).
 *
 * @param  wchar* str
 * @param  wchar* suffix
 * @param  BOOL   ignoreCase [optional] - whether to ignore case (default: no)
 *
 * @return BOOL
 */
BOOL WINAPI StrEndsWith(const wchar* str, const wchar* suffix) {
   if (!str)          return(FALSE);
   if (!suffix)       return(!error(ERR_INVALID_PARAMETER, "invalid parameter suffix: %S", suffix));
   if (str == suffix) return(TRUE);       // if pointers are equal values are too

   uint strLen    = wstrlen(str);
   uint suffixLen = wstrlen(suffix);
   if (!suffixLen) return(!error(ERR_INVALID_PARAMETER, "invalid parameter suffix: \"\""));

   if (strLen >= suffixLen)
      return(wstrcmp(str + strLen - suffixLen, suffix) == 0);
   return(FALSE);
}


/**
 * Whether a UTF-16 string ends with the specified substring (case-insensitive).
 *
 * @param  wchar* str
 * @param  wchar* suffix
 *
 * @return BOOL
 */
BOOL WINAPI StrEndsWithI(const wchar* str, const wchar* suffix) {
   if (!str)          return(FALSE);
   if (!suffix)       return(!error(ERR_INVALID_PARAMETER, "invalid parameter suffix: %S", suffix));
   if (str == suffix) return(TRUE);       // if pointers are equal values are too

   uint strLen    = wstrlen(str);
   uint suffixLen = wstrlen(suffix);
   if (!suffixLen) return(!error(ERR_INVALID_PARAMETER, "invalid parameter suffix: \"\""));

   if (strLen >= suffixLen)
      return(wstricmp(str + strLen - suffixLen, suffix) == 0);
   return(FALSE);
}


/**
 * Replace a substring of a string by another substring. The function modifies the string.
 *
 * @param  _InOut_ string &subject         - string to process
 * @param  _In_    string &search          - search string
 * @param  _In_    string &replace         - replacement string
 * @param  _In_    size_t count [optional] - max. number of replacements to perform (default: no limit)
 *
 * @return string& - the same string
 */
string& WINAPI StrReplace(string &subject, const string &search, const string &replace, size_t count/*=INT_MAX*/) {
   size_t subjectLen = subject.length();
   if (!subjectLen) return(subject);

   size_t searchLen = search.length();
   if (!searchLen) return(subject);

   size_t replaceLen = replace.length();
   if (subjectLen < searchLen || !search.compare(replace)) return(subject);

   size_t replacements = 0, pos = 0;
   while (replacements < count && (pos = subject.find(search, pos)) != string::npos) {
      if (searchLen == replaceLen) {
         subject.replace(pos, searchLen, replace);    // use replace() if both are of same length...
      }
      else {
         subject.erase(pos, searchLen);               // ...otherwise use erase() + insert()
         subject.insert(pos, replace);
      }
      pos += replaceLen;
      ++replacements;
   }
   return(subject);
}


/**
 * Convert a C string to lower-case. The function modifies the string.
 *
 * @param  _InOut_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI strToLower(char* str) {
   if (str) {
      char* c = str;
      while (*c) {
         *c = (char) tolower((uchar)*c);
         c++;
      }
   }
   return str;
}


/**
 * Convert a UTF-16 string to lower-case. The function modifies the string.
 *
 * @param  _InOut_ wchar* str
 *
 * @return wchar* - the same string
 */
wchar* WINAPI strToLower(wchar* str) {
   if (str) {
      wchar* c = str;
      while (*c) {
         *c = towlower(*c);
         c++;
      }
   }
   return str;
}


/**
 * Convert a std::string to lower-case. The function modifies the string.
 *
 * @param  _InOut_ string &str
 *
 * @return string& - the same string
 */
string& WINAPI strToLower(string &str) {
   for (size_t i=0, n=str.length(); i < n; i++) {
      str[i] = (char) tolower((uchar)str[i]);
   }
   return str;
}


/**
 * Convert a std::wstring to lower-case. The function modifies the string.
 *
 * @param  _InOut_ wstring &str
 *
 * @return wstring& - the same string
 */
wstring& WINAPI strToLower(wstring &str) {
   for (size_t i=0, n=str.length(); i < n; i++) {
      str[i] = towlower(str[i]);
   }
   return(str);
}


/**
 * Convert a C string to upper-case. The function modifies the string.
 *
 * @param  _InOut_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI strToUpper(char* str) {
   if (str) {
      char* c = str;
      while (*c) {
         *c = (char) toupper((uchar)*c);
         c++;
      }
   }
   return str;
}


/**
 * Convert a UTF-16 string to upper-case. The function modifies the string.
 *
 * @param  _InOut_ wchar* str
 *
 * @return wchar* - the same string
 */
wchar* WINAPI strToUpper(wchar* str) {
   if (str) {
      wchar* c = str;
      while (*c) {
         *c = towupper(*c);
         c++;
      }
   }
   return str;
}


/**
 * Convert a std::string to upper-case. The function modifies the string.
 *
 * @param  _InOut_ string &str
 *
 * @return string& - the same string
 */
string& WINAPI strToUpper(string &str) {
   for (size_t i=0, n=str.length(); i < n; i++) {
      str[i] = (char) toupper((uchar)str[i]);
   }
   return str;
}


/**
 * Convert a std::wstring to upper-case. The function modifies the string.
 *
 * @param  _InOut_ wstring &str
 *
 * @return wstring& - the same string
 */
wstring& WINAPI strToUpper(wstring &str) {
   for (size_t i=0, n=str.length(); i < n; i++) {
      str[i] = towupper(str[i]);
   }
   return str;
}


/**
 * Trim leading and trailing white-space off a C string. The function modifies the string.
 *
 * @param  _InOut_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI strim(char* str) {
   return(strim_left(strim_right(str)));
}


/**
 * Trim leading and trailing white-space off a std::string. The function modifies the string.
 *
 * @param  string &str
 *
 * @return string& - the same string
 */
string& WINAPI strim(string &str) {
   return(strim_left(strim_right(str)));
}


/**
 * Trim leading white-space off a C string. The function modifies the string.
 *
 * @param  _InOut_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI strim_left(char* str) {
   if (!str || !*str) return str;               // handle NULL pointer and empty string

   char* first = str;
   while (isspace((uchar)*first)) {
      ++first;
   }
   if (first != str) {
      memmove(str, first, strlen(first) + 1);   // include null terminator
   }
   return str;
}


/**
* Trim leading white-space off a std::string. The function modifies the string.
 *
 * @param  string &str
 *
 * @return string& - the same string
 */
string& WINAPI strim_left(string &str) {
   size_t start=0, size=str.length();
   while (start < size && isspace((uchar)(str[start]))) {
      ++start;
   }
   if (start) str.erase(0, start);

   return str;
}


/**
 * Trim trailing white-space off a C string. The function modifies the string.
 *
 * @param  _InOut_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI strim_right(char* str) {
   if (!str || !*str) return str;               // handle NULL pointer and empty string

   char* last = str + strlen(str);
   while (last > str && isspace((uchar)*(last-1))) {
      --last;
   }
   *last = '\0';
   return str;
}


/**
* Trim trailing white-space off a std::string. The function modifies the string.
 *
 * @param  string &str
 *
 * @return string& - the same string
 */
string& WINAPI strim_right(string &str) {
   size_t size=str.length(), end=size;

   while (end && isspace((uchar)(str[end-1]))) {
      --end;
   }
   if (end < size) str.erase(end);
   return str;
}


/**
 * Trim leading and trailing white-space off a UTF-16 string. The function modifies the string.
 *
 * @param  _InOut_ wchar* str
 *
 * @return wchar* - the same string
 */
wchar* WINAPI wstrim(wchar* str) {
   return(wstrim_left(wstrim_right(str)));
}


/**
 * Trim leading and trailing white-space off a std::wstring. The function modifies the string.
 *
 * @param  wstring &str
 *
 * @return wstring& - the same string
 */
wstring& WINAPI wstrim(wstring &str) {
   return(wstrim_left(wstrim_right(str)));
}


/**
 * Trim leading white-space off a UTF-16 string. The function modifies the string.
 *
 * @param  _InOut_ wchar* str
 *
 * @return wchar* - the same string
 */
wchar* WINAPI wstrim_left(wchar* str) {
   if (!str || !*str) return str;               // handle NULL pointer and empty string

   wchar* first = str;
   while (iswspace(*first)) {
      ++first;
   }
   if (first != str) {                          // include null terminator
      memmove(str, first, (wstrlen(first) + 1) * sizeof(wchar));
   }
   return str;
}


/**
 * Trim leading white-space off a std::wstring. The function modifies the string.
 *
 * @param  wstring &str
 *
 * @return wstring& - the same string
 */
wstring& WINAPI wstrim_left(wstring &str) {
   size_t start=0, size=str.length();
   while (start < size && iswspace(str[start])) {
      ++start;
   }
   if (start) str.erase(0, start);
   return str;
}


/**
 * Trim trailing white-space off a UTF-16 string. The function modifies the string.
 *
 * @param  _InOut_ wchar* str
 *
 * @return wchar* - the same string
 */
wchar* WINAPI wstrim_right(wchar* str) {
   if (!str || !*str) return str;               // handle NULL pointer and empty string

   wchar* last = str + wcslen(str);
   while (last > str && iswspace(*(last-1))) {
      --last;
   }
   *last = L'\0';
   return str;
}


/**
 * Trim trailing white-space off a std::wstring. The function modifies the string.
 *
 * @param  wstring &str
 *
 * @return wstring& - the same string
 */
wstring& WINAPI wstrim_right(wstring &str) {
   size_t length=str.length(), end=length;

   while (end && iswspace(str[end-1])) {
      --end;
   }
   if (end < length) str.erase(end);
   return str;
}


/**
 * Convert an ANSI string to UTF-8.
 *
 * @param  char* str - ANSI string
 *
 * @return char* - UTF-8 string or NULL in case of errors
 */
char* WINAPI ansiToUtf8(const char* str) {
   return utf16ToUtf8(ansiToUtf16(str));
}


/**
 * Convert an ANSI string to UTF-8.
 *
 * @param  string &str - ANSI string
 *
 * @return string - UTF-8 string or an empty string in case of errors
 */
string WINAPI ansiToUtf8(const string &str) {
   return utf16ToUtf8(ansiToUtf16(str));
}


/**
 * Convert an ANSI string to UTF-16.
 *
 * @param  char* str - ANSI C string
 *
 * @return wchar* - UTF-16 string or NULL in case of errors
 */
wchar* WINAPI ansiToUtf16(const char* str) {
   if (!str) return NULL;

   size_t length = strlen(str);
   if (!length) return wsdup(L"");

   uint codePage = CP_ACP;
   DWORD flags = MB_ERR_INVALID_CHARS;

   int bufSize = MultiByteToWideChar(codePage, flags, str, length, NULL, 0);
   if (bufSize) {
      wchar* wstr = (wchar*) malloc((bufSize+1) * sizeof(wchar));

      if (MultiByteToWideChar(codePage, flags, str, length, wstr, bufSize)) {
         wstr[bufSize] = L'\0';
         return wstr;
      }
      free(wstr);
   }
   error(ERR_WIN32_ERROR+GetLastError(), "cannot convert ANSI string to UTF-16");
   return NULL;
}


/**
 * Convert an ANSI string to UTF-16.
 *
 * @param  string &str - ANSI string
 *
 * @return wstring - UTF-16 string or an empty string in case of errors
 */
wstring WINAPI ansiToUtf16(const string &str) {
   size_t length = str.length();
   if (!length) return wstring(L"");

   uint codePage = CP_ACP;
   DWORD flags = MB_ERR_INVALID_CHARS;

   int bufSize = MultiByteToWideChar(codePage, flags, &str[0], length, NULL, 0);
   if (bufSize) {
      wstring wstr(bufSize + 1, 0);
      if (MultiByteToWideChar(codePage, flags, &str[0], length, &wstr[0], bufSize)) {
         return wstr;
      }
   }
   error(ERR_WIN32_ERROR+GetLastError(), "cannot convert ANSI string to UTF-16");
   return wstring(L"");
}


/**
 * Convert a UTF-8 string to ANSI.
 *
 * @param  char* str - UTF-8 string
 *
 * @return char* - ANSI string or NULL in case of errors
 */
char* WINAPI utf8ToAnsi(const char* str) {
   return utf16ToAnsi(utf8ToUtf16(str));
}


/**
 * Convert a UTF-8 string to ANSI.
 *
 * @param  string &str - UTF-8 string
 *
 * @return string - ANSI string or an empty string in case of errors
 */
string WINAPI utf8ToAnsi(const string &str) {
   return utf16ToAnsi(utf8ToUtf16(str));
}


/**
 * Convert a UTF-8 string to UTF-16.
 * * *
 * @param  char* str - UTF-8 string
 *
 * @return wchar* - UTF-16 string or NULL in case of errors
 */
wchar* WINAPI utf8ToUtf16(const char* str) {
   if (!str) return NULL;

   size_t length = strlen(str);
   if (!length) return wsdup(L"");

   uint codePage = CP_UTF8;
   DWORD flags = MB_ERR_INVALID_CHARS;

   int bufSize = MultiByteToWideChar(codePage, flags, str, length, NULL, 0);
   if (bufSize) {
      wchar* wstr = (wchar*) malloc((bufSize+1) * sizeof(wchar));

      if (MultiByteToWideChar(codePage, flags, str, length, wstr, bufSize)) {
         wstr[bufSize] = L'\0';
         return wstr;
      }
      free(wstr);
   }
   error(ERR_WIN32_ERROR+GetLastError(), "cannot convert UTF-8 string to UTF-16");
   return NULL;
}


/**
 * Convert a UTF-8 string to UTF-16.
 *
 * @param  string &str - UTF-8 string
 *
 * @return wstring - UTF-16 string or an empty string in case of errors
 */
wstring WINAPI utf8ToUtf16(const string &str) {
   size_t length = str.length();
   if (!length) return wstring(L"");

   uint codePage = CP_UTF8;
   DWORD flags = MB_ERR_INVALID_CHARS;

   int bufSize = MultiByteToWideChar(codePage, flags, &str[0], length, NULL, 0);
   if (bufSize) {
      wstring wstr(bufSize + 1, 0);
      if (MultiByteToWideChar(codePage, flags, &str[0], length, &wstr[0], bufSize)) {
         return wstr;
      }
   }
   error(ERR_WIN32_ERROR+GetLastError(), "cannot convert UTF-8 string to UTF-16");
   return wstring(L"");
}


/**
 * Convert a UTF-16 string to ANSI.
 *
 * @param  wchar* wstr - UTF-16 string
 *
 * @return char* - ANSI string or NULL in case of errors
 */
char* WINAPI utf16ToAnsi(const wchar* wstr) {
   if (!wstr) return NULL;

   size_t length = wstrlen(wstr);
   if (!length) return sdup("");

   uint codePage = CP_ACP;
   DWORD flags = WC_COMPOSITECHECK;

   int bufSize = WideCharToMultiByte(codePage, flags, wstr, length, NULL, 0, NULL, NULL);
   if (bufSize) {
      char* str = (char*) malloc(bufSize + 1);
      if (WideCharToMultiByte(codePage, flags, wstr, length, str, bufSize, NULL, NULL)) {
         str[bufSize] = '\0';
         return str;
      }
      free(str);
   }
   error(ERR_WIN32_ERROR+GetLastError(), "cannot convert UTF-16 string to ANSI");
   return NULL;
}


/**
 * Convert a UTF-16 string to ANSI.
 *
 * @param  wstring &wstr - UTF-16 string
 *
 * @return string - ANSI string or an empty string in case of errors
 */
string WINAPI utf16ToAnsi(const wstring &wstr) {
   size_t length = wstr.length();
   if (!length) return string("");

   uint codePage = CP_ACP;
   DWORD flags = WC_COMPOSITECHECK;

   int bufSize = WideCharToMultiByte(codePage, flags, &wstr[0], length, NULL, 0, NULL, NULL);
   if (bufSize) {
      string str(bufSize + 1, 0);
      if (WideCharToMultiByte(codePage, flags, &wstr[0], length, &str[0], bufSize, NULL, NULL)) {
         return str;
      }
   }
   error(ERR_WIN32_ERROR+GetLastError(), "cannot convert UTF-16 string to ANSI");
   return string("");
}


/**
 * Convert a UTF-16 string to UTF-8.
 *
 * @param  wchar* wstr - UTF-16 string
 *
 * @return char* UTF-8 string or NULL in case of errors
 */
char* WINAPI utf16ToUtf8(const wchar* wstr) {
   if (!wstr) return NULL;

   size_t length = wstrlen(wstr);
   if (!length) return sdup("");

   uint codePage = CP_UTF8;
   DWORD flags = WC_COMPOSITECHECK | WC_ERR_INVALID_CHARS;

   int bufSize = WideCharToMultiByte(codePage, flags, wstr, length, NULL, 0, NULL, NULL);
   if (bufSize) {
      char* str = (char*) malloc(bufSize + 1);
      if (WideCharToMultiByte(codePage, flags, wstr, length, str, bufSize, NULL, NULL)) {
         return str;
      }
      free(str);
   }
   error(ERR_WIN32_ERROR+GetLastError(), "cannot convert UTF-16 string to UTF-8");
   return NULL;
}


/**
 * Convert a UTF-16 string to UTF-8.
 *
 * @param  wstring &wstr - UTF-16 string
 *
 * @return string - UTF-8 string or an empty string in case of errors
 */
string WINAPI utf16ToUtf8(const wstring &wstr) {
   size_t length = wstr.length();
   if (!length) return string("");

   uint codePage = CP_UTF8;
   DWORD flags = WC_COMPOSITECHECK | WC_ERR_INVALID_CHARS;

   int bufSize = WideCharToMultiByte(codePage, flags, &wstr[0], length, NULL, 0, NULL, NULL);
   if (bufSize) {
      string str(bufSize + 1, 0);
      if (WideCharToMultiByte(codePage, flags, &wstr[0], length, &str[0], bufSize, NULL, NULL)) {
         return str;
      }
   }
   error(ERR_WIN32_ERROR+GetLastError(), "cannot convert UTF-16 string to UTF-8");
   return string("");
}


/**
 * Write formatted data to a C string and return the resulting string. Similar to GNU-C asprintf() this function allocates the
 * memory for the string itself.
 *
 * @param  char* format - string with format codes (see Microsoft extension for ANSI/UTF-16 specific codes)
 * @param        ...    - variable number of arguments (may contain UTF-16 strings)
 *
 * @return char* - formatted string or a NULL pointer in case of errors
 *
 * Note: The caller is responsible for releasing the string's memory after usage with "free()".
 *
 * @see  https://learn.microsoft.com/en-us/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=msvc-170
 * @see  https://www.tutorialspoint.com/format-specifiers-in-c
 */
char* __cdecl asformat(const char* format, ...) {
   if (!format)  return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: (null)"));
   if (!*format) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: \"\" (empty)"));

   va_list args;
   va_start(args, format);
   char* result = _asformat(format, args);
   va_end(args);

   return(result);
}


/**
 * Write formatted data to a UTF-16 string and return the resulting string. Similar to GNU-C asprintf() this
 * function allocates the memory for the string itself.
 *
 * @param  wchar* format - string with format codes (see Microsoft extension for ANSI/UTF-16 specific codes)
 * @param         ...    - variable number of arguments (may contain ANSI strings)
 *
 * @return wchar* - formatted string or a NULL pointer in case of errors
 *
 * Note: The caller is responsible for releasing the string's memory after usage with "free()".
 *
 * @see  https://learn.microsoft.com/en-us/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=msvc-170
 * @see  https://www.tutorialspoint.com/format-specifiers-in-c
 */
wchar* __cdecl asformat(const wchar* format, ...) {
   if (!format)  return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: (null)"));
   if (!*format) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: \"\" (empty)"));

   va_list args;
   va_start(args, format);
   wchar* result = _asformat(format, args);
   va_end(args);

   return(result);
}


/**
 * Write formatted data to a C string and return the resulting string. Similar to GNU-C asprintf() this function allocates the
 * memory for the string itself.
 *
 * @param  char*   format - string with format codes (see Microsoft extension for ANSI/UTF-16 specific codes)
 * @param  va_list &args  - variable list of arguments (may contain UTF-16 strings)
 *
 * @return char* - formatted string or a NULL pointer in case of errors
 *
 * Note: The caller is responsible for releasing the string's memory after usage with "free()".
 *
 * @see  https://learn.microsoft.com/en-us/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=msvc-170
 * @see  https://www.tutorialspoint.com/format-specifiers-in-c
 */
char* WINAPI _asformat(const char* format, const va_list &args) {
   if (!format)  return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: (null)"));
   if (!*format) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: \"\" (empty)"));

   uint size = vscprintf(format, args) + 1;        // +1 for the terminating null char
   char* buffer = (char*)malloc(size);
   if (buffer) {
      vsprintf_s(buffer, size, format, args);
   }
   return(buffer);
}


/**
 * Write formatted data to a UTF-16 string and return the resulting string. Similar to GNU-C asprintf() this
 * function allocates the memory for the string itself.
 *
 * @param  wchar*  format - string with format codes (see Microsoft extension for ANSI/UTF-16 specific codes)
 * @param  va_list &args  - variable list of arguments (may contain ANSI strings)
 *
 * @return wchar* - formatted string or a NULL pointer in case of errors
 *
 * Note: The caller is responsible for releasing the string's memory after usage with "free()".
 *
 * @see  https://learn.microsoft.com/en-us/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=msvc-170
 * @see  https://www.tutorialspoint.com/format-specifiers-in-c
 */
wchar* WINAPI _asformat(const wchar* format, const va_list &args) {
   if (!format)  return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: (null)"));
   if (!*format) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: \"\" (empty)"));

   uint size = vwscprintf(format, args) + 1;       // +1 for the terminating null wchar
   wchar* buffer = (wchar*)malloc(size*2);
   if (buffer) {
      vwsprintf_s(buffer, size, format, args);
   }
   return(buffer);
}
