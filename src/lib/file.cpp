/**
 * The Definitive Guide on Win32 to NT Path Conversion
 * @see  https://googleprojectzero.blogspot.com/2016/02/the-definitive-guide-on-win32-to-nt.html
 *
 *
 * Naming Files, Paths, and Namespaces
 * @see  https://docs.microsoft.com/en-us/windows/desktop/fileio/naming-a-file
 */
#include "expander.h"
#include "lib/file.h"
#include "lib/string.h"

#pragma warning(push)
#pragma warning(disable:4201)          // nonstandard extension used : nameless struct/union
#include <winioctl.h>
#pragma warning(pop)


/**
 * Create a directory.
 *
 * @param  char* path  - directory path
 * @param  DWORD flags - MODE_SYSTEM:   allow operation outside of the MQL sandbox (default)
 *                       MODE_MQL:      restrict operation to the MQL sandbox
 *                       MODE_TESTER:   apply operation to the tester sandbox (requires MODE_MQL)
 *                       MODE_MKPARENT: create parent directories as needed and report no error on an existing directory;
 *                                      otherwise create only the final directory and report an error if it exists
 * @return int - error status
 */
int WINAPI CreateDirectoryA(const char* path, DWORD flags/*= MODE_SYSTEM*/) {
   if ((uint)path < MIN_VALID_POINTER) return error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path);
   if (!*path)                         return error(ERR_INVALID_PARAMETER, "invalid parameter path: \"\" (empty)");

   wstring wpath = ansiToUtf16(string(path));
   return CreateDirectoryW(wpath.c_str(), flags);
   #pragma EXPANDER_EXPORT
}


/**
 * Create a directory.
 *
 * @param  wchar* path  - directory path
 * @param  DWORD  flags - MODE_SYSTEM:   allow operation outside of the MQL sandbox (default)
 *                        MODE_MQL:      restrict operation to the MQL sandbox
 *                        MODE_TESTER:   apply operation to the tester sandbox (requires MODE_MQL)
 *                        MODE_MKPARENT: create parent directories as needed and report no error on an existing directory;
 *                                       otherwise create only the final directory and report an error if it exists
 * @return int - error status
 */
int WINAPI CreateDirectoryW(const wchar* path, DWORD flags/*= MODE_SYSTEM*/) {
   if ((uint)path < MIN_VALID_POINTER) return error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path);
   if (!*path)                         return error(ERR_INVALID_PARAMETER, "invalid parameter path: \"\" (empty)");

   if (flags & MODE_MQL)    return error(ERR_NOT_IMPLEMENTED, "support for MODE_MQL not yet implemented");
   if (flags & MODE_TESTER) return error(ERR_NOT_IMPLEMENTED, "support for MODE_TESTER not yet implemented");

   // MODE_SYSTEM
   // check whether such a file or directory already exists
   if (IsFileOrDirectoryW(path)) {
      if (!IsDirectoryW(path))      return error(ERR_WIN32_ERROR + ERROR_FILE_EXISTS, "cannot create directory \"%S\" (a file of the same name already exists)", path);
      if (!(flags & MODE_MKPARENT)) return error(ERR_WIN32_ERROR + ERROR_ALREADY_EXISTS, "directory \"%S\" already exists", path);
      return NO_ERROR;
   }

   // make sure a parent directory exists
   if (flags & MODE_MKPARENT) {
      wstring sPath = wstring(path);
      size_t pos = sPath.find_last_of(L"\\/");
      if (pos != wstring::npos) {
         if (pos == 0) return error(ERR_INVALID_PARAMETER, "invalid parameter path: \"%S\"", path);
         int error = CreateDirectoryW(sPath.substr(0, pos).c_str(), flags);
         if (error) return error;
      }
   }

   // create the final directory
   if (CreateDirectoryW(path, (LPSECURITY_ATTRIBUTES)NULL)) {
      return NO_ERROR;
   }

   // with multiple path separators the directory may already exist
   if (GetLastError() == ERROR_ALREADY_EXISTS && (flags & MODE_MKPARENT)) {
      return NO_ERROR;
   }
   return error(ERR_WIN32_ERROR + GetLastError(), "creation of \"%S\" failed", path);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified directory exists and is not a regular file. Symbolic links and junctions are supported.
 *
 * @param  char* path - directory path with support for forward, backward and trailing slashes
 * @param  DWORD mode - MODE_SYSTEM: allow operation outside of the MQL sandbox (default)
 *                      MODE_MQL:    restrict operation to the MQL sandbox
 *                      MODE_TESTER: apply operation to the tester sandbox (requires MODE_MQL)
 * @return BOOL
 */
