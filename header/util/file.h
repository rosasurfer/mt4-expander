#pragma once

#include "expander.h"


BOOL WINAPI IsDirectoryA(const char* name);
BOOL WINAPI IsFileA(const char* name);
BOOL WINAPI IsJunctionA(const char* name);
BOOL WINAPI IsSymlinkA(const char* name);
