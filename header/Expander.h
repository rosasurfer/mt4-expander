#pragma once

#include "mql/defines.h"
#include "mql/errors.h"
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
 * MT4 struct SYMBOL (Dateiformat "symbols.raw")
 *
 * Die Symbole in der Datei sind alphabetisch nach Symbolnamen sortiert.
 */
struct SYMBOL {                                    // -- offset ---- size --- description ----------------------------------------------------------------------------
   char   name        [MAX_SYMBOL_LENGTH+1];       //         0        12     Symbol         (szchar) z.B. "AUDCAD.mkt"
   char   description [54];                        //        12        54     Beschreibung   (szchar)
   char   origin      [10];                        //        66        10     pewa: ehemaliges Symbol, 10 Zeichen (szchar)
   char   altName     [MAX_SYMBOL_LENGTH+1];       //        76        12     Standardsymbol (szchar) z.B. "AUDCAD" (falls ungleich 'name')
   char   baseCurrency[MAX_SYMBOL_LENGTH+1];       //        88        12     Base Currency
   uint   group;                                   //       100         4     Index der Symbolgruppe in "symgroups.raw"
   uint   digits;                                  //       104         4     Digits

   uint   tradeMode;                               //       108         4     0=No|1=Close|2=Full
   uint   backgroundColor;                         //       112         4     Farbe im "MarketWatch" Window
   uint   id;                                      //       116         4     eindeutige ID des Symbols

   BYTE   unknown_1[1508];                         //       120      1508
   uint   unknown_2;                               //      1628         4     ?
   BYTE   unknown_3[8];                            //      1632         8
   double unknown_4;                               //      1640         8     ?
   BYTE   unknown_5[12];                           //      1648        12

   uint   spread;                                  //      1660         4     Spread in Points: 0=current spread (variable)
   BYTE   unknown_6[16];                           //      1664        16

   double swapLong;                                //      1680         8     Swap Long
   double swapShort;                               //      1688         8     Swap Short

   uint   unknown_7;                               //      1696         4     ?
   DWORD  unknown_8;                               //      1700         4
   double contractSize;                            //      1704         8     Lot Size
   BYTE   unknown_9[16];                           //      1712        16

   uint   stopDistance;                            //      1728         4     Stop Level
   BYTE   unknown_10[12];                          //      1732        12

   double marginInit;                              //      1744         8     Margin Init        (0=ContractSize)
   double marginMaintenance;                       //      1752         8     Margin Maintenance (0=ContractSize)
   double marginHedged;                            //      1760         8     Margin Hedged
   double marginDivider;                           //      1768         8     Leverage: relativ zum Account oder absolut (CustomLeverage)

   double pointSize;                               //      1776         8     Point Size
   double pointsPerUnit;                           //      1784         8     Points per Unit

   BYTE   unknown_11[24];                          //      1792        24
   char   marginCurrency[MAX_SYMBOL_LENGTH+1];     //      1816        12     Margin Currency (szchar)

   BYTE   unknown_12[104];                         //      1828       104
   uint   unknown_13;                              //      1932         4     ?
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //              = 1936

/**
 * MT4 struct SYMBOL_GROUP (Dateiformat "symgroups.raw")
 *
 * Die Datei enthält immer 32 Gruppen, die Dateigröße ist fix 2.560 Bytes. Einzelne Gruppen können undefiniert sein.
 */
struct SYMBOL_GROUP {                              // -- offset ---- size --- description ----------------------------------------------------------------------------
   char name       [16];                           //         0        16     Name         (szchar)
   char description[60];                           //        16        60     Beschreibung (szchar)
   int  backgroundColor;          // custom (pewa) //        76         4     Farbe im "Market Watch" Window
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //                = 80

/**
 * MT4 struct SYMBOL_SELECTED (Dateiformat "symbols.sel")
 */
struct SYMBOL_SELECTED {                           // -- offset ---- size --- description ----------------------------------------------------------------------------
   char   symbol[MAX_SYMBOL_LENGTH+1];             //         0        12     Symbol (szchar)
   uint   digits;                                  //        12         4     Digits

