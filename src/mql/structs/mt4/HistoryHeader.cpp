/**
 * MT4 struct HISTORY_HEADER (Header der Kursreihen im "history"-Verzeichnis)
 *
 * HistoryFile Header
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"
#include "mql/structs/mt4/HistoryHeader.h"


/**
 * Gibt das Barformat eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return uint - Barformat
 */
uint WINAPI hh_BarFormat(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->barFormat);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return char* - Beschreibung
 */
const char* WINAPI hh_Description(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->description);
   #pragma EXPORT
}


/**
 * Gibt das Symbol eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return char* - Symbol
 */
const char* WINAPI hh_Symbol(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->symbol);
   #pragma EXPORT
}


/**
 * Gibt die Periode eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return uint - Periode
 */
uint WINAPI hh_Period(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->period);
   #pragma EXPORT
}


/**
 * Gibt die Digits eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return uint - Digits
 */
uint WINAPI hh_Digits(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->digits);
   #pragma EXPORT
}


/**
 * Gibt den SyncMarker eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return int - SyncMarker (timestamp)
 */
int WINAPI hh_SyncMarker(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->syncMarker);
   #pragma EXPORT
}


/**
 * Gibt die LastSyncTime eines HISTORY_HEADERs zurück.
 *
 * @param  HISTORY_HEADER* hh
 *
 * @return int - LastSyncTime (timestamp)
 */
int WINAPI hh_LastSyncTime(const HISTORY_HEADER* hh) {
   if ((uint)hh < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hh = 0x%p (not a valid pointer)", hh));
   return(hh->lastSyncTime);
   #pragma EXPORT
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
   if ((uint)hhs < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].barFormat);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return char* - Beschreibung
 */
const char* WINAPI hhs_Description(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].description);
   #pragma EXPORT
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
   if ((uint)hhs < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return((char*)debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].symbol);
   #pragma EXPORT
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
   if ((uint)hhs < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].period);
   #pragma EXPORT
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
   if ((uint)hhs < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].digits);
   #pragma EXPORT
}


/**
 * Gibt den SyncMarker eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return int - SyncMarker (timestamp)
 */
int WINAPI hhs_SyncMarker(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].syncMarker);
   #pragma EXPORT
}


/**
 * Gibt die LastSyncTime eines HISTORY_HEADERs in einem Array zurück.
 *
 * @param  HISTORY_HEADER hhs[] - Array
 * @param  int    index         - Array-Index
 *
 * @return int - LastSyncTime (timestamp)
 */
int WINAPI hhs_LastSyncTime(const HISTORY_HEADER hhs[], int index) {
   if ((uint)hhs < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(debug("ERROR:  invalid parameter index = %d (not a valid index)", index));
   return(hhs[index].lastSyncTime);
   #pragma EXPORT
}


/**
 * Setzt das Barformat eines HISTORY_HEADERs.
 *
 * @param  HISTORY_HEADER* hh
 * @param  int             format - bis Build 509: 400, danach: 401
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI hh_SetBarFormat(HISTORY_HEADER* hh, int format) {
   if ((uint)hh < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hh = 0x%p (not a valid pointer)", hh));
   if (format!=400 && format!=401)   return(debug("ERROR:  invalid parameter format = %d (must be 400 or 401)", format));
   hh->barFormat = format;
   return(TRUE);
   #pragma EXPORT
}


// --------------------------------------------------------------------------------------------------------------------------------


/**
 * Setzt das Barformat eines HISTORY_HEADERs in einem Array.
 *
 * @param  HISTORY_HEADER hhs[]  - Array
 * @param  int            index  - Array-Index
 * @param  int            format - bis Build 509: 400, danach: 401
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI hhs_SetBarFormat(HISTORY_HEADER hhs[], int index, int format) {
   if ((uint)hhs < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter hhs = 0x%p (not a valid pointer)", hhs));
   if (index     < 0)                 return(debug("ERROR:  invalid parameter index = %d", index));
   return(hh_SetBarFormat(&hhs[index], format));
   #pragma EXPORT
}
