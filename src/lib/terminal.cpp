#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/format.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include <shellapi.h>
#include <shlobj.h>


/**
 * Return the terminal's build number.
 *
 * @return uint - build number or 0 in case of errors
 */
uint WINAPI GetTerminalBuild() {
   static uint build = 0;
   if (!build) {

      // TODO: some old builds don't use the standard version string format "major.minor.hotfix.build"

      uint dummy, b;
      if (!GetTerminalVersion(&dummy, &dummy, &dummy, &b))
         return(NULL);
      build = b;
   }
   return(build);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the executable the terminal was launched from.
 *
 * @return char* - filename or a NULL pointer in case of errors
 */
const char* WINAPI GetTerminalModuleFileNameA() {
   static char* filename = NULL;

   if (!filename) {
      char* buffer;
      uint size = MAX_PATH >> 1, length=size;
      while (length >= size) {
         size   <<= 1;
         buffer = (char*) alloca(size);                              // on the stack
         length = GetModuleFileName(NULL, buffer, size);             // may return a path longer than MAX_PATH
      }
      if (!length) return((char*)error(ERR_WIN32_ERROR+GetLastError(), "=> GetModuleFileName()"));

      filename = copychars(buffer);                                  // on the heap
   }
   return(filename);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the executable the terminal was launched from.
 *
 * @return WCHAR* - filename or a NULL pointer in case of errors
 */
const WCHAR* WINAPI GetTerminalModuleFileNameW() {
   static WCHAR* filename = NULL;

   if (!filename) {
      WCHAR* buffer;
      uint size = MAX_PATH >> 1, length=size;
      while (length >= size) {
         size <<= 1;
         buffer = (WCHAR*) alloca(size * sizeof(WCHAR));             // on the stack
         length = GetModuleFileNameW(NULL, buffer, size);            // may return a path longer than MAX_PATH
      }
      if (!length) return((wchar_t*)error(ERR_WIN32_ERROR+GetLastError(), "=> GetModuleFileName()"));
      filename = copywchars(buffer);                                 // on the heap
   }
   return(filename);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the terminal's version string.
 *
 * @return char* - version or a NULL pointer in case of errors
 */
const char* WINAPI GetTerminalVersion() {
   static char* version = NULL;

   if (!version) {
      // get the version numbers
      uint major, minor, hotfix, build;
      BOOL result = GetTerminalVersion(&major, &minor, &hotfix, &build);
      if (!result) return(NULL);

      // compose version string
      char* format = "%d.%d.%d.%d";
      uint size = _scprintf(format, major, minor, hotfix, build) + 1;   // +1 for the terminating '\0'
      version = new char[size];                                         // on the heap
      sprintf_s(version, size, format, major, minor, hotfix, build);
   }

   return(version);
   #pragma EXPANDER_EXPORT
}


/**
 * Read the terminal's version numbers.
 *
 * @param  _Out_ uint* major  - variable holding the major version number after return
 * @param  _Out_ uint* minor  - variable holding the minor version number after return
 * @param  _Out_ uint* hotfix - variable holding the hotfix number after return
 * @param  _Out_ uint* build  - variable holding the build number after return
 *
 * @return BOOL - success status
 */
BOOL WINAPI GetTerminalVersion(uint* major, uint* minor, uint* hotfix, uint* build) {
   //
   // TODO: read version info from the PE image in memory, not from the file (it may disappear)
   //
   // HRSRC res = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
   //
   static uint resultMajor, resultMinor, resultHotfix, resultBuild;

   if (!resultMajor) {
      const char* fileName = GetTerminalModuleFileNameA();

      // get the file's version info
      DWORD infoSize = GetFileVersionInfoSize(fileName, &infoSize);
      if (!infoSize) return(error(ERR_WIN32_ERROR+GetLastError(), "=> GetFileVersionInfoSize()"));

      char* infoBuffer = (char*) alloca(infoSize);                      // on the stack
      BOOL result = GetFileVersionInfo(fileName, NULL, infoSize, infoBuffer);
      if (!result) return(error(ERR_WIN32_ERROR+GetLastError(), "=> GetFileVersionInfo()"));

      // query the version root values
      VS_FIXEDFILEINFO* fileInfo;
      uint len;
      result = VerQueryValue(infoBuffer, "\\", (LPVOID*)&fileInfo, &len);
      if (!result) return(error(ERR_WIN32_ERROR+GetLastError(), "=> VerQueryValue()"));

      // parse the version numbers
      resultMajor  = (fileInfo->dwFileVersionMS >> 16) & 0xffff;
      resultMinor  = (fileInfo->dwFileVersionMS      ) & 0xffff;
      resultHotfix = (fileInfo->dwFileVersionLS >> 16) & 0xffff;
      resultBuild  = (fileInfo->dwFileVersionLS      ) & 0xffff;
   }

   // assign results to parameters
   if (major)  *major  = resultMajor;
   if (minor)  *minor  = resultMinor;
   if (hotfix) *hotfix = resultHotfix;
   if (build)  *build  = resultBuild;

   return(TRUE);
}


/**
 * Return the name of the terminal's installation directory.
 *
 * @return char* - directory name without trailing path separator
 */
const char* WINAPI GetTerminalPathA() {
   static char* path = NULL;

   if (!path) {
      path = copychars(GetTerminalModuleFileNameA());                // on the heap
      string str(path);
      path[str.find_last_of("\\")] = 0;
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the name of the terminal's installation directory.
 *
 * @return wstring& - directory name or an empty string in case of errors
 */
const wstring& WINAPI GetTerminalPathWs() {
   static wstring path;

   if (path.empty()) {
      const WCHAR* filename = GetTerminalModuleFileNameW();
      if (filename) {
         wstring str(filename);
         path = str.substr(0, str.find_last_of(L"\\"));
      }
   }
   return(path);
}


/**
 * Return the full path of the data directory the terminal currently uses.
 *
 * @return char* - data directory name or a NULL pointer in case of errors
 *
 *
 * VirtualStore: File and Registry Virtualization (since Vista)
 * ============================================================
 * - Used on write to a protected directory without a full administrator access token.
 * - Affected programs cannot tell the difference between their native folder and the VirtualStore.
 * - File changes are redirected to %UserProfile%\AppData\Local\VirtualStore.
 * - Protected file system locations (including subfolders):
 *   %ProgramFiles%     e.g. "C:\Program Files"
 *   %AllUsersProfile%  e.g. "C:\ProgramData" (previously "C:\Documents and Settings\All Users")
 *   %SystemRoot%       e.g. "C:\Windows"
 *
 *
 * Roaming Data Directory (since terminal build 600)
 * =================================================
 * Used in the following cases:
 *   - If UAC system is enabled. Exception: The terminal is installed in a non-protected location or runs in portable mode.
 *   - If the current user has limited rights to write data to the installation directory.
 *
 * If none of the above conditions is true data is stored in the installation directory.
 *
 *
 * @see  https://www.mql5.com/en/articles/1388
 * @see  https://social.technet.microsoft.com/wiki/contents/articles/6083.windows-xp-folders-and-locations-vs-windows-7-and-vista.aspx
 * @see  https://msdn.microsoft.com/en-us/library/bb756960.aspx
 */
const char* WINAPI GetTerminalDataPathA() {
   //
   // 1. check if old build or launched in portable mode
   // 1.1  yes => use installation directory (WoW64 redirection of old builds to the virtual store)
   // 1.2  no  => new build in standard mode
   //      2. check if the executable was removed (the terminal removed a reparse point)
   //      2.1  yes
   //           3. check if a locked logfile exists
   //           3.1  yes => use the directory (write permission granted)
   //           3.2  no  => use roaming data directory
   //      2.2  no
   //           4. explicitely check write permission (the logfile location may be linked)
   //              4.1  permission granted => use the directory
   //              4.2  permission denied  => use roaming data directory
   //
   static char* dataPath = NULL;

   if (!dataPath) {
      const char* terminalPath    = GetTerminalPathA();
      const char* roamingDataPath = GetTerminalRoamingDataPathA();

      // 1. check if an old build or launched in portable mode
      if (TerminalIsPortableMode()) {
         // 1.1 yes => use installation directory (WoW64 redirection of old builds to the virtual store)
         //debug("1.1  TerminalIsPortableMode() = TRUE");
         dataPath = copychars(terminalPath);                            // on the heap
      }
      else {
         // 1.2 no => new build in standard mode => 2. check if the executable was removed (the terminal removed a reparse point)
         //debug("1.2  TerminalIsPortableMode() = FALSE");

         if (!IsFileA(GetTerminalModuleFileNameA())) {
            //debug("2.1  the executable was removed");

            // 2.1 yes => 3. check if a locked logfile exists
            if (TerminalIsLockedLogfile(string(terminalPath).append(localTimeFormat(GetGmtTime(), "\\logs\\%Y%m%d.log")))) {
               // 3.1 yes => use the directory (write permission must exist)
               //debug("3.1  a locked logfile exists");
               dataPath = copychars(terminalPath);                      // on the heap
            }
            else {
               // 3.2 no => use roaming data directory
               //debug("3.2  a locked logfile doesn't exist");
               dataPath = copychars(roamingDataPath);                   // on the heap
            }
         }
         else {
            //debug("2.1  the executable exists");
            // 2.2 no => 4. check write permission (the logfile location may be linked)
            if (TerminalHasWritePermission(terminalPath)) {
               // 4.1 permission granted => use the directory
               //debug("4.1  write permission in %s", terminalPath);
               dataPath = copychars(terminalPath);                      // on the heap
            }
            else {
               // 4.2 permission denied => use roaming data directory
               //debug("4.2  no write permission in %s", terminalPath);
               dataPath = copychars(roamingDataPath);                   // on the heap
            }
         }
      }
   }
   return(dataPath);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's common data directory. This function emulates the functionality of
 * TerminalInfoString(TERMINAL_COMMONDATA_PATH)) introduced by MQL4.5. The common data directory is shared between all
 * terminals installed by a user. The function does not check if the returned path exists.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors,
 *                 e.g. %UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common
 */
const char* WINAPI GetTerminalCommonDataPathA() {
   static char* result = NULL;

   if (!result) {
      char appDataPath[MAX_PATH];                                                      // resolve CSIDL_APPDATA
      if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath)))
         return((char*)error(ERR_WIN32_ERROR+GetLastError(), "=> SHGetFolderPath()"));

      string dir = string(appDataPath).append("\\MetaQuotes\\Terminal\\Common");       // create the resulting path
      result = copychars(dir);                                                         // on the heap
   }
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's roaming data directory. The function does not check if the returned path exists.
 * Depending on terminal version and runtime mode the currently used data directory may differ.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
 *                 i.e. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-id}"
 *
 * @see  GetTerminalDataPath() to get the path of the data directory currently used
 */
const char* WINAPI GetTerminalRoamingDataPathA() {
   static char* result = NULL;

   if (!result) {
      char appDataPath[MAX_PATH];                                                      // resolve CSIDL_APPDATA
      if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath)))
         return((char*)error(ERR_WIN32_ERROR+GetLastError(), "=> SHGetFolderPath()"));

      wstring terminalPath = GetTerminalPathWs();                                      // get terminal installation path
      StrToUpper(terminalPath);                                                        // convert to upper case
      char* md5 = MD5Hash(terminalPath.c_str(), terminalPath.length()* sizeof(WCHAR)); // calculate MD5 hash

      string dir = string(appDataPath).append("\\MetaQuotes\\Terminal\\")              // create the resulting path
                                      .append(StrToUpper(md5));
      result = copychars(dir);                                                         // on the heap
      delete[] md5;
   }
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not the terminal has write permission in the specified directory.
 *
 * @param  char* dir - directory name
 *
 * @return BOOL
 */
