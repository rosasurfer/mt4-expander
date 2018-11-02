#pragma once
#include "expander.h"


const char* WINAPI GetGlobalConfigPathA();
DWORD       WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, DWORD bufferSize);
const char* WINAPI GetLocalConfigPathA();
BOOL        WINAPI IsIniKey(const char* fileName, const char* section, const char* key);
BOOL        WINAPI IsIniSection(const char* fileName, const char* section);
