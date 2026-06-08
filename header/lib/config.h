#pragma once
#include "expander.h"

const char* WINAPI GetGlobalConfigPathA();
const char* WINAPI GetTerminalConfigPathA();

BOOL        WINAPI IsIniKeyA(const char* fileName, const char* section, const char* key);
BOOL        WINAPI IsTerminalConfigKeyA(const char* section, const char* key);
BOOL        WINAPI IsGlobalConfigKeyA(const char* section, const char* key);

DWORD       WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, DWORD bufferSize);
DWORD       WINAPI GetIniSectionsA(const char* fileName, char* buffer, DWORD bufferSize);
BOOL        WINAPI IsIniSectionA(const char* fileName, const char* section);

BOOL        WINAPI DeleteIniKeyA(const char* fileName, const char* section, const char* key);
BOOL        WINAPI DeleteIniSectionA(const char* fileName, const char* section);
BOOL        WINAPI EmptyIniSectionA(const char* fileName, const char* section);

char*       WINAPI GetIniStringA(const char* fileName, const char* section, const char* key, const char* defaultValue = "");
char*       WINAPI GetIniStringRawA(const char* fileName, const char* section, const char* key, const char* defaultValue = "");
