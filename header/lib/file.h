#pragma once
#include "expander.h"


struct REPARSE_DATA_BUFFER {
   ULONG  ReparseTag;
   USHORT ReparseDataLength;
   USHORT Reserved;
   union {
      struct {
         USHORT SubstituteNameOffset;
         USHORT SubstituteNameLength;
         USHORT PrintNameOffset;
         USHORT PrintNameLength;
         WCHAR  PathBuffer[1];
      } MountPointReparseBuffer;
      struct {
         USHORT SubstituteNameOffset;
         USHORT SubstituteNameLength;
         USHORT PrintNameOffset;
         USHORT PrintNameLength;
         ULONG  Flags;
         WCHAR  PathBuffer[1];
      } SymbolicLinkReparseBuffer;
      struct {
         UCHAR DataBuffer[1];
      } GenericReparseBuffer;
   };
};


#define SYMLINK_FLAG_RELATIVE    1


int         WINAPI CreateDirectoryA(const char* path, DWORD flags);
const char* WINAPI GetFinalPathNameA(const char* name);
const char* WINAPI GetReparsePointTargetA(const char* name);
BOOL        WINAPI IsDirectoryA(const char* path, DWORD mode);
BOOL        WINAPI IsFileA(const char* path, DWORD mode);
BOOL        WINAPI IsFileW(const wchar* path, DWORD mode);
BOOL        WINAPI IsFileW(const wstring &path, DWORD mode);
BOOL        WINAPI IsFileOrDirectoryA(const char* name);
BOOL        WINAPI IsJunctionA(const char* name);
BOOL        WINAPI IsSymlinkA(const char* name);
