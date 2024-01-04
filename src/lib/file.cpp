/**
 * The Definitive Guide on Win32 to NT Path Conversion
 * @see  https://googleprojectzero.blogspot.com/2016/02/the-definitive-guide-on-win32-to-nt.html
 *
 *
 * Naming Files, Paths, and Namespaces
 * @see  https://docs.microsoft.com/en-us/windows/desktop/fileio/naming-a-file
 *
 */
#include "expander.h"
#include "lib/file.h"
#include "lib/string.h"

#include <winioctl.h>


/**
 * Create a directory.
 *
 * @param  char* path  - directory path
 * @param  DWORD flags - MODE_MQL:      restrict the function's operation to the MQL sandbox
 *                       MODE_SYSTEM:   allow the function to operate outside of the MQL sandbox
 *                       MODE_MKPARENT: create parent directories as needed and report no error on an existing directory;
 *                                      otherwise create only the final directory and report an error if it exists
 * @return int - error status
 */
int WINAPI CreateDirectoryA(const char* path, DWORD flags) {
   if ((uint)path < MIN_VALID_POINTER)     return(error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path));
   if (!(~flags & (MODE_MQL|MODE_SYSTEM))) return(error(ERR_INVALID_PARAMETER, "invalid parameter flag: only one of MODE_MQL or MODE_SYSTEM can be specified"));
   if (!( flags & (MODE_MQL|MODE_SYSTEM))) return(error(ERR_INVALID_PARAMETER, "invalid parameter flag: one of MODE_MQL or MODE_SYSTEM must be specified"));

   if (flags & MODE_MQL) {
      return(_int(ERR_NOT_IMPLEMENTED, error(ERR_NOT_IMPLEMENTED, "support for flag MODE_MQL not yet implemented")));
   }
   else /*flags & MODE_SYSTEM*/ {
      // check whether such a file or directory already exists
      if (IsFileOrDirectoryA(path)) {
         if (!IsDirectoryA(path, MODE_SYSTEM)) return(error(ERR_WIN32_ERROR+ERROR_FILE_EXISTS, "cannot create directory \"%s\" (a file of the same name already exists)", path));
         if (flags & MODE_MKPARENT)
            return(NO_ERROR);
         return(error(ERR_WIN32_ERROR+ERROR_ALREADY_EXISTS, "directory \"%s\" already exists", path));
      }

      // make sure a parent directory exists
      if (flags & MODE_MKPARENT) {
         string sPath = string(path);
         size_t pos = sPath.find_last_of("\\/");
         if (pos != string::npos) {
            int error = CreateDirectoryA(sPath.substr(0, pos).c_str(), flags);
            if (error) return(error);
         }
      }

      // create the final directory
      if (CreateDirectory(path, (LPSECURITY_ATTRIBUTES)NULL))
         return(NO_ERROR);

      // with multiple path separators it may already exist
      int error = GetLastError();
      if (error==ERROR_ALREADY_EXISTS && (flags & MODE_MKPARENT))
         return(NO_ERROR);
      return(error(ERR_WIN32_ERROR+error, "creation of \"%s\" failed", path));
   }
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified directory exists and is not a regular file. Symbolic links and junctions are supported.
 *
 * @param  char* path - directory path with support for forward, backward and trailing slashes
 * @param  DWORD mode - MODE_MQL:    restrict the function's operation to the MQL sandbox
 *                      MODE_SYSTEM: allow the function to operate outside of the MQL sandbox
 * @return BOOL
 */
