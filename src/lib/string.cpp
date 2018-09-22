#include "expander.h"
#include "lib/string.h"
#include "struct/mt4/MqlStr.h"


/**
 * Wrap a C string in double quote characters.
 *
 * @param  char* value
 *
 * @return char* - wrapped C string or the string "(null)" if a NULL pointer was specified
 */
const char* WINAPI DoubleQuoteStr(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));
   if (!value) return("(null)");

   uint size = strlen(value) + 3;                                    // +2 for the quotes, +1 for the terminating '\0'
   char* buffer = new char[size];                                    // TODO: close memory leak
   sprintf_s(buffer, size, "\"%s\"", value);

   return(buffer);
   #pragma EXPANDER_EXPORT
}


/**
 * Wrap a C string in double quote characters.
 *
 * @param  char* value
 *
 * @return string - wrapped std::string or the value "(null)" if a NULL pointer was specified
 */
string WINAPI doubleQuoteStr(const char* value) {
   if (!value)
      return(string("(null)"));
   return(doubleQuoteStr(string(value)));
}


/**
 * Wrap a std::string in double quote characters.
 *
 * @param  string& value
 *
 * @return string - wrapped std::string
 */
string WINAPI doubleQuoteStr(const string& value) {
   return(string("\"").append(value).append("\""));
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zurück.
 *
 * @param  MqlStr values[] - MQL-String-Array
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 */
uint WINAPI GetStringsAddress(const MqlStr values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values = 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Speicheradresse eines C-Strings zurück.
 *
 * @param  char* value - C-String: MetaTrader übergibt für einen MQL-String das Element MqlStr.string
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 * Achtung: GetStringAddress() darf in MQL nur mit Array-Elementen verwendet werden. Ist der Parameter ein einfacher String,
 *          wird an die DLL eine Kopie dieses Strings übergeben. Diese Kopie wird u.U. sofort nach Rückkehr freigegeben und
 *          die erhaltene Adresse ist ungültig (z.B. im Tester bei mehrfachen Tests).
 */
uint WINAPI GetStringAddress(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter value = 0x%p (not a valid pointer)", value));
   return((uint) value);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den übergebenen Zeiger auf einen C-String selbst zurück. Wird in MQL zum Lesen eines Strings von
 * einer Adresse verwendet, da MetaTrader einen C-String automatisch in einen MQL-String konvertiert.
 *
 * @param  char* value - C-String
 *
 * @return char* - derselbe Zeiger oder NULL, falls ein Fehler auftrat
 */
const char* WINAPI GetString(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter value = 0x%p (not a valid pointer)", value));
   return((char*) value);
   #pragma EXPANDER_EXPORT
}


/**
 * Prüft, ob ein C-String initialisiert oder ein NULL-Pointer ist.
 *
 * @param  char* value - zu prüfender String
 *
 * @return BOOL
 */
BOOL WINAPI StringIsNull(const char* value) {
   return(!value);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not two strings are considered equal.
 *
 * @param  char* s1
 * @param  char* s2
 *
 * @return BOOL
 */
BOOL WINAPI StringCompare(const char* s1, const char* s2) {
   if ( s1 ==  s2) return(TRUE);                                     // if pointers are equal values are too
   if (!s1 || !s2) return(FALSE);                                    // if one is a NULL pointer the other can't
   return(strcmp(s1, s2) == 0);                                      // both are not NULL pointers
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not a string starts with the specified substring.
 *
 * @param  char* str
 * @param  char* prefix
 *
 * @return BOOL
 */
BOOL WINAPI StringStartsWith(const char* str, const char* prefix) {
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
 * Whether or not a string ends with the specified substring.
 *
 * @param  char* str
 * @param  char* suffix
 *
 * @return BOOL
 */
BOOL WINAPI StringEndsWith(const char* str, const char* suffix) {
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
char* WINAPI StrToLower(char* str) {
   char* c = str;
   while (*c) {
      *c = tolower(*c);
      ++c;
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
   for (string::iterator i=str.begin(); i != str.end(); ++i) {
      *i = tolower(*i);
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
   for (wstring::iterator i=str.begin(); i != str.end(); ++i) {
      *i = towlower(*i);
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
char* WINAPI StrToUpper(char* str) {
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
   for (string::iterator i=str.begin(); i != str.end(); ++i) {
      *i = toupper(*i);
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
   for (wstring::iterator i=str.begin(); i != str.end(); ++i) {
      *i = towupper(*i);
   }
   return(str);
}


/**
 * Convert a C ANSI string to a unicode string (UTF-16). Conversion stops at the end of the ANSI string or when the size
 * limit of the destination buffer is hit, whichever comes first. The resulting string is always NULL terminated.
 *
 * @param  _In_  char*    source   - C ANSI or multi-byte UTF-8 source string
 * @param  _Out_ wchar_t* dest     - buffer the converted unicode string is written to
 * @param  _In_  size_t   destSize - size of the destination buffer in bytes
 *
 * @return uint - number of converted characters (equal to the wide-character length of the resulting string)
 */
uint WINAPI AnsiToWCharStr(const char* source, wchar_t* dest, size_t destSize) {
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
 * Convert a unicode string (UTF-16) to a C ANSI string. Conversion stops at the end of the unicode string or when the
 * size limit of the destination buffer is hit, whichever comes first. The resulting string is always NULL terminated.
 *
 * @param  _In_  wchar_t* source   - unicode source string
 * @param  _Out_ char*    dest     - destination buffer the converted ANSI string is written to
 * @param  _In_  size_t   destSize - size of the destination buffer in bytes
 *
 * @return uint - the single-byte character length of the resulting string
 */
uint WINAPI WCharToAnsiStr(const wchar_t* source, char* dest, size_t destSize) {
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


/**
 * Convert a unicode string (UTF-16) to a C multi-byte string.
 *
 * @param  wchar_t* str - NULL terminated unicode string
 *
 * @return char* - NULL terminated C string or a NULL pointer in case of errors
 */
char* wchartombs(const wchar_t* str) {
   return(wchartombs(str, wcslen(str)));
}


/**
 * Convert a sequence of unicode characters (UTF-16) to a C multi-byte string.
 *
 * @param  wchar_t* sequence - sequence of wide characters
 * @param  size_t   count    - number of wide characters
 *
 * @return char* - NULL terminated C string or a NULL pointer in case of errors
 */
char* wchartombs(const wchar_t* sequence, size_t count) {
   wchar_t* source = wcsncpy(new wchar_t[count+1], sequence, count);
   source[count] = 0;

   size_t size = (count << 1) + 1;
   char* dest = new char[size];

   uint bytes = wcstombs(dest, source, size);

   if (bytes == -1) {
      error(ERR_WIN32_ERROR+GetLastError(), "cannot convert unicode to multi-byte characters");
      delete[] dest;
      dest = NULL;
   }
   else {
      dest[size-1] = 0;
   }

   delete[] source;
   return(dest);
}


/**
 * Convert a unicode string (UTF-16) to a C multi-byte string.
 *
 * @param  wstring& str
 *
 * @return char* - NULL terminated C string or a NULL pointer in case of errors
 */
char* wchartombs(const wstring& str) {
   return(wchartombs(str.c_str(), str.length()));
}
