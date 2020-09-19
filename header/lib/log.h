#pragma once
#include "stdafx.h"
#include "struct/rsf/ExecutionContext.h"


BOOL WINAPI AppendLogMessageA(EXECUTION_CONTEXT* ec, const char* message, int error, int level);
BOOL WINAPI SetLogfileA      (EXECUTION_CONTEXT* ec, const char* filename);
