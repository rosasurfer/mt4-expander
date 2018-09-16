#include "expander.h"


/**
 * Whether or not the specified directory exists and is not a regular file. Symbolic links and junctions are supported.
 *
 * @param  char* name - full directory name with support for forward, backward and trailing slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsDirectory(const char* name) {
   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));

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
BOOL WINAPI IsFile(const char* name) {
   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);
      return(attrib!=INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not the specified file or directory exists and is a symbolic link.
 *
 * @param  char* name - full filename with support for forward and backward slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsSymlink(const char* name) {
   BOOL result = FALSE;

   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);

      if (attrib!=INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_REPARSE_POINT)) {
         char* _name = strcpy(new char[strlen(name)+1], name);                // on the heap

         if (attrib & FILE_ATTRIBUTE_DIRECTORY) {
            int pos = strlen(_name);
            while (--pos >=0 && (_name[pos]=='\\' || _name[pos]=='/')) {      // trim trailing backslashes
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
         delete[] _name;
      }
   }
   return(result);
   #pragma EXPANDER_EXPORT
}
