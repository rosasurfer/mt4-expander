#include "expander.h"
#include "util/helper.h"
#include "util/string.h"
#include "util/terminal.h"

#include "shlobj.h"


/**
 * Return the terminal's build number.
 *
 * @return uint - build number or 0 in case of errors
 */
uint WINAPI GetTerminalBuild() {
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
         length   = GetModuleFileName(NULL, fileName, size);
      }
      if (!length) return(error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileName() failed"));

      // get the file's version info
      DWORD infoSize = GetFileVersionInfoSize(fileName, &infoSize);
      if (!infoSize) return(error(ERR_WIN32_ERROR+GetLastError(), "GetFileVersionInfoSize() returned 0"));

      char* infoBuffer = (char*) alloca(infoSize);                      // on the stack
      BOOL result = GetFileVersionInfo(fileName, NULL, infoSize, infoBuffer);
      if (!result) return(error(ERR_WIN32_ERROR+GetLastError(), "GetFileVersionInfo() returned FALSE"));

      // query the version root values
      VS_FIXEDFILEINFO* fileInfo;
      uint len;
      result = VerQueryValue(infoBuffer, "\\", (LPVOID*)&fileInfo, &len);
      if (!result) return(error(ERR_WIN32_ERROR+GetLastError(), "VerQueryValue() returned FALSE"));

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
      result = new char[MAX_PATH];                                   // on the heap
      GetModuleFileName(NULL, result, MAX_PATH);                     // TODO: handle errors

      string name(result);
      string::size_type pos = name.find_last_of("\\");
      result[pos] = '\0';
   }
   return(result);
}


/**
 * Return the name of the terminal's installation directory.
 *
 * @return wstring* - directory name without trailing path separator
 */
const wstring& WINAPI GetTerminalPathWs() {
   static wstring result;

   if (result.empty()) {
      wchar_t buffer[MAX_PATH];                                      // on the stack
      GetModuleFileNameW(NULL, buffer, MAX_PATH);                    // TODO: handle errors

      wstring name(buffer);
      wstring::size_type pos = name.find_last_of(L"\\");
      result = name.substr(0, pos);
   }
   return(result);
}


/**
 * Return the full path of the data directory the terminal currently uses.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
 *
 *
 * @see  https://www.mql5.com/en/articles/1388
 * @see  GetTerminalRoamingDataPath() to get the path of the roaming data directory (which might currently not be in use)
 */
