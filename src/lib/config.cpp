#include "expander.h"
#include "lib/conversion.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include <shlobj.h>


/**
 * Return the full filename of the MQL framework's global configuration file. The file is shared between all terminals used
 * by the current user. If the file does not exist an attempt is made to create it.
 *
 * @return char* - filename or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common\global-config.ini".
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
 * Return the full filename of the MQL framework's local (i.e. terminal specific) configuration file.
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
               return((char*)error(ERR_WIN32_ERROR+GetLastError(), "=> SHGetFolderPath()"));

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
 * Alias of GetPrivateProfileStringA() to enable the use of different function signatures in MQL4
 *
 * Return all keys of an .ini file section.
 *
 * @param  __In__  char* fileName   - initialization file name
 * @param  __In__  char* section    - initialization file section name
 * @param  __Out__ char* buffer     - Pointer to a buffer that receives the found keys. The buffer is filled with one or more
 *                                    null-terminated strings. The last string is followed by a second null character.
 * @param  __In__  DWORD bufferSize - size of the buffer in bytes (minimum 2 bytes)
 *
 * @return DWORD - Number of bytes copied to the specified buffer, not including the last terminating null character.
 *                 If the buffer is not large enough to hold all found keys the first non-fitting key is truncated and the
 *                 return value is equal to the size of bufferSize minus two.
 */
DWORD WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, DWORD bufferSize) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!strlen(fileName))                  return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)section  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section));
   if (!strlen(section))                   return(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)"));
   if ((uint)buffer   < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (bufferSize < 2)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d (min. 2 bytes)", bufferSize));

   return(GetPrivateProfileString(section, NULL, NULL, buffer, bufferSize, fileName));
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not a configuration key exists in an .ini file.
 *
 * @param  char* fileName - name of the .ini file
 * @param  char* section  - case-insensitive configuration section
 * @param  char* key      - case-insensitive configuration key
 *
 * @return BOOL
 */
BOOL WINAPI IsIniKey(const char* fileName, const char* section, const char* key) {
   if ((uint)fileName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
   if (!strlen(fileName))                  return(error(ERR_INVALID_PARAMETER, "invalid parameter fileName: \"\" (empty)"));
   if ((uint)section  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter section: 0x%p (not a valid pointer)", section));
   if (!strlen(section))                   return(error(ERR_INVALID_PARAMETER, "invalid parameter section: \"\" (empty)"));

   // read all keys and handle a too small buffer
   char* buffer    = NULL;
   uint bufferSize = 256;
   uint chars = bufferSize-2;

   while (chars == bufferSize-2) {
      bufferSize <<= 1;
      buffer = new char[bufferSize];                     // on the heap as a section may be big
      chars = GetPrivateProfileString(section, NULL, NULL, buffer, bufferSize, fileName);
   }

   // look for a case-insensitive match
   bufferSize  = strlen(key)+1;
   char* lKey  = StrToLower((char*)memcpy(alloca(bufferSize), key, bufferSize));
   BOOL result = FALSE;

   char* str = buffer;                                   // The buffer is filled with one or more null-terminated strings.
   while (*str) {                                        // The last string is followed by a second null character.
      // loop as long as there are non-empty strings
      if (StrCompare(StrToLower(str), lKey)) {
         result = TRUE;
         break;
      }
      str += strlen(str) + 1;
   }

   delete[] buffer;
   return(result);
   #pragma EXPANDER_EXPORT
}
