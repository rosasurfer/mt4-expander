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
#include "expander.h"


/**
 * Gibt die Dateiattribute eines struct WIN32_FIND_DATA zurück.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return DWORD
 */
DWORD WINAPI wfd_FileAttributes(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das ReadOnly-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_ReadOnly(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_READONLY);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Hidden-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Hidden(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das System-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_System(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Directory-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Directory(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Archive-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Archive(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Device-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Device(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_DEVICE);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Normal-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Normal(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_NORMAL);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Temporary-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Temporary(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das SparseFile-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_SparseFile(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das ReparsePoint-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_ReparsePoint(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Compressed-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Compressed(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Offline-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Offline(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_OFFLINE);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das ContentNotIndexed-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_NotIndexed(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Encrypted-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Encrypted(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob das Virtual-Dateiattribut eines struct WIN32_FIND_DATA gesetzt ist.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return BOOL
 */
BOOL WINAPI wfd_FileAttribute_Virtual(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Dateinamen eines struct WIN32_FIND_DATA zurück.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return char*
 */
const char* WINAPI wfd_FileName(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->cFileName);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den alternativen Dateinamen eines struct WIN32_FIND_DATA zurück.
 *
 * @param  WIN32_FIND_DATAA* wfd
 *
 * @return char*
 */
const char* WINAPI wfd_AlternateFileName(const WIN32_FIND_DATAA* wfd) {
   if ((uint)wfd < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter wfd: 0x%p (not a valid pointer)", wfd));
   return(wfd->cAlternateFileName);
   #pragma EXPANDER_EXPORT
}
