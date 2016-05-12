#pragma once

/**
 * MT4 struct HISTORY_HEADER (Header der Kursreihen im "history"-Verzeichnis)
 *
 * HistoryFile Header
 */
struct HISTORY_HEADER {                            // -- offset ---- size --- description ----------------------------------------------------------------------------
   uint format;                                    //         0         4     Barformat, bis Build 509: 400, danach: 401
   char description[64];                           //         4        64     Beschreibung (szchar)
   char symbol[MAX_SYMBOL_LENGTH+1];               //        68        12     Symbol       (szchar)
   uint period;                                    //        80         4     Timeframe in Minuten
   uint digits;                                    //        84         4     Digits
   uint syncMarker;                                //        88         4     SyncMarker   (timestamp), wird vom Terminal beim Refresh mit Serverversion überschrieben
   uint lastSyncTime;                              //        92         4     LastSyncTime (unbenutzt), wird vom Terminal nicht überschrieben
   uint timezoneId;              // custom: pewa   //        96         4     Timezone-ID  (default: 0 => Server-Timezone)
   BYTE reserved[48];                              //       100        48
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //               = 148