#pragma once
#include "expander.h"


#pragma pack(push, 1)
/**
 * MT4 struct HISTORY_HEADER (header format of history files in "<terminal-data-dir>/history/")
 *
 * History file header
 */
struct HISTORY_HEADER {                            // -- offset ---- size --- description ----------------------------------------------------------------------------
   uint     barFormat;                             //         0         4     bar format id, builds <= 509: 400, builds > 509: 401
   char     description[64];                       //         4        64     symbol description (overwritten by the terminal)
   char     symbol[MAX_SYMBOL_LENGTH+1];           //        68        12     symbol
   uint     period;                                //        80         4     timeframe in minutes
   uint     digits;                                //        84         4     digits
   datetime syncMarker;                            //        88         4     data server synchronization marker, updated by the terminal on Chart->Refresh
   datetime lastSyncTime;                          //        92         4     unused (not overwritten by the terminal)
   BYTE     reserved[52];                          //        96        52
};                                                 // ----------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //               = 148


// getters
uint        WINAPI hh_BarFormat    (const HISTORY_HEADER* hh);
const char* WINAPI hh_Description  (const HISTORY_HEADER* hh);
const char* WINAPI hh_Symbol       (const HISTORY_HEADER* hh);
uint        WINAPI hh_Period       (const HISTORY_HEADER* hh);
uint        WINAPI hh_Timeframe    (const HISTORY_HEADER* hh);
uint        WINAPI hh_Digits       (const HISTORY_HEADER* hh);
datetime    WINAPI hh_SyncMarker   (const HISTORY_HEADER* hh);
datetime    WINAPI hh_LastSyncTime (const HISTORY_HEADER* hh);

uint        WINAPI hhs_BarFormat   (const HISTORY_HEADER hhs[], int index);
const char* WINAPI hhs_Description (const HISTORY_HEADER hhs[], int index);
const char* WINAPI hhs_Symbol      (const HISTORY_HEADER hhs[], int index);
uint        WINAPI hhs_Period      (const HISTORY_HEADER hhs[], int index);
uint        WINAPI hhs_Timeframe   (const HISTORY_HEADER hhs[], int index);
uint        WINAPI hhs_Digits      (const HISTORY_HEADER hhs[], int index);
datetime    WINAPI hhs_SyncMarker  (const HISTORY_HEADER hhs[], int index);
datetime    WINAPI hhs_LastSyncTime(const HISTORY_HEADER hhs[], int index);

// setters
uint        WINAPI hh_SetBarFormat    (HISTORY_HEADER* hh, int         format     );
const char* WINAPI hh_SetDescription  (HISTORY_HEADER* hh, const char* description);
const char* WINAPI hh_SetSymbol       (HISTORY_HEADER* hh, const char* symbol     );
uint        WINAPI hh_SetPeriod       (HISTORY_HEADER* hh, int         period     );
uint        WINAPI hh_SetTimeframe    (HISTORY_HEADER* hh, int         timeframe  );
uint        WINAPI hh_SetDigits       (HISTORY_HEADER* hh, int         digits     );
datetime    WINAPI hh_SetSyncMarker   (HISTORY_HEADER* hh, datetime    timestamp  );
datetime    WINAPI hh_SetLastSyncTime (HISTORY_HEADER* hh, datetime    timestamp  );

uint        WINAPI hhs_SetBarFormat   (HISTORY_HEADER hhs[], int index, int         format     );
const char* WINAPI hhs_SetDescription (HISTORY_HEADER hhs[], int index, const char* description);
const char* WINAPI hhs_SetSymbol      (HISTORY_HEADER hhs[], int index, const char* symbol     );
uint        WINAPI hhs_SetPeriod      (HISTORY_HEADER hhs[], int index, int         period     );
uint        WINAPI hhs_SetTimeframe   (HISTORY_HEADER hhs[], int index, int         timeframe  );
uint        WINAPI hhs_SetDigits      (HISTORY_HEADER hhs[], int index, int         digits     );
datetime    WINAPI hhs_SetSyncMarker  (HISTORY_HEADER hhs[], int index, datetime    timestamp  );
datetime    WINAPI hhs_SetLastSyncTime(HISTORY_HEADER hhs[], int index, datetime    timestamp  );
