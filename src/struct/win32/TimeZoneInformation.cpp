/**
 * Win32 struct TIME_ZONE_INFORMATION
 *
 * struct TIME_ZONE_INFORMATION {
 *    LONG       Bias;                 //   4         in Minuten
 *    WCHAR      StandardName[32];     //  64         z.B. "G…T…B… …N…o…r…m…a…l…z…e…i…t", <NUL><NUL>-terminiert
 *    SYSTEMTIME StandardDate;         //  16
 *    LONG       StandardBias;         //   4         in Minuten
 *    WCHAR      DaylightName[32];     //  64         z.B. "G…T…B… …S…o…m…m…e…r…z…e…i…t", <NUL><NUL>-terminiert
 *    SYSTEMTIME DaylightDate;         //  16
 *    LONG       DaylightBias;         //   4         in Minuten
 * };                                  // 172 byte
 *
 *
 * Es gelten folgende Formeln:
 * ---------------------------
 *  Bias             = -Offset
 *  LocalTime + Bias = GMT
 *  GMT + Offset     = LocalTime
 */
#include "expander.h"
#include "lib/string.h"


/**
 * Gibt den aktuellen Bias einer TIME_ZONE_INFORMATION zurück.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return LONG - Bias in Minuten
 */
LONG WINAPI tzi_Bias(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter tzi: 0x%p (not a valid pointer)", tzi));
   return(tzi->Bias);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the standard name of a timezone.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return char* - standard name
 */
const char* WINAPI tzi_StandardName(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter tzi: 0x%p (not a valid pointer)", tzi));

   wstring name = wstring(tzi->StandardName, sizeof(tzi->StandardName));
   return(strdup(unicodeToAnsi(name).c_str()));                            // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}


/**
 * Kopiert das Standard-Date einer TIME_ZONE_INFORMATION in die übergebene SYSTEMTIME.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 * @param  SYSTEMTIME*            st
 */
void WINAPI tzi_StandardDate(const TIME_ZONE_INFORMATION* tzi, SYSTEMTIME* st) {
   if ((uint)tzi < MIN_VALID_POINTER) { error(ERR_INVALID_PARAMETER, "invalid parameter tzi: 0x%p (not a valid pointer)", tzi); return; }
   if ((uint)st  < MIN_VALID_POINTER) { error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)" , st ); return; }

   *st = tzi->StandardDate;
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Standard-Bias einer TIME_ZONE_INFORMATION zurück.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return LONG - Bias in Minuten
 */
LONG WINAPI tzi_StandardBias(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter tzi: 0x%p (not a valid pointer)", tzi));
   return(tzi->StandardBias);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the daylight name of a timezone.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return char* - daylight name
 */
const char* WINAPI tzi_DaylightName(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter tzi: 0x%p (not a valid pointer)", tzi));

   wstring name = wstring(tzi->DaylightName, sizeof(tzi->DaylightName));
   return(strdup(unicodeToAnsi(name).c_str()));                            // TODO: close memory leak
   #pragma EXPANDER_EXPORT
}


/**
 * Kopiert das Daylight-Date einer TIME_ZONE_INFORMATION in die übergebene SYSTEMTIME.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 * @param  SYSTEMTIME*            st
 */
void WINAPI tzi_DaylightDate(const TIME_ZONE_INFORMATION* tzi, SYSTEMTIME* st) {
   if ((uint)tzi < MIN_VALID_POINTER) { error(ERR_INVALID_PARAMETER, "invalid parameter tzi: 0x%p (not a valid pointer)", tzi); return; }
   if ((uint)st  < MIN_VALID_POINTER) { error(ERR_INVALID_PARAMETER, "invalid parameter st: 0x%p (not a valid pointer)" , st ); return; }

   *st = tzi->DaylightDate;
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Daylight-Bias einer TIME_ZONE_INFORMATION zurück.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return LONG - Bias in Minuten
 */
LONG WINAPI tzi_DaylightBias(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter tzi: 0x%p (not a valid pointer)", tzi));
   return(tzi->DaylightBias);
   #pragma EXPANDER_EXPORT
}
