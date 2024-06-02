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
   return(portableMode);
   #pragma EXPANDER_EXPORT
}


/**
 * Find and return the name of the history directory containing the specified file.
 *
 * @param  char* filename   - simple filename
 * @param  BOOL  removeFile - whether to remove a found file
 *
 * @return char* - directory name (last segment of the full path) or a NULL pointer in case of errors
 */
const char* WINAPI FindHistoryDirectoryA(const char* filename, BOOL removeFile) {
   if ((uint)filename < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter filename: 0x%p (not a valid pointer)", filename));
   if (!*filename)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter filename: \"\" (empty)"));

   char* hstDirectory = NULL;
   string pattern = string(GetHistoryRootPathA()).append("\\*");
   WIN32_FIND_DATA wfd = {};

   HANDLE hFind = FindFirstFileA(pattern.c_str(), &wfd);
   if (hFind == INVALID_HANDLE_VALUE) return((char*)!error(ERR_FILE_NOT_FOUND, "directory \"%s\" not found", pattern.c_str()));

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

   return(strdup(hstDirectory));
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
      if (!length) return((char*)!error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileNameA()"));

      filename = strdup(buffer);                                        // on the heap
   }
   return(filename);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full filename of the loaded MT4Expander DLL.
 *
 * @return wchar* - filename or a NULL pointer in case of errors
 */
