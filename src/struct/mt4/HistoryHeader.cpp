/**
 * MT4 struct HISTORY_HEADER (Header der Kursreihen im "history"-Verzeichnis)
 *
 * HistoryFile Header
 */
#include "expander.h"
#include "struct/mt4/HistoryHeader.h"


/**
 * Gibt das Barformat eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return uint - Barformat
 */
uint WINAPI hh_BarFormat(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->barFormat);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das Copyright eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return char* - Copyright
 */
const char* WINAPI hh_Copyright(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->copyright);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das Symbol eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return char* - Symbol
 */
const char* WINAPI hh_Symbol(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->symbol);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Periode eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return uint - Periode
 */
uint WINAPI hh_Period(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->period);
   #pragma EXPANDER_EXPORT
}


/**
 * Alias
 */
uint WINAPI hh_Timeframe(const HISTORY_HEADER* hh) {
   return(hh_Period(hh));
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Digits eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return uint - Digits
 */
uint WINAPI hh_Digits(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->digits);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den SyncMarker eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return datetime - SyncMarker
 */
datetime WINAPI hh_SyncMarker(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->syncMarker);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die LastSyncTime eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return datetime - LastSyncTime
 */
datetime WINAPI hh_LastSyncTime(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->lastSyncTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das Barformat eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return uint - Barformat
 */
uint WINAPI hhs_BarFormat(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].barFormat);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das Copyright eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int            index - Array-Index
 *
 * @return char* - Copyright
 */
const char* WINAPI hhs_Copyright(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].copyright);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das Symbol eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return char* - Symbol
 */
const char* WINAPI hhs_Symbol(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].symbol);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Periode eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return uint - Periode
 */
uint WINAPI hhs_Period(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].period);
   #pragma EXPANDER_EXPORT
}


/**
 * Alias
 */
uint WINAPI hhs_Timeframe(const HISTORY_HEADER hhs[], int index) {
   return(hhs_Period(hhs, index));
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Digits eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return uint - Digits
 */
uint WINAPI hhs_Digits(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].digits);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den SyncMarker eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return datetime - SyncMarker
 */
datetime WINAPI hhs_SyncMarker(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].syncMarker);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die LastSyncTime eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return datetime - LastSyncTime
 */
datetime WINAPI hhs_LastSyncTime(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].lastSyncTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt das Barformat eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  int             format - bis Build 509: 400, danach: 401
 *
 * @return uint - dasselbe Barformat
 */
uint WINAPI hh_SetBarFormat(HISTORY_HEADER* hh, int format) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   if (format!=400 && format!=401)   return(error(ERR_INVALID_PARAMETER, "invalid parameter format = %d (must be 400 or 401)", format));
   return(hh->barFormat = format);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt das Copyright eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  char*           copyright - ein vorhandenes Copyright kann mit einem Leerstring gelöscht werden
 *
 * @return char* - dasselbe Copyright
 */
