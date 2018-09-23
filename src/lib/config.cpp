#include "expander.h"
#include "lib/conversion.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include "shlobj.h"


/**
 * Return the full filename of the MQL framework's global configuration file. The file is shared between all terminals used
 * by the current user. If the file does not exist an attempt is made to create it.
 *
 * @return char* - filename or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common\global-config.ini".
 */
const char* WINAPI GetGlobalConfigPathA() {
   static char* configPath = NULL;

   if (!configPath) {
      const char* commonPath = GetTerminalCommonDataPathA();
      if (!commonPath) return(NULL);

      string iniFile = string(commonPath).append("\\global-config.ini");
      configPath = copychars(iniFile);                                              // on the heap

      if (!IsFileA(configPath)) {
         int error = NO_ERROR;

         if (!IsDirectoryA(commonPath)) {                                           // make sure the directory exists
            error = SHCreateDirectoryEx(NULL, commonPath, NULL);
            if (error==ERROR_ACCESS_DENIED || error==ERROR_PATH_NOT_FOUND) {        // log access errors but don't fail
               debug("cannot create directory \"%s\"  [%s]", commonPath, ErrorToStr(ERR_WIN32_ERROR+error));
            }
            else if (error) {                                                       // signal hard errors but don't fail
               error(ERR_WIN32_ERROR+error, "cannot create directory \"%s\"", commonPath);
            }
         }

         if (!error) {                                                              // try to create the file
            HFILE hFile = _lcreat(configPath, FILE_ATTRIBUTE_NORMAL);
            if (hFile == HFILE_ERROR) {
               error = GetLastError();
               if (error == ERROR_ACCESS_DENIED) {                                  // log access errors but don't fail
                  debug("cannot create file \"%s\"  [%s]", configPath, ErrorToStr(ERR_WIN32_ERROR+error));
               }
               else {                                                               // signal hard errors but don't fail
                  error(ERR_WIN32_ERROR+error, "cannot create file \"%s\"", configPath);
               }
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
   static char* configPath = NULL;

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
                  int error = NO_ERROR;
                  if (!IsDirectoryA(roamingDataPath)) {                                      // make sure the directory exists
                     error = SHCreateDirectoryEx(NULL, roamingDataPath, NULL);               // log errors but continue
                     if (error==ERROR_ACCESS_DENIED || error==ERROR_PATH_NOT_FOUND) debug("cannot create directory \"%s\"  [%s]", roamingDataPath, ErrorToStr(ERR_WIN32_ERROR+error));
                     else if (error)                                                error(ERR_WIN32_ERROR+error, "cannot create directory \"%s\"", roamingDataPath);
                  }
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
            char programFilesPath[MAX_PATH];             // resolve CSIDL_PROGRAM_FILES
            if (FAILED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, programFilesPath)))
               return((char*)error(ERR_WIN32_ERROR+GetLastError(), "SHGetFolderPath() failed"));

            // TODO: resolve symlinks/junctions in TerminalPath() before Comparison

            if (StringStartsWith(GetTerminalPathA(), programFilesPath)) {
               // 5.1 yes => continue with 4.2 => create a file in the roaming data directory
               HFILE hFile = _lcreat(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
               if (hFile == HFILE_ERROR) debug("cannot create file \"%s\"  [%s]", filename.c_str(), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
               else                      _lclose(hFile);
            }
            else {
               // 5.2 no => continue with 3 => check if a config file exists in the installation directory
               filename = string(GetTerminalPathA()).append("\\local-config.ini");
               if (!IsFileA(filename.c_str())) {   // use it or 3.2 no => 4. attempt to create it
                  HFILE hFile = _lcreat(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
                  if (hFile == HFILE_ERROR) {
                     debug("cannot create file \"%s\"  [%s]", filename.c_str(), ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
                     // 4.2 permission denied => create a file in the roaming data directory
                     int error = NO_ERROR;
                     if (!IsDirectoryA(roamingDataPath)) {                                   // make sure the directory exists
                        error = SHCreateDirectoryEx(NULL, roamingDataPath, NULL);            // log errors but continue
                        if (error==ERROR_ACCESS_DENIED || error==ERROR_PATH_NOT_FOUND) debug("cannot create directory \"%s\"  [%s]", roamingDataPath, ErrorToStr(ERR_WIN32_ERROR+error));
                        else if (error)                                                error(ERR_WIN32_ERROR+error, "cannot create directory \"%s\"", roamingDataPath);
                     }
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
      configPath = copychars(filename);                              // on the heap
   }

   return(configPath);
   #pragma EXPANDER_EXPORT
}
