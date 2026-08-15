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
      } MountPoint;
      struct {
         USHORT SubstituteNameOffset;
         USHORT SubstituteNameLength;
         USHORT PrintNameOffset;
         USHORT PrintNameLength;
         ULONG  Flags;
         WCHAR  PathBuffer[1];
      } SymbolicLink;
      struct {
         UCHAR DataBuffer[1];
      } Generic;
   };
};

#define SYMLINK_FLAG_RELATIVE 1

int    WINAPI CreateDirectoryA(const char* path, DWORD flags = MODE_SYSTEM);
int    WINAPI CreateDirectoryW(const wchar* path, DWORD flags = MODE_SYSTEM);
char*  WINAPI GetFinalPathNameA(const char* name);
char*  WINAPI GetReparsePointTargetA(const char* name);
BOOL   WINAPI IsDirectoryA(const char* path, DWORD mode = MODE_SYSTEM);
BOOL   WINAPI IsDirectoryW(const wchar* path, DWORD mode = MODE_SYSTEM);
BOOL   WINAPI IsFileA(const char* path, DWORD mode = MODE_SYSTEM);
BOOL   WINAPI IsFileW(const wchar* path, DWORD mode = MODE_SYSTEM);
BOOL   WINAPI IsFileOrDirectoryA(const char* name);
BOOL   WINAPI IsFileOrDirectoryW(const wchar* name);
BOOL   WINAPI IsJunctionA(const char* name);
BOOL   WINAPI IsJunctionW(const wchar* name);
BOOL   WINAPI IsSymlinkA(const char* name);
BOOL   WINAPI IsSymlinkW(const wchar* name);

char*  WINAPI SearchPathA(const char* file);
wchar* WINAPI SearchPathW(const wchar* file);
