#pragma once
/**
 * MQL-Interface zum Zugriff auf ein struct TIME_ZONE_INFORMATION.
 *
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
LONG        WINAPI tzi_Bias        (const TIME_ZONE_INFORMATION* tzi                );
const char* WINAPI tzi_StandardName(const TIME_ZONE_INFORMATION* tzi                );
void        WINAPI tzi_StandardDate(const TIME_ZONE_INFORMATION* tzi, SYSTEMTIME* st);
LONG        WINAPI tzi_StandardBias(const TIME_ZONE_INFORMATION* tzi                );
const char* WINAPI tzi_DaylightName(const TIME_ZONE_INFORMATION* tzi                );
void        WINAPI tzi_DaylightDate(const TIME_ZONE_INFORMATION* tzi, SYSTEMTIME* st);
LONG        WINAPI tzi_DaylightBias(const TIME_ZONE_INFORMATION* tzi                );
