#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/format.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/lock/Locker.h"

#include <shellapi.h>
#include <shlobj.h>


uint g_terminalBuild;                                 // terminal build number


/**
 * Find the window handle of the "Input parameters" dialog of the MQL program matching the specified type and
 * case-insensitive name.
 *
 * @param  ProgramType programType
 * @param  char*       programName
 *
 * @return HWND - window handle or NULL if no open "Input parameters" dialog was found;
 *                INVALID_HWND (-1) in case of errors
 */
HWND WINAPI FindInputDialog(ProgramType programType, const char* programName) {
   if ((uint)programName < MIN_VALID_POINTER) return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName)));
   if (!*programName)                         return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter programName: \"\" (empty)")));

   string title(programName);
   if (programType == PT_INDICATOR) title.insert(0, "Custom Indicator - ");

   char* className = "#32770";
   DWORD processId, self = GetCurrentProcessId();
   HWND hWndDlg = NULL;

   while (hWndDlg = FindWindowEx(NULL, hWndDlg, className, title.c_str())) {
      GetWindowThreadProcessId(hWndDlg, &processId);
      if (processId == self) {
         if (programType == PT_INDICATOR) {
            if (FindWindowEx(hWndDlg, NULL, className, "Common"))                      // common tab: "Common" (no tab "Parameters")
               break;
         }
         else if (programType==PT_EXPERT || programType==PT_SCRIPT) {
            if (FindWindowEx(hWndDlg, NULL, className, "Expert Advisor settings"))     // common tab: "Expert Advisor settings" (no tab "Parameters")
               break;                                                                  // TODO: separate experts and scripts
         }
         //else if (built-in-indicator) {
         //   if (FindWindowEx(hWndDlg, NULL, className, "Parameters"))                // tab "Parameters" (no common tab)
         //      break;
         //}
         else return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter programType: %d (unknown)", programType)));
      }
   }
   //if (hWndDlg) debug("input dialog \"%s\" found: %p", programName, hWndDlg);
   //else         debug("input dialog \"%s\" not found", programName);

   return(hWndDlg);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the loaded MT4Expander DLL.
 *
 * @return char* - filename or a NULL pointer in case of errors
 */
