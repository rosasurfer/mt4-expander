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
#include "stdafx.h"
#include "common.h"
#include "Expander.h"
#include "mql/structs/win32/SystemTime.h"


/**
 * Gibt das Jahr einer SYSTEMTIME zurück.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Year(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter st = 0x%p (not a valid pointer)", st));
   return(st->wYear);
   #pragma EXPORT
}


/**
 * Gibt den Monat einer SYSTEMTIME zurück.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Month(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter st = 0x%p (not a valid pointer)", st));
   return(st->wMonth);
   #pragma EXPORT
}


/**
 * Gibt den Wochentag einer SYSTEMTIME zurück.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_DayOfWeek(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter st = 0x%p (not a valid pointer)", st));
   return(st->wDayOfWeek);
   #pragma EXPORT
}


/**
 * Gibt den Tag einer SYSTEMTIME zurück.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Day(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter st = 0x%p (not a valid pointer)", st));
   return(st->wDay);
   #pragma EXPORT
}


/**
 * Gibt die Stunden einer SYSTEMTIME zurück.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Hour(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter st = 0x%p (not a valid pointer)", st));
   return(st->wHour);
   #pragma EXPORT
}


/**
 * Gibt die Minuten einer SYSTEMTIME zurück.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Minute(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter st = 0x%p (not a valid pointer)", st));
   return(st->wMinute);
   #pragma EXPORT
}


/**
 * Gibt die Sekunden einer SYSTEMTIME zurück.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Second(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter st = 0x%p (not a valid pointer)", st));
   return(st->wSecond);
   #pragma EXPORT
}


/**
 * Gibt die Millisekunden einer SYSTEMTIME zurück.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Milliseconds(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter st = 0x%p (not a valid pointer)", st));
   return(st->wMilliseconds);
   #pragma EXPORT
}