   uint   index;         // oder ID ?????          //        16         4     Index/ID des Symbols in "symbols.raw"
   DWORD  unknown_1;                               //        20         4     immer 0x0001

   uint   group;                                   //        24         4     Index der Symbolgruppe in "symgroups.raw"
   DWORD  unknown_2;                               //        28         4

   double pointSize;                               //        32         8     Point size
   uint   spread;                                  //        40         4     Spread: 0=current spread (variable)
   DWORD  unknown_3;                               //        44         4

   uint   tickType;                                //        48         4     Tick-Type: 0=Uptick, 1=Downtick, 2=n/a
   WORD   unknown_4;                               //        52         2     immer 0x0100
   WORD   unknown_5;                               //        54         2     unterschiedlich (oft NULL)

   uint   time;                                    //        56         4     Time
   DWORD  unknown_6;                               //        60         4
   double bid;                                     //        64         8     Bid
   double ask;                                     //        72         8     Ask
   double sessionHigh;                             //        80         8     Session High
   double sessionLow;                              //        88         8     Session Low

   BYTE   unknown_7[16];                           //        96        16     unterschiedlich (oft alles NULL)
   double bid_2;                                   //       112         8     Bid (Wiederholung)
   double ask_2;                                   //       120         8     Ask (Wiederholung)
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //               = 128

/**
 * MT4 struct TICK (Dateiformat "ticks.raw")
 */
struct TICK {                                      // -- offset ---- size --- description ----------------------------------------------------------------------------
   char   symbol[MAX_SYMBOL_LENGTH+1];             //         0        12     Symbol (szchar)
   uint   time;                                    //        12         4     Timestamp
   double bid;                                     //        16         8     Bid
   double ask;                                     //        24         8     Ask
   uint   counter;                                 //        32         4     fortlaufender Zähler innerhalb der Datei
   BYTE   unknown[4];                              //        36         4     ?
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //                = 40

/**
 * MT4 struct HISTORY_HEADER
 *
 * HistoryFile Header
 */
struct HISTORY_HEADER {                            // -- offset ---- size --- description ----------------------------------------------------------------------------
   uint  format;                                   //         0         4     Barformat, bis Build 509: 400, danach: 401
   char  description[64];                          //         4        64     Beschreibung (szchar)
   char  symbol[MAX_SYMBOL_LENGTH+1];              //        68        12     Symbol       (szchar)
   uint  period;                                   //        80         4     Timeframe in Minuten
   uint  digits;                                   //        84         4     Digits
   uint  syncMark;                                 //        88         4     SyncMarker (timestamp), wird vom Terminal beim Refresh mit Serverversion überschrieben
   uint  lastSync;                                 //        92         4     LastSync   (unbenutzt), wird vom Terminal nicht überschrieben
   uint  timezoneId;              // custom (pewa) //        96         4     Timezone-ID (default: 0 => Server-Timezone)
   BYTE  reserved[48];                             //       100        48
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //               = 148

/**
 * MT4 struct HISTORY_BAR_400
 *
 * HistoryFile Barformat v400 (bis MetaTrader Build 509)
 */
typedef struct HISTORY_BAR_400 {                   // -- offset ---- size --- description ----------------------------------------------------------------------------
   uint   time;                                    //         0         4     Open-Time (timestamp)
   double open;                                    //         4         8
   double low;                                     //        12         8
   double high;                                    //        20         8
   double close;                                   //        28         8
   double ticks;                                   //        36         8     immer Ganzzahl
} RateInfo;             // MetaQuotes-Terminologie // ----------------------------------------------------------------------------------------------------------------
                                                   //                = 44

/**
 * MT4 struct HISTORY_BAR_401
 *
 * HistoryFile Barformat v401 (ab MetaTrader Build 510)
 */
typedef struct HISTORY_BAR_401 {                   // -- offset ---- size --- description ----------------------------------------------------------------------------
   int64  time;                                    //         0         8     Open-Time (timestamp)
   double open;                                    //         8         8
   double high;                                    //        16         8
   double low;                                     //        24         8
   double close;                                   //        32         8
   uint64 ticks;                                   //        40         8
   int    spread;                                  //        48         4     (unbenutzt)
   uint64 volume;                                  //        52         8     (unbenutzt)
} MqlRates;             // MetaQuotes-Terminologie // ----------------------------------------------------------------------------------------------------------------
                                                   //                = 60

// @see ExpertSample.cpp, hier jedoch anders definiert: https://docs.mql4.com/mql4changes


/**
 * MT4 struct MqlStr: interne Darstellung eines MQL-Strings (MetaQuotes-Terminologie)
 */
struct MqlStr {
   int   size;                                     // Größe des Speicherblocks oder 0, wenn der String ein intern verwaltetes C-Literal ist
   char* string;
};


/**
 * MT4 struct FXT_HEADER version 405
 *
 * Tickdatei-Header.
 *
 *  • Tickdateien ab Version 405 haben je nach MetaTrader-Version unterschiedliche Tickdatenformate.
 *  • Tickdateien enthalten keine Infos zu MODE_MARGINREQUIRED, Tests benötigen also existierende und gültige Serverinformationen.
 *  • Vor den modellierten Ticks einer Datei kann sich ein Prolog von History-Bars mit Pseudo-Ticks befinden, der dem Expert eine
 *    History vor dem Startzeitpunkt des Test zur Verfügung stellt. Der vom Terminal erzeugte Prolog ist 1000 Bars lang, der erste
 *    modellierte Tick befindet sich an der 1000-und-ersten Bar. Der Expert muß prüfen, ob diese History für seine Strategie
 *    ausreichend ist und ggf. auf das Eintreffen ausreichender Bars warten.
 */
struct FXT_HEADER {                                // -- offset ---- size --- description ----------------------------------------------------------------------------
   uint   version;                                 //         0         4     Header-Version                               405
   char   description[64];                         //         4        64     z.B. Copyright (szchar)
   char   serverName[128];                         //        68       128     Name des Accountservers (szchar)
   char   symbol[MAX_SYMBOL_LENGTH+1];             //       196        12     Symbol (szchar)
   uint   period;                                  //       208         4     Timeframe in Minuten
   uint   modelType;                               //       212         4     0=EveryTick|1=ControlPoints|2=BarOpen
   uint   modeledBars;                             //       216         4     Anzahl der modellierten Bars (ohne Prolog)
   uint   firstBarTime;                            //       220         4     OpenTime der Bar des ersten Ticks (ohne Prolog)
   uint   lastBarTime;                             //       224         4     OpenTime der Bar des letzten Ticks (ohne Prolog)
   BYTE   reserved_1[4];                           //       228         4     (alignment to the next double)
   double modelQuality;                            //       232         8     max. 99.9

