#include "expander.h"
#include "lib/string.h"
#include "struct/mt4/MqlStr.h"

#include <ctype.h>


/**
 * Wrap a C string in double quote characters.
 *
 * @param  char* value
 *
 * @return char* - wrapped C string or the string "(null)" if a NULL pointer was specified
 */
char* WINAPI DoubleQuoteStr(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));
   if (!value)
      return(StrFormat("(null)", value));
   return(StrFormat("\"%s\"", value));
   #pragma EXPANDER_EXPORT
}


/**
 * Dropin-replacement for std::getline(). Read the next line from an input stream auto-detecting standard line endings.
 *
 * @param  istream& is   - input stream
 * @param  string&  line - string into which the next line is read
 *
 * @return istream& - the same input stream
 */
std::istream& getline(std::istream& is, string& line) {
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
   char* fileName = "local-config.ini";
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
 * Return the passed pointer to an Ansi string. Essentially this is a no-op, used by MQL to read and convert a C string to
 * it's MQL representation. The terminal will allocate new memory and copy the passed string to the resulting MQL string.
 *
 * @param  char* value - Ansi string
 *
 * @return char* - the same string or NULL in case of errors
 */
const char* WINAPI GetStringA(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));
   return(value);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the passed pointer to a Unicode string. Essentially this is a no-op, used by MQL to read and convert a wide-characer
 * string to it's MQL representation. The terminal will allocate new memory and copy the passed string to the resulting MQL
 * string.
 *
 * @param  wchar* value - Unicode string
 *
 * @return wchar* - the same string or NULL in case of errors
 */
const wchar* WINAPI GetStringW(const wchar* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((wchar*)error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));
   return(value);
   #pragma EXPANDER_EXPORT
}


/**
 * Resolve and return the difference between the passed string representations of input parameters.
 *
 * @param  char* initial - initial input parameters
 * @param  char* current - current input parameters
 *
 * @return char* - modified input parameters
 */
