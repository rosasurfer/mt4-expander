#pragma once
#include "expander.h"
#include "struct/ExecutionContext.h"

BOOL WINAPI AppendLogMessageA(EXECUTION_CONTEXT* ec, time32 serverTime, const char* message, int error, int level);
BOOL WINAPI SetLogfileA      (EXECUTION_CONTEXT* ec, const char* filename);
