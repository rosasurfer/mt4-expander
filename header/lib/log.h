#pragma once
#include "stdafx.h"


BOOL WINAPI LogA         (EXECUTION_CONTEXT* ec, const char* message, int error = NO_ERROR);
BOOL WINAPI SetCustomLogA(EXECUTION_CONTEXT* ec, const char* filename);
