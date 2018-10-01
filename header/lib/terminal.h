#pragma once

#include "expander.h"


uint                    WINAPI GetTerminalBuild();
const char*             WINAPI GetTerminalCommonDataPathA();
const char*             WINAPI GetTerminalDataPathA();
const char*             WINAPI GetTerminalModuleFileNameA();
const WCHAR*            WINAPI GetTerminalModuleFileNameW();
const char*             WINAPI GetTerminalPathA();
const wstring&          WINAPI GetTerminalPathWS();
const char*             WINAPI GetTerminalRoamingDataPathA();
const char*             WINAPI GetTerminalVersion();
const VS_FIXEDFILEINFO* WINAPI GetTerminalVersionFromFile();
const VS_FIXEDFILEINFO* WINAPI GetTerminalVersionFromImage();
BOOL                    WINAPI TerminalHasWritePermission(const char* dir);
BOOL                    WINAPI TerminalIsLockedLogfile(const string& filename);
BOOL                    WINAPI TerminalIsPortableMode();
