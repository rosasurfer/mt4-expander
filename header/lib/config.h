#pragma once
#include "expander.h"


const char* WINAPI GetGlobalConfigPathA();
const char* WINAPI GetLocalConfigPathA();

DWORD       WINAPI GetIniKeysA(const char* fileName, const char* section, char* buffer, DWORD bufferSize);
