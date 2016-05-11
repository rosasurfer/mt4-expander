#pragma once


/**
 * MQL-Interface zum Zugriff auf ein struct STARTUPINFOA (Ansi-Version).
 *
 *
 * struct STARTUPINFOA {
 *    DWORD   cb;                      // Getter/Setter mit Alias: si_Size
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
 * };
 */
DWORD WINAPI si_setSize      (STARTUPINFOA* si, DWORD size   );
DWORD WINAPI si_setFlags     (STARTUPINFOA* si, DWORD flags  );
WORD  WINAPI si_setShowWindow(STARTUPINFOA* si, WORD  cmdShow);
