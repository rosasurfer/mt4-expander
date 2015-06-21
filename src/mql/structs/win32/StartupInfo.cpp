/**
 * Win32 structure STARTUPINFOA
 *
 *
 * struct STARTUPINFOA {
 *    DWORD   cb;
 *    LPSTR   lpReserved;
 *    LPSTR   lpDesktop;
 *    LPSTR   lpTitle;
 *    DWORD   dwX;
 *    DWORD   dwY;
 *    DWORD   dwXSize;
 *    DWORD   dwYSize;
 *    DWORD   dwXCountChars;
 *    DWORD   dwYCountChars;
 *    DWORD   dwFillAttribute;
 *    DWORD   dwFlags;
 *    WORD    wShowWindow;
 *    WORD    cbReserved2;
 *    LPBYTE  lpReserved2;
 *    HANDLE  hStdInput;
 *    HANDLE  hStdOutput;
 *    HANDLE  hStdError;
 * } si;
 *
 *
 * MQL-Importdeklarationen:
 * ------------------------
 * int si.setCb        (/*STARTUPINFO/int si[], int size   );
 *     ...
 *     ...
 *     ...
 *     ...
 *     ...
 *     ...
 *     ...
 *     ...
 *     ...
 * int si.setFlags     (/*STARTUPINFO/int si[], int flags  );
 *     ...
 * int si.setShowWindow(/*STARTUPINFO/int si[], int cmdShow);
 *     ...
 *     ...
 *     ...
 *     ...
 *     ...
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Setzt die Größe einer STARTUPINFO.
 *
 * @param  STARTUPINFOA* si
 * @param  DWORD         size
 *
 * @return DWORD - dieselbe Größe
 */
DWORD WINAPI si_setCb(STARTUPINFOA* si, DWORD size) {
   if ((uint)si < MIN_VALID_POINTER) return(debug("invalid parameter si = 0x%p (not a valid pointer)", si));
   if (size != sizeof(STARTUPINFOA))  return(debug("invalid parameter size = %d (must be %d)", size, sizeof(STARTUPINFOA)));

   si->cb = size;
   return(size);
   #pragma EXPORT
}


/**
 * Setzt die Flags einer STARTUPINFO.
 *
 * @param  STARTUPINFOA* si
 * @param  DWORD         flags
 *
 * @return DWORD - dieselben Flags
 */
DWORD WINAPI si_setFlags(STARTUPINFOA* si, DWORD flags) {
   if ((uint)si < MIN_VALID_POINTER) return(debug("invalid parameter si = 0x%p (not a valid pointer)", si));

   si->dwFlags = flags;
   return(flags);
   #pragma EXPORT
}


/**
 * Setzt die ShowWindow-Konstante einer STARTUPINFO.
 *
 * @param  STARTUPINFOA* si
 * @param  WORD          cmdShow
 *
 * @return WORD - dieselbe Konstante
 */
WORD WINAPI si_setShowWindow(STARTUPINFOA* si, WORD cmdShow) {
   if ((uint)si < MIN_VALID_POINTER) return(debug("invalid parameter si = 0x%p (not a valid pointer)", si));

   si->wShowWindow = cmdShow;
   return(cmdShow);
   #pragma EXPORT
}