const char* WINAPI hh_SetCopyright(HISTORY_HEADER* hh, const char* copyright) {
   if ((uint)hh        < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   if ((uint)copyright < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter copyright = 0x%p (not a valid pointer)", copyright));
   if (strlen(copyright) > sizeof(hh->copyright)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter copyright = \"%s\" (max %d characters)", copyright, sizeof(hh->copyright)-1));

   if (!strcpy(hh->copyright, copyright))
      return(NULL);
   return(copyright);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt das Symbol eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  char*           symbol
 *
 * @return char* - dasselbe Symbol
 */
const char* WINAPI hh_SetSymbol(HISTORY_HEADER* hh, const char* symbol) {
   if ((uint)hh     < MIN_VALID_POINTER)   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   if ((uint)symbol < MIN_VALID_POINTER)   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   int len = strlen(symbol);
   if (!len || len > sizeof(hh->symbol)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol = \"%s\" (must be 1 to %d characters)", symbol, sizeof(hh->symbol)-1));

   if (!strcpy(hh->symbol, symbol))
      return(NULL);
   return(symbol);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Periode eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  int             period
 *
 * @return uint - dieselbe Periode
 */
uint WINAPI hh_SetPeriod(HISTORY_HEADER* hh, int period) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   if (period <= 0)                  return(error(ERR_INVALID_PARAMETER, "invalid parameter period = %d (must be greater than zero)", period));
   return(hh->period = period);
   #pragma EXPANDER_EXPORT
}


/**
 * Alias
 */
uint WINAPI hh_SetTimeframe(HISTORY_HEADER* hh, int timeframe) {
   return(hh_SetPeriod(hh, timeframe));
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Digits eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  int             digits
 *
 * @return uint - dieselben Digits
 */
uint WINAPI hh_SetDigits(HISTORY_HEADER* hh, int digits) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   if (digits < 0)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter digits = %d", digits));
   return(hh->digits = digits);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt den SyncMarker eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  int             timestamp
 *
 * @return int - derselbe Zeitpunkt
 */
int WINAPI hh_SetSyncMarker(HISTORY_HEADER* hh, int timestamp) {
   if ((uint)hh  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   if (timestamp < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp = %d (must be non-negative)", timestamp));
   return(hh->syncMarker = timestamp);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die LastSyncTime eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  int             timestamp
 *
 * @return int - derselbe Zeitpunkt
 */
int WINAPI hh_SetLastSyncTime(HISTORY_HEADER* hh, int timestamp) {
   if ((uint)hh  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh = 0x%p (not a valid pointer)", hh));
   if (timestamp < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp = %d (must be non-negative)", timestamp));
   return(hh->lastSyncTime = timestamp);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt das Barformat eines HISTORY_HEADERs in einem Array.
 *
 * @param  HISTORY_HEADER hhs[]  - Array
 * @param  int            index  - Array-Index
 * @param  int            format - bis Build 509: 400, danach: 401
 *
 * @return uint - dasselbe Barformat
 */
uint WINAPI hhs_SetBarFormat(HISTORY_HEADER hhs[], int index, int format) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d", index));
   return(hh_SetBarFormat(&hhs[index], format));
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt das Copyright eines HISTORY_HEADERs innerhalb eines Arrays.
 *
 * @param  HISTORY_HEADER hhs[]     - Array
 * @param  int            index     - Array-Index
 * @param  char*          copyright - Copyright
 *
 * @return char* - das gesetzte Copyright
 */
const char* WINAPI hhs_SetCopyright(HISTORY_HEADER hhs[], int index, const char* copyright) {
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hh_SetCopyright(&hhs[index], copyright));
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt das Symbol eines HISTORY_HEADERs innerhalb eines Arrays.
 *
 * @param  HISTORY_HEADER hhs[]  - Array
 * @param  int            index  - Array-Index
 * @param  char*          symbol - Symbol
 *
 * @return char* - das gesetzte Symbol
 */
const char* WINAPI hhs_SetSymbol(HISTORY_HEADER hhs[], int index, const char* symbol) {
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(hh_SetSymbol(&hhs[index], symbol));
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Periode eines HISTORY_HEADERs in einem Array.
 *
 * @param  HISTORY_HEADER hhs[]  - Array
 * @param  int            index  - Array-Index
 * @param  int            period
 *
 * @return uint - dieselbe Periode
 */
uint WINAPI hhs_SetPeriod(HISTORY_HEADER hhs[], int index, int period) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d", index));
   return(hh_SetPeriod(&hhs[index], period));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias
 */
uint WINAPI hhs_SetTimeframe(HISTORY_HEADER hhs[], int index, int timeframe) {
   return(hhs_SetPeriod(hhs, index, timeframe));
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Digits eines HISTORY_HEADERs in einem Array.
 *
 * @param  HISTORY_HEADER hhs[]  - Array
 * @param  int            index  - Array-Index
 * @param  int            digits - Digits
 *
 * @return uint - dieselben Digits
 */
uint WINAPI hhs_SetDigits(HISTORY_HEADER hhs[], int index, int digits) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d", index));
   return(hh_SetDigits(&hhs[index], digits));
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt den SyncMarker eines HISTORY_HEADERs in einem Array.
 *
 * @param  HISTORY_HEADER hhs[]     - Array
 * @param  int            index     - Array-Index
 * @param  int            timestamp - Zeitpunkt
 *
 * @return int - derselbe Zeitpunkt
 */
int WINAPI hhs_SetSyncMarker(HISTORY_HEADER hhs[], int index, int timestamp) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d", index));
   return(hh_SetSyncMarker(&hhs[index], timestamp));
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die LastSyncTime eines HISTORY_HEADERs in einem Array.
 *
 * @param  HISTORY_HEADER hhs[]     - Array
 * @param  int            index     - Array-Index
 * @param  int            timestamp - Zeitpunkt
 *
 * @return int - derselbe Zeitpunkt
 */
int WINAPI hhs_SetLastSyncTime(HISTORY_HEADER hhs[], int index, int timestamp) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d", index));
   return(hh_SetLastSyncTime(&hhs[index], timestamp));
   #pragma EXPANDER_EXPORT
}
