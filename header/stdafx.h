#pragma once

#define  NOMINMAX                      // another way to prevent std::min|max errors is the syntax std::min<int>(a, b)
#define _USE_32BIT_TIME_T

#ifdef _DEBUG
   #define _CRTDBG_MAP_ALLOC
   #include <crtdbg.h>
#endif

#define WIN32_LEAN_AND_MEAN            // exclude rarely-used stuff from Windows headers
#include <windows.h>
