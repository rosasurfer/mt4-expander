#include "expander.h"
#include "struct/mt4/MqlStr.h"


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
 * Whether or not a string ends with the specified substring.
 *
 * @param  char* str
 * @param  char* suffix
 *
 * @return BOOL
 */
BOOL WINAPI StringEndsWith(const char* str, const char* suffix) {
   if (!str)    return(FALSE);
   if (!suffix) return(warn(ERR_INVALID_PARAMETER, "invalid parameter suffix=%s", suffix));

   size_t strLen    = strlen(str);
   size_t suffixLen = strlen(suffix);
   if (!suffixLen) return(warn(ERR_INVALID_PARAMETER, "illegal parameter suffix=\"\""));

   if (strLen >= suffixLen)
      return(strncmp(str + strLen - suffixLen, suffix, suffixLen) == 0);
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
