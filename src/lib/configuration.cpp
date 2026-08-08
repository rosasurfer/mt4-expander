#include "expander.h"
#include "lib/configuration.h"
#include "lib/conversion.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/terminal.h"


/**
 * Return the full name of the framework's user configuration file.
 *
 * - This configuration file is used by all terminals installed by the user.
 * - The file is named "rsf-user-config.ini" and is located in the terminal's common data folder.
 * - If the file does not exist an attempt is made to create it.
 *
 * @return char* - file name or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common\rsf-user-config.ini"
 */
const char* WINAPI GetUserConfigPathA() {
   static char* configPath;

   if (!configPath) {
      const wchar* wpath = GetUserConfigPathW();
      if (!wpath) return NULL;

      char* tmp = utf16ToAnsi(wpath);
      if (!configPath) configPath = tmp;
      else             free(tmp);                  // another thread may have been faster
   }
   return configPath;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full name of the framework's user configuration file.
 *
 * - This configuration file is used by all terminals installed by the user.
 * - The file is named "rsf-user-config.ini" and is located in the terminal's common data folder.
 * - If the file does not exist an attempt is made to create it.
 * - An existing legacy config file "global-config.ini" is renamed to the new name.
 *
 * @return char* - file name or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common\rsf-user-config.ini"
 */
const wchar* WINAPI GetUserConfigPathW() {
   static wchar* configPath;

   if (!configPath) {
      const wchar* commonDataPath = GetTerminalCommonDataPathW();
      if (!commonDataPath) return NULL;

      wstring fileName = wstring(commonDataPath).append(L"\\rsf-user-config.ini");
      wchar* result = NULL;

      // make sure the directory exists
      int error = CreateDirectoryW(commonDataPath, MODE_SYSTEM|MODE_MKPARENT);
      if (error) {
         static int done = warn(error, "cannot create directory \"%S\"", commonDataPath);
         result = wsdup(fileName.c_str());
      }

      // check "rsf-user-config.ini" for existence
      else if (IsFileW(fileName.c_str(), MODE_SYSTEM)) {
         result = wsdup(fileName.c_str());
      }

      // rename an existing legacy config file
      else {
         wstring legacyName = wstring(commonDataPath).append(L"\\global-config.ini");

         BOOL isLegacyFile = IsFileW(legacyName.c_str(), MODE_SYSTEM);
         if (isLegacyFile) {                                      // check legacy file for existence and rename it
            if (MoveFileExW(legacyName.c_str(), fileName.c_str(), MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH|MOVEFILE_FAIL_IF_NOT_TRACKABLE)) {
               info("renamed \"global-config.ini\" to \"rsf-user-config.ini\"");
               result = wsdup(fileName.c_str());
            }
            else {
               DWORD error = GetLastError();
               static int done = warn(ERR_WIN32_ERROR + error, "cannot rename \"%S\" to \"%S\"", legacyName.c_str(), fileName.c_str());
               if (isLegacyFile = (error != ERROR_FILE_NOT_FOUND)) {
                  result = wsdup(legacyName.c_str());             // keep using the old "global-config.ini"
               }
            }
         }

         if (!isLegacyFile) {                                     // create "rsf-user-config.ini"
            HANDLE hFile = CreateFileW(fileName.c_str(), 0, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
               if (GetLastError() != ERROR_SHARING_VIOLATION) {   // ignore if open elsewhere
                  static int done = warn(ERR_WIN32_ERROR + GetLastError(), "cannot create file \"%S\"", fileName.c_str());
               }
            }
            else {
               CloseHandle(hFile);
            }
            result = wsdup(fileName.c_str());
         }
      }

      if (!configPath) configPath = result;
      else             free(result);                              // another thread may have been faster
   }
   return configPath;
   #pragma EXPANDER_EXPORT
}


/**
 * Returns the full name of the terminal-specific configuration file.
 *
 * - This configuration file is used by the currently active terminal only.
 * - The file is named "rsf-terminal-config.ini" and is located in the terminal-specific data folder. If the terminal runs in
 *   "portable mode", the data folder is the terminal's installation folder.
 * - If the file does not exist an attempt is made to create it.
 *
 * @return char* - file name or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-id}\rsf-terminal-config.ini"
 */
const char* WINAPI GetTerminalConfigPathA() {
   static char* configPath;

   if (!configPath) {
      const wchar* wpath = GetTerminalConfigPathW();
      if (!wpath) return NULL;

      char* tmp = utf16ToAnsi(wpath);
      if (!configPath) configPath = tmp;
      else             free(tmp);                  // another thread may have been faster
   }
   return configPath;
   #pragma EXPANDER_EXPORT
}


/**
 * Returns the full name of the terminal-specific configuration file.
 *
 * - This configuration file is used by the currently active terminal only.
 * - The file is named "rsf-terminal-config.ini" and is located in the terminal-specific data folder. If the terminal runs in
 *   "portable mode", then the data folder is the terminal's installation folder.
 * - If the file does not exist an attempt is made to create it.
 * - An existing legacy config file "terminal-config.ini" is renamed to the new name.
 *
 * @return char* - file name or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-id}\rsf-terminal-config.ini"
 */
const wchar* WINAPI GetTerminalConfigPathW() {
   static wchar* configPath;

   if (!configPath) {
      const wchar* dataPath = GetTerminalDataPathW();
      if (!dataPath) return NULL;

      wstring fileName = wstring(dataPath).append(L"\\rsf-terminal-config.ini");
      wchar* result = NULL;

      // make sure the directory exists
      if (!IsDirectoryW(dataPath, MODE_SYSTEM)) {
         int error = CreateDirectoryW(dataPath, MODE_SYSTEM|MODE_MKPARENT);
         if (error) {
            static int done = warn(error, "cannot create directory \"%S\"", dataPath);
         }
         else if (!IsPortableMode()) {                            // in non-portable mode we have to create file "origin.txt"
            wstring originFile = wstring(dataPath).append(L"\\origin.txt");

            HANDLE hFile = CreateFileW(originFile.c_str(), GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
               DWORD error = GetLastError();                      // ignore if open elsewhere
               if (error != ERROR_FILE_EXISTS && error != ERROR_SHARING_VIOLATION) {
                  static int done = warn(ERR_WIN32_ERROR + error, "cannot create file \"%S\"", originFile.c_str());
               }
            }
            else if (const wchar* terminalPath = GetTerminalPathW()) {
               string content = utf16ToAnsi(wstring(terminalPath)).append(CRLF);
               DWORD bytesWritten;
               if (!WriteFile(hFile, content.c_str(), (DWORD)content.length(), &bytesWritten, NULL)) {
                  static int done = warn(ERR_WIN32_ERROR + GetLastError(), "cannot write to file \"%S\"", originFile.c_str());
               }
               CloseHandle(hFile);
            }
         }
      }

      if (!IsDirectoryW(dataPath, MODE_SYSTEM)) {
         result = wsdup(fileName.c_str());                        // directory creation attempt failed
      }

      // check "rsf-terminal-config.ini" for existence
      else if (IsFileW(fileName.c_str(), MODE_SYSTEM)) {
         result = wsdup(fileName.c_str());
      }

      else {
         // rename an existing legacy config file
         wstring legacyName = wstring(dataPath).append(L"\\terminal-config.ini");

         BOOL isLegacyFile = IsFileW(legacyName.c_str(), MODE_SYSTEM);
         if (isLegacyFile) {                                      // check legacy file for existence and rename it
            if (MoveFileExW(legacyName.c_str(), fileName.c_str(), MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH|MOVEFILE_FAIL_IF_NOT_TRACKABLE)) {
               info("renamed \"terminal-config.ini\" to \"rsf-terminal-config.ini\"");
               result = wsdup(fileName.c_str());
            }
            else {
               DWORD error = GetLastError();
               static int done = warn(ERR_WIN32_ERROR + error, "cannot rename \"%S\" to \"%S\"", legacyName.c_str(), fileName.c_str());
               if (isLegacyFile = (error != ERROR_FILE_NOT_FOUND)) {
                  result = wsdup(legacyName.c_str());             // keep using the old "terminal-config.ini"
               }
            }
         }

         if (!isLegacyFile) {                                     // create "rsf-terminal-config.ini"
            HANDLE hFile = CreateFileW(fileName.c_str(), 0, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
               if (GetLastError() != ERROR_SHARING_VIOLATION) {   // ignore if open elsewhere
                  static int done = warn(ERR_WIN32_ERROR + GetLastError(), "cannot create file \"%S\"", fileName.c_str());
               }
            }
            else {
               CloseHandle(hFile);
            }
            result = wsdup(fileName.c_str());
         }
      }

      if (!configPath) configPath = result;
      else             free(result);                              // another thread may have been faster
   }
   return configPath;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a config key exists in the user configuration.
 *
 * @param  char* section - case insensitive config section name
 * @param  char* key     - case insensitive config key
 *
 * @return BOOL
 */
BOOL WINAPI IsUserConfigKeyA(const char* section, const char* key) {
   const char* userConfig = GetUserConfigPathA();
   return userConfig && IsIniKeyA(userConfig, section, key);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a config key exists in the terminal configuration.
 *
 * @param  char* section - case insensitive config section name
 * @param  char* key     - case insensitive config key
 *
 * @return BOOL
 */
BOOL WINAPI IsTerminalConfigKeyA(const char* section, const char* key) {
   const char* terminalConfig = GetTerminalConfigPathA();
   return terminalConfig && IsIniKeyA(terminalConfig, section, key);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a config key exists in the specified .ini file.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case insensitive config section
 * @param  char* key      - case insensitive config key
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

   // look for a case insensitive match
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
 * Delete a config key from the specified .ini file. If the file does not exist an attempt is made to create it. No error is
 * returned if creation fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case insensitive config section name
 * @param  char* key      - case insensitive config key to delete
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
      if (GetLastError() != ERROR_PATH_NOT_FOUND) return !error(ERR_WIN32_ERROR + GetLastError(), "WritePrivateProfileStringA()  fileName=\"%s\", section=\"%s\", key=\"%s\"", fileName, section, key);
   }
   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a config section exists in the specified .ini file.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case insensitive config section name
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

   // look for a case insensitive match
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
 * Delete a config section from the specified .ini file. If the file does not exist an attempt is made to create it. No error
 * is returned if creation fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case insensitive config section name
 *
 * @return BOOL - success status
 */
BOOL WINAPI DeleteIniSectionA(const char* fileName, const char* section) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section  < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");

   if (!WritePrivateProfileStringA(section, NULL, NULL, fileName)) {
      if (GetLastError() != ERROR_PATH_NOT_FOUND) return !error(ERR_WIN32_ERROR + GetLastError(), "WritePrivateProfileStringA()  fileName=\"%s\", section=\"%s\"", fileName, section);
   }
   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Delete all keys from a config section but don't delete the section itself. If the section does not exist it is created.
 * If the file does not exist an attempt is made to create it. No error is returned if file creation fails.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case insensitive config section name
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
      if (GetLastError() != ERROR_PATH_NOT_FOUND) return !error(ERR_WIN32_ERROR + GetLastError(), "WritePrivateProfileSectionA()  fileName=\"%s\", section=\"%s\"", fileName, section);
   }
   return TRUE;
   #pragma EXPANDER_EXPORT
}


/**
 * Return all keys of the specified .ini file section.
 *
 * Alias of GetPrivateProfileString(). Required for MQL4.0 which doesn't support function overloading (multiple signatures).
 *
 * @param  _In_  char* fileName   - initialization file name
 * @param  _In_  char* section    - case insensitive section name
 * @param  _Out_ char* buffer     - Pointer to a buffer that receives the found keys. The buffer is filled with one or more
 *                                  NUL terminated strings. The last string is followed by a second NUL character.
 * @param  _In_  uint  bufferSize - size of the buffer in bytes (note: MQL4.0 has no unsigned integer type)
 *
 * @return uint - Number of bytes copied to the specified buffer, not including the last terminating NUL character.
 *                If the buffer is to small to hold all keys the first non-fitting key is truncated and followed by two NUL
 *                characters. In this case, the return value is equal to `bufferSize-2`.
 */
uint WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, uint bufferSize) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)section  < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section);
   if (!*section)                          return !error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)");
   if ((uint)buffer   < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer);
   if ((int)bufferSize < 2)                return !error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d (min. 2 bytes)", bufferSize);

   return GetPrivateProfileStringA(section, NULL, NULL, buffer, bufferSize, fileName);
   #pragma EXPANDER_EXPORT
}