   // common parameters                            // ----------------------------------------------------------------------------------------------------------------
   char   baseCurrency[MAX_SYMBOL_LENGTH+1];       //       240        12     Base currency (szchar)                     = StringLeft(symbol, 3)
   uint   spread;                                  //       252         4     Spread in Points: 0=current spread         = MarketInfo(MODE_SPREAD)
   uint   digits;                                  //       256         4     Digits                                     = MarketInfo(MODE_DIGITS)
   BYTE   reserved_2[4];                           //       260         4     (alignment to the next double)
   double pointSize;                               //       264         8     Auflösung, z.B. 0.0000'1                   = MarketInfo(MODE_POINT)
   uint   minLotsize;                              //       272         4     Mindest-Lotsize in Hundertsteln Lot        = MarketInfo(MODE_MINLOT)  * 100
   uint   maxLotsize;                              //       276         4     Höchst-Lotsize in Hundertsteln Lot         = MarketInfo(MODE_MAXLOT)  * 100
   uint   lotStepsize;                             //       280         4     Lot-Stepsize in Hundertsteln Lot           = MarketInfo(MODE_LOTSTEP) * 100
   uint   stopDistance;                            //       284         4     Stop level in Points                       = MarketInfo(MODE_STOPLEVEL)
   BOOL   pendingsGTC;                             //       288         4     close pending orders at end of day or GTC    @see struct SYMBOL
   BYTE   reserved_3[4];                           //       292         4     (alignment to the next double)

