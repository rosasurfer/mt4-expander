#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/file.h"
#include "lib/helper.h"
#include "lib/sound.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include <shlobj.h>

extern BOOL  g_cliOptionPortableMode;                    // whether cmd line option /portable is set

extern "C" IMAGE_DOS_HEADER          __ImageBase;        // this DLL's module handle
#define HMODULE_EXPANDER ((HMODULE) &__ImageBase)


/**
 * Whether the terminal operates in portable mode, i.e. it was launched with the command line parameter "/portable".
 * In portable mode the terminal behaves like under Windows XP or ealier. It uses the installation directory for program data
 * and ignores a UAC-aware environment. Terminal builds <= 509 always operate in portable mode.
 *
 * @return BOOL
 */
BOOL WINAPI IsPortableMode() {
   static int portableMode = -1;

   if (portableMode < 0) {
      portableMode = (GetTerminalBuild() <= 509 || g_cliOptionPortableMode);
   }
   return portableMode;
   #pragma EXPANDER_EXPORT
}


/**
 * Find and return the name of the history directory containing the specified file.
 *
 * @param  char* filename   - simple filename
 * @param  BOOL  removeFile - whether to remove a found file
 *
 * @return char* - directory name (last segment of the full path) or NULL in case of errors
 */
const char* WINAPI FindHistoryDirectoryA(const char* filename, BOOL removeFile) {
   if ((uint)filename < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter filename: 0x%p (not a valid pointer)", filename));
   if (!*filename)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter filename: \"\" (empty)"));

   const char* hstRootPath = GetHistoryRootPathA();
   if (!hstRootPath) return (char*)!error(ERR_RUNTIME_ERROR, "->GetHistoryRootPathA() => NULL");

   string pattern = string(hstRootPath).append("\\*");
   WIN32_FIND_DATA wfd = {};
   HANDLE hFind = FindFirstFileA(pattern.c_str(), &wfd);
   if (hFind == INVALID_HANDLE_VALUE) return((char*)!error(ERR_FILE_NOT_FOUND, "directory \"%s\" not found", pattern.c_str()));

   char* hstDirectory = NULL;
   BOOL next = TRUE;

   while (next) {
      if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
         char* dirName = wfd.cFileName;

         if (!StrCompare(dirName, ".") && !StrCompare(dirName, "..")) {
            string fullFilename = string(GetHistoryRootPathA()).append("\\").append(dirName).append("\\").append(filename);
            if (IsFileA(fullFilename.c_str(), MODE_SYSTEM)) {
               hstDirectory = dirName;
               if (removeFile && !DeleteFileA(fullFilename.c_str())) warn(ERR_WIN32_ERROR+GetLastError(), "cannot delete file %s", DoubleQuoteStr(fullFilename.c_str()));
               break;
            }
         }
      }
      next = FindNextFileA(hFind, &wfd);
   }
   FindClose(hFind);

   return sdup(hstDirectory);
   #pragma EXPANDER_EXPORT
}


/**
 * Find the window handle of the open configuration dialog of the specified MQL program.
 *
 * @param  ProgramType programType - one of PT_INDICATOR | PT_EXPERT | PT_SCRIPT
 * @param  char*       programName - name
 *
 * @return HWND - window handle or NULL if no open configuration dialog was found;
 *                INVALID_HWND (-1) in case of errors
 */