const char* WINAPI InputParamsDiff(const char* initial, const char* current) {
   if (initial && (uint)initial < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter initial: 0x%p (not a valid pointer)", initial));
   if (           (uint)current < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter current: 0x%p (not a valid pointer)", current));

   if (!initial || !*initial)                // all current parameters
      return(current);

   // diff input strings
   std::stringstream stInitial(initial), stCurrent(current);
   string lineInitial, lineCurrent, diff;

   while (getline(stInitial, lineInitial)) {
      if (getline(stCurrent, lineCurrent)) {
         if (lineInitial != lineCurrent)
            diff.append(lineCurrent).append(NL);
      }
      else {
         diff.append("REMOVED ").append(lineInitial).append(NL);
      }
   }

   while (getline(stCurrent, lineCurrent)) {
      diff.append("ADDED ").append(lineCurrent).append(NL);
   }

   return(strdup(diff.c_str()));
   #pragma EXPANDER_EXPORT
}


/**
 * Write formatted data to a string similar to sprintf() and return the resulting string. This function is identical to
 * strformat() but registers the allocated memory for the returned string at the internal memory manager. The memory manager
 * will release the memory at a time of its choice but the earliest at the next tick of the currently executed MQL program.
 *
 * @param  char* format - string with format codes
 * @param        ...    - variable number of additional arguments
 *
 * @return char*
 */
char* WINAPI StrFormat(const char* format, ...) {
   if (format && (uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   va_list args;
   va_start(args, format);
   char* result = strformat(format, args);
   va_end(args);

   return(result);                                          // TODO: add to GC (close memory leak)
}


/**
 * Whether two strings are considered equal. Convenient helper to hide the non-intuitive strcmp() syntax.
 *
 * @param  char* a
 * @param  char* b
 *
 * @return BOOL
 */
BOOL WINAPI StrCompare(const char* a, const char* b) {
   if (a == b)   return(TRUE);                              // if pointers are equal values are too
   if (!a || !b) return(FALSE);                             // if one is a NULL pointer the other can't
   return(strcmp(a, b) == 0);                               // both are not NULL pointers
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a C string is initialized or a NULL pointer.
 *
 * @param  char* value - the string to check
 *
 * @return BOOL
 */
BOOL WINAPI StrIsNull(const char* value) {
   return(!value);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a string starts with the specified substring.
 *
 * @param  char* str
 * @param  char* prefix
 *
 * @return BOOL
 */
BOOL WINAPI StrStartsWith(const char* str, const char* prefix) {
   if (!str)          return(FALSE);
   if (!prefix)       return(error(ERR_INVALID_PARAMETER, "invalid parameter prefix: %s", prefix));
   if (str == prefix) return(TRUE);                                  // if pointers are equal values are too

   size_t strLen    = strlen(str);
   size_t prefixLen = strlen(prefix);
   if (!prefixLen) return(error(ERR_INVALID_PARAMETER, "invalid parameter prefix: \"\""));

   if (strLen >= prefixLen)
      return(strncmp(str, prefix, prefixLen) == 0);
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a string starts with the specified substring.
 *
 * @param  wchar* str
 * @param  wchar* prefix
 *
 * @return BOOL
 */
BOOL WINAPI StrStartsWith(const wchar* str, const wchar* prefix) {
   if (!str)          return(FALSE);
   if (!prefix)       return(error(ERR_INVALID_PARAMETER, "invalid parameter prefix: %S", prefix));
   if (str == prefix) return(TRUE);                                  // if pointers are equal values are too

   size_t strLen    = wcslen(str);
   size_t prefixLen = wcslen(prefix);
   if (!prefixLen) return(error(ERR_INVALID_PARAMETER, "invalid parameter prefix: \"\""));

   if (strLen >= prefixLen)
      return(wcsncmp(str, prefix, prefixLen) == 0);
   return(FALSE);
}


/**
 * Whether a string ends with the specified substring.
 *
 * @param  char* str
 * @param  char* suffix
 *
 * @return BOOL
 */
BOOL WINAPI StrEndsWith(const char* str, const char* suffix) {
   if (!str)          return(FALSE);
   if (!suffix)       return(error(ERR_INVALID_PARAMETER, "invalid parameter suffix: %s", suffix));
   if (str == suffix) return(TRUE);                                  // if pointers are equal values are too

   size_t strLen    = strlen(str);
   size_t suffixLen = strlen(suffix);
   if (!suffixLen) return(error(ERR_INVALID_PARAMETER, "invalid parameter suffix: \"\""));

   if (strLen >= suffixLen)
      return(strcmp(str + strLen - suffixLen, suffix) == 0);
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Convert a C string to lower-case.
 *
 * @param  _In_Out_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI StrToLower(char* const str) {
   if (str) {
      char* c = str;
      while (*c) {
         *c = tolower(*c);
         ++c;
      }
   }
   return(str);
}


/**
 * Convert a std::string to lower-case.
 *
 * @param  _In_Out_ string& str
 *
 * @return string& - the same string
 */
string& WINAPI StrToLower(string& str) {
   for (string::iterator it=str.begin(), end=str.end(); it != end; ++it) {
      *it = tolower(*it);
   }
   return(str);
}


/**
 * Convert a std::wstring to lower-case.
 *
 * @param  _In_Out_ wstring& str
 *
 * @return wstring& - the same string
 */
wstring& WINAPI StrToLower(wstring& str) {
   for (wstring::iterator it=str.begin(), end=str.end(); it != end; ++it) {
      *it = towlower(*it);
   }
   return(str);
}


/**
 * Convert a C string to upper-case.
 *
 * @param  _In_Out_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI StrToUpper(char* const str) {
   char* c = str;
   while (*c) {
      *c = toupper(*c);
      ++c;
   }
   return(str);
}


/**
 * Convert a std::string to upper-case.
 *
 * @param  _In_Out_ string& str
 *
 * @return string& - the same string
 */
string& WINAPI StrToUpper(string& str) {
   for (string::iterator it=str.begin(), end=str.end(); it != end; ++it) {
      *it = toupper(*it);
   }
   return(str);
}


/**
 * Convert a std::wstring to upper-case.
 *
 * @param  _In_Out_ wstring& str
 *
 * @return wstring& - the same string
 */
wstring& WINAPI StrToUpper(wstring& str) {
   for (wstring::iterator it=str.begin(), end=str.end(); it != end; ++it) {
      *it = towupper(*it);
   }
   return(str);
}


/**
 * Trim leading white-space from a C string.
 *
 * @param  _In_Out_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI strLTrim(char* const str) {
   if (str && *str) {                           // handle NULL pointer and empty string
      char* first = str;
      while (isspace(*first)) {                 // find first non-space character
         ++first;
      }
      if (first > str) {
         uint pos = 0;
         char* c = first;
         while (*c) {
            str[pos++] = *c;                    // move trimmed string to start of str
            ++c;                                // (should be faster then memmove() as overlapping is handled better)
         }
         str[pos] = '\0';
      }
   }
   return(str);
}


/**
 * Trim trailing white-space from a C string.
 *
 * @param  _In_Out_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI strRTrim(char* const str) {
   if (str && *str) {                           // handle NULL pointer and empty string
      char* last = str + strlen(str);

      while (isspace(*--last)) {
         if (last == str) {
            --last;
            break;
         }
      }
      *(last+1) = '\0';
   }
   return(str);
}


/**
 * Trim leading and trailing white-space from a C string.
 *
 * @param  _In_Out_ char* str
 *
 * @return char* - the same string
 */
char* WINAPI strTrim(char* const str) {
   strRTrim(str);
   strLTrim(str);
   return(str);
}


/**
 * Convert a C Ansi string to a Unicode string (UTF-16). Conversion stops at the end of the Ansi string or when the size
 * limit of the destination buffer is hit, whichever comes first. The resulting string is always null-terminated.
 *
 * @param  _In_  char*   source   - C Ansi or multi-byte UTF-8 source string
 * @param  _Out_ wchar*  dest     - buffer the converted Unicode string is written to
 * @param  _In_  size_t  destSize - size of the destination buffer in bytes
 *
 * @return uint - number of converted characters (equal to the wide-character length of the resulting string)
 */
uint WINAPI AnsiToWCharStr(const char* source, wchar* dest, size_t destSize) {
   if ((uint)source < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter source: 0x%p (not a valid pointer)", source));
   if ((uint)dest   < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter dest: 0x%p (not a valid pointer)", dest));
   if (destSize < 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter destSize: %d (must be non-negative)", destSize));

   uint wchars         = destSize >> 1;
   uint charsToConvert = wchars - 1;
   uint convertedChars = 0;

   if (charsToConvert)
      convertedChars = mbstowcs(dest, source, charsToConvert);

   if (wchars)
      dest[convertedChars] = 0;

   return(convertedChars);
   #pragma EXPANDER_EXPORT
}


/**
 * Convert a Unicode string (UTF-16) to a C Ansi string. Conversion stops at the end of the Unicode string or when the
 * size limit of the destination buffer is hit, whichever comes first. The resulting string is always null-terminated.
 *
 * @param  _In_  wchar* source   - Unicode source string
 * @param  _Out_ char*  dest     - destination buffer the converted Ansi string is written to
 * @param  _In_  size_t destSize - size of the destination buffer in bytes
 *
 * @return uint - the single-byte character length of the resulting string
 */
uint WINAPI WCharToAnsiStr(const wchar* source, char* dest, size_t destSize) {
   if ((uint)source < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter source: 0x%p (not a valid pointer)", source));
   if ((uint)dest   < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter dest: 0x%p (not a valid pointer)", dest));
   if (destSize < 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter destSize: %d (must be non-negative)", destSize));

   uint destLength = 0;

   if (destSize) {
      if (destSize > 1)
         wcstombs(dest, source, destSize);
      dest[destSize-1] = 0;
      destLength = strlen(dest);
   }
   return(destLength);
   #pragma EXPANDER_EXPORT
}


namespace rsf {


/**
 * Write formatted data to a string similar to sprintf() and return the resulting string. This function allocates the memory
 * for the string itself. The caller is responsible for releasing the string's memory after usage with free().
 *
 * @param  char* format - string with format codes
 * @param        ...    - variable number of additional arguments
 *
 * @return char*
 */
char* WINAPI strformat(const char* format, ...) {
   if (!format)  return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: NULL (null pointer)"));
   if (!*format) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: \"\" (empty)"));

   va_list args;
   va_start(args, format);
   char* result = strformat(format, args);
   va_end(args);

   return(result);
}


/**
 * Write formatted data to a string similar to sprintf() and return the resulting string. This function allocates the memory
 * for the string itself. The caller is responsible for releasing the string's memory after usage with free().
 *
 * @param  char*    format - string with format codes
 * @param  va_list& args   - variable list of additional arguments
 *
 * @return char*
 */
char* WINAPI strformat(const char* format, const va_list& args) {
   if (!format)  return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: NULL (null pointer)"));
   if (!*format) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: \"\" (empty)"));

   uint size = _vscprintf(format, args) + 1;                // +1 for the terminating '\0'
   char * buffer = (char*)malloc(size);
   vsprintf_s(buffer, size, format, args);

   return(buffer);
}


/**
 * Convert a Unicode string (UTF-16) to a C multi-byte string.
 *
 * @param  wchar* str - null-terminated Unicode string
 *
 * @return char* - null-terminated C string or a NULL pointer in case of errors
 *
 *
 * Note: The memory for the returned string was allocated with "malloc" and should be released after usage (with "free").
 */
char* WINAPI wchartombs(const wchar* str) {
   return(wchartombs(str, wcslen(str)));
}


/**
 * Convert a sequence of Unicode characters (UTF-16) to a C multi-byte string.
 *
 * @param  wchar* sequence - sequence of wide characters
 * @param  size_t count    - number of wide characters
 *
 * @return char* - null-terminated C string or a NULL pointer in case of errors
 *
 *
 * Note: The memory for the returned string was allocated with "malloc" and should be released after usage (with "free").
 */
char* WINAPI wchartombs(const wchar* sequence, size_t count) {
   wchar* source = wcsncpy(new wchar[count+1], sequence, count);
   source[count] = 0;

   size_t size = (count << 1) + 1;
   char* dest = (char*)malloc(size);

   uint bytes = wcstombs(dest, source, size);

   if (bytes == -1) {
      error(ERR_WIN32_ERROR+GetLastError(), "cannot convert Unicode to multi-byte characters");
      free(dest);
      dest = NULL;
   }
   else {
      dest[size-1] = 0;
   }

   delete[] source;
   return(dest);
}


/**
 * Convert a Unicode string (UTF-16) to a C multi-byte string.
 *
 * @param  wstring& str
 *
 * @return char* - null-terminated C string or a NULL pointer in case of errors
 *
 *
 * Note: The memory for the returned string was allocated with "malloc" and should be released after usage (with "free").
 */
char* WINAPI wchartombs(const wstring& str) {
   return(wchartombs(str.c_str(), str.length()));
}
}
