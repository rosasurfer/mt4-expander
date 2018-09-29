#pragma once

#include "expander.h"


uint           WINAPI GetTerminalBuild();
const char*    WINAPI GetTerminalCommonDataPathA();
const char*    WINAPI GetTerminalDataPathA();
const char*    WINAPI GetTerminalModuleFileNameA();
const WCHAR*   WINAPI GetTerminalModuleFileNameW();
const char*    WINAPI GetTerminalPathA();
const wstring& WINAPI GetTerminalPathWs();
const char*    WINAPI GetTerminalRoamingDataPathA();
const char*    WINAPI GetTerminalVersion();
BOOL           WINAPI GetTerminalVersion(uint* major, uint* minor, uint* hotfix, uint* build);
BOOL           WINAPI TerminalHasWritePermission(const char* dir);
BOOL           WINAPI TerminalIsLockedLogfile(const string& filename);
BOOL           WINAPI TerminalIsPortableMode();
