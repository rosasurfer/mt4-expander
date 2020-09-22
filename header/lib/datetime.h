#pragma once
#include "expander.h"


datetime    WINAPI GetGmtTime();
datetime    WINAPI GetLocalTime();
const char* WINAPI GmtTimeFormatA(datetime timestamp, const char* format);
const char* WINAPI LocalTimeFormatA(datetime timestamp, const char* format);
