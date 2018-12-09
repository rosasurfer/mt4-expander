#pragma once
#include "expander.h"


// gmtime()/localtime() format codes:
//
// @see  http://www.cplusplus.com/reference/ctime/strftime/
// @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm

const char* WINAPI GmtTimeFormat(datetime timestamp, const char* format);
const char* WINAPI LocalTimeFormat(datetime timestamp, const char* format);


// printf() format codes:
//
// @see  https://alvinalexander.com/programming/printf-format-cheat-sheet
// @see  http://www.cplusplus.com/reference/cstdio/printf/
// @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm

char* WINAPI NumberFormat(double value, const char* format);
