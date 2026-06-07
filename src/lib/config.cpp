#include "expander.h"
#include "lib/config.h"
#include "lib/conversion.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include <fstream>


/**
 * Return the full name of the global framework configuration file. The global configuration is used by all installed terminals
 * of the current user. The file is located in the common MetaTrader data folder and is named "global-config.ini". If the file
 * does not exist an attempt is made to create it.
 *
 * @return char* - filename or NULL in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common\global-config.ini"
 */
const char* WINAPI GetGlobalConfigPathA() {
   static char* configPath;

   if (!configPath) {
      const char* commonDataPath = GetTerminalCommonDataPathA();
      if (!commonDataPath) return NULL;

      string filename = string(commonDataPath).append("\\global-config.ini");
      char* tmp = sdup(filename.c_str());
      if (!configPath) configPath = tmp;
      else             free(tmp);                  // another thread may have been faster

      if (!IsFileA(configPath, MODE_SYSTEM)) {
         // make sure the config directory exists
         int error = CreateDirectoryA(commonDataPath, MODE_SYSTEM|MODE_MKPARENT);
         if (error) {
            warn(error, "cannot create directory \"%s\" (%s)", commonDataPath, strerror(errno));
         }
         else {
            // make sure the config file exists
            std::ofstream file(configPath);
            if (file.is_open()) file.close();
            else                warn(ERR_WIN32_ERROR + GetLastError(), "cannot create file \"%s\" (%s)", configPath, strerror(errno));
         }
      }
   }

   return configPath;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full name of the terminal-specific framework configuration file. This configuration file is used by the currently
 * active terminal only. The file is located in the terminal-specific data folder and is named "terminal-config.ini". If the
 * file does not exist an attempt is made to create it.
 *
 * @return char* - filename or NULL in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-id}\terminal-config.ini"
 */
