#include "expander.h"


/**
 * Whether or not the specified directory exists and is not a regular file. Symbolic links and junctions are supported.
 *
 * @param  char* name - full directory name with support for forward, backward and trailing slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsDirectoryA(const char* name) {
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
BOOL WINAPI IsFileA(const char* name) {
   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);
      return(attrib!=INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
   }
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not the specified directory exists and is a junction.
 *
 * @param  char* name - full directory name with support for forward and backward slashes
 *
 * @return BOOL
 */
BOOL WINAPI IsJunctionA(const char* name) {
   BOOL result = FALSE;

   if (name) {
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);
      if (attrib!=INVALID_FILE_ATTRIBUTES && (attrib & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))) {
         char* _name = strcpy(new char[strlen(name)+1], name);                // on the heap

         int pos = strlen(_name);
         while (--pos >=0 && (_name[pos]=='\\' || _name[pos]=='/')) {         // cut-off trailing slashes
            _name[pos] = '\0';
         }

         WIN32_FIND_DATA wfd = {};
         HANDLE hFind = FindFirstFile(_name, &wfd);
         if (hFind == INVALID_HANDLE_VALUE) error(ERR_WIN32_ERROR+GetLastError(), "cannot find path \"%s\"", _name);
         else {
            FindClose(hFind);
            result = (wfd.dwReserved0 == IO_REPARSE_TAG_MOUNT_POINT);
         }
         delete[] _name;
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
      if ((uint)name < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));

      DWORD attrib = GetFileAttributes(name);

      if (attrib!=INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_REPARSE_POINT)) {
         char* _name = strcpy(new char[strlen(name)+1], name);                // on the heap

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
         delete[] _name;
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
   if ((uint)name < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));

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
      size <<= 2;                                                                      // increase buffer size
      delete[] path;
   }
   CloseHandle(hFile);

   if (!len) {
      delete[] path;
      return((char*)error(ERR_WIN32_ERROR+GetLastError(), "GetFinalPathNameByHandle() failed"));
   }
   return(path);                                                                       // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}


/**
 *
 */
void WINAPI Test() {
   char path[MAX_PATH];
   debug("path: %s", path);
   //#pragma EXPANDER_EXPORT
}


// @see  https://stackoverflow.com/questions/1816691/how-do-i-resolve-a-canonical-filename-in-windows
// @see  http://pdh11.blogspot.com/2009/05/pathcanonicalize-versus-what-it-says-on.html
// @see  PathCanonicalize()
//
// @see  https://stackoverflow.com/questions/221417/how-do-i-programmatically-access-the-target-path-of-a-windows-symbolic-link
// @see  https://stackoverflow.com/questions/46383428/get-the-immediate-target-path-from-symlink-reparse-point
// @see  https://www.codeproject.com/Articles/58950/How-to-develop-a-virtual-disk-for-Windows?msg=5371595
// @see  https://msdn.microsoft.com/en-us/library/cc232006.aspx


/*
#include "ntifs.h"

volatile UCHAR guz;

ULONG TestReparsePoint(PCWSTR FileName) {

   HANDLE hFile = CreateFileW(FileName, 0, FILE_SHARE_VALID_FLAGS, 0, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS, 0);
   if (hFile == INVALID_HANDLE_VALUE) return GetLastError();

   union {
      PVOID                     pv;
      PULONG                    ReparseTag;
      //PREPARSE_DATA_BUFFER      prdb;
      PREPARSE_GUID_DATA_BUFFER prgdb;
   };

    PVOID stack = alloca(guz);

    ULONG cb = 0, rcb = sizeof(REPARSE_DATA_BUFFER) + 0x100, BytesReturned;

    ULONG dwError;

    do
    {
        if (cb < rcb) cb = RtlPointerToOffset(pv = alloca(rcb - cb), stack);

        if (DeviceIoControl(hFile, FSCTL_GET_REPARSE_POINT, 0, 0, pv, cb, &BytesReturned, 0))
        {
            dwError = NOERROR;

            if (IsReparseTagMicrosoft(*ReparseTag))
            {
                char cc[16];
                LPCSTR name;

                switch (*ReparseTag)
                {
                case IO_REPARSE_TAG_SYMLINK:
                    name = " SYMLINK";
                    stack = prdb->SymbolicLinkReparseBuffer.PathBuffer;
                    break;
                case IO_REPARSE_TAG_MOUNT_POINT:
                    name = " MOUNT_POINT";
                    stack = prdb->MountPointReparseBuffer.PathBuffer;
                    break;
                default:
                    sprintf(cc, " %08x", prdb->ReparseTag);
                    name = cc;
                }

                DbgPrint(" %s->%.*S <%.*S>\n", name,
                    prdb->MountPointReparseBuffer.SubstituteNameLength >> 1,
                    RtlOffsetToPointer(stack, prdb->MountPointReparseBuffer.SubstituteNameOffset),
                    prdb->MountPointReparseBuffer.PrintNameLength >> 1,
                    RtlOffsetToPointer(stack, prdb->MountPointReparseBuffer.PrintNameOffset)
                    );
            }
            else
            {
                PGUID g = &prgdb->ReparseGuid;
                DbgPrint(" tag=%x {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x} size=%x\n", *ReparseTag,
                    g->Data1, g->Data2, g->Data3,
                    g->Data4[0],g->Data4[1],g->Data4[2],g->Data4[3],g->Data4[4],g->Data4[5],g->Data4[6],g->Data4[7],
                    prgdb->ReparseDataLength);

            }
            break;
        }

        rcb = IsReparseTagMicrosoft(*ReparseTag)
            ? REPARSE_DATA_BUFFER_HEADER_SIZE + prdb->ReparseDataLength
            : REPARSE_GUID_DATA_BUFFER_HEADER_SIZE + prgdb->ReparseDataLength;

    } while((dwError = GetLastError()) == ERROR_MORE_DATA);

    CloseHandle(hFile);

    return dwError;
}
*/