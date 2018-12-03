#pragma once
#include "expander.h"


// time format codes:
//
// @see  http://www.cplusplus.com/reference/ctime/strftime/
// @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm

string      WINAPI gmtTimeFormat(datetime timestamp, const char* format);
const char* WINAPI GmtTimeFormat(datetime timestamp, const char* format);
string      WINAPI localTimeFormat(datetime timestamp, const char* format);
const char* WINAPI LocalTimeFormat(datetime timestamp, const char* format);


// number format codes:
//
// @see  https://alvinalexander.com/programming/printf-format-cheat-sheet
// @see  http://www.cplusplus.com/reference/cstdio/printf/
// @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm

char*       WINAPI NumberFormat(int    value, const char* format);
char*       WINAPI NumberFormat(uint   value, const char* format);
char*       WINAPI NumberFormat(double value, const char* format);
