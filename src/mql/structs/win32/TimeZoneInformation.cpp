/**
 * Win32 structure SYSTEMTIME
 *
 *
 * struct TIME_ZONE_INFORMATION {
 *    LONG       Bias;                 //   4         in Minuten
 *    WCHAR      StandardName[32];     //  64         z.B. "G…T…B… …N…o…r…m…a…l…z…e…i…t", <NUL>-terminiert
 *    SYSTEMTIME StandardDate;         //  16
 *    LONG       StandardBias;         //   4
 *    WCHAR      DaylightName[32];     //  64         z.B. "G…T…B… …S…o…m…m…e…r…z…e…i…t", <NUL>-terminiert
 *    SYSTEMTIME DaylightDate;         //  16
 *    LONG       DaylightBias;         //   4
 * } tzi;                              // 172 byte
 *
 *
 * Es gelten folgende Formeln:
 * ---------------------------
 *  Bias             = -Offset
 *  LocalTime + Bias = GMT
 *  GMT + Offset     = LocalTime
 *
 *
 * MQL-Importdeklarationen:
 * ------------------------
 * int    tzi_Bias        (/*TIME_ZONE_INFORMATION/int tzi[]);
 * string tzi_StandardName(/*TIME_ZONE_INFORMATION/int tzi[]);
 * void   tzi_StandardDate(/*TIME_ZONE_INFORMATION/int tzi[], /*SYSTEMTIME/int st[]);
 * int    tzi_StandardBias(/*TIME_ZONE_INFORMATION/int tzi[]);
 * string tzi_DaylightName(/*TIME_ZONE_INFORMATION/int tzi[]);
 * void   tzi_DaylightDate(/*TIME_ZONE_INFORMATION/int tzi[], /*SYSTEMTIME/int st[]);
 * int    tzi_DaylightBias(/*TIME_ZONE_INFORMATION/int tzi[]);
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt den aktuellen Bias einer TIME_ZONE_INFORMATION zurück.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return LONG - Bias in Minuten
 */
LONG WINAPI tzi_Bias(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return(debug("invalid parameter tzi = 0x%p (not a valid pointer)", tzi));
   return(tzi->Bias);
   #pragma EXPORT
}


/**
 * Gibt den Standard-Namen einer TIME_ZONE_INFORMATION zurück.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return char* - Name als C-String
 */
char* WINAPI tzi_StandardName(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return((char*)debug("invalid parameter tzi = 0x%p (not a valid pointer)", tzi));

   char* mbstr = NULL;
   int bytes = wcstombs(mbstr, tzi->StandardName, 0);
   if (bytes == -1) return((char*)debug("cannot convert WCHAR tzi->StandardName to CHAR string"));

   mbstr = new char[bytes+1];                               // TODO: Speicherleck schließen
   wcstombs(mbstr, tzi->StandardName, bytes+1);             // +1, damit wcstombs() auch das terminierende <NUL> schreibt

   return(mbstr);
   #pragma EXPORT
}


/**
 * Kopiert das Standard-Date einer TIME_ZONE_INFORMATION in die übergebene SYSTEMTIME.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 * @param  SYSTEMTIME*            st
 */
void WINAPI tzi_StandardDate(const TIME_ZONE_INFORMATION* tzi, SYSTEMTIME* st) {
   if ((uint)tzi < MIN_VALID_POINTER) { debug("invalid parameter tzi = 0x%p (not a valid pointer)", tzi); return; }
   if ((uint)st  < MIN_VALID_POINTER) { debug("invalid parameter st = 0x%p (not a valid pointer)" , st ); return; }

   *st = tzi->StandardDate;
   #pragma EXPORT
}


/**
 * Gibt den Standard-Bias einer TIME_ZONE_INFORMATION zurück.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return LONG - Bias in Minuten
 */
LONG WINAPI tzi_StandardBias(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return(debug("invalid parameter tzi = 0x%p (not a valid pointer)", tzi));
   return(tzi->StandardBias);
   #pragma EXPORT
}


/**
 * Gibt den Daylight-Namen einer TIME_ZONE_INFORMATION zurück.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return char* - Name als C-String
 */
char* WINAPI tzi_DaylightName(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return((char*)debug("invalid parameter tzi = 0x%p (not a valid pointer)", tzi));

   char* mbstr = NULL;
   int bytes = wcstombs(mbstr, tzi->DaylightName, 0);
   if (bytes == -1) return((char*)debug("cannot convert WCHAR tzi->StandardName to CHAR string"));

   mbstr = new char[bytes+1];                               // TODO: Speicherleck schließen
   wcstombs(mbstr, tzi->DaylightName, bytes+1);             // +1, damit wcstombs() auch das terminierende <NUL> schreibt

   return(mbstr);
   #pragma EXPORT
}


/**
 * Kopiert das Daylight-Date einer TIME_ZONE_INFORMATION in die übergebene SYSTEMTIME.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 * @param  SYSTEMTIME*            st
 */
void WINAPI tzi_DaylightDate(const TIME_ZONE_INFORMATION* tzi, SYSTEMTIME* st) {
   if ((uint)tzi < MIN_VALID_POINTER) { debug("invalid parameter tzi = 0x%p (not a valid pointer)", tzi); return; }
   if ((uint)st  < MIN_VALID_POINTER) { debug("invalid parameter st = 0x%p (not a valid pointer)" , st ); return; }

   *st = tzi->DaylightDate;
   #pragma EXPORT
}


/**
 * Gibt den Daylight-Bias einer TIME_ZONE_INFORMATION zurück.
 *
 * @param  TIME_ZONE_INFORMATION* tzi
 *
 * @return LONG - Bias in Minuten
 */
LONG WINAPI tzi_DaylightBias(const TIME_ZONE_INFORMATION* tzi) {
   if ((uint)tzi < MIN_VALID_POINTER) return(debug("invalid parameter tzi = 0x%p (not a valid pointer)", tzi));
   return(tzi->DaylightBias);
   #pragma EXPORT
}
