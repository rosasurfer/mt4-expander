#pragma once
#include "expander.h"


const char* WINAPI mciErrorToStr(const DWORD error);
DWORD       WINAPI PlaySoundA(const char* soundfile);
DWORD       WINAPI PlaySoundW(const wchar* soundfile);
