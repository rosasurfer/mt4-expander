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


// MQL-UninitializeReasons
enum UninitializeReason {
   UNINITREASON_UNDEFINED   = REASON_UNDEFINED,
   UNINITREASON_REMOVE      = REASON_REMOVE,
   UNINITREASON_RECOMPILE   = REASON_RECOMPILE,
   UNINITREASON_CHARTCHANGE = REASON_CHARTCHANGE,
   UNINITREASON_CHARTCLOSE  = REASON_CHARTCLOSE,
   UNINITREASON_PARAMETERS  = REASON_PARAMETERS,
   UNINITREASON_ACCOUNT     = REASON_ACCOUNT,
   UNINITREASON_TEMPLATE    = REASON_TEMPLATE,     // ab Build > 509
   UNINITREASON_INITFAILED  = REASON_INITFAILED,   // ...
   UNINITREASON_CLOSE       = REASON_CLOSE         // ...
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
 * Wrapper für eine nach MQL zu übermittelnde Logmessage.
 */
struct LOG_MESSAGE {
   int   level;                                    // Loglevel: L_ERROR, L_WARN oder L_INFO
   int   code;
   char* text;                                     // szchar
};


/**
 * Ausführungskontext eines MQL-Programms für Laufzeitinformationen und Datenaustausch zwischen MQL-Modulen und DLL
 */
struct EXECUTION_CONTEXT {                         // -- size ------- offset --- description ----------------------------------------------------------------------------------------
   uint               programId;                   //       4     => ec[  0]     eindeutige Programm-ID (größer 0)               (konstant)   => Index in programs[i]
   ProgramType        programType;                 //       4     => ec[  1]     Programmtyp                                     (konstant)   => was bin ich
   char               programName[MAX_PATH];       //     260     => ec[  2]     Programmname (szchar)                           (konstant)   => wie heiße ich
   ModuleType         moduleType;                  //       4     => ec[ 67]     Modultyp                                        (konstant)   => was bin ich
   char               moduleName[MAX_PATH];        //     260     => ec[ 68]     Modulname (szchar)                              (konstant)   => wie heiße ich

   LaunchType         launchType;                  //       4     => ec[133]     Launchtyp                                       (konstant)   => wie wurde ich gestartet
   EXECUTION_CONTEXT* superContext;                //       4     => ec[134]     übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   uint               initFlags;                   //       4     => ec[135]     init-Flags                                      (konstant)   => wie werde ich initialisiert
   uint               deinitFlags;                 //       4     => ec[136]     deinit-Flags                                    (konstant)   => wie werde ich deinitialisiert
   RootFunction       rootFunction;                //       4     => ec[137]     letzte Rootfunktion des aktuellen Modules       (variabel)   => wo bin ich
   UninitializeReason uninitializeReason;          //       4     => ec[138]     letzter Uninitialize-Reason (nur Hauptmodule)   (variabel)   => woher komme ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //      12     => ec[139]     aktuelles Symbol (szchar) (nicht in Libraries)  (variabel)   => auf welchem Symbol laufe ich
   uint               timeframe;                   //       4     => ec[142]     aktuelle Bar-Periode (nicht in Libraries)       (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChartWindow;                //       4     => ec[143]     Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen
   HWND               hChart;                      //       4     => ec[144]     Chart-Frame:   MQL::WindowHandle()              (konstant)   => ...
   uint               testFlags;                   //       4     => ec[145]     Test-Flags: Off|On|VisualMode|Optimization      (konstant)   => laufe ich im Tester und wenn ja, wie

