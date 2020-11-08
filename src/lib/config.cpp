#include "expander.h"
#include "lib/config.h"
#include "lib/conversion.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include <fstream>


/**
 * Return a terminal configuration value as a boolean. Queries the global and the local configuration with the local configu-
 * ration superseding the global one. Boolean values can be expressed by "0" or "1", "On" or "Off", "Yes" or "No" and "true" or
 * "false" (case insensitive). An empty value of an existing key is considered FALSE and a numeric value is considered TRUE if
 * its nominal value is non-zero. Trailing configuration comments (text following the ";" character) are ignored.
 *
 * @param  char* section      - configuration section name
 * @param  char* key          - configuration key
 * @param  BOOL  defaultValue - alternative value to return if the specified value was not found
 *
 * @return BOOL - configuration value
 */
//BOOL WINAPI GetConfigBool(const char* section, const char* key, BOOL defaultValue/*=FALSE*/) {
//   // Es ist schneller, immer globale und lokale Konfiguration auszuwerten (intern jeweils nur ein Aufruf von GetPrivateProfileString()).
//   //BOOL result = GetGlobalConfigBool(section, key, defaultValue);
//   //return(GetLocalConfigBool (section, key, result));
//   return(FALSE);
//}


/**
 * Delete a configuration key from an .ini file. If the file does not exist an attempt is made to create it. No error is
 * returned if creation of a non-existing file fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive configuration section name
 * @param  char* key      - case-insensitive configuration key to delete
 *
 * @return BOOL - success status
 */
BOOL WINAPI DeleteIniKeyA(const char* fileName, const char* section, const char* key) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!*fileName)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)section  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section));
   if (!*section)                          return(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)"));
   if ((uint)key      < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter key: 0x%p (not a valid pointer)", key));
   if (!*key)                              return(error(ERR_INVALID_PARAMETER, "invalid parameter key: \"\" (empty)"));

   if (!WritePrivateProfileStringA(section, key, NULL, fileName)) {
      int error = GetLastError();
      if (error != ERROR_PATH_NOT_FOUND) return(error(ERR_WIN32_ERROR+error, "WritePrivateProfileStringA()  fileName=\"%s\", section=\"%s\", key=\"%s\"", fileName, section, key));
   }
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Delete a configuration section from an .ini file. If the file does not exist an attempt is made to create it. No error is
 * returned if creation of a non-existing file fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive configuration section name to delete
 *
 * @return BOOL - success status
 */
BOOL WINAPI DeleteIniSectionA(const char* fileName, const char* section) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!*fileName)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)section  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section));
   if (!*section)                          return(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)"));

   if (!WritePrivateProfileStringA(section, NULL, NULL, fileName)) {
      int error = GetLastError();
      if (error != ERROR_PATH_NOT_FOUND) return(error(ERR_WIN32_ERROR+error, "WritePrivateProfileStringA()  fileName=\"%s\", section=\"%s\"", fileName, section));
   }
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Delete all keys from an .ini file's configuration section but don't delete the section itself. If the section does not
 * exist it is created. If the file does not exist an attempt is made to create it. No error is returned if creation of a
 * non-existing file fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive configuration section name to empty
 *
 * @return BOOL - success status
 */
BOOL WINAPI EmptyIniSectionA(const char* fileName, const char* section) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!*fileName)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)section  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section));
   if (!*section)                          return(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)"));

   char values[2] = {};                   // a NUL-terminated string followed by a second NUL terminator

   if (!WritePrivateProfileSectionA(section, values, fileName)) {
      int error = GetLastError();
      if (error != ERROR_PATH_NOT_FOUND) return(error(ERR_WIN32_ERROR+error, "WritePrivateProfileSectionA()  fileName=\"%s\", section=\"%s\"", fileName, section));
   }
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the global framework configuration file. This configuration file is read by all terminals of
 * the current user. If the file does not exist an attempt is made to create it. The file is located in the common MetaTrader
 * data folder and is named "global-config.ini".
 *
 * @return char* - filename or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common\global-config.ini"
 */