/**
 * Return all section names of the specified .ini file.
 *
 * @param  _In_  char* fileName   - initialization file name
 * @param  _Out_ char* buffer     - Pointer to a buffer that receives the found section names. The buffer is filled with one
 *                                  or more NUL terminated strings. The last string is followed by a second NUL character.
 * @param  _In_  uint  bufferSize - size of the buffer in bytes (note: MQL4.0 has no unsigned integer type)
 *
 * @return uint - Number of bytes copied to the specified buffer, not including the last terminating NUL character.
 *                If the buffer is to small to hold all section names the first non-fitting name is truncated and followed by
 *                two NUL characters. In this case, the return value is equal to `bufferSize-2`.
 */
uint WINAPI GetIniSectionsA(const char* fileName, char* buffer, uint bufferSize) {
   if ((uint)fileName < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName);
   if (!*fileName)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)");
   if ((uint)buffer   < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer);
   if ((int)bufferSize < 2)                return !error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d (min. 2 bytes)", bufferSize);

   return GetPrivateProfileStringA(NULL, NULL, NULL, buffer, bufferSize, fileName);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a config value from an .ini file as a string. Enclosing white space and trailing comments are removed.
 *
 * @param  char* fileName                - name of the .ini file
 * @param  char* section                 - case insensitive config section name
 * @param  char* key                     - case insensitive config key
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
 * @param  char* section                 - case insensitive config section name
 * @param  char* key                     - case insensitive config key
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
 * Return a config value as a boolean. Boolean values can be expressed by "0" or "1", "On" or "Off", "Yes" or "No" and "true"
 * or "false" (case insensitive). An empty value of an existing key is considered FALSE and a numeric value is considered TRUE
 * if its nominal value is non-zero. Trailing configuration comments are ignored.
 *
 * @param  char* section      - configuration section name
 * @param  char* key          - configuration key
 * @param  BOOL  defaultValue - alternative value to return if the specified value was not found
 *
 * @return BOOL - configuration value
 */
//BOOL WINAPI GetConfigBool(const char* section, const char* key, BOOL defaultValue/*=FALSE*/) {
//   //BOOL result = GetUserConfigBool(section, key, defaultValue);
//   //return GetTerminalConfigBool(section, key, result);
//   return FALSE;
//}