   int                lastError;                   //       4     => ec[146]     letzter MQL-Fehler                              (variabel)   => welcher MQL-Fehler ist aufgetreten
   int                error;                       //       4     => ec[147]     DLL-Fehlercode                                  (variabel)   => welcher DLL-Fehler ist aufgetreten
   char*              errorMsg;                    //       4     => ec[148]     Text des DLL-Fehlers                            (variabel)   => ...
   int                warn;                        //       4     => ec[149]     Code einer DLL-Warnung oder NULL                (variabel)   => ...
   char*              warnMsg;                     //       4     => ec[150]     Text der DLL-Warnung                            (variabel)   => ...
   int                info;                        //       4     => ec[151]     Code einer DLL-Info oder NULL                   (variabel)   => ...
   char*              infoMsg;                     //       4     => ec[152]     Text der DLL-Info                               (variabel)   => ...
   BOOL               logging;                     //       4     => ec[153]     Logstatus                                       (konstant)   => was logge ich
   char               logFile[MAX_PATH];           //     260     => ec[154]     Name der Logdatei (szchar)                      (konstant)   => wohin logge ich
};                                                 // -------------------------------------------------------------------------------------------------------------------------------
                                                   //     876     = int[219]                                                                     warum bin ich nicht auf Ibiza

 //LOG_MESSAGE**      dllErrors;                   //       4     => ec[147]     Array von Logmessages des Typs L_ERROR          (variabel)   => welche DLL-Fehler sind aufgetreten
 //uint               dllErrorsSize;               //       4     => ec[148]     Anzahl von Logmessages (Arraygröße)             (variabel)   => wieviele DLL-Fehler sind aufgetreten


typedef std::vector<EXECUTION_CONTEXT*> pec_vector;


// Deklaration Thread- und EXECUTION_CONTEXT-Verwaltung (Initialisierung in Expander.cpp)
extern std::vector<DWORD>      threadIds;
extern std::vector<uint>       threadIdsProgramIds;
extern std::vector<pec_vector> contextChains;


// Funktionsdeklarationen
BOOL onProcessAttach();
BOOL onProcessDetach();
uint ecc_setProgramId(pec_vector &chain, uint id);
void RemoveTickTimers();

ProgramType  WINAPI ec_setProgramType (EXECUTION_CONTEXT* ec, ProgramType type);
const char*  WINAPI ec_setProgramName (EXECUTION_CONTEXT* ec, const char* name);
ModuleType   WINAPI ec_setModuleType  (EXECUTION_CONTEXT* ec, ModuleType type);
const char*  WINAPI ec_setModuleName  (EXECUTION_CONTEXT* ec, const char* name);
RootFunction WINAPI ec_setRootFunction(EXECUTION_CONTEXT* ec, RootFunction id);
const char*  WINAPI ec_setSymbol      (EXECUTION_CONTEXT* ec, const char* symbol);
      uint   WINAPI ec_setTimeframe   (EXECUTION_CONTEXT* ec, uint timeframe);
const char*  WINAPI ec_setLogFile     (EXECUTION_CONTEXT* ec, const char* fileName);

BOOL  WINAPI SyncMainExecutionContext(EXECUTION_CONTEXT* ec, ProgramType type, const char* name, RootFunction functionId, UninitializeReason reason, const char* symbol, int period);
BOOL  WINAPI SyncLibExecutionContext (EXECUTION_CONTEXT* ec,                   const char* name, RootFunction functionId,                            const char* symbol, int period);

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
uint  WINAPI MT4InternalMsg();

const char* WINAPI ModuleTypeDescription  (ModuleType  type);
const char* WINAPI ModuleTypeToStr        (ModuleType  type);
const char* WINAPI PeriodDescription      (int period);        const char* WINAPI TimeframeDescription(int timeframe);   // Alias
const char* WINAPI PeriodToStr            (int period);        const char* WINAPI TimeframeToStr      (int timeframe);   // Alias
const char* WINAPI ProgramTypeDescription (ProgramType type);
const char* WINAPI ProgramTypeToStr       (ProgramType type);
const char* WINAPI RootFunctionName       (RootFunction id);
const char* WINAPI RootFunctionToStr      (RootFunction id);
const char* WINAPI UninitializeReasonToStr(UninitializeReason reason);