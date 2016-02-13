#pragma once

#include "mql4/include/shared/defines.h"
#include "mql4/include/shared/errors.h"
#include <vector>


#pragma pack(1)


// MQL-Programmtypen
enum ProgramType {
   PT_INDICATOR = PROGRAMTYPE_INDICATOR,
   PT_EXPERT    = PROGRAMTYPE_EXPERT,
   PT_SCRIPT    = PROGRAMTYPE_SCRIPT
};


// MQL-Modultypen                            // als Flag implementiert
enum ModuleType {
   MT_INDICATOR = MODULETYPE_INDICATOR,      // 1
   MT_EXPERT    = MODULETYPE_EXPERT,         // 2
   MT_SCRIPT    = MODULETYPE_SCRIPT,         // 4
   MT_LIBRARY   = MODULETYPE_LIBRARY         // 8 - Libraries sind keine eigenständigen Programme
};


// MQL-Rootfunktionen
enum RootFunction {
   RF_INIT      = ROOTFUNCTION_INIT,
   RF_START     = ROOTFUNCTION_START,
   RF_DEINIT    = ROOTFUNCTION_DEINIT
};


// Launchtypen eines MQL-Programms: via Template, via iCustom() oder von Hand
enum LaunchType {
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,
   LT_MANUAL    = LAUNCHTYPE_MANUAL
};


/**
 * MT4 structure MqlStr: interne Darstellung eines MQL-Strings (MetaQuotes-Terminologie)
 */
struct MqlStr {
   int   size;                                     // Größe des Speicherblocks oder 0, wenn der String ein intern verwaltetes C-Literal ist
   char* string;
};


/**
 * MT4 structure SYMBOL: Dateiformat "symbols.raw"
 *
 * Die Symbole in der Datei sind alphabetisch nach Symbolnamen sortiert.
 */
struct SYMBOL {                                    // -- offset ---- size --- description ----------------------------------------------------------------------------
   char   name        [MAX_SYMBOL_LENGTH+1];       //         0        12     Symbol         (szchar) z.B. "AUDCAD.mkt"
   char   description [64];                        //        64        64     Beschreibung   (szchar)
   char   stdName     [MAX_SYMBOL_LENGTH+1];       //        76        12     Standardsymbol (szchar) z.B. "AUDCAD" (falls ungleich 'name')
   char   baseCurrency[MAX_SYMBOL_LENGTH+1];       //        88        12     Base Currency
   uint   groupIndex;                              //       100         4     Index der Symbolgruppe in "symgroups.raw"
   uint   digits;                                  //       104         4     Digits

   uint   undocumented_1;                          //       108         4     ?
   uint   backgroundColor;                         //       112         4     Farbe im "MarketWatch" Window
   uint   id;                                      //       116         4     eindeutige ID des Symbols

   BYTE   undocumented_2[1520];                    //       120      1520
   double undocumented_3;                          //      1640         8     ?
   BYTE   undocumented_4[12];                      //      1648        12

   uint   fixedSpread;                             //      1660         4     fester Spread in Points oder NULL bei variablem Spread
   BYTE   undocumented_5[16];                      //      1664        16

   double swapLong;                                //      1680         8     Swap Long
   double swapShort;                               //      1688         8     Swap Short

   uint   undocumented_6;                          //      1696         4     ?
   DWORD  undocumented_7;                          //      1700         4
   double lotSize;                                 //      1704         8     Lot Size
   BYTE   undocumented_8[16];                      //      1712        16

   uint   stopLevel;                               //      1728         4     Stop Level
   BYTE   undocumented_9[12];                      //      1732        12

   double marginInit;                              //      1744         8     Margin Init        (wenn NULL, dann wie lotSize)
   double marginMaintenance;                       //      1752         8     Margin Maintenance (wenn NULL, dann wie lotSize)
   double marginHedged;                            //      1760         8     Margin Hedged

   double undocumented_10;                         //      1768         8     ?
   double point;                                   //      1776         8     Point Size
   double pointsPerUnit;                           //      1784         8     Points per Unit

   BYTE   undocumented_11[24];                     //      1792        24
   char   currency[MAX_SYMBOL_LENGTH+1];           //      1816        12     Currency (szchar)    - Margin??? -

