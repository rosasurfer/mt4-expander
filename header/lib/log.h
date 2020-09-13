#pragma once
#include "stdafx.h"
#include "struct/rsf/ExecutionContext.h"


BOOL WINAPI LogMessageA  (EXECUTION_CONTEXT* ec, const char* message, int error, int level);
BOOL WINAPI SetCustomLogA(EXECUTION_CONTEXT* ec, const char* filename);