BOOL WINAPI TerminalHasWritePermission(const char* dir) {
   //debug("checking dir: %s", dir);

   if (!IsDirectoryA(dir))
      return(FALSE);

   char tmpFilename[MAX_PATH];

   if (!GetTempFileName(dir, "rsf", 0, tmpFilename)) {
      //debug("GetTempFileName()  [%s]", ErrorToStr(ERR_WIN32_ERROR+GetLastError()));
      return(FALSE);
   }
   if (!DeleteFile(tmpFilename)) {
      error(ERR_WIN32_ERROR+GetLastError(), "=> DeleteFile(%s)", tmpFilename);
      return(FALSE);
   }
   return(TRUE);
}


/**
 * Whether or not the specified file exists and is locked with the sharing modes of a logfile. This function canot see which
 * process is holding the lock.
 *
 * @param  string& filename - full filename
 *
 * @return BOOL
 */
BOOL WINAPI TerminalIsLockedLogfile(const string& filename) {
   //debug("checking: %s", filename.c_str());

   if (IsFileA(filename.c_str())) {
      // OF_READWRITE|OF_SHARE_COMPAT must succeed
      HFILE hFile = _lopen(filename.c_str(), OF_READWRITE|OF_SHARE_COMPAT);
      if (hFile == HFILE_ERROR)
         return(FALSE);
      _lclose(hFile);                                                // success

      // OF_READWRITE|OF_SHARE_EXCLUSIVE must fail with ERROR_SHARING_VIOLATION
      hFile = _lopen(filename.c_str(), OF_READWRITE|OF_SHARE_EXCLUSIVE);
      if (hFile == HFILE_ERROR)
         return(GetLastError() == ERROR_SHARING_VIOLATION);
      _lclose(hFile);                                                // success is an error
   }
   else {
      //debug("file not found: %s", filename.c_str());
   }
   return(FALSE);
}


/**
 * Whether or not the terminal was launched in portable mode.
 *
 * @return BOOL
 *
 * Bug: The terminal's command line parser checks parameters incorrectly. It also enables the "portable mode" switch if one
 *      of the command line parameters *starts* with the string "/portable" (e.g. "/portablepoo" enables portable mode too).
 *      The logic of this function mirrors the bug.
 */
BOOL WINAPI TerminalIsPortableMode() {
   static int isPortable = -1;

   if (isPortable < 0) {
      uint build = GetTerminalBuild(); if (!build) return(FALSE);

      if (build <= 509) {
         isPortable = TRUE;                                    // always TRUE, on access errors the system uses virtualization
      }
      else {
         int argc;
         LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

         for (int i=1; i < argc; ++i) {
            if (StringStartsWith(argv[i], L"/portable")) {     // StartsWith() instead of Compare()
               isPortable = TRUE;
               break;
            }
         }
         if (isPortable < 0)
            isPortable= FALSE;
      }
   }
   return(isPortable);
   #pragma EXPANDER_EXPORT
}
