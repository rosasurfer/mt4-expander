#pragma once
#include "expander.h"


BOOL        WINAPI DeleteIniKeyA(const char* fileName, const char* section, const char* key);
BOOL        WINAPI DeleteIniSectionA(const char* fileName, const char* section);
BOOL        WINAPI EmptyIniSectionA(const char* fileName, const char* section);
const char* WINAPI GetGlobalConfigPathA();
DWORD       WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, DWORD bufferSize);
DWORD       WINAPI GetIniSectionsA(const char* fileName, char* buffer, DWORD bufferSize);
char*       WINAPI GetIniStringA(const char* fileName, const char* section, const char* key, const char* defaultValue = "");
char*       WINAPI GetIniStringRawA(const char* fileName, const char* section, const char* key, const char* defaultValue = "");
const char* WINAPI GetLocalConfigPathA();
BOOL        WINAPI IsGlobalConfigKeyA(const char* section, const char* key);
BOOL        WINAPI IsIniKeyA(const char* fileName, const char* section, const char* key);
BOOL        WINAPI IsIniSectionA(const char* fileName, const char* section);
BOOL        WINAPI IsLocalConfigKeyA(const char* section, const char* key);