const char* WINAPI GetGlobalConfigPathA() {
   // The string returned by this function is static and its memory must not be released.
   static char* configPath;

   if (!configPath) {
      const char* commonPath = GetTerminalCommonDataPathA();
      if (!commonPath) return(NULL);

      string iniFile = string(commonPath).append("\\global-config.ini");
      configPath = strdup(iniFile.c_str());                                // on the heap

      if (!IsFileA(configPath)) {
         int error = CreateDirectoryA(commonPath, MKDIR_PARENT);           // make sure the directory exists
         if (error==ERROR_ACCESS_DENIED || error==ERROR_PATH_NOT_FOUND) debug("cannot create directory \"%s\"  [%s]", commonPath, ErrorToStr(ERR_WIN32_ERROR+error));
         else if (error)                                                error(ERR_WIN32_ERROR+error, "cannot create directory \"%s\"", commonPath);

         if (!error) {                                                     // try to create the file
            HFILE hFile = _lcreat(configPath, FILE_ATTRIBUTE_NORMAL);
            if (hFile == HFILE_ERROR) {
               error = GetLastError();                                     // log errors but continue
               if (error == ERROR_ACCESS_DENIED) debug("cannot create file \"%s\"  [%s]", configPath, ErrorToStr(ERR_WIN32_ERROR+error));
               else                              error(ERR_WIN32_ERROR+error, "cannot create file \"%s\"", configPath);
            }
            else _lclose(hFile);
         }
      }
   }

   return(configPath);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the terminal-specific framework configuration file. This configuration file is read by the
 * current terminal only. If the file does not exist an attempt is made to create it. The file is located in the currently
 * active terminal-specific data folder and is named "terminal-config.ini".
 *
 * @return char* - filename or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-id}\terminal-config.ini"
 */
const char* WINAPI GetLocalConfigPathA() {
   // The string returned by this function is static and its memory must not be released.
   static char* configPath;

   if (!configPath) {
      const char* dataPath = GetTerminalDataPathA();
      if (!dataPath) return(NULL);

      string iniFile = string(dataPath).append("\\terminal-config.ini");
      configPath = strdup(iniFile.c_str());                                // on the heap

      if (!IsDirectoryA(dataPath)) {
         int error = CreateDirectoryA(dataPath, MKDIR_PARENT);             // make sure the directory exists
         if (error==ERROR_ACCESS_DENIED || error==ERROR_PATH_NOT_FOUND)
            return((char*)debug("cannot create directory \"%s\"  [%s]", dataPath, ErrorToStr(ERR_WIN32_ERROR+error)));
         if (error)
            return((char*)error(ERR_WIN32_ERROR+error, "cannot create directory \"%s\"", dataPath));

         const char* terminalPath = GetTerminalPathA();

         if (!StrCompare(dataPath, terminalPath)) {
            string originFile = string(dataPath).append("\\origin.txt");   // store file "origin.txt"
            std::ofstream file(originFile.c_str());
            if (file.is_open()) {
               file << terminalPath << NL;
               file.close();
            }
            else debug("cannot create file \"%s\" (%s)  [%s]", originFile.c_str(), strerror(errno), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
         }
      }

      if (!IsFileA(configPath)) {
         std::ofstream file(configPath);                                   // create the file
         if (file.is_open()) file.close();
         else debug("cannot create file \"%s\" (%s)  [%s]", configPath, strerror(errno), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
      }
   }

   return(configPath);
   #pragma EXPANDER_EXPORT
}


/**
 * Return all keys of an .ini file section.
 *
 * Alias of GetPrivateProfileString(). Required for MQL4 which doesn't support multiple different function signatures.
 *
 * @param  _In_  char* fileName   - initialization file name
 * @param  _In_  char* section    - case-insensitive section name
 * @param  _Out_ char* buffer     - Pointer to a buffer that receives the found keys. The buffer is filled with one or more
 *                                  null terminated strings. The last string is followed by a second null character.
 * @param  _In_  DWORD bufferSize - size of the buffer in bytes
 *
 * @return DWORD - Number of bytes copied to the specified buffer, not including the last terminating null character.
 *                 If the buffer is to small to hold all keys the first non-fitting key is truncated and followed by two null
 *                 characters. In this case, the return value is equal to bufferSize-2.
 */
DWORD WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, DWORD bufferSize) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!*fileName)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)section  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section));
   if (!*section)                          return(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)"));
   if ((uint)buffer   < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (bufferSize < 2)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d (min. 2 bytes)", bufferSize));

   return(GetPrivateProfileStringA(section, NULL, NULL, buffer, bufferSize, fileName));
   #pragma EXPANDER_EXPORT
}


