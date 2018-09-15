#pragma once

#include "expander.h"


uint           WINAPI GetTerminalBuild();
const char*    WINAPI GetTerminalCommonDataPathA();
const char*    WINAPI GetTerminalDataPathA();
const char*    WINAPI GetTerminalPathA();
const wstring& WINAPI GetTerminalPathWs();
const char*    WINAPI GetTerminalRoamingDataPathA();
const char*    WINAPI GetTerminalVersion();
BOOL           WINAPI GetTerminalVersion(uint* major, uint* minor, uint* hotfix, uint* build);