BOOL WINAPI IsDirectoryA(const char* path, DWORD mode) {
   if (path) {
      if ((uint)path < MIN_VALID_POINTER)    return(!error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path));
      if (!(~mode & (MODE_MQL|MODE_SYSTEM))) return(!error(ERR_INVALID_PARAMETER, "invalid parameter mode: only one of MODE_MQL or MODE_SYSTEM can be specified"));
      if (!( mode & (MODE_MQL|MODE_SYSTEM))) return(!error(ERR_INVALID_PARAMETER, "invalid parameter mode: one of MODE_MQL or MODE_SYSTEM must be specified"));

      if (mode & MODE_MQL) {
         return(!error(ERR_NOT_IMPLEMENTED, "support for MODE_MQL not yet implemented"));
      }
      else /*mode & MODE_SYSTEM*/ {
         DWORD attributes = GetFileAttributesA(path);
         return((attributes!=INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_DIRECTORY));
      }
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file exists and is not a directory. Symbolic links are supported.
 *
 * @param  char* path - file path (supports forward and backward slashes)
 * @param  DWORD mode - MODE_MQL:    restrict the function to operate only in the MQL sandbox
 *                      MODE_SYSTEM: allow the function to operate anywhere in the system
 * @return BOOL
 */
BOOL WINAPI IsFileA(const char* path, DWORD mode) {
   if (path) {
      if ((uint)path < MIN_VALID_POINTER)    return(!error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path));
      if (!(~mode & (MODE_MQL|MODE_SYSTEM))) return(!error(ERR_INVALID_PARAMETER, "invalid parameter mode: only one of MODE_MQL or MODE_SYSTEM can be specified"));
      if (!( mode & (MODE_MQL|MODE_SYSTEM))) return(!error(ERR_INVALID_PARAMETER, "invalid parameter mode: one of MODE_MQL or MODE_SYSTEM must be specified"));

      if (mode & MODE_MQL) {
         return(!error(ERR_NOT_IMPLEMENTED, "support for MODE_MQL not yet implemented"));
      }
      else /*MODE_SYSTEM*/ {
         DWORD attributes = GetFileAttributesA(path);
         return((attributes!=INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
      }
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file exists and is not a directory. Symbolic links are supported.
 *
 * @param  string &path - file path with support for forward and backward slashes
 * @param  DWORD  mode  - MODE_MQL:    restrict the function to operate only in the MQL sandbox
 *                        MODE_SYSTEM: allow the function to operate anywhere in the system
 * @return BOOL
 */
BOOL WINAPI IsFileA(const string &path, DWORD mode) {
   return(IsFileA(path.c_str(), mode));
}


/**
 * Whether the specified file exists and is not a directory. Symbolic links are supported.
 *
 * @param  wchar* path - file path (supports forward and backward slashes)
 * @param  DWORD mode  - MODE_MQL:    restrict the function to operate only in the MQL sandbox
 *                       MODE_SYSTEM: allow the function to operate anywhere in the system
 * @return BOOL
 */
BOOL WINAPI IsFileW(const wchar* path, DWORD mode) {
   if (path) {
      if ((uint)path < MIN_VALID_POINTER)    return(!error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path));
      if (!(~mode & (MODE_MQL|MODE_SYSTEM))) return(!error(ERR_INVALID_PARAMETER, "invalid parameter mode: only one of MODE_MQL or MODE_SYSTEM can be specified"));
      if (!( mode & (MODE_MQL|MODE_SYSTEM))) return(!error(ERR_INVALID_PARAMETER, "invalid parameter mode: one of MODE_MQL or MODE_SYSTEM must be specified"));

      if (mode & MODE_MQL) {
         return(!error(ERR_NOT_IMPLEMENTED, "support for MODE_MQL not yet implemented"));
      }
      else /*MODE_SYSTEM*/ {
         DWORD attributes = GetFileAttributesW(path);
         return((attributes!=INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
      }
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file exists and is not a directory. Symbolic links are supported.
 *
 * @param  wstring &path - file path with support for forward and backward slashes
 * @param  DWORD   mode  - MODE_MQL:    restrict the function to operate only in the MQL sandbox
 *                         MODE_SYSTEM: allow the function to operate anywhere in the system
 * @return BOOL
 */
BOOL WINAPI IsFileW(const wstring &path, DWORD mode) {
   return(IsFileW(path.c_str(), mode));
}


/**
 * Whether the specified file or directory exists. Symbolic links and junctions are supported.
 *
 * @param  char* name - full name with support for forward, backward and trailing slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsFileOrDirectoryA(const char* name) {
   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

      DWORD attributes = GetFileAttributes(name);
      return(attributes != INVALID_FILE_ATTRIBUTES);
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the specified file or directory exists. Symbolic links and junctions are supported.
 *
 * @param  string &name - full name with support for forward, backward and trailing slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsFileOrDirectoryA(const string &name) {
   return(IsFileOrDirectoryA(name.c_str()));
}


/**
 * Whether the specified directory exists and is a junction.
 *
 * @param  char* name - full directory name with support for forward and backward slashes
 *
 * @return BOOL
 *
 * TODO: distinguish between junctions and volume mount points
 */
BOOL WINAPI IsJunctionA(const char* name) {
   BOOL result = FALSE;

   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

      DWORD attributes = GetFileAttributes(name);
      if ((attributes!=INVALID_FILE_ATTRIBUTES) && (attributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))) {
         char* _name = strdup(name);                                          // on the heap

         int pos = strlen(_name);
         while (--pos >=0 && (_name[pos]=='\\' || _name[pos]=='/')) {         // cut-off trailing slashes
            _name[pos] = 0;
         }

         WIN32_FIND_DATA wfd = {};
         HANDLE hFind = FindFirstFile(_name, &wfd);
         if (hFind == INVALID_HANDLE_VALUE) error(ERR_WIN32_ERROR+GetLastError(), "cannot find path \"%s\"", _name);
         else {
            FindClose(hFind);
            result = (wfd.dwReserved0 == IO_REPARSE_TAG_MOUNT_POINT);
         }
         free(_name);
      }
   }
   return(result);
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
   BOOL result = FALSE;

   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

      DWORD attributes = GetFileAttributes(name);

      if ((attributes!=INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
         char* _name = strdup(name);                                          // on the heap

         if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
            int pos = strlen(_name);
            while (--pos >=0 && (_name[pos]=='\\' || _name[pos]=='/')) {      // cut-off trailing slashes
               _name[pos] = '\0';
            }
         }

         WIN32_FIND_DATA wfd = {};
         HANDLE hFind = FindFirstFile(_name, &wfd);
         if (hFind == INVALID_HANDLE_VALUE) error(ERR_WIN32_ERROR+GetLastError(), "cannot find path \"%s\"", _name);
         else {
            FindClose(hFind);
            result = (wfd.dwReserved0 == IO_REPARSE_TAG_SYMLINK);
         }
         free(_name);
      }
   }
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Get a path's final name. Resolves all reparse points in the path (symlinks, junctions, mount points, subst).
 *
 * @param  char* name - path
 *
 * @return char* - resolved name in "\\?\" or UNC format or a NULL pointer in case of errors
 */
const char* WINAPI GetFinalPathNameA(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

   HANDLE hFile = CreateFileA(name,                                                    // file name
                              GENERIC_READ, FILE_SHARE_READ,                           // open for shared reading
                              NULL,                                                    // default security
                              OPEN_EXISTING,                                           // open existing file only
                              FILE_ATTRIBUTE_NORMAL,                                   // normal file
                              NULL);                                                   // no attribute template
   if (hFile == INVALID_HANDLE_VALUE) return((char*)!error(ERR_WIN32_ERROR+GetLastError(), "CreateFileA() cannot open \"%s\"", name));

   uint size = MAX_PATH;
   char* path;
   DWORD len;

   while (true) {
      path = new char[size];                                                           // on the heap
      len  = GetFinalPathNameByHandleA(hFile, path, size, VOLUME_NAME_DOS|FILE_NAME_OPENED);
      if (len < size)
         break;
      size <<= 1;                                                                      // increase buffer size
      delete[] path;
   }
   CloseHandle(hFile);

   if (!len) {
      delete[] path;
      return((char*)!error(ERR_WIN32_ERROR+GetLastError(), "GetFinalPathNameByHandleA()"));
   }
   return(path);                                                                       // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Resolve the target path of a Windows reparse point (a symlink, junction or volume mount point).
 *
 * @param  char* name - path
 *
 * @return char* - resolved target path or a NULL pointer in case of errors
 *
 * @see    http://blog.kalmbach-software.de/2008/02/28/howto-correctly-read-reparse-data-in-vista/
 * @see    https://tyranidslair.blogspot.com/2016/02/tracking-down-root-cause-of-windows.html
 *
 * Note: The caller is responsible for releasing the returned string's memory after usage with "free()".
 */
const char* WINAPI GetReparsePointTargetA(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

   // open the reparse point
   HANDLE hFile = CreateFileA(name,                                                    // file name
                              FILE_READ_EA,                                            // request reading of extended attributes
                              FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,      // open for shared access
                              NULL,                                                    // default security
                              OPEN_EXISTING,                                           // open existing file only
                              FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS, // open reparse point itself
                              NULL);                                                   // no attribute template
   if (hFile == INVALID_HANDLE_VALUE) return((char*)!error(ERR_WIN32_ERROR+GetLastError(), "CreateFileA() cannot open \"%s\"", name));

   // create a reparse data structure
   DWORD bufferSize = MAXIMUM_REPARSE_DATA_BUFFER_SIZE;
   REPARSE_DATA_BUFFER* rdata = (REPARSE_DATA_BUFFER*) malloc(bufferSize);             // on the heap

   // query the reparse data
   DWORD bytesReturned;
   BOOL success = DeviceIoControl(hFile, FSCTL_GET_REPARSE_POINT, NULL, 0, rdata, bufferSize, &bytesReturned, NULL);
   CloseHandle(hFile);
   if (!success) {
      free(rdata);
      return((char*)!error(ERR_WIN32_ERROR+GetLastError(), "DeviceIoControl() cannot query reparse data of \"%s\"", name));
   }

   char* result = NULL;

   // read the reparse data
   if (IsReparseTagMicrosoft(rdata->ReparseTag)) {
      if (rdata->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT) {
         uint   offset = rdata->MountPointReparseBuffer.SubstituteNameOffset >> 1;
         uint   len    = rdata->MountPointReparseBuffer.SubstituteNameLength >> 1;
         string target = unicodeToAnsi(wstring(&rdata->MountPointReparseBuffer.PathBuffer[offset], len));
         result = strdup(target.c_str() + strlen("\\??\\"));
      }
      else if (rdata->ReparseTag == IO_REPARSE_TAG_SYMLINK) {
         uint   offset = rdata->SymbolicLinkReparseBuffer.SubstituteNameOffset >> 1;
         uint   len    = rdata->SymbolicLinkReparseBuffer.SubstituteNameLength >> 1;
         string target = unicodeToAnsi(wstring(&rdata->SymbolicLinkReparseBuffer.PathBuffer[offset], len));

         BOOL isRelative = rdata->SymbolicLinkReparseBuffer.Flags & SYMLINK_FLAG_RELATIVE;
         if (isRelative) {
            char drive[MAX_DRIVE], dir[MAX_DIR];
            _splitpath(name, drive, dir, NULL, NULL);
            result = strdup(string(drive).append(dir).append(target).c_str());
         }
         else {
            result = strdup(target.c_str() + strlen("\\??\\"));
         }
      }
      else error(ERR_RUNTIME_ERROR, "cannot interpret \"%s\" (not a mount point or symbolic link)", name);
   }
   else error(ERR_RUNTIME_ERROR, "cannot interpret \"%s\" (not a Microsoft reparse point)", name);

   free(rdata);

   return(result);                                                                           // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


// @see  PathCanonicalize()
// @see  https://stackoverflow.com/questions/1816691/how-do-i-resolve-a-canonical-filename-in-windows
// @see  http://pdh11.blogspot.com/2009/05/pathcanonicalize-versus-what-it-says-on.html
//
// @see  https://stackoverflow.com/questions/2487237/detect-symbolic-links-junction-points-mount-points-and-hard-links
