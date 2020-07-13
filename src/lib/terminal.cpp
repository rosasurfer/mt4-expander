#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include <shellapi.h>
#include <shlobj.h>


extern "C" IMAGE_DOS_HEADER          __ImageBase;        // this DLL's module handle
#define HMODULE_EXPANDER ((HMODULE) &__ImageBase)


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
 * Whether the specified file exists and is locked with the sharing modes of a terminal logfile. The function cannot see which
 * process is holding a lock.
 *
 * @param  string &filename - full filename
 *
 * @return BOOL
 */
BOOL WINAPI IsLockedFile(const string &filename) {
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
 * Return the full filename of the loaded MT4Expander DLL.
 *
 * @return char* - filename or a NULL pointer in case of errors
 */
const char* WINAPI GetExpanderFileNameA() {
   static char* filename;

   if (!filename) {
      char* buffer;
      uint size=MAX_PATH >> 1, length=size;
      while (length >= size) {
         size <<= 1;
         buffer = (char*) alloca(size);                                 // on the stack
         length = GetModuleFileNameA(HMODULE_EXPANDER, buffer, size);   // may return a path longer than MAX_PATH
      }
      if (!length) return((char*)error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileNameA()"));

      filename = strdup(buffer);                                        // on the heap
   }
   return(filename);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the module handle of the loaded MT4Expander DLL under Windows 2000.
 *
 * @return HMODULE - DLL module handle
 */
HMODULE WINAPI GetExpanderModuleW2K() {
   MEMORY_BASIC_INFORMATION mbi = {};
   VirtualQuery(GetExpanderModuleW2K, &mbi, sizeof(mbi));
   return((HMODULE)mbi.AllocationBase);
   #pragma EXPANDER_EXPORT
}


/**
 * Get the module handle of the loaded MT4Expander DLL under Windows XP, Windows Server 2003 or higher.
 *
 * @return HMODULE - DLL module handle
 */
HMODULE WINAPI GetExpanderModuleXP() {
   HMODULE hModule = NULL;
   GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)GetExpanderModuleXP, &hModule);
   return(hModule);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL directory the terminal currently uses.
 *
 * @return char* - directory name or a NULL pointer in case of errors
 */
const char* WINAPI GetMqlDirectoryA() {
   static char* mqlDirectory;

   if (!mqlDirectory) {
      const char* dataPath = GetTerminalDataPathA();
      if (!dataPath) return(NULL);

      string dir(dataPath);
      if (GetTerminalBuild() <= 509) dir.append("\\experts");
      else                           dir.append("\\mql4");

      mqlDirectory = strdup(dir.c_str());
   }
   return(mqlDirectory);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the terminal's build number (same value as returned by TerminalInfoInteger(TERMINAL_BUILD) introduced in MQL4.5).
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
 * Return the full path of the terminal's common data directory (same value as returned by TerminalInfoString(TERMINAL_COMMONDATA_PATH)
 * introduced in MQL4.5). The common data directory is shared between all terminals installed by a user. The function does
 * not check whether the returned directory exists.
 *
 * @return char* - directory name or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common"
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
 * Return the full path of the currently used data directory (same value as returned by TerminalInfoString(TERMINAL_DATA_PATH)
 * introduced in MQL4.5). The function does not check whether the returned directory exists.
 *
 * @return char* - directory name or a NULL pointer in case of errors, e.g. "%ProgramFiles%\MetaTrader4"
 */
const char* WINAPI GetTerminalDataPathA() {
   // VirtualStore: File and Registry Virtualization (since Vista)
   // ------------------------------------------------------------
   // - Used on write to a protected directory without a full administrator access token.
   // - Affected programs cannot tell the difference between their native folder and the VirtualStore.
   // - File changes are redirected to %UserProfile%\AppData\Local\VirtualStore.
   // - Protected file system locations:
   //    %ProgramFiles%     e.g. "C:\Program Files"
   //    %AllUsersProfile%  e.g. "C:\ProgramData" (previously "C:\Documents and Settings\All Users")
   //    %SystemRoot%       e.g. "C:\Windows"
   //
   //
   // Roaming Data Directory (terminal build > 509)
   // ---------------------------------------------
   // Used in any of the following cases if the terminal was not launched in portable mode:
   //  - If UAC is enabled and the terminal was not launched from a portable device.
   //  - If the user cannot write to the installation directory, e.g. a read-only network share.
   //  - If the user is working via remote desktop (RDP).
   //
   // If none of these conditions apply data is stored in the installation directory.
   //
   // @see  https://www.mql5.com/en/articles/1388
   // @see  https://social.technet.microsoft.com/wiki/contents/articles/6083.windows-xp-folders-and-locations-vs-windows-7-and-vista.aspx
   // @see  https://msdn.microsoft.com/en-us/library/bb756960.aspx
   //
   //
   // 1) Is terminal old or launched in portable mode?
   //    yes => data path is %InstallDir% (independant of write permissions, redirection to VirtualStore where appropriate)
   //    no  => new terminal in non-portable mode with UAC-aware behavior => (2)
   //
   // 2) Check for locked terminal logfiles in "%InstallDir%" and "%UserProfile%\AppData\Roaming"
   //    1 file       => data path according to the locked file
   //    0 or 2 files => logfiles can't be used for data path determination => (3)
   //
   // 3) Check UAC status
   //    on  => data path is "%UserProfile%\AppData\Roaming"
   //    off => (3)
   //
   static char* dataPath;

   if (!dataPath) {
      const char* terminalPath    = GetTerminalPathA();
      const char* roamingDataPath = GetTerminalRoamingDataPathA();

      //BOOL debugOn = StrEndsWith(GetExpanderFileNameA(), "Release.dll");
      //debugOn && debug("terminalPath:    %s", terminalPath);
      //debugOn && debug("roamingDataPath: %s", roamingDataPath);

      // 1) check portable mode
      if (GetTerminalBuild() <= 509 || TerminalIsPortableMode()) {
         // data path is always installation directory, independant of write permissions
         dataPath = strdup(terminalPath);                                                 // on the heap
         //debugOn && debug("TerminalBuild <= 509 || PortableMode = 1");
      }
      // 2 check for locked terminal logs
      else {
         const char* dateName = LocalTimeFormat(GetGmtTime(), "\\logs\\%Y%m%d.log");
         BOOL terminalPathIsLocked = IsLockedFile(string(terminalPath).append(dateName));
         BOOL roamingPathIsLocked  = IsLockedFile(string(roamingDataPath).append(dateName));
         //debugOn && debug("TerminalBuild > 509 && PortableMode = 0");
         //debugOn && debug("terminalPathIsLocked: %d", terminalPathIsLocked);
         //debugOn && debug("roamingPathIsLocked:  %d", roamingPathIsLocked);

         if      (roamingPathIsLocked)  dataPath = strdup(roamingDataPath);               // on the heap, check UAC status
         else if (terminalPathIsLocked) dataPath = strdup(terminalPath);                  // on the heap
         else return((char*)error(ERR_RUNTIME_ERROR, "no open terminal logfile found"));  // both directories are write-protected
      }
   }
   return(dataPath);
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
const char* WINAPI GetTerminalFileNameA() {
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
const wchar* WINAPI GetTerminalFileNameW() {
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
 * Return the name of the terminal's installation directory (same value as returned by TerminalInfoString(TERMINAL_PATH)
 * introduced in MQL4.5).
 *
 * @return char* - directory name without trailing path separator
 */
const char* WINAPI GetTerminalPathA() {
   static char* path;

   if (!path) {
      path = strdup(GetTerminalFileNameA());                         // on the heap
      path[string(path).find_last_of("\\")] = '\0';
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
* Return the name of the terminal's installation directory (same value as returned by TerminalInfoString(TERMINAL_PATH)
* introduced in MQL4.5).
 *
 * @return wstring& - directory name or an empty string in case of errors
 */
const wstring& WINAPI GetTerminalPathW() {
   static wstring path;

   if (path.empty()) {
      const wchar* filename = GetTerminalFileNameW();
      if (filename) {
         wstring str(filename);
         path = str.substr(0, str.find_last_of(L"\\"));
      }
   }
   return(path);
}


/**
 * Return the full path of the terminal's roaming data directory. Depending on terminal version and runtime mode the data
 * directory may differ. The function does not check if the returned path exists.
 *
 * @return char* - directory name or a NULL pointer in case of errors
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-hash}"
 *
 * @see  GetTerminalDataPath() to get the path of the data directory currently used
 */
const char* WINAPI GetTerminalRoamingDataPathA() {
   static char* result;

   if (!result) {
      char appDataPath[MAX_PATH];                                                      // resolve CSIDL_APPDATA
      if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath)))
         return((char*)error(ERR_WIN32_ERROR+GetLastError(), "SHGetFolderPath()"));

      wstring terminalPath = GetTerminalPathW();                                       // get terminal installation path
      StrToUpper(terminalPath);                                                        // convert to upper case
      char* md5 = MD5Hash(terminalPath.c_str(), terminalPath.length()*sizeof(wchar));  // calculate MD5 hash

      string dir = string(appDataPath).append("\\MetaQuotes\\Terminal\\")              // create the resulting path
                                      .append(StrToUpper(md5));
      free(md5);
      result = strdup(dir.c_str());                                                    // on the heap
   }
   return(result);
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
      version = strformat("%d.%d.%d.%d", major, minor, hotfix, build);
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
      const char* fileName = GetTerminalFileNameA();
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
 * Load a custom MQL program for execution on the specified chart.
 *
 * @param  HWND        hChart      - handle of the chart to load the program on = value of MQL::WindowHandle()
 * @param  ProgramType programType - MQL program type: PT_INDICATOR | PT_EXPERT | PT_SCRIPT
 * @param  char*       programName - MQL program name (ANSI string)
 *
 * @return BOOL - whether the load command was successfully queued; not whether the program was indeed launched
 */
BOOL WINAPI LoadMqlProgramA(HWND hChart, ProgramType programType, const char* programName) {
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
         file.append(GetTerminalBuild() <= 509 ? "\\":"\\experts\\").append(programName).append(".ex4");
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
   GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCTSTR)LoadMqlProgramA, &hModule);

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Load a custom MQL program for execution on the specified chart.
 *
 * @param  HWND        hChart      - handle of the chart to load the program on = value of MQL::WindowHandle()
 * @param  ProgramType programType - MQL program type: PT_INDICATOR | PT_EXPERT | PT_SCRIPT
 * @param  wchar*      programName - MQL program name (Unicode string)
 *
 * @return BOOL - whether the load command was successfully queued; not whether the program was indeed launched
 */
BOOL WINAPI LoadMqlProgramW(HWND hChart, ProgramType programType, const wchar* programName) {
   if ((uint)programName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName));

   uint bufSize = wcslen(programName) + 1;
   char* ansiName = new char[bufSize];
   WCharToAnsiStr(programName, ansiName, bufSize);

   BOOL result = LoadMqlProgramA(hChart, programType, ansiName);
   delete[] ansiName;

   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the terminal operates in portable mode, i.e. it was launched with the command line parameter "/portable". In portable
 * mode the terminal behaves like in Windows XP or ealier. It uses the installation directory for program data and ignores a
 * UAC-aware environment. Terminal builds <= 509 always operate in portable mode.
 *
 * @return BOOL
 *
 * Note: The terminal also enables portable mode if a command line parameter *starts* with the prefix "/portable". For example
 *       passing the parameter "/portablepoo" enables portable mode, too. This function mirrors that behavior.
 */
BOOL WINAPI TerminalIsPortableMode() {
   static int isPortable = -1;

   if (isPortable < 0) {
      static uint terminalBuild = GetTerminalBuild();
      if (!terminalBuild) return(FALSE);

      if (terminalBuild <= 509) {
         isPortable = TRUE;                                    // always TRUE
      }
      else {
         int argc;
         LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

         for (int i=1; i < argc; ++i) {
            if (StrStartsWith(argv[i], L"/portable")) {        // starts-with instead of compare
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
int WINAPI Test() {
   debug("hello world: %s", "Radegast");
   return(0);
   //#pragma EXPANDER_EXPORT
}
