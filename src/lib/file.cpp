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

#include <shlobj.h>
#include <winioctl.h>


/**
 * Create a directory recursively. No error is returned if the directory already exists.
 *
 * @param  char* path - full directory path
 *
 * @return int - ERROR_SUCCESS or an error code
 */
int WINAPI CreateDirectoryRecursive(const char* path) {
   if ((uint)path < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter path: 0x%p (not a valid pointer)", path));

   int error = SHCreateDirectoryEx(NULL, path, NULL);

   if (error==ERROR_FILE_EXISTS || error==ERROR_ALREADY_EXISTS)
      error = ERROR_SUCCESS;
   return(error);
   #pragma EXPANDER_EXPORT
}


/**
 * Create a directory recursively. No error is returned if the directory already exists.
 *
 * @param  string& path - full directory path
 *
 * @return int - ERROR_SUCCESS or an error code
 */
int WINAPI CreateDirectoryRecursive(const string& path) {
   int error = SHCreateDirectoryEx(NULL, path.c_str(), NULL);

   if (error==ERROR_FILE_EXISTS || error==ERROR_ALREADY_EXISTS)
      error = ERROR_SUCCESS;
   return(error);
}


/**
 * Whether or not the specified directory exists and is not a regular file. Symbolic links and junctions are supported.
 *
 * @param  char* name - full directory name with support for forward, backward and trailing slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsDirectoryA(const char* name) {
   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);
      return(attrib!=INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not the specified file exists and is not a directory. Symbolic links are supported.
 *
 * @param  char* name - full filename with support for forward and backward slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsFileA(const char* name) {
   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);
      return(attrib!=INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not the specified file exists and is not a directory. Symbolic links are supported.
 *
 * @param  string& name - full filename with support for forward and backward slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsFileA(const string& name) {
   return(IsFileA(name.c_str()));
}


/**
 * Whether or not the specified directory exists and is a junction.
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
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);
      if (attrib!=INVALID_FILE_ATTRIBUTES && (attrib & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))) {
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
 * Whether or not the specified file or directory exists and is a symbolic link.
 *
 * @param  char* name - full filename with support for forward and backward slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsSymlinkA(const char* name) {
   BOOL result = FALSE;

   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);

      if (attrib!=INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_REPARSE_POINT)) {
         char* _name = strdup(name);                                          // on the heap

         if (attrib & FILE_ATTRIBUTE_DIRECTORY) {
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
 * Get a path's final name. This function resolves all reparse points in the path (symlinks, junctions, mount points, subst).
 *
 * @param  char* name - path
 *
 * @return char* - resolved name in "\\?\" or UNC format or a NULL pointer in case of errors
 */
const char* WINAPI GetFinalPathNameA(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

   HANDLE hFile = CreateFile(name,                                                     // file name
                             GENERIC_READ, FILE_SHARE_READ,                            // open for shared reading
                             NULL,                                                     // default security
                             OPEN_EXISTING,                                            // open existing file only
                             FILE_ATTRIBUTE_NORMAL,                                    // normal file
                             NULL);                                                    // no attribute template
   if (hFile == INVALID_HANDLE_VALUE) return((char*)error(ERR_WIN32_ERROR+GetLastError(), "CreateFile() cannot open \"%s\"", name));

   uint size = MAX_PATH;
   char* path;
   DWORD len;

   while (true) {
      path = new char[size];                                                           // on the heap
      len  = GetFinalPathNameByHandle(hFile, path, size, VOLUME_NAME_DOS|FILE_NAME_OPENED);
      if (len < size)
         break;
      size <<= 1;                                                                      // increase buffer size
      delete[] path;
   }
   CloseHandle(hFile);

   if (!len) {
      delete[] path;
      return((char*)error(ERR_WIN32_ERROR+GetLastError(), "=> GetFinalPathNameByHandle()"));
   }
   return(path);                                                                       // TODO: close memory leak
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
 *
 * Note: The memory holding the returned string was allocated with malloc() and should be released after usage.
 *       Calling code must use free() to do so.
 */
const char* WINAPI GetReparsePointTargetA(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

   // open the reparse point
   HANDLE hFile = CreateFile(name,                                                     // file name
                             FILE_READ_EA,                                             // request reading of extended attributes
                             FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,       // open for shared access
                             NULL,                                                     // default security
                             OPEN_EXISTING,                                            // open existing file only
                             FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS,  // open reparse point itself
                             NULL);                                                    // no attribute template
   if (hFile == INVALID_HANDLE_VALUE) return((char*)error(ERR_WIN32_ERROR+GetLastError(), "CreateFile() cannot open \"%s\"", name));

   // create a reparse data structure
   DWORD bufferSize = MAXIMUM_REPARSE_DATA_BUFFER_SIZE;
   REPARSE_DATA_BUFFER* rdata = (REPARSE_DATA_BUFFER*) malloc(bufferSize);             // on the heap

   // query the reparse data
   DWORD bytesReturned;
   BOOL success = DeviceIoControl(hFile, FSCTL_GET_REPARSE_POINT, NULL, 0, rdata, bufferSize, &bytesReturned, NULL);
   CloseHandle(hFile);
   if (!success) {
      free(rdata);
      return((char*)error(ERR_WIN32_ERROR+GetLastError(), "DeviceIoControl() cannot query reparse data of \"%s\"", name));
   }

   char* result = NULL;

   // read the reparse data
   if (IsReparseTagMicrosoft(rdata->ReparseTag)) {
      if (rdata->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT) {
         size_t offset = rdata->MountPointReparseBuffer.SubstituteNameOffset >> 1;
         size_t len    = rdata->MountPointReparseBuffer.SubstituteNameLength >> 1;
         char* target = wchartombs(&rdata->MountPointReparseBuffer.PathBuffer[offset], len);
         //debug("mount point to \"%s\"", target);
         result = strdup(target + strlen("\\??\\"));
         free(target);
      }
      else if (rdata->ReparseTag == IO_REPARSE_TAG_SYMLINK) {
         size_t offset = rdata->SymbolicLinkReparseBuffer.SubstituteNameOffset >> 1;
         size_t len    = rdata->SymbolicLinkReparseBuffer.SubstituteNameLength >> 1;
         char* target = wchartombs(&rdata->SymbolicLinkReparseBuffer.PathBuffer[offset], len);
         BOOL isRelative = rdata->SymbolicLinkReparseBuffer.Flags & SYMLINK_FLAG_RELATIVE;
         //debug("%s symlink to \"%s\"", isRelative ? "relative":"absolute", target);

         if (isRelative) {
            char drive[_MAX_DRIVE], dir[_MAX_DIR];
            _splitpath(name, drive, dir, NULL, NULL);
            string s = string(drive).append(dir).append(target);
            result = strdup(s.c_str());
         }
         else {
            result = strdup(target + strlen("\\??\\"));
         }
         free(target);
      }
      else error(ERR_RUNTIME_ERROR, "cannot interpret \"%s\" (not a mount point or symbolic link)", name);
   }
   else error(ERR_RUNTIME_ERROR, "cannot interpret \"%s\" (not a Microsoft reparse point)", name);

   free(rdata);

   return(result);                                                                           // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}


// @see  PathCanonicalize()
// @see  https://stackoverflow.com/questions/1816691/how-do-i-resolve-a-canonical-filename-in-windows
// @see  http://pdh11.blogspot.com/2009/05/pathcanonicalize-versus-what-it-says-on.html
//
// @see  https://stackoverflow.com/questions/2487237/detect-symbolic-links-junction-points-mount-points-and-hard-links
