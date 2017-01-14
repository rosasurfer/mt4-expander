#pragma once

#include "stdafx.h"


/**
 * Win32 struct SYSTEMTIME
 *
 * struct SYSTEMTIME {
 *    WORD wYear;                   //  2
 *    WORD wMonth;                  //  2
 *    WORD wDayOfWeek;              //  2
 *    WORD wDay;                    //  2
 *    WORD wHour;                   //  2
 *    WORD wMinute;                 //  2
 *    WORD wSecond;                 //  2
 *    WORD wMilliseconds;           //  2
 * };                               // 16 byte
 */
WORD WINAPI st_Year        (const SYSTEMTIME* st);
WORD WINAPI st_Month       (const SYSTEMTIME* st);
WORD WINAPI st_DayOfWeek   (const SYSTEMTIME* st);
WORD WINAPI st_Day         (const SYSTEMTIME* st);
WORD WINAPI st_Hour        (const SYSTEMTIME* st);
WORD WINAPI st_Minute      (const SYSTEMTIME* st);
WORD WINAPI st_Second      (const SYSTEMTIME* st);
WORD WINAPI st_Milliseconds(const SYSTEMTIME* st);
