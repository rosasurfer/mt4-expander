#include "expander.h"
#include "util/file.h"
#include "util/terminal.h"
#include "util/toString.h"

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
         int error = NO_ERROR;

         if (!IsDirectory(commonPath)) {                                            // create the directory if needed
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
         }
      }
   }

   return(configPath);
   #pragma EXPANDER_EXPORT
}
