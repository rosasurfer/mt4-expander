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


#define SYMLINK_FLAG_RELATIVE    0x00000001


int         WINAPI CreateDirectoryRecursive(const char* path);
int         WINAPI CreateDirectoryRecursive(const string& path);
const char* WINAPI GetFinalPathNameA(const char* name);
const char* WINAPI GetReparsePointTargetA(const char* name);
BOOL        WINAPI IsDirectoryA(const char* name);
BOOL        WINAPI IsFileA(const char* name);
BOOL        WINAPI IsJunctionA(const char* name);
BOOL        WINAPI IsSymlinkA(const char* name);