   BYTE   undocumented_12[104];                    //      1828       104
   uint   undocumented_13;                         //      1932         4     ?
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //              = 1936

/**
 * MT4 structure SYMBOL_GROUP: Dateiformat "symgroups.raw"
 *
 * Die Datei enthält immer 32 Gruppen, die Dateigröße ist fix 2.560 Bytes. Einzelne Gruppen können undefiniert sein.
 */
struct SYMBOL_GROUP {                              // -- size --- description ----------------------------------------------------------------------------------------
   char name       [16];                           //      16     Name         (szchar)
   char description[60];                           //      60     Beschreibung (szchar)
   uint backgroundColor;               /*custom*/  //       4     Farbe im "MarketWatch" Window
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //    = 80

/**
 * MT4 structure SYMBOL_SELECTED: Dateiformat "symbols.sel"
 */
struct SYMBOL_SELECTED {                           // -- size --- description ----------------------------------------------------------------------------------------
   char   symbol[MAX_SYMBOL_LENGTH+1];             //      12     Symbol (szchar)
   uint   digits;                                  //       4     Digits

   uint   symbolIndex;                             //       4     Index des Symbols in "symbols.raw"
   DWORD  undocumented_1;                          //       4     immer 0x0001

   uint   groupIndex;                              //       4     Index der Symbolgruppe in "symgroups.raw"
   DWORD  undocumented_2;                          //       4

   double point;                                   //       8     Point
   uint   fixedSpread;                             //       4     fester Spread in Points oder NULL bei variablem Spread
   DWORD  undocumented_3;                          //       4

   uint   tickType;                                //       4     Tick-Type: 0=Uptick, 1=Downtick, 2=n/a
   WORD   undocumented_4;                          //       2     immer 0x0100
   WORD   undocumented_5;                          //       2     unterschiedlich (oft NULL)

   uint   time;                                    //       4     Time
   DWORD  undocumented_6;                          //       4
   double bid;                                     //       8     Bid
   double ask;                                     //       8     Ask
   double sessionHigh;                             //       8     Session High
   double sessionLow;                              //       8     Session Low

   BYTE   undocumented_7[16];                      //      16     unterschiedlich (oft alles NULL)
   double bid_2;                                   //       8     Bid (Wiederholung)
   double ask_2;                                   //       8     Ask (Wiederholung)
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //   = 128

/**
 * MT4 structure HISTORY_HEADER
 *
 * HistoryFile Header
 */
struct HISTORY_HEADER {                            // -- size ------- offset --- description -------------------------------------------------------------------------
   uint  format;                                   //       4      => hh[ 0]     Barformat, bis Build 509: 400, danach: 401
   char  description[64];                          //      64      => hh[ 1]     Beschreibung (szchar)
   char  symbol[MAX_SYMBOL_LENGTH+1];              //      12      => hh[17]     Symbol       (szchar)
   uint  period;                                   //       4      => hh[20]     Timeframe
   uint  digits;                                   //       4      => hh[21]     Digits
   uint  syncMark;                                 //       4      => hh[22]     SyncMarker (timestamp), wird vom Terminal beim Refresh mit Serverversion überschrieben
   uint  lastSync;                                 //       4      => hh[23]     LastSync   (unbenutzt), wird vom Terminal nicht überschrieben
   uint  timezoneId;                   /*custom*/  //       4      => hh[24]     Timezone-ID (default: 0 => Server-Timezone)
   BYTE  reserved[48];                             //      48      => hh[25]
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //   = 148

/**
 * MT4 structure HISTORY_BAR_400
 *
 * HistoryFile Barformat v400 (bis MetaTrader Build 509)
 */
struct HISTORY_BAR_400 {                           // -- size --- description ----------------------------------------------------------------------------------------
   uint   time;                                    //       4     Open-Time (timestamp)
   double open;                                    //       8
   double low;                                     //       8
   double high;                                    //       8
   double close;                                   //       8
   double ticks;                                   //       8     immer Ganzzahl
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //    = 44
typedef HISTORY_BAR_400 RateInfo;                  // MetaQuotes-Terminologie


/**
 * MT4 structure HISTORY_BAR_401
 *
 * HistoryFile Barformat v401 (ab MetaTrader Build 510)
 */
struct HISTORY_BAR_401 {                           // -- size --- description ----------------------------------------------------------------------------------------
   int64  time;                                    //       8     Open-Time (timestamp)
   double open;                                    //       8
   double high;                                    //       8
   double low;                                     //       8
   double close;                                   //       8
   uint64 ticks;                                   //       8
   int    spread;                                  //       4     (unbenutzt)
   uint64 volume;                                  //       8     (unbenutzt)
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //    = 60
typedef HISTORY_BAR_401 MqlRates;                  // MetaQuotes-Terminologie


/**
 * In der DLL aufgetretener Fehler.
 */
struct DLL_ERROR {
   int   code;
   char* message;                                  // szchar
};


/**
 * Ausführungskontext eines MQL-Programms für Laufzeitinformationen und Datenaustausch zwischen MQL-Modulen und DLL
 */
struct EXECUTION_CONTEXT {                         // -- size ------- offset --- description ----------------------------------------------------------------------------------------
   uint               programId;                   //       4      => ec[ 0]     eindeutige Programm-ID (größer 0)               (konstant)   => Index in programs[i]
   ProgramType        programType;                 //       4      => ec[ 1]     Programmtyp                                     (konstant)   => was bin ich
   char               programName[MAX_PATH];       //     260      => ec[ 2]     Programmname (szchar)                           (konstant)   => wie heiße ich
   LaunchType         launchType;                  //       4      => ec[67]     Launchtyp                                       (konstant)   => wie wurde ich gestartet
   EXECUTION_CONTEXT* superContext;                //       4      => ec[68]     übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   uint               initFlags;                   //       4      => ec[69]     init-Flags                                      (konstant)   => wie werde ich initialisiert
   uint               deinitFlags;                 //       4      => ec[70]     deinit-Flags                                    (konstant)   => wie werde ich deinitialisiert
   RootFunction       rootFunction;                //       4      => ec[71]     aktuelle Rootfunktion                           (variabel)   => wo bin ich
   uint               uninitializeReason;          //       4      => ec[72]     letzter Uninitialize-Reason                     (variabel)   => woher komme ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //      12      => ec[73]     aktuelles Symbol (szchar)                       (variabel)   => auf welchem Symbol laufe ich
   uint               timeframe;                   //       4      => ec[76]     aktuelle Bar-Periode                            (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChartWindow;                //       4      => ec[77]     Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen
   HWND               hChart;                      //       4      => ec[78]     Chart-Frame:   MQL::WindowHandle()              (konstant)   => ...
   uint               testFlags;                   //       4      => ec[79]     Test-Flags: Off|On|VisualMode|Optimization      (konstant)   => laufe ich im Tester und wenn ja, wie