const char* WINAPI GetLibraryModuleFileNameA() {
   static char* filename;

   if (!filename) {
      char* buffer;
      uint size=MAX_PATH >> 1, length=size;
      while (length >= size) {
         size <<= 1;
         buffer = (char*) alloca(size);                              // on the stack
         length = GetModuleFileName(HMODULE_DLL, buffer, size);      // may return a path longer than MAX_PATH
      }
      if (!length) return((char*)error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileName()"));

      filename = strdup(buffer);                                     // on the heap
   }
   return(filename);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the module handle of the current module, i.e. of this DLL.
 *
 * @return HMODULE - DLL module handle (not the terminal.exe's module handle)
 *
 * Note: backward-compatible to Windows 2000
 */
HMODULE WINAPI GetLibraryModuleW2K() {
   MEMORY_BASIC_INFORMATION mbi = {};
   VirtualQuery(GetLibraryModuleW2K, &mbi, sizeof(mbi));
   return((HMODULE)mbi.AllocationBase);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the module handle of the current module, i.e. of this DLL.
 *
 * @return HMODULE - DLL module handle (not the terminal.exe's module handle)
 *
 * Note: requires at least Windows XP or Windows Server 2003
 */
HMODULE WINAPI GetLibraryModuleXP() {
   HMODULE hModule = NULL;
   GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)GetLibraryModuleXP, &hModule);
   return(hModule);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL directory the terminal currently uses.
 *
 * @return char* - MQL directory name as an Ansi string or a NULL pointer in case of errors
 */
const char* WINAPI GetMqlDirectoryA() {
   static char* mqlDirectory;

   if (!mqlDirectory) {
      const char* dataPath = GetTerminalDataPathA();
      if (!dataPath) return(NULL);

      string dir(dataPath);
      if (g_terminalBuild <= 509) dir.append("\\experts");
      else                        dir.append("\\mql4");

      mqlDirectory = strdup(dir.c_str());
   }
   return(mqlDirectory);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the terminal's build number.
 *
 * @return uint - build number or 0 in case of errors
 */
uint WINAPI GetTerminalBuild() {
   static uint build;                                                      // global cache (g_terminalBuild) for DLL,
   if (!build) {                                                           // local cache for external calls (MQL)
      const VS_FIXEDFILEINFO* fileInfo = GetTerminalVersionFromImage();
      if (!fileInfo)          fileInfo = GetTerminalVersionFromFile();
      if (fileInfo)
         build = fileInfo->dwFileVersionLS & 0xffff;
   }
   return(build);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the window handle of the application's main window.
 *
 * @return HWND - handle or NULL (0) in case of errors
 */
HWND WINAPI GetTerminalMainWindow() {
   static HWND hWndMain;

   if (!hWndMain) {
      DWORD processId, self = GetCurrentProcessId();
      uint  size = 255;
      char* className = (char*) alloca(size);                        // on the stack: buffer for window class name

      HWND hWndNext = GetTopWindow(NULL);

      while (hWndNext) {                                             // iterate over all top-level windows
         GetWindowThreadProcessId(hWndNext, &processId);
         if (processId == self) {
            if (!GetClassName(hWndNext, className, size)) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetClassName()"));
            if (StrCompare(className, "MetaQuotes::MetaTrader::4.00"))
               break;
         }
         hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
      }
      if (!hWndNext) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "cannot find terminal main window"));

      hWndMain = hWndNext;
   }
   return(hWndMain);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the executable the terminal was launched from.
 *
 * @return char* - filename or a NULL pointer in case of errors
 */
const char* WINAPI GetTerminalModuleFileNameA() {
   static char* filename;

   if (!filename) {
      char* buffer;
      uint size = MAX_PATH >> 1, length=size;
      while (length >= size) {
         size   <<= 1;
         buffer = (char*) alloca(size);                              // on the stack
         length = GetModuleFileName(NULL, buffer, size);             // may return a path longer than MAX_PATH
      }
      if (!length) return((char*)error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileName()"));

      filename = strdup(buffer);                                     // on the heap
   }
   return(filename);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the executable the terminal was launched from.
 *
 * @return wchar* - filename or a NULL pointer in case of errors
 */
const wchar* WINAPI GetTerminalModuleFileNameW() {
   static wchar* filename;

   if (!filename) {
      wchar* buffer;
      uint size = MAX_PATH >> 1, length=size;
      while (length >= size) {
         size <<= 1;
         buffer = (wchar*) alloca(size * sizeof(wchar));             // on the stack
         length = GetModuleFileNameW(NULL, buffer, size);            // may return a path longer than MAX_PATH
      }
      if (!length) return((wchar*)error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileName()"));
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
   static char* version;

   if (!version) {
      // get the version numbers
      const VS_FIXEDFILEINFO* fileInfo = GetTerminalVersionFromImage();
      if (!fileInfo)          fileInfo = GetTerminalVersionFromFile();
      if (!fileInfo) return(NULL);

      uint major  = (fileInfo->dwFileVersionMS >> 16) & 0xffff;
      uint minor  = (fileInfo->dwFileVersionMS      ) & 0xffff;
      uint hotfix = (fileInfo->dwFileVersionLS >> 16) & 0xffff;
      uint build  = (fileInfo->dwFileVersionLS      ) & 0xffff;

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
 * Read the terminal's version infos from file.
 *
 * @return VS_FIXEDFILEINFO* - pointer to a VS_FIXEDFILEINFO structure or NULL in case of errors
 */
const VS_FIXEDFILEINFO* WINAPI GetTerminalVersionFromFile() {
   static VS_FIXEDFILEINFO* fileInfo;

   if (!fileInfo) {
      const char* fileName = GetTerminalModuleFileNameA();
      DWORD fileInfoSize = GetFileVersionInfoSize(fileName, &fileInfoSize);
      if (!fileInfoSize) return((VS_FIXEDFILEINFO*)error(ERR_WIN32_ERROR+GetLastError(), "GetFileVersionInfoSize()"));

      char* infos = (char*) alloca(fileInfoSize);                       // on the stack
      BOOL result = GetFileVersionInfo(fileName, NULL, fileInfoSize, infos);
      if (!result) return((VS_FIXEDFILEINFO*)error(ERR_WIN32_ERROR+GetLastError(), "GetFileVersionInfo()"));

      uint len;
      result = VerQueryValue(infos, "\\", (void**)&fileInfo, &len);
      if (!result) return((VS_FIXEDFILEINFO*)error(ERR_WIN32_ERROR+GetLastError(), "VerQueryValue()"));
   }
   return(fileInfo);
}


/**
 * Read the terminal's version infos from the PE image in memory.
 *
 * @return VS_FIXEDFILEINFO* - pointer to a VS_FIXEDFILEINFO structure or NULL in case of errors
 */
const VS_FIXEDFILEINFO* WINAPI GetTerminalVersionFromImage() {
   static VS_FIXEDFILEINFO* fileInfo;

   if (!fileInfo) {
      HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
      if (!hRes) return((VS_FIXEDFILEINFO*)error(ERR_WIN32_ERROR+GetLastError(), "FindResource()"));

      void* infos = LoadResource(NULL, hRes);
      if (!infos) return((VS_FIXEDFILEINFO*)error(ERR_WIN32_ERROR+GetLastError(), "LoadResource()"));

      int offset = 6;
      if (!wcscmp((wchar*)((char*)infos + offset), L"VS_VERSION_INFO")) {
         offset += sizeof(L"VS_VERSION_INFO");
         offset += offset % 4;                        // align to 32 bit
         fileInfo = (VS_FIXEDFILEINFO*)((char*)infos + offset);
         if (fileInfo->dwSignature != 0xfeef04bd) {
            debug("unknown VS_FIXEDFILEINFO signature: 0x%p", fileInfo->dwSignature);
            fileInfo = NULL;
         }
      }
   }
   return(fileInfo);
}


/**
 * Return the name of the terminal's installation directory.
 *
 * @return char* - directory name without trailing path separator
 */
const char* WINAPI GetTerminalPathA() {
   static char* path;

   if (!path) {
      path = strdup(GetTerminalModuleFileNameA());                   // on the heap
      path[string(path).find_last_of("\\")] = '\0';
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the name of the terminal's installation directory.
 *
 * @return wstring& - directory name or an empty string in case of errors
 */
const wstring& WINAPI GetTerminalPathWS() {
   static wstring path;

   if (path.empty()) {
      const wchar* filename = GetTerminalModuleFileNameW();
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
   static char* dataPath;

   if (!dataPath) {
      const char* terminalPath    = GetTerminalPathA();
      const char* roamingDataPath = GetTerminalRoamingDataPathA();

      // 1. check if an old build or launched in portable mode
      if (TerminalIsPortableMode()) {
         // 1.1 yes => use installation directory (WoW64 redirection of old builds to the virtual store)
         //debug("1.1  TerminalIsPortableMode() = TRUE");
         dataPath = strdup(terminalPath);                               // on the heap
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
               dataPath = strdup(terminalPath);                         // on the heap
            }
            else {
               // 3.2 no => use roaming data directory
               //debug("3.2  a locked logfile doesn't exist");
               dataPath = strdup(roamingDataPath);                      // on the heap
            }
         }
         else {
            //debug("2.1  the executable exists");
            // 2.2 no => 4. check write permission (the logfile location may be linked)
            if (TerminalHasWritePermission(terminalPath)) {
               // 4.1 permission granted => use the directory
               //debug("4.1  write permission in %s", terminalPath);
               dataPath = strdup(terminalPath);                         // on the heap
            }
            else {
               // 4.2 permission denied => use roaming data directory
               //debug("4.2  no write permission in %s", terminalPath);
               dataPath = strdup(roamingDataPath);                      // on the heap
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
   static char* result;

   if (!result) {
      char appDataPath[MAX_PATH];                                                      // resolve CSIDL_APPDATA
      if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath)))
         return((char*)error(ERR_WIN32_ERROR+GetLastError(), "SHGetFolderPath()"));

      string dir = string(appDataPath).append("\\MetaQuotes\\Terminal\\Common");       // create the resulting path
      result = strdup(dir.c_str());                                                    // on the heap
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
   static char* result;

   if (!result) {
      char appDataPath[MAX_PATH];                                                      // resolve CSIDL_APPDATA
      if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath)))
         return((char*)error(ERR_WIN32_ERROR+GetLastError(), "SHGetFolderPath()"));

      wstring terminalPath = GetTerminalPathWS();                                      // get terminal installation path
      StrToUpper(terminalPath);                                                        // convert to upper case
      char* md5 = MD5Hash(terminalPath.c_str(), terminalPath.length()* sizeof(wchar)); // calculate MD5 hash

      string dir = string(appDataPath).append("\\MetaQuotes\\Terminal\\")              // create the resulting path
                                      .append(StrToUpper(md5));
      free(md5);
      result = strdup(dir.c_str());                                                    // on the heap
   }
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Load a custom MQL program for execution on the specified chart.
 *
 * @param  HWND        hChart      - handle of the chart to load the program to = value of MQL::WindowHandle()
 * @param  ProgramType programType - MQL program type: PT_INDICATOR | PT_EXPERT | PT_SCRIPT
 * @param  char*       programName - MQL program name (Ansi string)
 *
 * @return BOOL - whether or not the load command was successfully queued; not if the MQL program was indeed launched
 */
BOOL WINAPI LoadMqlProgramA(HWND hChart, ProgramType programType, const char* programName) {
   if (hChart <= 0)                           return(error(ERR_INVALID_PARAMETER, "invalid parameter hChart: %p (not a window handle)", hChart));
   if (!IsWindow(hChart))                     return(error(ERR_INVALID_PARAMETER, "invalid parameter hChart: %p (not an existing window)", hChart));
   if ((uint)programName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName));
   if (!strlen(programName))                  return(error(ERR_INVALID_PARAMETER, "invalid parameter programName: \"\" (must be non-empty)"));

   string file(GetMqlDirectoryA());
   uint cmd = 0;

   // check the file for existence
   switch (programType) {
      case PT_INDICATOR:
         file.append("\\indicators\\").append(programName).append(".ex4");
         cmd = MT4_LOAD_CUSTOM_INDICATOR;
         break;

      case PT_EXPERT:
         file.append(g_terminalBuild <= 509 ? "\\":"\\experts\\").append(programName).append(".ex4");
         cmd = MT4_LOAD_EXPERT;
         break;

      case PT_SCRIPT:
         file.append("\\scripts\\").append(programName).append(".ex4");
         cmd = MT4_LOAD_SCRIPT;
         break;

      default:
         return(error(ERR_INVALID_PARAMETER, "invalid parameter programType: %d (unknown)", programType));
   }
   if (!IsFileA(file)) return(error(ERR_FILE_NOT_FOUND, "file not found: \"%s\"", file.c_str()));

   // trigger the launch of the program
   if (!PostMessage(hChart, WM_MT4(), cmd, (LPARAM)strdup(programName)))   // pass a copy of "name" from the heap
      return(error(ERR_WIN32_ERROR+GetLastError(), "=>PostMessage()"));

   // prevent the DLL from getting unloaded before the message is processed
   HMODULE hModule = NULL;
   if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN|GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)LoadMqlProgramA, &hModule))
      return(error(ERR_WIN32_ERROR+GetLastError(), "=>GetModuleHandleEx()"));

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Load a custom MQL program for execution on the specified chart.
 *
 * @param  HWND        hChart      - handle of the chart to load the program to = value of MQL::WindowHandle()
 * @param  ProgramType programType - MQL program type: PT_INDICATOR | PT_EXPERT | PT_SCRIPT
 * @param  wchar*      programName - MQL program name (Unicode string)
 *
 * @return BOOL - whether or not the load command was successfully queued; not if the MQL program was indeed launched
 */
BOOL WINAPI LoadMqlProgramW(HWND hChart, ProgramType programType, const wchar* programName) {
   if ((uint)programName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName));

   size_t bufSize = wcslen(programName) + 1;
   char* ansiName = new char[bufSize];
   WCharToAnsiStr(programName, ansiName, bufSize);

   BOOL result = LoadMqlProgramA(hChart, programType, ansiName);
   delete[] ansiName;

   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not the terminal has write permission to the specified directory.
 *
 * @param  char* dir - directory name
 *
 * @return BOOL
 */
BOOL WINAPI TerminalHasWritePermission(const char* dir) {
   if (!IsDirectoryA(dir))
      return(FALSE);

   char tmpFilename[MAX_PATH];

   if (!GetTempFileName(dir, "rsf", 0, tmpFilename))
      return(FALSE);

   if (!DeleteFile(tmpFilename))
      return(error(ERR_WIN32_ERROR+GetLastError(), "DeleteFile(%s)", tmpFilename));

   return(TRUE);
}


/**
 * Whether or not the specified file exists and is locked with the sharing modes of a logfile. This function cannot see which
 * process is holding the lock.
 *
 * @param  string& filename - full filename
 *
 * @return BOOL
 */
BOOL WINAPI TerminalIsLockedLogfile(const string& filename) {
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
      if (!g_terminalBuild)
         return(FALSE);

      if (g_terminalBuild <= 509) {
         isPortable = TRUE;                                    // always TRUE, on access errors the system uses virtualization
      }
      else {
         int argc;
         LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

         for (int i=1; i < argc; ++i) {
            if (StrStartsWith(argv[i], L"/portable")) {        // StartsWith() instead of Compare()
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


/**
 * @return int
 */
int WINAPI Test_synchronize() {
   { synchronize();
      debug("inside synchronized block");
   }
   return(0);

   char* s1 = "Hello";
   char* s2 = " world";
   char* result = strcat(strcat((char*)alloca(strlen(s1) + strlen(s2) + 2), s1), s2);
   debug("s1=\"%s\"  s2=\"%s\"  result=\"%s\"", s1, s2, result);  // TODO: fix me

   return(0);
}


/**
 * @return int
 */
int WINAPI Test() {
   return(NULL);
   #pragma EXPANDER_EXPORT
}