/**
 * Return all section names of an .ini file.
 *
 * @param  _In_  char* fileName   - initialization file name
 * @param  _Out_ char* buffer     - Pointer to a buffer that receives the found section names. The buffer is filled with one
 *                                  or more null terminated strings. The last string is followed by a second null character.
 * @param  _In_  DWORD bufferSize - size of the buffer in bytes
 *
 * @return DWORD - Number of bytes copied to the specified buffer, not including the last terminating null character.
 *                 If the buffer is to small to hold all section names the first non-fitting name is truncated and followed by
 *                 two null characters. In this case, the return value is equal to bufferSize-2.
 */
DWORD WINAPI GetIniSectionsA(const char* fileName, char* buffer, DWORD bufferSize) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!*fileName)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)buffer   < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (bufferSize < 2)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d (min. 2 bytes)", bufferSize));

   return(GetPrivateProfileStringA(NULL, NULL, NULL, buffer, bufferSize, fileName));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a configuration value from an .ini file as a string. Enclosing white space and trailing comments are removed.
 *
 * @param  char* fileName                - name of the .ini file
 * @param  char* section                 - case-insensitive configuration section name
 * @param  char* key                     - case-insensitive configuration key
 * @param  char* defaultValue [optional] - value to return if the specified key does not exist (default: empty string)
 *
 * @return char* - Configuration value, the default value or an empty string in case of errors. Enclosing white space and
 *                 trailing comments are removed.
 *
 * Note: The caller is responsible for releasing the returned string's memory after usage with "delete".
 */
char* WINAPI GetIniStringA(const char* fileName, const char* section, const char* key, const char* defaultValue/*=""*/) {
   char* value = GetIniStringRawA(fileName, section, key, defaultValue);
   if (!value || !*value)
      return(value);

   size_t pos = string(value).find_first_of(";");  // drop trailing comments
   if (pos == string::npos)
      return(value);
   value[pos] = '\0';

   return(StrTrimRight(value));                    // trim white space
   #pragma EXPANDER_EXPORT
}


/**
 * Return a configuration value from an .ini file as a string. Configuration comments are not removed.
 *
 * @param  char* fileName                - name of the .ini file
 * @param  char* section                 - case-insensitive configuration section name
 * @param  char* key                     - case-insensitive configuration key
 * @param  char* defaultValue [optional] - value to return if the specified key does not exist (default: empty string)
 *
 * @return char* - Configuration value, the default value or an empty string in case of errors. Enclosing white space is
 *                 removed.
 *
 * Note: The caller is responsible for releasing the returned string's memory after usage with "delete".
 */
