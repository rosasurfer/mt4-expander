#include "expander.h"


/**
 * Whether or not the specified directory exists and is not a regular file. Symbolic links and junctions are supported.
 *
 * @param  char* name - full directory name with support for forward and backward slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsDirectory(const char* name) {
   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));

      DWORD dwAttrib = GetFileAttributes(name);
      return(dwAttrib!=INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
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

      DWORD dwAttrib = GetFileAttributes(name);
      return(dwAttrib!=INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}
