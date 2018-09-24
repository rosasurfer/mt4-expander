#include "expander.h"
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

   // TODO: some old builds don't use the standard version string format "major.minor.hotfix.build"

   uint dummy, build;
   if (!GetTerminalVersion(&dummy, &dummy, &dummy, &build))
      return(NULL);
   return(build);
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
   static uint resultMajor, resultMinor, resultHotfix, resultBuild;

   if (!resultMajor) {
      // resolve the executable's full file name
      char* fileName;
      uint size = MAX_PATH >> 1, length=size;
      while (length >= size) {
         size   <<= 1;
         fileName = (char*) alloca(size);                               // on the stack
         length   = GetModuleFileName(NULL, fileName, size);            // may return a path longer than MAX_PATH
      }
      if (!length) return(error(ERR_WIN32_ERROR+GetLastError(), "=> GetModuleFileName()"));

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
   static char* result = NULL;

   if (!result) {
      char* fileName;
      uint size = MAX_PATH >> 1, length=size;
      while (length >= size) {
         size   <<= 1;
         fileName = (char*) alloca(size);                            // on the stack
         length   = GetModuleFileName(NULL, fileName, size);         // may return a path longer than MAX_PATH
      }
      if (!length) return((char*)error(ERR_WIN32_ERROR+GetLastError(), "=> GetModuleFileName()"));

      result = copychars(fileName);                                  // on the heap
      string name(result);
      result[name.find_last_of("\\")] = 0;
   }
   return(result);
}


/**
 * Return the name of the terminal's installation directory.
 *
 * @return wstring& - directory name without trailing path separator
 */
const wstring& WINAPI GetTerminalPathWs() {
   static wstring result;

   if (result.empty()) {
      wchar_t* fileName;
      uint size = MAX_PATH >> 1, length=size;
      while (length >= size) {
         size   <<= 1;
         fileName = (wchar_t*) alloca(size * sizeof(WCHAR));         // on the stack
         length   = GetModuleFileNameW(NULL, fileName, size);        // may return a path longer than MAX_PATH
      }
      if (!length) {
         error(ERR_WIN32_ERROR+GetLastError(), "=> GetModuleFileName()");
         return(result);
      }
      wstring str(fileName);
      result = str.substr(0, str.find_last_of(L"\\"));
   }
   return(result);
}


/**
 * Return the full path of the data directory the terminal currently uses.
 *
 * @param  _In_     char* filename - path to a guiding data file in the "files" directory accessible to MQL
 * @param  _In_Out_ int*  status   - in:  MQL error status when creating the file
 *                                   out: error status indicating the error detail of the function (if any)
 *
 * @return char* - data directory name or a NULL pointer in case of errors with parameter error containing the error detail
 */
const char* WINAPI GetTerminalDataPathA(const char* filename, int* status) {
   if (filename && (uint)filename < MIN_VALID_POINTER) return((char*)error(*status=ERR_INVALID_PARAMETER, "invalid parameter filename: 0x%p (not a valid pointer)", filename));
   if (status   && (uint)status   < MIN_VALID_POINTER) return((char*)error(        ERR_INVALID_PARAMETER, "invalid parameter status: 0x%p (not a valid pointer)", status));
   //
   // VirtualStore: File and Registry Virtualization (since Vista)
   // ============================================================
   // - Used on write to a protected directory without a full administrator access token.
   // - Affected programs cannot tell the difference between their native folder and the VirtualStore.
   // - File changes are redirected to %UserProfile%\AppData\Local\VirtualStore.
   // - Protected file system locations (including subfolders):
   //   %ProgramFiles%     e.g. "C:\Program Files"
   //   %AllUsersProfile%  e.g. "C:\ProgramData" (previously "C:\Documents and Settings\All Users")
   //   %SystemRoot%       e.g. "C:\Windows"
   //
   //
   // Roaming Data Directory (since Terminal build 600)
   // =================================================
   // - Used in the following cases:
   //   • If UAC system is enabled (exception: the terminal is installed in a non-protected location or runs in portable mode).
   //   • If the current user has limited rights to write data to the installation directory.
   // - If none of the above conditions is met terminal data is stored in the installation directory.
   //
   //
   // @see  https://www.mql5.com/en/articles/1388
   // @see  https://social.technet.microsoft.com/wiki/contents/articles/6083.windows-xp-folders-and-locations-vs-windows-7-and-vista.aspx
   // @see  https://msdn.microsoft.com/en-us/library/bb756960.aspx
   //
   *status = ERROR_SUCCESS;


   // old builds and new builds in portable mode: installation directory or virtual store (we rely on WoW64 redirection)
   if (TerminalIsPortableMode())
      return(GetTerminalPathA());

   // new builds: installation directory, on restricted access roaming data directory

   /*
   if (!IsFile(terminalModule)) {
      // MetaTrader tries to delete links by removing it's own parent directory
      // data path is roaming directory
   }
   else {
   }



   if (installDirInProgramFiles) {
   }
   else {
   }


   const char* terminalPath    = GetTerminalPathA();
   const char* roamingDataPath = GetTerminalRoamingDataPathA();

   string logfile = string(roamingDataPath) +"\\logs\\20180915.log";

   HFILE hFile = _lopen(logfile.c_str(), OF_READWRITE|OF_SHARE_EXCLUSIVE);
   if (hFile == HFILE_ERROR) return((char*)warn(ERR_WIN32_ERROR+GetLastError(), "error opening logfile with OF_READWRITE|OF_SHARE_EXCLUSIVE"));
   _lclose(hFile);
   debug("successfully opened the logfile with OF_READWRITE|OF_SHARE_EXCLUSIVE");
   */

   return(NULL);
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
 * Whether or not the terminal was launched in portable mode.
 *
 * Bug: The terminal's command line parser checks parameters incorrectly. It also enables the "portable mode" switch if one
 *      of the command line parameters *starts* with the string "/portable" (e.g. "/portablepoo" enables portable mode too).
 *      The logic of this function mirrors the bug.
 *
 * @return BOOL
 */
BOOL WINAPI TerminalIsPortableMode() {
   static int result = -1;

   if (result < 0) {
      if (GetTerminalBuild() <= 509) {
         result = TRUE;                                        // always TRUE, on access errors the system uses virtualization
      }
      else {
         int argc;
         LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

         for (int i=1; i < argc; ++i) {
            if (StringStartsWith(argv[i], L"/portable")) {     // StartsWith() instead of Compare()
               result = TRUE;
               break;
            }
         }
         if (result < 0)
            result = FALSE;
      }
   }
   return(result);
   #pragma EXPANDER_EXPORT
}
