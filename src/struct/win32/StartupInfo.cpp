/**
 * Win32 struct STARTUPINFOA (ANSI-Version)
 *
 * struct STARTUPINFOA {
 *    DWORD   cb;                         //  4    Getter/Setter-Alias: si_Size() / si_setSize()
 *    LPSTR   lpReserved;                 //  4
 *    LPSTR   lpDesktop;                  //  4
 *    LPSTR   lpTitle;                    //  4
 *    DWORD   dwX;                        //  4
 *    DWORD   dwY;                        //  4
 *    DWORD   dwXSize;                    //  4
 *    DWORD   dwYSize;                    //  4
 *    DWORD   dwXCountChars;              //  4
 *    DWORD   dwYCountChars;              //  4
 *    DWORD   dwFillAttribute;            //  4
 *    DWORD   dwFlags;                    //  4
 *    WORD    wShowWindow;                //  2
 *    WORD    cbReserved2;                //  2
 *    LPBYTE  lpReserved2;                //  4
 *    HANDLE  hStdInput;                  //  4
 *    HANDLE  hStdOutput;                 //  4
 *    HANDLE  hStdError;                  //  4
 * } STARTUPINFO;                         // 68 byte
 */
#include "expander.h"


/**
 * Setzt die Gr��e einer STARTUPINFO.
 *
 * @param  STARTUPINFOA* si
 * @param  DWORD         size
 *
 * @return DWORD - dieselbe Gr��e
 */
DWORD WINAPI si_setSize(STARTUPINFOA* si, DWORD size) {
   if ((uint)si < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter si: 0x%p (not a valid pointer)", si));
   if (size != sizeof(STARTUPINFOA)) return(!error(ERR_INVALID_PARAMETER, "invalid parameter size: %d (must be %d)", size, sizeof(STARTUPINFOA)));

   return(si->cb = size);
   #pragma EXPANDER_EXPORT
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
   if ((uint)si < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter si: 0x%p (not a valid pointer)", si));

   return(si->dwFlags = flags);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die ShowWindow-Konstante einer STARTUPINFO.
 *
 * @param  STARTUPINFOA* si
 * @param  WORD          cmdShow
 *
 * @return WORD - dieselbe Konstante oder -1, falls ein Fehler auftrat
 */
WORD WINAPI si_setShowWindow(STARTUPINFOA* si, WORD cmdShow) {
   if ((uint)si < MIN_VALID_POINTER) return((WORD)_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter si: 0x%p (not a valid pointer)", si)));

   return(si->wShowWindow = cmdShow);
   #pragma EXPANDER_EXPORT
}