HWND WINAPI FindInputDialogA(ProgramType programType, const char* programName) {
   if (!IsProgramType(programType))           return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter programType: %d (unknown)", programType)));
   if ((uint)programName < MIN_VALID_POINTER) return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName)));
   if (!*programName)                         return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter programName: \"\" (empty)")));

   string title(programName);
   if (programType == PT_INDICATOR) title.insert(0, "Custom Indicator - ");

   char* className = "#32770";
   DWORD processId, self=GetCurrentProcessId();
   HWND hWndDlg = NULL;

   while (hWndDlg = FindWindowExA(NULL, hWndDlg, className, title.c_str())) {
      GetWindowThreadProcessId(hWndDlg, &processId);

      if (processId == self) {
         if (programType == PT_INDICATOR) {
            if (FindWindowExA(hWndDlg, NULL, className, "Common"))                     // FIX-ME: this text is subject to i18n
               break;
         }
         else if (programType==PT_EXPERT || programType==PT_SCRIPT) {                  // expert and script dialogs are indistinguishable
            if (FindWindowExA(hWndDlg, NULL, className, "Expert Advisor settings"))    // FIX-ME: this text is subject to i18n
               break;
         }
         //else if (built-in-indicator) {
         //   if (FindWindowExA(hWndDlg, NULL, className, "Parameters"))               // FIX-ME: this text is subject to i18n
         //      break;
         //}
      }
   }
   return(hWndDlg);
   //#pragma EXPANDER_EXPORT                                                           // not exported as i18n issues are not fixed
}


/**
 * Whether the specified file exists and is locked with the sharing modes of a terminal logfile.
 * The function cannot see which process is holding a lock.
 *
 * @param  string &filename - full filename
 *
 * @return BOOL
 */
BOOL WINAPI IsLockedFile(const string &filename) {
   if (!IsFileA(filename.c_str(), MODE_SYSTEM)) return(FALSE);

   // for logfile: OF_READWRITE|OF_SHARE_COMPAT must succeed
   HFILE hFile = _lopen(filename.c_str(), OF_READWRITE|OF_SHARE_COMPAT);
   if (hFile == HFILE_ERROR) return(FALSE);                       // not succeeded
   _lclose(hFile);

   // for logfile: OF_READWRITE|OF_SHARE_EXCLUSIVE must fail with ERROR_SHARING_VIOLATION
   hFile = _lopen(filename.c_str(), OF_READWRITE|OF_SHARE_EXCLUSIVE);
   if (hFile == HFILE_ERROR) {
      return(GetLastError() == ERROR_SHARING_VIOLATION);
   }
   _lclose(hFile);
   return(FALSE);                                                 // not failed
}


/**
 * Return the full filename of the loaded MT4Expander DLL.
 *
 * @return char* - filename or NULL in case of errors
 */