BOOL WINAPI IsDirectoryA(const char* path, DWORD mode/*= MODE_SYSTEM*/) {
   if (!path)                          return FALSE;
   if ((uint)path < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path);

   wstring wpath = ansiToUtf16(string(path));
   return IsDirectoryW(wpath.c_str(), mode);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified directory exists and is not a regular file. Symbolic links and junctions are supported.
 *
 * @param  wchar* path - directory path with support for forward, backward and trailing slashes
 * @param  DWORD  mode - MODE_SYSTEM: allow operation outside of the MQL sandbox (default)
 *                       MODE_MQL:    restrict operation to the MQL sandbox
 *                       MODE_TESTER: apply operation to the tester sandbox (requires MODE_MQL)
 * @return BOOL
 */
BOOL WINAPI IsDirectoryW(const wchar* path, DWORD mode/*= MODE_SYSTEM*/) {
   if (!path)                          return FALSE;
   if ((uint)path < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path);

   if (mode & MODE_MQL)    return !error(ERR_NOT_IMPLEMENTED, "support for MODE_MQL not yet implemented");
   if (mode & MODE_TESTER) return !error(ERR_NOT_IMPLEMENTED, "support for MODE_TESTER not yet implemented");

   // MODE_SYSTEM
   DWORD attributes = GetFileAttributesW(path);
   return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file exists and is not a directory. Symbolic links are supported.
 *
 * @param  char* path - file path (supports forward and backward slashes)
 * @param  DWORD mode - MODE_SYSTEM: allow operation outside of the MQL sandbox (default)
 *                      MODE_MQL:    restrict operation to the MQL sandbox
 *                      MODE_TESTER: apply operation to the tester sandbox (requires MODE_MQL)
 * @return BOOL
 */
BOOL WINAPI IsFileA(const char* path, DWORD mode/*= MODE_SYSTEM*/) {
   if (!path)                          return FALSE;
   if ((uint)path < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path);

   wstring wpath = ansiToUtf16(string(path));
   return IsFileW(wpath.c_str(), mode);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file exists and is not a directory. Symbolic links are supported.
 *
 * @param  wchar* path - file path (supports forward and backward slashes)
 * @param  DWORD  mode - MODE_SYSTEM: allow operation outside of the MQL sandbox (default)
 *                       MODE_MQL:    restrict operation to the MQL sandbox
 *                       MODE_TESTER: apply operation to the tester sandbox (requires MODE_MQL)
 * @return BOOL
 */
BOOL WINAPI IsFileW(const wchar* path, DWORD mode/*= MODE_SYSTEM*/) {
   if (!path)                          return FALSE;
   if ((uint)path < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path);

   if (mode & MODE_MQL)    return !error(ERR_NOT_IMPLEMENTED, "support for MODE_MQL not yet implemented");
   if (mode & MODE_TESTER) return !error(ERR_NOT_IMPLEMENTED, "support for MODE_TESTER not yet implemented");

   // MODE_SYSTEM
   DWORD attributes = GetFileAttributesW(path);
   return (attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file or directory exists. Symbolic links and junctions are supported.
 *
 * @param  char* name - full name with support for forward, backward and trailing slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsFileOrDirectoryA(const char* name) {
   if (!name)                          return FALSE;
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);

   wstring wname = ansiToUtf16(string(name));
   return IsFileOrDirectoryW(wname.c_str());
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file or directory exists. Symbolic links and junctions are supported.
 *
 * @param  wchar* name - full name with support for forward, backward and trailing slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsFileOrDirectoryW(const wchar* name) {
   if (!name)                          return FALSE;
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);

   DWORD attributes = GetFileAttributesW(name);
   return (attributes != INVALID_FILE_ATTRIBUTES);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified directory exists and is a Windows junction.
 *
 * @param  char* name - full directory name with support for forward and backward slashes
 *
 * @return BOOL
 *
 * TODO: distinguish between junctions and volume mount points
 */
BOOL WINAPI IsJunctionA(const char* name) {
   if (!name)                          return FALSE;
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", name);

   wstring ws = ansiToUtf16(string(name));
   return IsJunctionW(ws.c_str());
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified directory exists and is a Windows junction.
 *
 * @param  wchar* name - full directory name with support for forward and backward slashes
 *
 * @return BOOL
 *
 * TODO: distinguish between junctions and volume mount points
 */
BOOL WINAPI IsJunctionW(const wchar* name) {
   if (!name)                          return FALSE;
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);

   DWORD attributes = GetFileAttributesW(name);
   if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))) {
      return FALSE;
   }

   wchar* path = wsdupa(name);                                    // on the stack
   int pos = wstrlen(path);
   while (--pos >= 0 && (path[pos]==L'\\' || path[pos]==L'/')) {  // cut-off trailing slashes
      path[pos] = 0;
   }

   WIN32_FIND_DATAW wfd = {};
   HANDLE hFind = FindFirstFileW(path, &wfd);
   if (hFind == INVALID_HANDLE_VALUE) return !info(ERR_WIN32_ERROR + GetLastError(), "path not found: \"%S\"", path);
   FindClose(hFind);

   return (wfd.dwReserved0 == IO_REPARSE_TAG_MOUNT_POINT);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file or directory exists and is a symbolic link.
 *
 * @param  char* name - full filename with support for forward and backward slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsSymlinkA(const char* name) {
   if (!name)                          return FALSE;
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);

   wstring ws = ansiToUtf16(string(name));
   return IsSymlinkW(ws.c_str());
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file or directory exists and is a symbolic link.
 *
 * @param  wchar* name - full filename with support for forward and backward slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsSymlinkW(const wchar* name) {
   if (!name)                          return FALSE;
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);

   DWORD attributes = GetFileAttributesW(name);
   if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
      return FALSE;
   }

   wchar* path = wsdupa(name);                                       // on the stack
   if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
      int pos = wstrlen(path);
      while (--pos >= 0 && (path[pos]==L'\\' || path[pos]==L'/')) {  // cut-off trailing slashes
         path[pos] = 0;
      }
   }

   WIN32_FIND_DATAW wfd = {};
   HANDLE hFind = FindFirstFileW(path, &wfd);
   if (hFind == INVALID_HANDLE_VALUE) return !info(ERR_WIN32_ERROR + GetLastError(), "path not found: \"%S\"", path);
   FindClose(hFind);

   return (wfd.dwReserved0 == IO_REPARSE_TAG_SYMLINK);
   #pragma EXPANDER_EXPORT
}