const char* WINAPI GetTerminalDataPathA() {
   //if (hstPath && (uint)hstPath < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hstPath = 0x%p (not a valid pointer)", hstPath));
   //
   // @param  _In_ char* hstPath - path to a guiding history file; may not be needed if the terminal's data directory has already
   //                              been resolved before
   //
   // @return char* - Directory name without trailing path separator or a NULL pointer in case of errors. The function always
   //                 resets EXECUTION_CONTEXT.dllError. If NULL is returned and EXECUTION_CONTEXT.dllError doesn't indicate an
   //                 actual error the terminal's data directory cannot be resolved without a guiding history file in parameter
   //                 hstPath.
   //
   //
   // VirtualStore: File and Registry Virtualization (since Vista)
   // ============================================================
   // - Used on write to a protected directory without a full administrator access token.
   // - Affected programs cannot tell the difference between their native folder and the VirtualStore.
   // - File changes are redirected to %UserProfile%\AppData\Local\VirtualStore.
   // - Protected file system locations (including subfolders):
   //   %ProgramFiles%    (e.g. C:\Program Files)
   //   %AllUsersProfile% (e.g. C:\ProgramData – was previously C:\Documents and Settings\All Users)
   //   %SystemRoot%      (e.g. C:\Windows)
   //
   //
   // Roaming Data Directory (since Terminal build 600)
   // =================================================
   // - Used in the following cases:
   //   • If UAC system is enabled (exception: the terminal is installed in a non-protected location or runs in portable mode).
   //   • If the current user has limited rights to write data to the installation directory.
   // - If none of the above conditions is met terminal data is stored in the installation directory.
   //


   // old builds: installation directory or VirtualStore => we rely on virtualization
   if (GetTerminalBuild() <= 509)
      return(GetTerminalPathA());


   // new builds: installation directory or roaming data directory
   static char* result = NULL;
   if (!result) {
      debug("build %d", GetTerminalBuild());

      const char* terminalPath = GetTerminalPathA();
      debug("installation path: %s", terminalPath);

      const char* roamingDataPath = GetTerminalRoamingDataPathA();
      debug("roaming data path: %s", roamingDataPath);

      string logfile = string(roamingDataPath) +"\\logs\\20180915.log";

      HFILE hFile = _lopen(logfile.c_str(), OF_READWRITE|OF_SHARE_EXCLUSIVE);
      if (hFile == HFILE_ERROR) return((char*)warn(ERR_WIN32_ERROR+GetLastError(), "error opening logfile with OF_READWRITE|OF_SHARE_EXCLUSIVE"));
      _lclose(hFile);
      debug("successfully opened the logfile with OF_READWRITE|OF_SHARE_EXCLUSIVE");
   }
   return(result);

   // @see  https://stackoverflow.com/questions/95510/how-to-detect-whether-vista-uac-is-enabled
   // @see  http://www.itwriting.com/blog/198-c-code-to-detect-uac-elevation-on-vista.html
   // @see  https://gist.github.com/cstrahan/1174974/3b7171830b9fc45b838551f628d647d410c3a99f
   // @see  https://msdn.microsoft.com/en-us/library/bb756960.aspx
   //
   // @see  https://stackoverflow.com/questions/1816691/how-do-i-resolve-a-canonical-filename-in-windows
   // @see  http://pdh11.blogspot.com/2009/05/pathcanonicalize-versus-what-it-says-on.html
   // @see  PathCanonicalize()
   // @see  GetFinalPathNameByHandle();
   //
   // @see  https://blogs.msdn.microsoft.com/oldnewthing/20100212-00/?p=14963
   // @see  https://stackoverflow.com/questions/221417/how-do-i-programmatically-access-the-target-path-of-a-windows-symbolic-link
   //
   // @see  https://social.technet.microsoft.com/wiki/contents/articles/6083.windows-xp-folders-and-locations-vs-windows-7-and-vista.aspx

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
         return((char*)error(ERR_WIN32_ERROR+GetLastError(), "SHGetFolderPath() failed"));

      string dir = string(appDataPath).append("\\MetaQuotes\\Terminal\\Common");       // create the resulting path
      result = strcpy(new char[dir.length()+1], dir.c_str());                          // on the heap
   }
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's roaming data directory. The function does not check if the returned path exists.
 * Depending on terminal version and runtime mode the currently used data directory may differ.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
 *                 i.e. %UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installationId}
 *
 * @see  GetTerminalDataPath() to get the path of the data directory currently used
 */
const char* WINAPI GetTerminalRoamingDataPathA() {
   static char* result = NULL;

   if (!result) {
      char appDataPath[MAX_PATH];                                                      // resolve CSIDL_APPDATA
      if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath)))
         return((char*)error(ERR_WIN32_ERROR+GetLastError(), "SHGetFolderPath() failed"));

      wstring terminalPath = GetTerminalPathWs();                                      // get terminal installation path
      StrToUpper(terminalPath);                                                        // convert to upper case
      char* md5 = MD5Hash(terminalPath.c_str(), terminalPath.length()* sizeof(WCHAR)); // calculate MD5 hash

      string dir = string(appDataPath).append("\\MetaQuotes\\Terminal\\")              // create the resulting path
                                      .append(StrToUpper(md5));
      delete[] md5;
      result = strcpy(new char[dir.length()+1], dir.c_str());                          // on the heap
   }
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 *
 * @return char*
 */
void WINAPI Test() {

   char path[MAX_PATH];

   if (FAILED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, path))) {
      error(ERR_WIN32_ERROR+GetLastError(), "SHGetFolderPath() failed");
      return;
   }

   debug("path: %s", path);
   //#pragma EXPANDER_EXPORT
}
