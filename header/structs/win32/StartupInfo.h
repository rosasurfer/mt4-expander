#pragma once

/**
 * Win32 struct STARTUPINFOA (Ansi-Version)
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
DWORD WINAPI si_setSize      (STARTUPINFOA* si, DWORD size   );
DWORD WINAPI si_setFlags     (STARTUPINFOA* si, DWORD flags  );
WORD  WINAPI si_setShowWindow(STARTUPINFOA* si, WORD  cmdShow);
