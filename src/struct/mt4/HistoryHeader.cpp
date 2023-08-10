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
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   return(hh->barFormat);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Beschreibung eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return char* - Beschreibung
 */
const char* WINAPI hh_Description(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   return(hh->description);
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
   if ((uint)hh < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
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
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
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
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   return(hh->digits);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den SyncMarker eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return time32 - SyncMarker
 */
time32 WINAPI hh_SyncMarker(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   return(hh->syncMarker);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die LastSyncTime eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return time32 - LastSyncTime
 */
time32 WINAPI hh_LastSyncTime(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
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
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(hhs[index].barFormat);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Beschreibung eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int            index - Array-Index
 *
 * @return char* - Beschreibung
 */
const char* WINAPI hhs_Description(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(hhs[index].description);
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
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
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
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
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
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(hhs[index].digits);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den SyncMarker eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return time32 - SyncMarker
 */
time32 WINAPI hhs_SyncMarker(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(hhs[index].syncMarker);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die LastSyncTime eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return time32 - LastSyncTime
 */
time32 WINAPI hhs_LastSyncTime(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
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
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   if (format!=400 && format!=401)   return(error(ERR_INVALID_PARAMETER, "invalid parameter format: %d (must be 400 or 401)", format));
   return(hh->barFormat = format);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Beschreibung eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  char*           description - eine vorhandene Beschreibung kann mit einem Leerstring gelöscht werden
 *
 * @return char* - dieselbe Beschreibung
 */
const char* WINAPI hh_SetDescription(HISTORY_HEADER* hh, const char* description) {
   if ((uint)hh          < MIN_VALID_POINTER)           return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   if ((uint)description < MIN_VALID_POINTER)           return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter description: 0x%p (not a valid pointer)", description));
   if (strlen(description) > sizeof(hh->description)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter description: \"%s\" (max. %d characters)", description, sizeof(hh->description)-1));

   if (!strcpy(hh->description, description))
      return(NULL);
   return(description);
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
   if ((uint)hh     < MIN_VALID_POINTER)   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   if ((uint)symbol < MIN_VALID_POINTER)   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   int len = strlen(symbol);
   if (!len || len > sizeof(hh->symbol)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol: \"%s\" (must be 1 to %d characters)", symbol, sizeof(hh->symbol)-1));

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
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   if (period <= 0)                  return(error(ERR_INVALID_PARAMETER, "invalid parameter period: %d (must be greater than zero)", period));
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
   if ((uint)hh < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   if (digits < 0)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d", digits));
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
   if ((uint)hh  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   if (timestamp == NaT)              return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: Not-a-Time"));
   if (timestamp < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp));
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
   if ((uint)hh  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hh: 0x%p (not a valid pointer)", hh));
   if (timestamp == NaT)              return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: Not-a-Time"));
   if (timestamp < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter timestamp: %d (must be non-negative)", timestamp));
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
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d", index));
   return(hh_SetBarFormat(&hhs[index], format));
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Beschreibung eines HISTORY_HEADERs innerhalb eines Arrays.
 *
 * @param  HISTORY_HEADER hhs[]       - Array
 * @param  int            index       - Array-Index
 * @param  char*          description - Beschreibung
 *
 * @return char* - die gesetzte Beschreibung
 */
const char* WINAPI hhs_SetDescription(HISTORY_HEADER hhs[], int index, const char* description) {
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
   return(hh_SetDescription(&hhs[index], description));
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
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (not a valid index)", index));
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
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d", index));
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
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d", index));
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
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d", index));
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
   if ((uint)hhs < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hhs: 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d", index));
   return(hh_SetLastSyncTime(&hhs[index], timestamp));
   #pragma EXPANDER_EXPORT
}