   int                lastError;                   //       4      => ec[80]     letzter MQL-Fehler                              (variabel)   => welcher MQL-Fehler ist aufgetreten
   DLL_ERROR**        dllErrors;                   //       4      => ec[81]     Array von DLL-Fehlern                           (variabel)   => welche DLL-Fehler sind aufgetreten
   uint               dllErrorsSize;               //       4      => ec[82]     Anzahl von DLL-Fehlern (Arraygröße)             (variabel)   => wieviele DLL-Fehler sind aufgetreten
   BOOL               logging;                     //       4      => ec[83]     Logstatus                                       (konstant)   => was logge ich
   char               logFile[MAX_PATH];           //     260      => ec[84]     Name der Logdatei (szchar)                      (konstant)   => wohin logge ich
};                                                 // -------------------------------------------------------------------------------------------------------------------------------
                                                   //     596     = int[149]                                                                     warum bin ich nicht auf Ibiza

typedef std::vector<EXECUTION_CONTEXT*> pec_vector;


// Funktionsdeklarationen
BOOL  onProcessAttach();
BOOL  onProcessDetach();

uint  ecc_setProgramId(pec_vector &chain, uint id);

BOOL  WINAPI SetMainExecutionContext(EXECUTION_CONTEXT* ec, const char* name, const char* symbol, int period);
BOOL  WINAPI SyncExecutionContext   (EXECUTION_CONTEXT* ec, const char* name, const char* symbol, int period);

void  WINAPI SetLogLevel(int level);
HWND  WINAPI GetApplicationWindow();
DWORD WINAPI GetUIThreadId();
BOOL  WINAPI IsUIThread();
int   WINAPI GetBoolsAddress  (BOOL   values[]);
int   WINAPI GetIntsAddress   (int    values[]);   int WINAPI GetBufferAddress(int values[]);   // Alias
int   WINAPI GetDoublesAddress(double values[]);
int   WINAPI GetStringsAddress(MqlStr values[]);
int   WINAPI GetStringAddress (char*  value   );
char* WINAPI GetString        (char*  value   );
int   WINAPI GetLastWin32Error();
BOOL  WINAPI IsBuiltinTimeframe(int timeframe);
BOOL  WINAPI IsCustomTimeframe(int timeframe);
char* WINAPI IntToHexStr(int value);

const char*  ModuleTypeToStr       (ModuleType  type);
const char*  ModuleTypeDescription (ModuleType  type);
const char*  ProgramTypeToStr      (ProgramType type);
const char*  ProgramTypeDescription(ProgramType type);
const char*  RootFunctionToStr     (RootFunction id);
const char*  RootFunctionName      (RootFunction id);

const char*  PeriodToStr      (int period);  const char* TimeframeToStr      (int timeframe);   // Alias
const char*  PeriodDescription(int period);  const char* TimeframeDescription(int timeframe);   // Alias

void         RemoveTickTimers();
uint         MT4InternalMsg();