const char* WINAPI GetTerminalConfigPathA() {
   static char* configPath;

   if (!configPath) {
      const char* dataPath = GetTerminalDataPathA();
      if (!dataPath) return NULL;

      string iniFile = string(dataPath).append("\\terminal-config.ini");
      char* tmp = sdup(iniFile.c_str());
      if (!configPath) configPath = tmp;
      else             free(tmp);                                          // another thread may have been faster

      // make sure the config directory exists (e.g. if in non-portable mode)
      if (!IsDirectoryA(dataPath, MODE_SYSTEM)) {
         int error = CreateDirectoryA(dataPath, MODE_SYSTEM|MODE_MKPARENT);
         if (error) {
            warn(ERR_WIN32_ERROR + error, "cannot create directory \"%s\" (%s)", dataPath, strerror(errno));
            return configPath;
         }

         // if in non-portable mode (terminalPath != dataPath): make sure file "origin.txt" exists
         const char* terminalPath = GetTerminalPathA();
         if (!StrCompare(terminalPath, dataPath)) {
            string originFile = string(dataPath).append("\\origin.txt");   // store file "origin.txt"
            std::ofstream file(originFile.c_str());
            if (file.is_open()) {
               file << terminalPath << NL;
               file.close();
            }
            else {
               warn(ERR_WIN32_ERROR + GetLastError(), "cannot create file \"%s\" (%s)", originFile.c_str(), strerror(errno));
            }
         }
      }

      // make sure the config file exists
      if (!IsFileA(configPath, MODE_SYSTEM)) {
         std::ofstream file(configPath);
         if (file.is_open()) file.close();
         else                warn(ERR_WIN32_ERROR + GetLastError(), "cannot create file \"%s\" (%s)", configPath, strerror(errno));
      }
   }

   return configPath;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a config key exists in the specified .ini file.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive config section
 * @param  char* key      - case-insensitive config key
 *
 * @return BOOL
 */
BOOL WINAPI IsIniKeyA(const char* fileName, const char* section, const char* key) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section  < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");
   if ((uint)key      < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter key: 0x%p (not a valid pointer)", key);
   if (!*key)                              return !error(ERR_INVALID_PARAMETER, "invalid parameter key: \"\" (empty)");

   // read all keys
   char* buffer = NULL;
   uint bufferSize = 512;
   uint chars = bufferSize-2;

   while (chars == bufferSize-2) {                       // handle a too small buffer
      free(buffer);
      bufferSize <<= 1;
      buffer = (char*) malloc(bufferSize);               // on the heap as a section may be big
      if (!buffer) return FALSE;
      chars = GetIniKeysA(fileName, section, buffer, bufferSize);
   }
   char* lKey = strToLower(strim(sdupa(key)));

   // look for a case-insensitive match
   BOOL result = FALSE;
   char* name = buffer;                                  // The buffer is filled with one or more trimmed and NUL terminated
   while (*name) {                                       // strings. The last string is followed by a second NUL character.
      if (StrCompare(strToLower(name), lKey)) {
         result = TRUE;
         break;
      }
      name += strlen(name) + 1;
   }
   free(buffer);

   return result;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a config key exists in the terminal configuration.
 *
 * @param  char* section - case-insensitive config section name
 * @param  char* key     - case-insensitive config key
 *
 * @return BOOL
 */
BOOL WINAPI IsTerminalConfigKeyA(const char* section, const char* key) {
   const char* terminalConfig = GetTerminalConfigPathA();
   return terminalConfig && IsIniKeyA(terminalConfig, section, key);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a config key exists in the global configuration.
 *
 * @param  char* section - case-insensitive config section name
 * @param  char* key     - case-insensitive config key
 *
 * @return BOOL
 */
BOOL WINAPI IsGlobalConfigKeyA(const char* section, const char* key) {
   const char* globalConfig = GetGlobalConfigPathA();
   return globalConfig && IsIniKeyA(globalConfig, section, key);
   #pragma EXPANDER_EXPORT
}


/**
 * Return all keys of the specified .ini file section.
 *
 * Alias of GetPrivateProfileString(). Required for MQL4.0 which doesn't support function overloading (multiple signatures).
 *
 * @param  _In_  char* fileName   - initialization file name
 * @param  _In_  char* section    - case-insensitive section name
 * @param  _Out_ char* buffer     - Pointer to a buffer that receives the found keys. The buffer is filled with one or more
 *                                  NUL terminated strings. The last string is followed by a second NUL character.
 * @param  _In_  DWORD bufferSize - size of the buffer in bytes
 *
 * @return DWORD - Number of bytes copied to the specified buffer, not including the last terminating NUL character.
 *                 If the buffer is to small to hold all keys the first non-fitting key is truncated and followed by two NUL
 *                 characters. In this case, the return value is equal to bufferSize-2.
 */
DWORD WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, DWORD bufferSize) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section  < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");
   if ((uint)buffer   < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer);
   if (bufferSize < 2)                     return !error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d (min. 2 bytes)", bufferSize);

   return GetPrivateProfileStringA(section, NULL, NULL, buffer, bufferSize, fileName);
   #pragma EXPANDER_EXPORT
}


/**
 * Return all section names of the specified .ini file.
 *
 * @param  _In_  char* fileName   - initialization file name
 * @param  _Out_ char* buffer     - Pointer to a buffer that receives the found section names. The buffer is filled with one
 *                                  or more NUL terminated strings. The last string is followed by a second NUL character.
 * @param  _In_  DWORD bufferSize - size of the buffer in bytes
 *
 * @return DWORD - Number of bytes copied to the specified buffer, not including the last terminating NUL character.
 *                 If the buffer is to small to hold all section names the first non-fitting name is truncated and followed by
 *                 two NUL characters. In this case, the return value is equal to bufferSize-2.
 */
DWORD WINAPI GetIniSectionsA(const char* fileName, char* buffer, DWORD bufferSize) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)buffer   < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer);
   if (bufferSize < 2)                     return !error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d (min. 2 bytes)", bufferSize);

   return GetPrivateProfileStringA(NULL, NULL, NULL, buffer, bufferSize, fileName);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a config section exists in the specified .ini file.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive config section name
 *
 * @return BOOL
 */
BOOL WINAPI IsIniSectionA(const char* fileName, const char* section) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section  < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");

   // read all sections
   char* buffer = NULL;
   uint bufferSize = 512;
   uint chars = bufferSize-2;

   while (chars == bufferSize-2) {                       // handle a too small buffer
      free(buffer);
      bufferSize <<= 1;
      buffer = (char*) malloc(bufferSize);               // on the heap as there may be may sections
      if (!buffer) return FALSE;
      chars = GetIniSectionsA(fileName, buffer, bufferSize);
   }
   char* lSection = strToLower(strim(sdupa(section)));

   // look for a case-insensitive match
   BOOL result = FALSE;
   char* name = buffer;                                  // The buffer is filled with one or more trimmed and NUL terminated
   while (*name) {                                       // strings. The last string is followed by a second NUL character.
      if (StrCompare(strToLower(name), lSection)) {
         result = TRUE;
         break;
      }
      name += strlen(name) + 1;
   }
   free(buffer);

   return result;
   #pragma EXPANDER_EXPORT
}


/**
 * Delete a config key from the specified .ini file. If the file does not exist an attempt is made to create it. No error is
 * returned if creation fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive config section name
 * @param  char* key      - case-insensitive config key to delete
 *
 * @return BOOL - success status
 */
