#pragma once
#include "expander.h"


const char* WINAPI mciErrorToStr(const DWORD error);
BOOL        WINAPI PlaySoundA(const char* soundfile);
BOOL        WINAPI PlaySoundW(const wchar* soundfile);