   // profit calculation parameters                // ----------------------------------------------------------------------------------------------------------------
   double contractSize;                            //       296         8     Größe eines Lots, z.B. 100000              = MarketInfo(MODE_LOTSIZE)
   double tickValue;                               //       304         8     Tick-Value in Quote-Currency ???           = MarketInfo(MODE_TICKVALUE)
   double tickSize;                                //       312         8     Tick-Größe, z.B. 0.0000'1                  = MarketInfo(MODE_TICKSIZE)
   uint   profitCalculationMode;                   //       320         4     0=Forex|1=CFD|2=Futures                    = MarketInfo(MODE_PROFITCALCMODE)

   // swap calculation parameters                  // ----------------------------------------------------------------------------------------------------------------
   BOOL   swapEnabled;                             //       324         4     ob Swaps berechnet werden
   uint   swapCalculationMode;                     //       328         4     0=Points|1=BaseCurrency|2=Interest|3=MarginCurrency   = MarketInfo(MODE_SWAPTYPE)
   BYTE   reserved_4[4];                           //       332         4     (alignment to the next double)
   double swapLongValue;                           //       336         8     long overnight swap value                  = MarketInfo(MODE_SWAPLONG)
   double swapShortValue;                          //       344         8     short overnight swap values                = MarketInfo(MODE_SWAPSHORT)
   uint   tripleRolloverDay;                       //       352         4     weekday of triple swaps                    = WEDNESDAY

   // margin calculation parameters                // ----------------------------------------------------------------------------------------------------------------
   uint   accountLeverage;                         //       356         4     account leverage                           = AccountLeverage()
   uint   freeMarginCalculationType;               //       360         4     free margin calculation type               = AccountFreeMarginMode()
   uint   marginCalculationMode;                   //       364         4     margin calculation mode                    = MarketInfo(MODE_MARGINCALCMODE)
   uint   marginStopoutLevel;                      //       368         4     margin stopout level                       = AccountStopoutLevel()
   uint   marginStopoutType;                       //       372         4     margin stopout type                        = AccountStopoutMode()
   double marginInit;                              //       376         8     initial margin requirement (in units)      = MarketInfo(MODE_MARGININIT)
   double marginMaintenance;                       //       384         8     maintainance margin requirement (in units) = MarketInfo(MODE_MARGINMAINTENANCE)
   double marginHedged;                            //       392         8     hedged margin requirement (in units)       = MarketInfo(MODE_MARGINHEDGED)
   double marginDivider;                           //       400         8     Leverage: relativ=zum Account, absolut=CustomLeverage
   char   marginCurrency[MAX_SYMBOL_LENGTH+1];     //       408        12                                                = AccountCurrency()
   BYTE   reserved_5[4];                           //       420         4     (alignment to the next double)

   // commission calculation parameters            // ----------------------------------------------------------------------------------------------------------------
   double commissionValue;                         //       424         8     commission rate
   uint   commissionCalculationMode;               //       432         4     0=Money|1=Pips|2=Percent                     @see COMMISSION_MODE_*
   uint   commissionType;                          //       436         4     0=RoundTurn|1=PerDeal                        @see COMMISSION_TYPE_*

   // later additions                              // ----------------------------------------------------------------------------------------------------------------
   uint   firstBar;                                //       440         4     bar number/index??? of first bar (ohne Prolog) or 0 for first bar
   uint   lastBar;                                 //       444         4     bar number/index??? of last bar (ohne Prolog) or 0 for last bar
   uint   startPeriod[6];                          //       448        24     [0] = firstBar (bar where the smaller period modeling started)
   uint   testerSettingFrom;                       //       472         4     begin date from tester settings
   uint   testerSettingTo;                         //       476         4     end date from tester settings
   uint   freezeDistance;                          //       480         4     freeze level in points                     = MarketInfo(MODE_FREEZELEVEL)
   uint   modelErrors;                             //       484         4     number of errors during model generation
   BYTE   reserved_6[240];                         //       488       240     unused
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //               = 728     warum bin ich nicht auf Ibiza


// Tickdata, letztes Feld: // expert flag 0-bar is modified, but expert is not run


/**
 * MyFX struct: Wrapper für eine nach MQL zu übermittelnde Logmessage.
 */
struct LOG_MESSAGE {
   int   level;                                    // Loglevel: L_ERROR, L_WARN oder L_INFO
   int   code;
   char* text;                                     // szchar
};


/**
 * MyFX struct: Ausführungskontext eines MQL-Programms für Datenaustausch zwischen MQL-Modulen und DLL
 */
struct EXECUTION_CONTEXT {                         // -- offset ---- size --- description ----------------------------------------------------------------------------------------
   uint               programId;                   //         0         4     eindeutige Programm-ID (größer 0)               (konstant)   => Index in programs[i]
   ProgramType        programType;                 //         4         4     Programmtyp                                     (konstant)   => was bin ich
   char               programName[MAX_PATH];       //         8       260     Programmname (szchar)                           (konstant)   => wie heiße ich
   ModuleType         moduleType;                  //       268         4     Modultyp                                        (konstant)   => was bin ich
   char               moduleName[MAX_PATH];        //       272       260     Modulname (szchar)                              (konstant)   => wie heiße ich