BOOL WINAPI DeleteIniKeyA(const char* fileName, const char* section, const char* key) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section  < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");
   if ((uint)key      < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter key: 0x%p (not a valid pointer)", key);
   if (!*key)                              return !error(ERR_INVALID_PARAMETER, "invalid parameter key: \"\" (empty)");

   if (!WritePrivateProfileStringA(section, key, NULL, fileName)) {
      int error = GetLastError();
      if (error != ERROR_PATH_NOT_FOUND) return !error(ERR_WIN32_ERROR + error, "WritePrivateProfileStringA()  fileName=\"%s\", section=\"%s\", key=\"%s\"", fileName, section, key);
   }
   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Delete a config section from the specified .ini file. If the file does not exist an attempt is made to create it. No error
 * is returned if creation fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive config section name
 *
 * @return BOOL - success status
 */
BOOL WINAPI DeleteIniSectionA(const char* fileName, const char* section) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section  < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");

   if (!WritePrivateProfileStringA(section, NULL, NULL, fileName)) {
      int error = GetLastError();
      if (error != ERROR_PATH_NOT_FOUND) return !error(ERR_WIN32_ERROR + error, "WritePrivateProfileStringA()  fileName=\"%s\", section=\"%s\"", fileName, section);
   }
   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Delete all keys from a config section but don't delete the section itself. If the section does not exist it is created.
 * If the file does not exist an attempt is made to create it. No error is returned if file creation fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive config section name
 *
 * @return BOOL - success status
 */
BOOL WINAPI EmptyIniSectionA(const char* fileName, const char* section) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section  < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");

   char values[2] = {};                   // an empty string (NUL) followed by a second NUL terminator

   if (!WritePrivateProfileSectionA(section, values, fileName)) {
      int error = GetLastError();
      if (error != ERROR_PATH_NOT_FOUND) return !error(ERR_WIN32_ERROR + error, "WritePrivateProfileSectionA()  fileName=\"%s\", section=\"%s\"", fileName, section);
   }
   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Return a config value from an .ini file as a string. Enclosing white space and trailing comments are removed.
 *
 * @param  char* fileName                - name of the .ini file
 * @param  char* section                 - case-insensitive config section name
 * @param  char* key                     - case-insensitive config key
 * @param  char* defaultValue [optional] - value to return if the specified key does not exist (default: empty string)
 *
 * @return char* - Config value or the default value if the config value does not exist (enclosing white space and inline
 *                 comments are removed. NULL in case of errors.
 */
char* WINAPI GetIniStringA(const char* fileName, const char* section, const char* key, const char* defaultValue/*=""*/) {
   char* value = GetIniStringRawA(fileName, section, key, defaultValue);
   if (!value || !*value) return value;

   size_t pos = string(value).find_first_of(";");  // drop trailing comments
   if (pos != string::npos) {
      value[pos] = '\0';
      strim_right(value);
   }
   return value;                                   // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return a config value from an .ini file as a raw string, including config line comments.
 *
 * @param  char* fileName                - name of the .ini file
 * @param  char* section                 - case-insensitive config section name
 * @param  char* key                     - case-insensitive config key
 * @param  char* defaultValue [optional] - value to return if the specified key does not exist (default: empty string)
 *
 * @return char* - Config value or the default value if the config value does not exist (enclosing white space is removed).
 *                 NULL in case of errors.
 */
char* WINAPI GetIniStringRawA(const char* fileName, const char* section, const char* key, const char* defaultValue/*=""*/) {
   if ((uint)fileName     < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                             return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section      < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                              return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");
   if ((uint)key          < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter key: 0x%p (not a valid pointer)", key);
   if (!*key)                                  return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter key: \"\" (empty)");
   if ((uint)defaultValue < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter defaultValue: 0x%p (not a valid pointer)", defaultValue);

   char* buffer = NULL;
   uint bufferSize = 128;
   uint chars = bufferSize-1;

   while (chars == bufferSize-1) {              // handle a too small buffer
      free(buffer);
      bufferSize <<= 1;
      buffer = (char*)malloc(bufferSize);       // on the heap
      chars = GetPrivateProfileString(section, key, defaultValue, buffer, bufferSize, fileName);
   }
   return buffer;                               // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Return a terminal config value as a boolean. Queries the global and the terminal configuration with the terminal configu-
 * ration superseeding the global one. Boolean values can be expressed by "0" or "1", "On" or "Off", "Yes" or "No" and "true" or
 * "false" (case insensitive). An empty value of an existing key is considered FALSE and a numeric value is considered TRUE if
 * its nominal value is non-zero. Trailing configuration comments are ignored.
 *
 * @param  char* section      - configuration section name
 * @param  char* key          - configuration key
 * @param  BOOL  defaultValue - alternative value to return if the specified value was not found
 *
 * @return BOOL - configuration value
 */
//BOOL WINAPI GetConfigBool(const char* section, const char* key, BOOL defaultValue/*=FALSE*/) {
//   // It's faster to always evaluate global and local configuration: internally only one call of GetPrivateProfileString().    // TODO: true?
//   //BOOL result = GetGlobalConfigBool(section, key, defaultValue);
//   //return GetTerminalConfigBool(section, key, result);
//   return FALSE;
//}