/**
 * Get a path's final name. Resolves all reparse points in the path (symlinks, junctions, mount points, subst).
 *
 * @param  char* name - path
 *
 * @return char* - resolved name in "\\?\" or UNC format, or a NULL pointer in case of errors
 */
char* WINAPI GetFinalPathNameA(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);

   HANDLE hFile = CreateFileA(name,                            // file name
                              GENERIC_READ, FILE_SHARE_READ,   // open for shared reading
                              NULL,                            // default security
                              OPEN_EXISTING,                   // open existing file only
                              FILE_ATTRIBUTE_NORMAL,           // normal file
                              NULL);                           // no attribute template
   if (hFile == INVALID_HANDLE_VALUE) return (char*)!error(ERR_WIN32_ERROR + GetLastError(), "CreateFileA() cannot open \"%s\"", name);

   uint size = MAX_PATH;

   while (true) {
      char* path = (char*) alloca(size);                       // on the stack
      DWORD len = GetFinalPathNameByHandleA(hFile, path, size, VOLUME_NAME_DOS|FILE_NAME_OPENED);
      if (len < size) {
         CloseHandle(hFile);
         if (!len) return (char*)!error(ERR_WIN32_ERROR + GetLastError(), "GetFinalPathNameByHandleA()");
         return sdup(path);                                    // caller must free()
      }
      size <<= 1;
   }
   #pragma EXPANDER_EXPORT
}


/**
 * Resolve the target path of a Windows reparse point (a symlink, junction or volume mount point).
 *
 * @param  char* name - path
 *
 * @return char* - resolved target path, or a NULL pointer in case of errors
 *
 * @see    http://blog.kalmbach-software.de/2008/02/28/howto-correctly-read-reparse-data-in-vista/
 * @see    https://tyranidslair.blogspot.com/2016/02/tracking-down-root-cause-of-windows.html
 */
