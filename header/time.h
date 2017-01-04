#pragma once


//
// strftime() format codes:
//
// @see  http://www.cplusplus.com/reference/ctime/strftime/
// @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm

const char* WINAPI GmTimeFormat(datetime timestamp, const char* format);
string      WINAPI gmTimeFormat(datetime timestamp, const char* format);

const char* WINAPI LocalTimeFormat(datetime timestamp, const char* format);
string      WINAPI localTimeFormat(datetime timestamp, const char* format);


//
// printf() format codes:
//
// @see  http://www.cplusplus.com/reference/cstdio/printf/
// @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm

const char* WINAPI NumberFormat(double value, const char* format);
string      WINAPI numberFormat(double value, const char* format);
