#pragma once
#include "stdafx.h"


/**
 * Win32 struct WIN32_FIND_DATAA (Ansi-Version)
 *
 * struct WIN32_FIND_DATAA {
 *    DWORD    dwFileAttributes;          //   4
 *    FILETIME ftCreationTime;            //   8
 *    FILETIME ftLastAccessTime;          //   8
 *    FILETIME ftLastWriteTime;           //   8
 *    DWORD    nFileSizeHigh;             //   4
 *    DWORD    nFileSizeLow;              //   4
 *    DWORD    dwReserved0;               //   4
 *    DWORD    dwReserved1;               //   4
 *    CHAR     cFileName[MAX_PATH];       // 260
 *    CHAR     cAlternateFileName[14];    //  14
 * } WIN32_FIND_DATA;                     // 318 byte
 */
DWORD       WINAPI wfd_FileAttributes            (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_ReadOnly    (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Hidden      (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_System      (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Directory   (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Archive     (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Device      (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Normal      (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Temporary   (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_SparseFile  (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_ReparsePoint(const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Compressed  (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Offline     (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_NotIndexed  (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Encrypted   (const WIN32_FIND_DATAA* wfd);
BOOL        WINAPI wfd_FileAttribute_Virtual     (const WIN32_FIND_DATAA* wfd);
//                 ...
const char* WINAPI wfd_FileName                  (const WIN32_FIND_DATAA* wfd);
const char* WINAPI wfd_AlternateFileName         (const WIN32_FIND_DATAA* wfd);