char* WINAPI GetReparsePointTargetA(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);

   // open the reparse point
   HANDLE hFile = CreateFileA(name,                                                    // file name
                              FILE_READ_EA,                                            // request reading of extended attributes
                              FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,      // open for shared access
                              NULL,                                                    // default security
                              OPEN_EXISTING,                                           // open existing file only
                              FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS, // open reparse point itself
                              NULL);                                                   // no attribute template
   if (hFile == INVALID_HANDLE_VALUE) return (char*)!error(ERR_WIN32_ERROR + GetLastError(), "CreateFileA() cannot open \"%s\"", name);

   // create a reparse data structure
   DWORD bufferSize = MAXIMUM_REPARSE_DATA_BUFFER_SIZE;
   REPARSE_DATA_BUFFER* rdata = (REPARSE_DATA_BUFFER*) malloc(bufferSize);             // must be on the heap (dynamic size)

   // query the reparse data
   DWORD bytesReturned;
   BOOL success = DeviceIoControl(hFile, FSCTL_GET_REPARSE_POINT, NULL, 0, rdata, bufferSize, &bytesReturned, NULL);
   CloseHandle(hFile);
   if (!success) {
      free(rdata);
      return (char*)!error(ERR_WIN32_ERROR + GetLastError(), "DeviceIoControl() cannot query reparse data of \"%s\"", name);
   }

   char* result = NULL;

   // read the reparse data
   if (IsReparseTagMicrosoft(rdata->ReparseTag)) {
      if (rdata->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT) {
         uint   offset = rdata->MountPoint.SubstituteNameOffset >> 1;
         uint   len    = rdata->MountPoint.SubstituteNameLength >> 1;
         string target = utf16ToAnsi(wstring(&rdata->MountPoint.PathBuffer[offset], len));
         result = sdup(target.c_str() + strlen("\\??\\"));
      }
      else if (rdata->ReparseTag == IO_REPARSE_TAG_SYMLINK) {
         uint   offset = rdata->SymbolicLink.SubstituteNameOffset >> 1;
         uint   len    = rdata->SymbolicLink.SubstituteNameLength >> 1;
         string target = utf16ToAnsi(wstring(&rdata->SymbolicLink.PathBuffer[offset], len));

         BOOL isRelative = rdata->SymbolicLink.Flags & SYMLINK_FLAG_RELATIVE;
         if (isRelative) {
            char drive[MAX_DRIVE] = {}, dir[MAX_DIR] = {};
            _splitpath(name, drive, dir, NULL, NULL);
            result = sdup(string(drive).append(dir).append(target).c_str());
         }
         else {
            result = sdup(target.c_str() + strlen("\\??\\"));
         }
      }
      else error(ERR_RUNTIME_ERROR, "cannot interpret \"%s\" (not a mount point or symbolic link)", name);
   }
   else error(ERR_RUNTIME_ERROR, "cannot interpret \"%s\" (not a Windows reparse point)", name);

   free(rdata);
   return result;                                  // caller must free()
   #pragma EXPANDER_EXPORT
}

// @see  PathCanonicalize()
// @see  https://stackoverflow.com/questions/1816691/how-do-i-resolve-a-canonical-filename-in-windows
// @see  http://pdh11.blogspot.com/2009/05/pathcanonicalize-versus-what-it-says-on.html
// @see  https://stackoverflow.com/questions/2487237/detect-symbolic-links-junction-points-mount-points-and-hard-links


/**
 * Searches the PATH for the specified file and returns the full file path.
 *
 * @param  char* file - file
 *
 * @return char* - found file path, or a NULL pointer in case of errors
 */
char* WINAPI SearchPathA(const char* file) {
   if ((uint)file < MIN_VALID_POINTER) return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter file: 0x%p (not a valid pointer)", file);
   if (!*file)                         return (char*)!error(ERR_INVALID_PARAMETER, "invalid parameter file: \"\" (empty)");

   wstring ws = ansiToUtf16(string(file));
   wchar* wpath = SearchPathW(ws.c_str());
   char* path = utf16ToAnsi(wpath);
   free(wpath);
   return path;                                    // caller must free()
   #pragma EXPANDER_EXPORT
}


/**
 * Searches the PATH for the specified file and returns the full file path.
 *
 * @param  wchar* file - file
 *
 * @return wchar* - found file path, or a NULL pointer in case of errors
 */
wchar* WINAPI SearchPathW(const wchar* file) {
   if ((uint)file < MIN_VALID_POINTER) return (wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter file: 0x%p (not a valid pointer)", file);
   if (!*file)                         return (wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter file: \"\" (empty)");

   wchar fullPath[MAX_PATH] = {};                  // on the stack
   DWORD result = SearchPathW(NULL, file, L".exe", MAX_PATH, fullPath, NULL);
   if (!result) return NULL;                       // ERROR_FILE_NOT_FOUND
   if (result < MAX_PATH) return wsdup(fullPath);  // copy to heap

   // buffer too small, result is the required size (including NUL)
   wchar* buffer = (wchar*)malloc(result * sizeof(wchar));
   if (!buffer) return NULL;

   DWORD result2 = SearchPathW(NULL, file, L".exe", result, buffer, NULL);
   if (!result2 || result2 >= result) {            // failed or still too small (concurrent PATH modification)
      free(buffer);
      return (wchar*)!error(ERR_WIN32_ERROR + GetLastError(), "for \"%S\" failed or concurrent PATH modification", file);
   }
   return buffer;                                  // caller must free()
   #pragma EXPANDER_EXPORT
}
