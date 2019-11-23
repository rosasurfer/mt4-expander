#include "expander.h"
#include "lib/config.h"
#include "lib/conversion.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include <shlobj.h>


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
BOOL WINAPI GetConfigBool(const char* section, const char* key, BOOL defaultValue/*=FALSE*/) {
   // Es ist schneller, immer globale und lokale Konfiguration auszuwerten (intern jeweils nur ein Aufruf von GetPrivateProfileString()).
   //BOOL result = GetGlobalConfigBool(section, key, defaultValue);
   //return(GetLocalConfigBool (section, key, result));
   return(FALSE);
}


/**
 * Return the full filename of the MQL framework's global configuration file. The global file is used for configuration of all
 * terminals executed by the current user. If the file does not exist an attempt is made to create it.
 *
 * @return char* - filename or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common\global-config.ini".
 *
 * Note: The string returned by this function is static and the memory must not be released.
 */
const char* WINAPI GetGlobalConfigPathA() {
   static char* configPath;

   if (!configPath) {
      const char* commonPath = GetTerminalCommonDataPathA();
      if (!commonPath) return(NULL);

      string iniFile = string(commonPath).append("\\global-config.ini");
      configPath = strdup(iniFile.c_str());                                // on the heap

      if (!IsFileA(configPath)) {
         int error = CreateDirectoryRecursive(string(commonPath));         // make sure the directory exists
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
 * Return the full filename of the MQL framework's local configuration file. The local file is used for configuration of the
 * currently executed terminal.
 *
 * - If a config file exists in the user's roaming data directory the file is used, no matter whether the terminal was
 *   launched in portable mode or not.
 * - If no file was found it is created in a location the user has write permission for. If the terminal was launched
 *   in portable mode an attempt is made to create it in the terminal's installation directory. If the attempt fails the
 *   config file will be created in the user's roaming data directory.
 * - Once a config file was created in the user's roaming data directory it has to be (re)moved manually if the user wants
 *   the file to be located in the terminal's installation directory.
 * - The resulting config file location may differ from the data directory the terminal uses.
 *
 * @return char* - filename or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\1DAFD9A7C67DC84FE37EAA1FC1E5CF75\local-config.ini".
 *
 * Note: The string returned by this function is static and the memory must not be released.
 */
const char* WINAPI GetLocalConfigPathA() {
   //
   // 1. check if a config file exists in the roaming data directory
   // 1.1  yes => use it
   // 1.2  no
   //      2. check if the terminal was launched in portable mode
   //      2.1  yes
   //           3. check if a config file exists in the terminal's installation directory
   //           3.1  yes => use it
   //           3.2  no  => check write permission
   //                4.1  permission granted => create a file in the terminal's installation directory and use it
   //                4.2  permission denied  => create a file in the user's roaming data directory and use it
   //      2.2  no
   //           5. check if the terminal is installed in the system's program folder
   //           5.1  yes => continue with 4.2
   //           5.2  no  => continue with 3.
   //
   static char* configPath;

   if (!configPath) {
      // 1. check if a config file exists in the roaming data directory
      const char* roamingDataPath = GetTerminalRoamingDataPathA();
      string filename = string(roamingDataPath).append("\\local-config.ini");

      if (!IsFileA(filename.c_str())) {
         // 1.2 no file => 2. check if the terminal was launched in portable mode
         if (TerminalIsPortableMode()) {
            // 2.1 yes => 3. check if a config file exists in the terminal's installation directory
            filename = string(GetTerminalPathA()).append("\\local-config.ini");
            if (!IsFileA(filename.c_str())) {
               // use it or => 4. attempt to create it
               HFILE hFile = _lcreat(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
               if (hFile == HFILE_ERROR) {
                  debug("cannot create file \"%s\"  [%s]", filename.c_str(), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
                  // 4.2 permission denied => create a file in the roaming data directory
                  int error = CreateDirectoryRecursive(roamingDataPath);                  // make sure the directory exists
                  if (error==ERROR_ACCESS_DENIED || error==ERROR_PATH_NOT_FOUND) debug("cannot create directory \"%s\"  [%s]", roamingDataPath, ErrorToStr(ERR_WIN32_ERROR+error));
                  else if (error)                                                error(ERR_WIN32_ERROR+error, "cannot create directory \"%s\"", roamingDataPath);
                  if (!error) {
                     filename = string(roamingDataPath).append("\\local-config.ini");
                     hFile = _lcreat(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
                     if (hFile == HFILE_ERROR) debug("cannot create file \"%s\"  [%s]", filename.c_str(), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
                     else                      _lclose(hFile);
                  }
               }
               else _lclose(hFile); // 4.1 permission granted
            }
         }
         else {
            // 2.2 terminal was not launched in portable mode
            //  => 5. check if the terminal is installed in the system's program folder
            char programFilesPath[MAX_PATH];                                              // resolve CSIDL_PROGRAM_FILES
            if (FAILED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, programFilesPath)))
               return((char*)error(ERR_WIN32_ERROR+GetLastError(), "SHGetFolderPath()"));

            // TODO: resolve reparse points in TerminalPath() before comparison

            if (StrStartsWith(GetTerminalPathA(), programFilesPath)) {
               // 5.1 yes => continue with 4.2 => create a file in the roaming data directory
               HFILE hFile = _lcreat(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
               if (hFile == HFILE_ERROR) debug("cannot create file \"%s\"  [%s]", filename.c_str(), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
               else                      _lclose(hFile);
            }
            else {
               // 5.2 no => continue with 3 => check if a config file exists in the installation directory
               filename = string(GetTerminalPathA()).append("\\local-config.ini");
               if (!IsFileA(filename.c_str())) {
                  // use it or 3.2 no => 4. attempt to create it
                  HFILE hFile = _lcreat(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
                  if (hFile == HFILE_ERROR) {
                     debug("cannot create file \"%s\"  [%s]", filename.c_str(), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
                     // 4.2 permission denied => create a file in the roaming data directory
                     int error = CreateDirectoryRecursive(roamingDataPath);               // make sure the directory exists
                     if (error==ERROR_ACCESS_DENIED || error==ERROR_PATH_NOT_FOUND) debug("cannot create directory \"%s\"  [%s]", roamingDataPath, ErrorToStr(ERR_WIN32_ERROR+error));
                     else if (error)                                                error(ERR_WIN32_ERROR+error, "cannot create directory \"%s\"", roamingDataPath);
                     if (!error) {
                        filename = string(roamingDataPath).append("\\local-config.ini");
                        hFile = _lcreat(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
                        if (hFile == HFILE_ERROR) debug("cannot create file \"%s\"  [%s]", filename.c_str(), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
                        else                      _lclose(hFile);
                     }
                  }
                  else _lclose(hFile); // 4.1 permission granted
               }
            }
         }
      }
      configPath = strdup(filename.c_str());                         // on the heap
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

   int pos = string(value).find_first_of(";");     // drop trailing comments
   if (pos == -1)
      return(value);
   value[pos] = '\0';

   return(StrRTrim(value));                        // trim white space
   #pragma EXPANDER_EXPORT
}


/**
 * Return a configuration value from an .ini file as a string. Trailing configuration comments are not removed.
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
