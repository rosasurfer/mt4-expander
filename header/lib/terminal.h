#pragma once
#include "expander.h"


extern "C" IMAGE_DOS_HEADER     __ImageBase;             // this DLL's module handle
#define HMODULE_DLL ((HMODULE) &__ImageBase)             // this DLL's module handle


HWND                    WINAPI FindInputDialog(ProgramType programType, const char* programName);
const char*             WINAPI GetExpanderFileNameA();
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
const wstring&          WINAPI GetTerminalPathW();
const char*             WINAPI GetTerminalRoamingDataPathA();
const char*             WINAPI GetTerminalVersion();
const VS_FIXEDFILEINFO* WINAPI GetTerminalVersionFromFile();
const VS_FIXEDFILEINFO* WINAPI GetTerminalVersionFromImage();
BOOL                    WINAPI LoadMqlProgramA(HWND hChart, ProgramType programType, const char* programName);
BOOL                    WINAPI LoadMqlProgramW(HWND hChart, ProgramType programType, const wchar* programName);
BOOL                    WINAPI TerminalHasWritePermission(const char* dir);
BOOL                    WINAPI TerminalIsLockedLogfile(const string &filename);
BOOL                    WINAPI TerminalIsPortableMode();
