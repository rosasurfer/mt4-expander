#pragma once
#include "expander.h"


HWND                    WINAPI FindInputDialog(ProgramType programType, const char* programName);
const char*             WINAPI GetExpanderFileNameA();
const wchar*            WINAPI GetExpanderFileNameW();
HMODULE                 WINAPI GetExpanderModuleW2K();
HMODULE                 WINAPI GetExpanderModuleXP();
const char*             WINAPI GetMqlDirectoryA();
uint                    WINAPI GetTerminalBuild();
const char*             WINAPI GetTerminalCommonDataPathA();
const char*             WINAPI GetTerminalDataPathA();
const char*             WINAPI GetTerminalFileNameA();
const wchar*            WINAPI GetTerminalFileNameW();
HWND                    WINAPI GetTerminalMainWindow();
const char*             WINAPI GetTerminalPathA();
const wchar*            WINAPI GetTerminalPathW();
const wstring&          WINAPI GetTerminalPathWs();
const char*             WINAPI GetTerminalRoamingDataPathA();
const wchar*            WINAPI GetTerminalRoamingDataPathW();
const char*             WINAPI GetTerminalVersion();
const VS_FIXEDFILEINFO* WINAPI GetTerminalVersionFromFile();
const VS_FIXEDFILEINFO* WINAPI GetTerminalVersionFromImage();
BOOL                    WINAPI LoadMqlProgramA(HWND hChart, ProgramType programType, const char* programName);
BOOL                    WINAPI LoadMqlProgramW(HWND hChart, ProgramType programType, const wchar* programName);
BOOL                    WINAPI ReopenAlertDialog(BOOL sound = TRUE);
BOOL                    WINAPI TerminalIsPortableMode();
