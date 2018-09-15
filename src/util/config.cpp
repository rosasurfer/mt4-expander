#include "expander.h"
#include "util/file.h"
#include "util/terminal.h"

#include "shlobj.h"


/**
 * Return the full filename of the MQL framework's global configuration file. The file is shared between all terminals used
 * by the user. If the file does not exist it is created.
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
      configPath = strcpy(new char[iniFile.length()+1], iniFile.c_str());           // on the heap

      if (!IsFile(configPath)) {
         // try to create the file
         int error = NO_ERROR;
         if (!IsDirectory(commonPath)) {
            error = SHCreateDirectoryEx(NULL, commonPath, NULL);                    // create directory
            if (error == ERROR_ACCESS_DENIED) {
               debug("access denied while trying to create \"%s\"", commonPath);    // log access errors but don't fail
            }
            else if (error) {                                                       // signal hard errors but don't fail
               error(ERR_WIN32_ERROR+GetLastError(), "creation of path \"%s\" failed", commonPath);
            }
         }
         if (!error) {
            HFILE hFile = _lcreat(configPath, FILE_ATTRIBUTE_NORMAL);
            if (hFile == HFILE_ERROR) {
               error = GetLastError();
               if (error == ERROR_ACCESS_DENIED) {
                  debug("access denied while trying to create \"%s\"", configPath); // log access errors but don't fail
               }
               else {                                                               // signal hard errors but don't fail
                  error(ERR_WIN32_ERROR+error, "creation of file \"%s\" failed", configPath);
               }
            }
         }
      }
   }

   return(configPath);
   #pragma EXPANDER_EXPORT
}
