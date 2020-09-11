#pragma once
#include "expander.h"


datetime    WINAPI GetGmtTime();
datetime    WINAPI GetLocalTime();
const char* WINAPI GmtTimeFormat(datetime timestamp, const char* format);
const char* WINAPI LocalTimeFormat(datetime timestamp, const char* format);
