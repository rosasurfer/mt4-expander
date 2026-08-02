#pragma once
#include "expander.h"

const char* WINAPI GetGlobalConfigPathA();
const char* WINAPI GetTerminalConfigPathA();

BOOL        WINAPI IsGlobalConfigKeyA(const char* section, const char* key);
BOOL        WINAPI IsTerminalConfigKeyA(const char* section, const char* key);

BOOL        WINAPI IsIniKeyA(const char* fileName, const char* section, const char* key);
BOOL        WINAPI DeleteIniKeyA(const char* fileName, const char* section, const char* key);

BOOL        WINAPI IsIniSectionA(const char* fileName, const char* section);
BOOL        WINAPI DeleteIniSectionA(const char* fileName, const char* section);
BOOL        WINAPI EmptyIniSectionA(const char* fileName, const char* section);

uint        WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, uint bufferSize);
uint        WINAPI GetIniSectionsA(const char* fileName, char* buffer, uint bufferSize);

char*       WINAPI GetIniStringA(const char* fileName, const char* section, const char* key, const char* defaultValue = "");
char*       WINAPI GetIniStringRawA(const char* fileName, const char* section, const char* key, const char* defaultValue = "");