const char* WINAPI GetExpanderFileNameA() {
   static char* filename;

   if (!filename) {
      const wchar* wname = GetExpanderFileNameW();
      if (!wname) return (char*)!error(ERR_RUNTIME_ERROR, "->GetExpanderFileNameW() => NULL");

      char* tmp = utf16ToAnsi(wname);
      if (!filename) filename = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return filename;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the loaded MT4Expander DLL.
 *
 * @return wchar* - filename or NULL in case of errors
 */
const wchar* WINAPI GetExpanderFileNameW() {
   static wchar* filename;

   if (!filename) {
      wchar* buffer = NULL;
      uint size=MAX_PATH >> 1, length=size;
      while (length >= size) {
         size <<= 1;
         buffer = (wchar*) alloca(size * sizeof(wchar));
         length = GetModuleFileNameW(HMODULE_EXPANDER, buffer, size);   // may return a path longer than MAX_PATH
      }
      if (!length) return((wchar*)!error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileNameW()"));

      wchar* tmp = wsdup(buffer);
      if (!filename) filename = tmp;
      else free(tmp);                     // another thread may have been faster
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
 * Return the full path of the history root directory. The function doesn't check whether the directory exists.
 *
 * @return char* - directory name without trailing path separator or NULL in case of errors
 */
const char* WINAPI GetHistoryRootPathA() {
   static char* hstDirectory;

   if (!hstDirectory) {
      const wchar* wpath = GetHistoryRootPathW();
      if (!wpath) return (char*)!error(ERR_RUNTIME_ERROR, "->GetHistoryRootPathW() => NULL");

      char* tmp = utf16ToAnsi(wpath);
      if (!hstDirectory) hstDirectory = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return hstDirectory;
   #pragma EXPANDER_EXPORT
}


/**
* Return the full path of the history root directory. The function doesn't check whether the directory exists.
 *
 * @return wchar* - directory name without trailing path separator or NULL in case of errors
 */
const wchar* WINAPI GetHistoryRootPathW() {
   static wchar* hstDirectory;

   if (!hstDirectory) {
      const wchar* dataPath = GetTerminalDataPathW();
      if (!dataPath) return (wchar*)!error(ERR_RUNTIME_ERROR, "->GetTerminalDataPathW() => NULL");

      wstring path = wstring(dataPath).append(L"\\history");
      wchar* tmp = wsdup(path.c_str());
      if (!hstDirectory) hstDirectory = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return(hstDirectory);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL directory.
 *
 * @return char* - directory name without trailing path separator or NULL in case of errors
 */
const char* WINAPI GetMqlDirectoryA() {
   static char* mqlDirectory;

   if (!mqlDirectory) {
      const wchar* wdirectory = GetMqlDirectoryW();
      if (!wdirectory) return (char*)!error(ERR_RUNTIME_ERROR, "->GetMqlDirectoryW() => NULL");

      char* tmp = utf16ToAnsi(wdirectory);
      if (!mqlDirectory) mqlDirectory = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return mqlDirectory;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL directory.
 *
 * @return wchar* - directory name without trailing path separator or NULL in case of errors
 */
const wchar* WINAPI GetMqlDirectoryW() {
   static wchar* mqlDirectory;

   if (!mqlDirectory) {
      const wchar* dataPath = GetTerminalDataPathW();
      if (!dataPath) return (wchar*)!error(ERR_RUNTIME_ERROR, "->GetTerminalDataPathW() => NULL");

      wstring path(dataPath);
      if (GetTerminalBuild() <= 509) path.append(L"\\experts");
      else                           path.append(L"\\mql4");

      wchar* tmp = wsdup(path.c_str());
      if (!mqlDirectory) mqlDirectory = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return mqlDirectory;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL sandbox directory. This is the only directory accessible to built-in MQL file functions.
 *
 * @param  BOOL inTester - whether to return the online path or the one in the tester
 *
 * @return char* - directory name without trailing path separator or NULL in case of errors
 */
const char* WINAPI GetMqlSandboxPathA(BOOL inTester) {
   static char* testerPath, *onlinePath;

   if (inTester) {
      if (!testerPath) {
         const wchar* wpath = GetMqlSandboxPathW(inTester);
         if (!wpath) return (char*)!error(ERR_RUNTIME_ERROR, "->GetMqlSandboxPathW() => NULL");

         char* tmp = utf16ToAnsi(wpath);
         if (!testerPath) testerPath = tmp;
         else free(tmp);               // another thread may have been faster
      }
      return testerPath;
   }

   if (!onlinePath) {
      const wchar* wpath = GetMqlSandboxPathW(inTester);
      if (!wpath) return (char*)!error(ERR_RUNTIME_ERROR, "->GetMqlSandboxPathW() => NULL");

      char* tmp = utf16ToAnsi(wpath);
      if (!onlinePath) onlinePath = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return onlinePath;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL sandbox directory. This is the only directory accessible to built-in MQL file functions.
 *
 * @param  BOOL inTester - whether to return the online path or the one in the tester
 *
 * @return wchar* - directory name without trailing path separator or NULL in case of errors
 */
const wchar* WINAPI GetMqlSandboxPathW(BOOL inTester) {
   static wchar* testerPath, *onlinePath;

   if (inTester) {
      if (!testerPath) {
         const wchar* dataDirectory = GetTerminalDataPathW();
         if (!dataDirectory) return (wchar*)!error(ERR_RUNTIME_ERROR, "->GetTerminalDataPathW() => NULL");

         wstring path = wstring(dataDirectory).append(L"\\tester\\files");
         wchar* tmp = wsdup(path.c_str());
         if (!testerPath) testerPath = tmp;
         else free(tmp);               // another thread may have been faster
      }
      return(testerPath);
   }

   if (!onlinePath) {
      const wchar* mqlDirectory = GetMqlDirectoryW();
      if (!mqlDirectory) return (wchar*)!error(ERR_RUNTIME_ERROR, "->GetMqlDirectoryW() => NULL");

      wstring path = wstring(mqlDirectory).append(L"\\files");
      wchar* tmp = wsdup(path.c_str());
      if (!onlinePath) onlinePath = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return(onlinePath);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's common data directory (same value as returned by TerminalInfoString(TERMINAL_COMMONDATA_PATH)
 * introduced in MQL4.5). The common data directory is shared between all terminals installed by a user. The function does not
 * check whether the returned directory exists.
 *
 * @return char* - directory name without trailing path separator or NULL in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common"
 */
const char* WINAPI GetTerminalCommonDataPathA() {
   static char* path;

   if (!path) {
      const wchar* wpath = GetTerminalCommonDataPathW();
      if (!wpath) return (char*)!error(ERR_RUNTIME_ERROR, "->GetTerminalCommonDataPathW() => NULL");

      char* tmp = utf16ToAnsi(wpath);
      if (!path) path = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return path;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's common data directory (same value as returned by TerminalInfoString(TERMINAL_COMMONDATA_PATH)
 * introduced in MQL4.5). The common data directory is shared between all terminals installed by a user. The function does not
 * check whether the returned directory exists.
 *
 * @return wchar* - directory name without trailing path separator or NULL in case of errors,
 *                  e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common"
 */
const wchar* WINAPI GetTerminalCommonDataPathW() {
   static wchar* path;

   if (!path) {
      wchar appDataPath[MAX_PATH];
      if (FAILED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath))) {
         return((wchar*)!error(ERR_WIN32_ERROR+GetLastError(), "->SHGetFolderPath()"));
      }
      wstring dir = wstring(appDataPath).append(L"\\MetaQuotes\\Terminal\\Common");

      wchar* tmp = wsdup(dir.c_str());
      if (!path) path = tmp;
      else free(tmp);                     // another thread may have been faster
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the currently used data directory (same TerminalInfoString(TERMINAL_DATA_PATH) introduced in MQL4.5).
 * The function does not check whether the returned directory exists.
 *
 * @return char* - directory name without trailing path separator or NULL in case of errors,
 *                 e.g. "%ProgramFiles%\MetaTrader4"
 */
const char* WINAPI GetTerminalDataPathA() {
   static char* dataPath;

   if (!dataPath) {
      const wchar* wpath = GetTerminalDataPathW();
      if (!wpath) return (char*)!error(ERR_RUNTIME_ERROR, "->GetTerminalDataPathW() => NULL");

      char* tmp = utf16ToAnsi(wpath);
      if (!dataPath) dataPath = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return dataPath;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the currently used data directory (same TerminalInfoString(TERMINAL_DATA_PATH) introduced in MQL4.5).
 * The function does not check whether the returned directory exists.
 *
 * @return wchar* - directory name without trailing path separator or NULL in case of errors,
 *                  e.g. "%ProgramFiles%\MetaTrader4"
 */
const wchar* WINAPI GetTerminalDataPathW() {
   // VirtualStore (since Vista): file and registry virtualization
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
   // Logical function flow
   // ---------------------
   // (1) Is terminal operating in portable mode?
   //     yes => data path is %InstallDir% (independant of write permissions, redirection to VirtualStore where appropriate)
   //     no  => new terminal in non-portable mode with UAC-aware behavior => continue with (2)
   //
   // (2) Check for locked terminal logfiles, 1st in "%UserProfile%\AppData\Roaming", 2nd in "%InstallDir%"
   //     data path according to the first found locked file
   //     no locked files  => missing write permissions in both places (system config error) => continue with (3)
   //     two locked files => multiple parallel running terminals (ambiguous) => continue with (3)
   //
   // (3) Check UAC status
   //     on  => data path is "%UserProfile%\AppData\Roaming"
   //     off => data path is "%InstallDir%"
   //
   static wchar* dataPath;

   if (!dataPath) {
      const wchar* terminalPath = GetTerminalPathW();
      if (!terminalPath) return (wchar*)!error(ERR_RUNTIME_ERROR, "->GetTerminalPathW() => NULL");

      const wchar* roamingDataPath = GetTerminalRoamingDataPathW();
      if (!roamingDataPath) return (wchar*)!error(ERR_RUNTIME_ERROR, "->GetTerminalRoamingDataPathW() => NULL");

      wchar* wtmp = NULL;

      // check portable mode
      if (IsPortableMode()) {
         // data path is the installation directory, independant of write permissions
         wtmp = wsdup(terminalPath);
      }
      else {
         // check for locked terminal logs
         char* logFilename = LocalTimeFormatA(GetGmtTime32(), "\\logs\\%Y%m%d.log");

         char* path1 = utf16ToAnsi(terminalPath);
         BOOL terminalPathIsLocked = IsLockedFile(string(path1).append(logFilename));
         free(path1);

         char* path2 = utf16ToAnsi(roamingDataPath);
         BOOL roamingPathIsLocked = IsLockedFile(string(path2).append(logFilename));
         free(path2);
         free(logFilename);

         if      (roamingPathIsLocked)  wtmp = wsdup(roamingDataPath);
         else if (terminalPathIsLocked) wtmp = wsdup(terminalPath);
         else return (wchar*)!error(ERR_RUNTIME_ERROR, "no open terminal logfile found");   // no write permissions in both places

         // TODO: implement UAC check
      }
      if (!dataPath) dataPath = wtmp;
      else free(wtmp);                    // another thread may have been faster
   }
   return dataPath;
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
            if (!GetClassNameA(hWndNext, className, size)) return((HWND)!error(ERR_WIN32_ERROR+GetLastError(), "GetClassNameA()"));
            if (StrCompare(className, "MetaQuotes::MetaTrader::4.00")) {
               break;
            }
         }
         hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
      }
      if (!hWndNext) return((HWND)!error(ERR_WIN32_ERROR+GetLastError(), "cannot find terminal main window"));

      hWndMain = hWndNext;
   }
   return(hWndMain);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the executable the terminal was launched from.
 *
 * @return char* - filename or NULL in case of errors
 */
const char* WINAPI GetTerminalFileNameA() {
   static char* filename;

   if (!filename) {
      const wchar* wfilename = GetTerminalFileNameW();
      if (!wfilename) return (char*)!error(ERR_RUNTIME_ERROR, "->GetTerminalFileNameW() => NULL");

      char* tmp = utf16ToAnsi(wfilename);
      if (!filename) filename = tmp;
      else free(tmp);                                    // another thread may have been faster
   }
   return filename;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the executable the terminal was launched from.
 *
 * @return wchar* - filename or NULL in case of errors
 */
const wchar* WINAPI GetTerminalFileNameW() {
   static wchar* filename;

   if (!filename) {
      wchar* buffer = NULL;
      uint size=MAX_PATH >> 1, length=size;

      while (length >= size) {
         size <<= 1;
         buffer = (wchar*) alloca(size * sizeof(wchar));    // on the stack
         length = GetModuleFileNameW(NULL, buffer, size);   // may return a path longer than MAX_PATH
      }
      if (!length) return((wchar*)!error(ERR_WIN32_ERROR+GetLastError(), "->GetModuleFileName()"));

      wchar* tmp = wsdup(buffer);
      if (!filename) filename = tmp;
      else free(tmp);                                       // another thread may have been faster
   }
   return(filename);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the name of the terminal's installation directory (same value as returned by TerminalInfoString(TERMINAL_PATH)
 * introduced in MQL4.5).
 *
 * @return char* - directory name without trailing path separator or NULL in case of errors
 */
const char* WINAPI GetTerminalPathA() {
   static char* path;

   if (!path) {
      const wchar* wpath = GetTerminalPathW();
      if (!wpath) return (char*)!error(ERR_RUNTIME_ERROR, "->GetTerminalPathW() => NULL");

      char* tmp = utf16ToAnsi(wpath);
      if (!path) path = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return path;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the name of the terminal's installation directory (same value as returned by TerminalInfoString(TERMINAL_PATH)
 * introduced in MQL4.5).
 *
 * @return wchar* - directory name without trailing path separator or NULL in case of errors
 */
const wchar* WINAPI GetTerminalPathW() {
   static wchar* path;

   if (!path) {
      const wchar* filename = GetTerminalFileNameW();
      if (!filename) return (wchar*)!error(ERR_RUNTIME_ERROR, "->GetTerminalFileNameW() => NULL");

      wchar* tmp = wsdup(filename);
      tmp[wstring(tmp).find_last_of(L"\\")] = '\0';
      if (!path) path = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's roaming data directory. Depending on terminal version and runtime mode the used
 * data directory may differ. The function does not check whether the returned path exists.
 *
 * @return char* - directory name without trailing path separator or NULL in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-hash}"
 */
const char* WINAPI GetTerminalRoamingDataPathA() {
   static char* path;

   if (!path) {
      const wchar* wpath = GetTerminalRoamingDataPathW();
      if (!wpath) return (char*)!error(ERR_RUNTIME_ERROR, "->GetTerminalRoamingDataPathW() => NULL");

      char* tmp = utf16ToAnsi(wpath);
      if (!path) path = tmp;
      else free(tmp);                  // another thread may have been faster
   }
   return path;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's roaming data directory. Depending on terminal version and runtime mode the used
 * data directory may differ. The function does not check whether the returned path exists.
 *
 * @return char* - directory name without trailing path separator or NULL in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-hash}"
 */
const wchar* WINAPI GetTerminalRoamingDataPathW() {
   static wchar* result;

   if (!result) {
      wchar appDataPath[MAX_PATH];
      if (FAILED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath))) {
         return((wchar*)!error(ERR_WIN32_ERROR+GetLastError(), "->SHGetFolderPath()"));
      }
      wstring terminalPath(GetTerminalPathW());
      strToUpper(terminalPath);

      string md5Hash(MD5Hash(terminalPath.c_str(), terminalPath.length()*sizeof(wchar)));
      strToUpper(md5Hash);

      wstring dir = wstring(appDataPath).append(L"\\MetaQuotes\\Terminal\\").append(ansiToUtf16(md5Hash));

      wchar* tmp = wsdup(dir.c_str());
      if (!result) result = tmp;
      else free(tmp);                     // another thread may have been faster
   }
   return result;
   #pragma EXPANDER_EXPORT
}


/**
 * Return the terminal's build number. Same value as returned by TerminalInfoInteger(TERMINAL_BUILD) introduced in MQL4.5.
 *
 * @return uint - build number or 0 in case of errors
 */
uint WINAPI GetTerminalBuild() {
   static uint build;
   if (!build) {
      VS_FIXEDFILEINFO fileInfo = {};
      if      (GetTerminalVersionFromImage(fileInfo)) {}
      else if (GetTerminalVersionFromFile(fileInfo)) {}
      else return(NULL);

      build = fileInfo.dwFileVersionLS & 0xffff;
   }
   return(build);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the terminal's version string.
 *
 * @return char* - version or NULL in case of errors
 */
const char* WINAPI GetTerminalVersion() {
   static char* version;

   if (!version) {
      VS_FIXEDFILEINFO fileInfo = {};
      if      (GetTerminalVersionFromImage(fileInfo)) {}
      else if (GetTerminalVersionFromFile(fileInfo)) {}
      else return(NULL);

      uint major  = (fileInfo.dwFileVersionMS >> 16) & 0xffff;
      uint minor  = (fileInfo.dwFileVersionMS      ) & 0xffff;
      uint hotfix = (fileInfo.dwFileVersionLS >> 16) & 0xffff;
      uint build  = (fileInfo.dwFileVersionLS      ) & 0xffff;

      char* tmp = asformat("%d.%d.%d.%d", major, minor, hotfix, build);
      if (!version) version = tmp;
      else          free(tmp);         // another thread may have been faster
   }
   return(version);
   #pragma EXPANDER_EXPORT
}


/**
 * Read the terminal's version infos from the executable file.
 *
 * @param  VS_FIXEDFILEINFO &fileInfo - struct the version infos are written to
 *
 * @return BOOL - success status
 */
BOOL WINAPI GetTerminalVersionFromFile(VS_FIXEDFILEINFO &fileInfo) {
   const char* fileName = GetTerminalFileNameA();
   DWORD infoSize = GetFileVersionInfoSizeA(fileName, &infoSize);
   if (!infoSize) return(!error(ERR_WIN32_ERROR+GetLastError(), "->GetFileVersionInfoSize()"));

   char* infos = (char*) alloca(infoSize);      // on the stack
   BOOL success = GetFileVersionInfoA(fileName, NULL, infoSize, infos);
   if (!success) return(!error(ERR_WIN32_ERROR+GetLastError(), "->GetFileVersionInfo()"));

   VS_FIXEDFILEINFO* result;                    // points to an offset in 'infos' (on the stack)
   uint len;
   success = VerQueryValueA(infos, "\\", (void**)&result, &len);
   if (!success) return(!error(ERR_WIN32_ERROR+GetLastError(), "->VerQueryValue()"));

   fileInfo = *result;                          // copy content of result to fileInfo
   return(TRUE);
}


/**
 * Read the terminal's version infos from the PE image in memory.
 *
 * @param  VS_FIXEDFILEINFO &fileInfo - struct the version infos are written to
 *
 * @return BOOL - success status
 */
BOOL WINAPI GetTerminalVersionFromImage(VS_FIXEDFILEINFO &fileInfo) {
   static VS_FIXEDFILEINFO* result;             // the resource stays in memory until the process terminates

   if (!result) {
      HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
      if (!hRes) return(!error(ERR_WIN32_ERROR+GetLastError(), "->FindResource()"));

      HGLOBAL hVersionInfo = LoadResource(NULL, hRes);
      if (!hVersionInfo) return(!error(ERR_WIN32_ERROR+GetLastError(), "->LoadResource()"));

      void* infos = LockResource(hVersionInfo);
      if (!infos) return(!error(ERR_WIN32_ERROR+GetLastError(), "->LockResource()"));

      int offset = 6;
      if (!wstrcmp((wchar*)((BYTE*)infos + offset), L"VS_VERSION_INFO")) {
         offset += sizeof(L"VS_VERSION_INFO");
         offset += offset % 4;                  // align to next 32 bit
         VS_FIXEDFILEINFO* tmp = (VS_FIXEDFILEINFO*)((BYTE*)infos + offset);
         if (tmp->dwSignature != 0xfeef04bd) return(!warn(ERR_ILLEGAL_STATE, "unknown VS_FIXEDFILEINFO signature: 0x%p", tmp->dwSignature));
         result = tmp;
      }
   }

   fileInfo = *result;                          // copy content of result to fileInfo
   return(TRUE);
}


/**
 * Load a custom MQL program for execution on the specified chart.
 *
 * @param  HWND        hChart      - handle of the chart to load the program on = value of MQL::WindowHandle()
 * @param  ProgramType programType - MQL program type: PT_INDICATOR | PT_EXPERT | PT_SCRIPT
 * @param  char*       programName - MQL program name (C string)
 *
 * @return BOOL - whether the load command was successfully queued; not whether the program was indeed launched
 */
BOOL WINAPI LoadMqlProgramA(HWND hChart, ProgramType programType, const char* programName) {
   if ((uint)programName < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName));

   wstring name = ansiToUtf16(string(programName));
   return LoadMqlProgramW(hChart, programType, name.c_str());
   #pragma EXPANDER_EXPORT
}


/**
 * Load a custom MQL program for execution on the specified chart.
 *
 * @param  HWND        hChart      - handle of the chart to load the program on = value of MQL::WindowHandle()
 * @param  ProgramType programType - MQL program type: PT_INDICATOR | PT_EXPERT | PT_SCRIPT
 * @param  wchar*      programName - MQL program name (UTF-16)
 *
 * @return BOOL - whether the load command was successfully queued; not whether the program was indeed launched
 */
BOOL WINAPI LoadMqlProgramW(HWND hChart, ProgramType programType, const wchar* programName) {
   if (!IsWindow(hChart))                     return(!error(ERR_INVALID_PARAMETER, "invalid parameter hChart: 0x%p (not an existing window)", hChart));
   if (!IsProgramType(programType))           return(!error(ERR_INVALID_PARAMETER, "invalid parameter programType: %d (unknown)", programType));
   if ((uint)programName < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName));
   if (!*programName)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter programName: \"\" (empty)"));

   wstring file(GetMqlDirectoryW());
   char* sProgramType = NULL;
   uint cmd = 0;

   // check the MQL program for existence
   switch (programType) {
      case PT_INDICATOR:
         file.append(L"\\indicators\\").append(programName).append(L".ex4");
         sProgramType = "indicator";
         cmd = MT4_LOAD_CUSTOM_INDICATOR;
         break;

      case PT_EXPERT:
         file.append(GetTerminalBuild() <= 509 ? L"\\":L"\\experts\\").append(programName).append(L".ex4");
         sProgramType = "expert";
         cmd = MT4_LOAD_EXPERT;
         break;

      case PT_SCRIPT:
         file.append(L"\\scripts\\").append(programName).append(L".ex4");
         sProgramType = "script";
         cmd = MT4_LOAD_SCRIPT;
         break;
   }
   if (!IsFileW(file.c_str(), MODE_SYSTEM)) return(!error(ERR_FILE_NOT_FOUND, "file not found: \"%S\"", file.c_str()));

   // trigger the launch of the program
   PostMessageW(hChart, WM_MT4(), cmd, (LPARAM)wsdup(programName));     // pass a copy of programName (the passed value may get immediately destroyed)
   debug("queued launch of %s \"%S\"", sProgramType, programName);

   // prevent the DLL from getting unloaded before the message is processed
   HMODULE hModule = NULL;
   GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCWSTR)LoadMqlProgramW, &hModule);

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Reopen the terminal's "Alert" dialog box after it was closed by the user.
 *
 * @param  BOOL sound - Whether to confirm the re-opening with the standard "Alert" sound. No sound is played if the dialog
 *                      is already visible.
 *
 * @return BOOL - success status, especially:
 *                TRUE if the dialog was successfully opened or already visible
 *                FALSE if the dialog was not yet opened before (dialog window not found)
 */
BOOL WINAPI ReopenAlertDialog(BOOL sound) {
   HWND hWndAlert=NULL, hWndNext=GetTopWindow(NULL);
   DWORD processId, self=GetCurrentProcessId();

   uint bufSize = 8;                                        // big enough to hold class name "#32770"
   wchar* className = (wchar*) alloca(bufSize * sizeof(wchar));
   wchar* wndTitle = NULL;

   // enumerate top-level windows
   while (hWndNext) {
      GetWindowThreadProcessId(hWndNext, &processId);
      if (processId == self) {                              // the window belongs to us
         free(wndTitle);
         wndTitle = GetInternalWindowTextW(hWndNext); if (!wndTitle) return(FALSE);
         GetClassNameW(hWndNext, className, bufSize);       // TODO: because of i18n we can't rely on the window's text
         if (StrCompare(wndTitle, L"Alert") && StrCompare(className, L"#32770")) {
            hWndAlert = hWndNext;
            break;
         }
      }
      hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
   }
   free(wndTitle);
   if (!hWndAlert) return(debug("\"Alert\" dialog window not found"));

   // show the "Alert" window
   bool wasHidden = !ShowWindow(hWndAlert, SW_SHOW);
   SetForegroundWindow(hWndAlert);

   // play the standard "Alert" sound
   if (wasHidden && sound) {
      PlaySoundW(L"alert.wav");
   }
   return(TRUE);
   #pragma EXPANDER_EXPORT
}