const wchar* WINAPI GetExpanderFileNameW() {
   static wchar* filename;

   if (!filename) {
      wchar* buffer;
      uint size=MAX_PATH >> 1, length=size;
      while (length >= size) {
         size <<= 1;
         buffer = (wchar*) alloca(size);                                // on the stack
         length = GetModuleFileNameW(HMODULE_EXPANDER, buffer, size);   // may return a path longer than MAX_PATH
      }
      if (!length) return((wchar*)!error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileNameW()"));

      filename = wstrdup(buffer);                                       // on the heap
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
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
 */
const char* WINAPI GetHistoryRootPathA() {
   static char* hstDirectory;
   if (!hstDirectory) {
      const char* dataPath = GetTerminalDataPathA();

      if (dataPath) {
         string path = string(dataPath).append("\\history");
         hstDirectory = strdup(path.c_str());
      }
   }
   return(hstDirectory);
   #pragma EXPANDER_EXPORT
}


/**
* Return the full path of the history root directory. The function doesn't check whether the directory exists.
 *
 * @return wchar* - directory name without trailing path separator or a NULL pointer in case of errors
 */
const wchar* WINAPI GetHistoryRootPathW() {
   static wchar* hstDirectory;
   if (!hstDirectory) {
      const wchar* dataPath = GetTerminalDataPathW();

      if (dataPath) {
         wstring path = wstring(dataPath).append(L"\\history");
         hstDirectory = wstrdup(path.c_str());
      }
   }
   return(hstDirectory);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL directory.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
 */
const char* WINAPI GetMqlDirectoryA() {
   static char* mqlDirectory;
   if (!mqlDirectory) {
      const char* dataPath = GetTerminalDataPathA();

      if (dataPath) {
         string path(dataPath);
         if (GetTerminalBuild() <= 509) path.append("\\experts");
         else                           path.append("\\mql4");
         mqlDirectory = strdup(path.c_str());
      }
   }
   return(mqlDirectory);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL directory.
 *
 * @return wchar* - directory name without trailing path separator or a NULL pointer in case of errors
 */
const wchar* WINAPI GetMqlDirectoryW() {
   static wchar* mqlDirectory;

   if (!mqlDirectory) {
      const wchar* dataPath = GetTerminalDataPathW();

      if (dataPath) {
         wstring path(dataPath);
         if (GetTerminalBuild() <= 509) path.append(L"\\experts");
         else                           path.append(L"\\mql4");
         mqlDirectory = wstrdup(path.c_str());
      }
   }
   return(mqlDirectory);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL sandbox directory. This is the only directory accessible to built-in MQL file functions.
 *
 * @param  BOOL inTester - whether to return the online path or the one in the tester
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
 */
const char* WINAPI GetMqlSandboxPathA(BOOL inTester) {
   if (inTester) {
      static char* testerPath;
      if (!testerPath) {
         const char* dataDirectory = GetTerminalDataPathA();
         if (!dataDirectory) return(NULL);
         string path = string(dataDirectory).append("\\tester\\files");
         testerPath = strdup(path.c_str());
      }
      return(testerPath);
   }

   static char* onlinePath;
   if (!onlinePath) {
      const char* mqlDirectory = GetMqlDirectoryA();
      if (!mqlDirectory) return(NULL);
      string path = string(mqlDirectory).append("\\files");
      onlinePath = strdup(path.c_str());
   }
   return(onlinePath);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the MQL sandbox directory. This is the only directory accessible to built-in MQL file functions.
 *
 * @param  BOOL inTester - whether to return the online path or the one in the tester
 *
 * @return wchar* - directory name without trailing path separator or a NULL pointer in case of errors
 */
const wchar* WINAPI GetMqlSandboxPathW(BOOL inTester) {
   static wchar* testerPath, *onlinePath;

   if (inTester) {
      if (!testerPath) {
         const wchar* dataDirectory = GetTerminalDataPathW();
         if (!dataDirectory) return(NULL);
         wstring path = wstring(dataDirectory).append(L"\\tester\\files");

         wchar* tmp = wstrdup(path.c_str());
         if (!testerPath) testerPath = tmp;
         else free(tmp);                        // another thread may have been faster
      }
      return(testerPath);
   }

   if (!onlinePath) {
      const wchar* mqlDirectory = GetMqlDirectoryW();
      if (!mqlDirectory) return(NULL);
      wstring path = wstring(mqlDirectory).append(L"\\files");

      wchar* tmp = wstrdup(path.c_str());
      if (!onlinePath) onlinePath = tmp;
      else free(tmp);                           // another thread may have been faster
   }
   return(onlinePath);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's common data directory (same value as returned by TerminalInfoString(TERMINAL_COMMONDATA_PATH)
 * introduced in MQL5). The common data directory is shared between all terminals installed by a user. The function does not
 * check whether the returned directory exists.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors,
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\Common"
 */
const char* WINAPI GetTerminalCommonDataPathA() {
   static char* path;

   if (!path) {
      const wchar* wpath = GetTerminalCommonDataPathW();
      if (wpath) {
         char* tmp = strdup(unicodeToAnsi(wstring(wpath)).c_str());
         if (!path) path = tmp;
         else free(tmp);                  // another thread may have been faster
      }
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's common data directory (same value as returned by TerminalInfoString(TERMINAL_COMMONDATA_PATH)
 * introduced in MQL5). The common data directory is shared between all terminals installed by a user. The function does not
 * check whether the returned directory exists.
 *
 * @return wchar* - directory name without trailing path separator or a NULL pointer in case of errors,
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

      wchar* tmp = wstrdup(dir.c_str());
      if (!path) path = tmp;
      else free(tmp);                     // another thread may have been faster
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the currently used data directory (same value as returned by TerminalInfoString(TERMINAL_DATA_PATH)
 * introduced in MQL5). The function does not check whether the returned directory exists.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors,
 *                 e.g. "%ProgramFiles%\MetaTrader4"
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
      const char* terminalPath = GetTerminalPathA();
      const char* roamingDataPath = GetTerminalRoamingDataPathA();
      char* tmp = NULL;

      // check portable mode
      if (IsPortableMode()) {
         // data path is the installation directory, independant of write permissions
         tmp = strdup(terminalPath);
      }
      else {
         // check for locked terminal logs
         char* logFilename = LocalTimeFormatA(GetGmtTime32(), "\\logs\\%Y%m%d.log");
         BOOL terminalPathIsLocked = IsLockedFile(string(terminalPath).append(logFilename));
         BOOL roamingPathIsLocked  = IsLockedFile(string(roamingDataPath).append(logFilename));
         free(logFilename);

         if      (roamingPathIsLocked)  tmp = strdup(roamingDataPath);
         else if (terminalPathIsLocked) tmp = strdup(terminalPath);
         else return((char*)!error(ERR_RUNTIME_ERROR, "no open terminal logfile found"));  // both directories are write-protected
      }

      if (!dataPath) dataPath = tmp;
      else free(tmp);                     // another thread may have been faster
   }
   return(dataPath);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the currently used data directory (same value as returned by TerminalInfoString(TERMINAL_DATA_PATH)
 * introduced in MQL5). The function does not check whether the returned directory exists.
 *
 * @return wchar* - directory name without trailing path separator or a NULL pointer in case of errors,
 *                  e.g. "%ProgramFiles%\MetaTrader4"
 */
const wchar* WINAPI GetTerminalDataPathW() {
   // @see  GetTerminalDataPathA()
   static wchar* dataPath;

   if (!dataPath) {
      const wchar* terminalPath = GetTerminalPathW();
      const wchar* roamingDataPath = GetTerminalRoamingDataPathW();
      wchar* tmp = NULL;

      // check portable mode
      if (IsPortableMode()) {
         // data path is the installation directory, independant of write permissions
         tmp = wstrdup(terminalPath);
      }
      else {
         // check for locked terminal logs
         char* logFilename = LocalTimeFormatA(GetGmtTime32(), "\\logs\\%Y%m%d.log");
         BOOL terminalPathIsLocked = IsLockedFile(unicodeToAnsi(wstring(terminalPath)).append(logFilename));
         BOOL roamingPathIsLocked  = IsLockedFile(unicodeToAnsi(wstring(roamingDataPath)).append(logFilename));
         free(logFilename);

         if      (roamingPathIsLocked)  tmp = wstrdup(roamingDataPath);
         else if (terminalPathIsLocked) tmp = wstrdup(terminalPath);
         else return((wchar*)!error(ERR_RUNTIME_ERROR, "no open terminal logfile found"));   // both directories are write-protected
      }

      if (!dataPath) dataPath = tmp;
      else free(tmp);                     // another thread may have been faster
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
 * @return char* - filename or a NULL pointer in case of errors
 */
const char* WINAPI GetTerminalFileNameA() {
   static char* filename;

   if (!filename) {
      const wchar* wfilename = GetTerminalFileNameW();
      if (wfilename) {
         char* tmp = strdup(unicodeToAnsi(wstring(wfilename)).c_str());
         if (!filename) filename = tmp;
         else free(tmp);                  // another thread may have been faster
      }
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
      uint size=MAX_PATH >> 1, length=size;

      while (length >= size) {
         size <<= 1;
         buffer = (wchar*) alloca(size * sizeof(wchar));    // on the stack
         length = GetModuleFileNameW(NULL, buffer, size);   // may return a path longer than MAX_PATH
      }
      if (!length) return((wchar*)!error(ERR_WIN32_ERROR+GetLastError(), "->GetModuleFileName()"));

      wchar* tmp = wstrdup(buffer);
      if (!filename) filename = tmp;
      else free(tmp);                                       // another thread may have been faster
   }
   return(filename);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the name of the terminal's installation directory (same value as returned by TerminalInfoString(TERMINAL_PATH)
 * introduced in MQL5).
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
 */
const char* WINAPI GetTerminalPathA() {
   static char* path;

   if (!path) {
      const wchar* wpath = GetTerminalPathW();
      if (wpath) {
         char* tmp = strdup(unicodeToAnsi(wstring(wpath)).c_str());
         if (!path) path = tmp;
         else free(tmp);                  // another thread may have been faster
      }
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the name of the terminal's installation directory (same value as returned by TerminalInfoString(TERMINAL_PATH)
 * introduced in MQL5).
 *
 * @return wchar* - directory name without trailing path separator or a NULL pointer in case of errors
 */
const wchar* WINAPI GetTerminalPathW() {
   static wchar* path;

   if (!path) {
      const wchar* filename = GetTerminalFileNameW();
      if (filename) {
         wchar* tmp = wstrdup(filename);
         tmp[wstring(tmp).find_last_of(L"\\")] = '\0';

         if (!path) path = tmp;
         else free(tmp);                  // another thread may have been faster
      }
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's roaming data directory. Depending on terminal version and runtime mode the used
 * data directory may differ. The function does not check whether the returned path exists.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
 *                 e.g. "%UserProfile%\AppData\Roaming\MetaQuotes\Terminal\{installation-hash}"
 */
const char* WINAPI GetTerminalRoamingDataPathA() {
   static char* path;

   if (!path) {
      const wchar* wpath = GetTerminalRoamingDataPathW();
      if (wpath) {
         char* tmp = strdup(unicodeToAnsi(wstring(wpath)).c_str());
         if (!path) path = tmp;
         else free(tmp);                  // another thread may have been faster
      }
   }
   return(path);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the full path of the terminal's roaming data directory. Depending on terminal version and runtime mode the used
 * data directory may differ. The function does not check whether the returned path exists.
 *
 * @return char* - directory name without trailing path separator or a NULL pointer in case of errors
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
      StrToUpper(terminalPath);

      string md5Hash(MD5Hash(terminalPath.c_str(), terminalPath.length()*sizeof(wchar)));
      StrToUpper(md5Hash);

      wstring dir = wstring(appDataPath).append(L"\\MetaQuotes\\Terminal\\").append(ansiToUnicode(md5Hash));

      wchar* tmp = wstrdup(dir.c_str());
      if (!result) result = tmp;
      else free(tmp);                     // another thread may have been faster
   }
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the terminal's build number. Same value as returned by TerminalInfoInteger(TERMINAL_BUILD) introduced in MQL5.
 *
 * @return uint - build number or 0 in case of errors
 */
uint WINAPI GetTerminalBuild() {
   static uint build;
   if (!build) {
      VS_FIXEDFILEINFO fileInfo;
      if      (GetTerminalVersionFromImage(&fileInfo)) {}
      else if (GetTerminalVersionFromFile(&fileInfo)) {}
      else return(NULL);

      build = fileInfo.dwFileVersionLS & 0xffff;
   }
   return(build);
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
      VS_FIXEDFILEINFO fileInfo;
      if      (GetTerminalVersionFromImage(&fileInfo)) {}
      else if (GetTerminalVersionFromFile(&fileInfo)) {}
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
 * @param  VS_FIXEDFILEINFO* fileInfo - struct the version infos are written to
 *
 * @return BOOL - success status
 */
BOOL WINAPI GetTerminalVersionFromFile(VS_FIXEDFILEINFO* fileInfo) {
   const char* fileName = GetTerminalFileNameA();
   DWORD infoSize = GetFileVersionInfoSizeA(fileName, &infoSize);
   if (!infoSize) return(!error(ERR_WIN32_ERROR+GetLastError(), "->GetFileVersionInfoSize()"));

   char* infos = (char*)alloca(infoSize);       // on the stack
   BOOL success = GetFileVersionInfoA(fileName, NULL, infoSize, infos);
   if (!success) return(!error(ERR_WIN32_ERROR+GetLastError(), "->GetFileVersionInfo()"));

   VS_FIXEDFILEINFO* result;                    // points to an offset in 'infos' (on the stack)
   uint len;
   success = VerQueryValueA(infos, "\\", (void**)&result, &len);
   if (!success) return(!error(ERR_WIN32_ERROR+GetLastError(), "->VerQueryValue()"));

   *fileInfo = *result;                         // copy content of result to fileInfo
   return(TRUE);
}


/**
 * Read the terminal's version infos from the PE image in memory.
 *
 * @param  VS_FIXEDFILEINFO* fileInfo - struct the version infos are written to
 *
 * @return BOOL - success status
 */
BOOL WINAPI GetTerminalVersionFromImage(VS_FIXEDFILEINFO* fileInfo) {
   static VS_FIXEDFILEINFO* result;             // the resource stays in memory until the process terminates

   if (!result) {
      HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
      if (!hRes) return(!error(ERR_WIN32_ERROR+GetLastError(), "->FindResource()"));

      HGLOBAL hVersionInfo = LoadResource(NULL, hRes);
      if (!hVersionInfo) return(!error(ERR_WIN32_ERROR+GetLastError(), "->LoadResource()"));

      void* infos = LockResource(hVersionInfo);
      if (!infos) return(!error(ERR_WIN32_ERROR+GetLastError(), "->LockResource()"));

      int offset = 6;
      if (!wcscmp((wchar*)((BYTE*)infos + offset), L"VS_VERSION_INFO")) {
         offset += sizeof(L"VS_VERSION_INFO");
         offset += offset % 4;                  // align to next 32 bit
         VS_FIXEDFILEINFO* tmp = (VS_FIXEDFILEINFO*)((BYTE*)infos + offset);
         if (tmp->dwSignature != 0xfeef04bd) return(!warn(ERR_ILLEGAL_STATE, "unknown VS_FIXEDFILEINFO signature: 0x%p", tmp->dwSignature));
         result = tmp;
      }
   }

   *fileInfo = *result;                         // copy content of result to fileInfo
   return(TRUE);
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
   if (!IsWindow(hChart))                     return(!error(ERR_INVALID_PARAMETER, "invalid parameter hChart: %p (not an existing window)", hChart));
   if (!IsProgramType(programType))           return(!error(ERR_INVALID_PARAMETER, "invalid parameter programType: %d (unknown)", programType));
   if ((uint)programName < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName));
   if (!*programName)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter programName: \"\" (must be non-empty)"));

   string file(GetMqlDirectoryA());
   char* sType = NULL;
   uint cmd = 0;

   // check the program for existence
   switch (programType) {
      case PT_INDICATOR:
         file.append("\\indicators\\").append(programName).append(".ex4");
         sType = "indicator";
         cmd = MT4_LOAD_CUSTOM_INDICATOR;
         break;

      case PT_EXPERT:
         file.append(GetTerminalBuild() <= 509 ? "\\":"\\experts\\").append(programName).append(".ex4");
         sType = "expert";
         cmd = MT4_LOAD_EXPERT;
         break;

      case PT_SCRIPT:
         file.append("\\scripts\\").append(programName).append(".ex4");
         sType = "script";
         cmd = MT4_LOAD_SCRIPT;
         break;
   }
   if (!IsFileA(file.c_str(), MODE_SYSTEM)) return(!error(ERR_FILE_NOT_FOUND, "file not found: \"%s\"", file.c_str()));

   // trigger the launch of the program
   PostMessageA(hChart, WM_MT4(), cmd, (LPARAM)strdup(programName));    // pass a copy of the name (the passed parameter may be immediately destroyed)
   debug("queued launch of %s \"%s\"", sType, programName);

   // prevent the DLL from getting unloaded before the message is processed
   HMODULE hModule = NULL;
   GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCTSTR)LoadMqlProgramA, &hModule);

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
   if ((uint)programName < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName));

   string name = unicodeToAnsi(wstring(programName));
   return(LoadMqlProgramA(hChart, programType, name.c_str()));
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
   wchar* className = (wchar*)alloca(bufSize*2);            // on the stack
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