   LaunchType         launchType;                  //       532         4     Launchtyp                                       (konstant)   => wie wurde ich gestartet
   EXECUTION_CONTEXT* superContext;                //       536         4     übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   uint               initFlags;                   //       540         4     init-Flags                                      (konstant)   => wie werde ich initialisiert
   uint               deinitFlags;                 //       544         4     deinit-Flags                                    (konstant)   => wie werde ich deinitialisiert
   RootFunction       rootFunction;                //       548         4     letzte Rootfunktion des aktuellen Modules       (variabel)   => wo bin ich
   UninitializeReason uninitializeReason;          //       552         4     letzter Uninitialize-Reason (nur Hauptmodule)   (variabel)   => woher komme ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //       556        12     aktuelles Symbol (szchar) (nicht in Libraries)  (variabel)   => auf welchem Symbol laufe ich
   uint               timeframe;                   //       568         4     aktuelle Bar-Periode (nicht in Libraries)       (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChartWindow;                //       572         4     Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen
   HWND               hChart;                      //       576         4     Chart-Frame:   MQL::WindowHandle()              (konstant)   => ...
   uint               testFlags;                   //       580         4     Test-Flags: Off|On|VisualMode|Optimization      (konstant)   => laufe ich im Tester und wenn ja, wie

   int                lastError;                   //       584         4     letzter MQL-Fehler                              (variabel)   => welcher MQL-Fehler ist aufgetreten
   int                error;                       //       588         4     DLL-Fehlercode                                  (variabel)   => welcher DLL-Fehler ist aufgetreten
   char*              errorMsg;                    //       592         4     Text des DLL-Fehlers                            (variabel)   => ...
   int                warn;                        //       596         4     Code einer DLL-Warnung oder NULL                (variabel)   => ...
   char*              warnMsg;                     //       600         4     Text der DLL-Warnung                            (variabel)   => ...
   int                info;                        //       604         4     Code einer DLL-Info oder NULL                   (variabel)   => ...
   char*              infoMsg;                     //       608         4     Text der DLL-Info                               (variabel)   => ...
   BOOL               logging;                     //       612         4     Logstatus                                       (konstant)   => was logge ich
   char               logFile[MAX_PATH];           //       616       260     Name der Logdatei (szchar)                      (konstant)   => wohin logge ich
};                                                 // ----------------------------------------------------------------------------------------------------------------------------
                                                   //               = 876                                                                     warum bin ich nicht auf Ibiza

 //LOG_MESSAGE**      dllErrors;                   //         4               Array von Logmessages des Typs L_ERROR          (variabel)   => welche DLL-Fehler sind aufgetreten
 //uint               dllErrorsSize;               //         4               Anzahl von Logmessages (Arraygröße)             (variabel)   => wieviele DLL-Fehler sind aufgetreten


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
const char* WINAPI GetString        (char*  value   );
      int   WINAPI GetLastWin32Error();
      BOOL  WINAPI IsBuiltinTimeframe(int timeframe);
      BOOL  WINAPI IsCustomTimeframe(int timeframe);
const char* WINAPI IntToHexStr(int value);
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