char* WINAPI GetIniStringRawA(const char* fileName, const char* section, const char* key, const char* defaultValue/*=""*/) {
   if ((uint)fileName     < MIN_VALID_POINTER) return(_EMPTY_NEW_STR(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName)));
   if (!*fileName)                             return(_EMPTY_NEW_STR(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)")));
   if ((uint)section      < MIN_VALID_POINTER) return(_EMPTY_NEW_STR(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section)));
   if (!*section)                              return(_EMPTY_NEW_STR(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)")));
   if ((uint)key          < MIN_VALID_POINTER) return(_EMPTY_NEW_STR(error(ERR_INVALID_PARAMETER, "invalid parameter key: 0x%p (not a valid pointer)", key)));
   if (!*key)                                  return(_EMPTY_NEW_STR(error(ERR_INVALID_PARAMETER, "invalid parameter key: \"\" (empty)")));
   if ((uint)defaultValue < MIN_VALID_POINTER) return(_EMPTY_NEW_STR(error(ERR_INVALID_PARAMETER, "invalid parameter defaultValue: 0x%p (not a valid pointer)", defaultValue)));

   char* buffer = NULL;
   uint bufferSize = 128;
   uint chars = bufferSize-1;

   while (chars == bufferSize-1) {                       // handle a too small buffer
      delete[] buffer;
      bufferSize <<= 1;
      buffer = new char[bufferSize];                     // on the heap as the value may be long
      chars = GetPrivateProfileString(section, key, defaultValue, buffer, bufferSize, fileName);
   }
   return(buffer);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified global configuration key exists.
 *
 * @param  char* section - case-insensitive configuration section name
 * @param  char* key     - case-insensitive configuration key
 *
 * @return BOOL
 */
BOOL WINAPI IsGlobalConfigKeyA(const char* section, const char* key) {
   const char* globalConfig = GetGlobalConfigPathA();
   if (globalConfig)
      return(IsIniKeyA(globalConfig, section, key));
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a configuration key exists in an .ini file.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive configuration section
 * @param  char* key      - case-insensitive configuration key
 *
 * @return BOOL
 */
BOOL WINAPI IsIniKeyA(const char* fileName, const char* section, const char* key) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!*fileName)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)section  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section));
   if (!*section)                          return(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)"));
   if ((uint)key      < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter key: 0x%p (not a valid pointer)", key));
   if (!*key)                              return(error(ERR_INVALID_PARAMETER, "invalid parameter key: \"\" (empty)"));

   // read all keys
   char* buffer    = NULL;
   uint bufferSize = 512;
   uint chars = bufferSize-2;
   while (chars == bufferSize-2) {                       // handle a too small buffer
      delete[] buffer;
      bufferSize <<= 1;
      buffer = new char[bufferSize];                     // on the heap as a section may be big
      chars = GetIniKeysA(fileName, section, buffer, bufferSize);
   }

   // convert the passed key to lower-case
   bufferSize = strlen(key)+1;
   char* lKey = StrToLower(StrTrim((char*)memcpy(alloca(bufferSize), key, bufferSize)));

   // look for a case-insensitive match
   BOOL result = FALSE;
   char* name = buffer;                                  // The buffer is filled with one or more trimmed and null terminated
   while (*name) {                                       // strings. The last string is followed by a second null character.
      if (StrCompare(StrToLower(name), lKey)) {
         result = TRUE;
         break;
      }
      name += strlen(name) + 1;
   }

   delete[] buffer;
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a configuration section exists in an .ini file.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive configuration section
 *
 * @return BOOL
 */
BOOL WINAPI IsIniSectionA(const char* fileName, const char* section) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!*fileName)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)section  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section));
   if (!*section)                          return(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)"));

   // read all sections
   char* buffer    = NULL;
   uint bufferSize = 512;
   uint chars = bufferSize-2;
   while (chars == bufferSize-2) {                       // handle a too small buffer
      delete[] buffer;
      bufferSize <<= 1;
      buffer = new char[bufferSize];                     // on the heap as there may be plenty of sections
      chars = GetIniSectionsA(fileName, buffer, bufferSize);
   }

   // convert the passed section to lower-case
   bufferSize = strlen(section)+1;
   char* lSection = StrToLower(StrTrim((char*)memcpy(alloca(bufferSize), section, bufferSize)));

   // look for a case-insensitive match
   BOOL result = FALSE;
   char* name = buffer;                                  // The buffer is filled with one or more trimmed and null terminated
   while (*name) {                                       // strings. The last string is followed by a second null character.
      if (StrCompare(StrToLower(name), lSection)) {
         result = TRUE;
         break;
      }
      name += strlen(name) + 1;
   }

   delete[] buffer;
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified local configuration key exists.
 *
 * @param  char* section - case-insensitive configuration section name
 * @param  char* key     - case-insensitive configuration key
 *
 * @return BOOL
 */
BOOL WINAPI IsLocalConfigKeyA(const char* section, const char* key) {
   const char* localConfig = GetLocalConfigPathA();
   if (localConfig)
      return(IsIniKeyA(localConfig, section, key));
   return(FALSE);
   #pragma EXPANDER_EXPORT
}
