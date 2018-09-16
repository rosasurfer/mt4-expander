#pragma once

#include "expander.h"


BOOL WINAPI IsDirectory(const char* name);
BOOL WINAPI IsFile(const char* name);
BOOL WINAPI IsJunction(const char* name);
BOOL WINAPI IsSymlink(const char* name);
