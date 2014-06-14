#pragma once

#define WIN32_LEAN_AND_MEAN             
#include <windows.h>
#include <stdlib.h> 
#include <malloc.h>


#define EXPORT_FUNCTION comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)


// interne Darstellung eines MQL-Strings
struct MqlStr {
   int   length;
   char* string;
};
