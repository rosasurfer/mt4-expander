#pragma once

/**
 * MT4 struct HISTORY_HEADER (Header der Kursreihen im "history"-Verzeichnis)
 *
 * HistoryFile Header
 */
struct HISTORY_HEADER {                            // -- offset ---- size --- description ----------------------------------------------------------------------------
   uint barFormat;                                 //         0         4     Barformat, bis Build 509: 400, danach: 401
   char description[64];                           //         4        64     Beschreibung (szchar), wird vom Terminal bei Write mit Copyright überschrieben
   char symbol[MAX_SYMBOL_LENGTH+1];               //        68        12     Symbol       (szchar)
   uint period;                                    //        80         4     Timeframe in Minuten
   uint digits;                                    //        84         4     Digits
   int  syncMarker;                                //        88         4     SyncMarker   (timestamp), wird vom Terminal beim Refresh mit Serverversion überschrieben
   int  lastSyncTime;                              //        92         4     LastSyncTime (unbenutzt), wird vom Terminal nicht überschrieben
   BYTE reserved[52];                              //        96        52
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //               = 148

// Getter
uint        WINAPI hh_BarFormat    (const HISTORY_HEADER* hh);
const char* WINAPI hh_Description  (const HISTORY_HEADER* hh);
const char* WINAPI hh_Symbol       (const HISTORY_HEADER* hh);
uint        WINAPI hh_Period       (const HISTORY_HEADER* hh);
uint        WINAPI hh_Timeframe    (const HISTORY_HEADER* hh);
uint        WINAPI hh_Digits       (const HISTORY_HEADER* hh);
int         WINAPI hh_SyncMarker   (const HISTORY_HEADER* hh);
int         WINAPI hh_LastSyncTime (const HISTORY_HEADER* hh);

uint        WINAPI hhs_BarFormat   (const HISTORY_HEADER hhs[], int index);
const char* WINAPI hhs_Description (const HISTORY_HEADER hhs[], int index);
const char* WINAPI hhs_Symbol      (const HISTORY_HEADER hhs[], int index);
uint        WINAPI hhs_Period      (const HISTORY_HEADER hhs[], int index);
uint        WINAPI hhs_Timeframe   (const HISTORY_HEADER hhs[], int index);
uint        WINAPI hhs_Digits      (const HISTORY_HEADER hhs[], int index);
int         WINAPI hhs_SyncMarker  (const HISTORY_HEADER hhs[], int index);
int         WINAPI hhs_LastSyncTime(const HISTORY_HEADER hhs[], int index);

// Setter
BOOL        WINAPI hh_SetBarFormat    (HISTORY_HEADER* hh, int         format     );
BOOL        WINAPI hh_SetDescription  (HISTORY_HEADER* hh, const char* description);
BOOL        WINAPI hh_SetSymbol       (HISTORY_HEADER* hh, const char* symbol     );
BOOL        WINAPI hh_SetPeriod       (HISTORY_HEADER* hh, int         period     );
BOOL        WINAPI hh_SetTimeframe    (HISTORY_HEADER* hh, int         timeframe  );
BOOL        WINAPI hh_SetDigits       (HISTORY_HEADER* hh, int         digits     );
BOOL        WINAPI hh_SetSyncMarker   (HISTORY_HEADER* hh, int         timestamp  );
BOOL        WINAPI hh_SetLastSyncTime (HISTORY_HEADER* hh, int         timestamp  );

BOOL        WINAPI hhs_SetBarFormat   (HISTORY_HEADER hhs[], int index, int         format     );
BOOL        WINAPI hhs_SetDescription (HISTORY_HEADER hhs[], int index, const char* description);
BOOL        WINAPI hhs_SetSymbol      (HISTORY_HEADER hhs[], int index, const char* symbol     );
BOOL        WINAPI hhs_SetPeriod      (HISTORY_HEADER hhs[], int index, int         period     );
BOOL        WINAPI hhs_SetTimeframe   (HISTORY_HEADER hhs[], int index, int         timeframe  );
BOOL        WINAPI hhs_SetDigits      (HISTORY_HEADER hhs[], int index, int         digits     );
BOOL        WINAPI hhs_SetSyncMarker  (HISTORY_HEADER hhs[], int index, int         timestamp  );
BOOL        WINAPI hhs_SetLastSyncTime(HISTORY_HEADER hhs[], int index, int         timestamp  );
