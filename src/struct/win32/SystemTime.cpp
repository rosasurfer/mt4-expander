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
#include "expander.h"


/**
 * Gibt das Jahr einer SYSTEMTIME zur�ck.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Year(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)", st));
   return(st->wYear);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Monat einer SYSTEMTIME zur�ck.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Month(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)", st));
   return(st->wMonth);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Wochentag einer SYSTEMTIME zur�ck.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_DayOfWeek(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)", st));
   return(st->wDayOfWeek);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Tag einer SYSTEMTIME zur�ck.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Day(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)", st));
   return(st->wDay);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Stunden einer SYSTEMTIME zur�ck.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Hour(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)", st));
   return(st->wHour);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Minuten einer SYSTEMTIME zur�ck.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Minute(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)", st));
   return(st->wMinute);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Sekunden einer SYSTEMTIME zur�ck.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Second(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)", st));
   return(st->wSecond);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Millisekunden einer SYSTEMTIME zur�ck.
 *
 * @param  SYSTEMTIME* st
 *
 * @return WORD
 */
WORD WINAPI st_Milliseconds(const SYSTEMTIME* st) {
   if ((uint)st < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)", st));
   return(st->wMilliseconds);
   #pragma EXPANDER_EXPORT
}
