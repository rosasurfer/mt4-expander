#pragma once
#include "struct/xtrade/Test.h"


/**
 * XTrade struct EXECUTION_CONTEXT
 *
 * Ausführungskontext von MQL-Programmen zur Kommunikation zwischen MQL und DLL
 *
 * Die EXECUTION_CONTEXTe dienen dem Datenaustausch zwischen mehreren MQL-Programmen, zwischen einzelnen Modulen desselben
 * Programms und zwischen einem Programm und der DLL. Jedes MQL-Modul verfügt über einen eigenen Kontext, alle Kontexte eines
 * MQL-Programms bilden gemeinsam eine Context-Chain. An erster Stelle einer Context-Chain liegt der Master-Context, der in der
 * DLL verwaltet wird. An zweiter Stelle liegt der Context des MQL-Hauptmodules (Expert, Script oder Indikator). Alle weiteren
 * Contexte einer Chain sind Library-Contexte. Über die Kontexte werden wie folgt Daten ausgetauscht:
 *
 *  • Data exchange between MQL program main module and DLL:
 *    The DLL stores DLL error informations in the EXECUTION_CONTEXT for further processing by the MQL main module. The main
 *    module then signals these errors to the user. On the other hand the main module stores price and market informations in
 *    the context to make it available to the DLL.
 *
 *  • Data exchange between MQL program main module and MQL libraries:
 *
 *  • Data exchange between multiple MQL programs:
 *
 *
 * Notes:
 * ------
 *  • Der Master-Context eines Programms ist immer gültig. Alle anderen Kontexte des Programms können je nach Modultyp und
 *    Situation ungültig bzw. der Speicher nicht verfügbar sein. Von einem MQL-Modul darf generell nur auf den eigenen und auf
 *    den Master-Context zugegriffen werden. Ein Zugriff aus einer Library auf den Hauptkontext und umgekehrt ist jedoch in
 *    Sonderfällen möglich.
 *
 *  • Experts und Scripte verfügen während ihrer gesamten Laufzeit nur über eine Instanz ihres Hauptmodulkontextes. Indikatoren
 *    hingegen erhalten nach jedem Init-Cycle eine neue Instanz, da MetaTrader den Speicher für Variablen in Indicator::init()
 *    jeweils neu alloziiert.
 *
 *  • Im Indikator gibt es während eines init()-Cycles in der Zeitspanne vom Verlassen von Indicator::deinit() bis zum Wieder-
 *    eintritt in Indicator::init() keinen gültigen Hauptkontext. Der alte Speicherblock wird sofort freigegeben, in init()
 *    wird ein neuer alloziiert. Während dieser Zeitspanne wird der init()-Cycle von bereits geladenen Libraries durchgeführt
 *    und es darf nicht auf den zu dem Zeitpunkt ungültigen Hauptkontext zugegriffen werden.
 *
 *  • Nach Recompilation oder Crash einer Library wird ihr ExecutionContext ungültig und auf ihn darf nicht mehr zugegriffen
 *    werden.
 */
#pragma pack(push, 1)

struct EXECUTION_CONTEXT {                         // -- offset --- size --- description ----------------------------------------------------------------------------------------
   uint               programIndex;                //         0        4     MQL program index starting from 1               (constant) => equals index in g_contextChains[]
   ProgramType        programType;                 //         4        4     MQL program type                                (constant) => type of MQL program
   char               programName[MAX_PATH];       //         8      260     MQL program name                                (constant) => MQL program name
   ModuleType         moduleType;                  //       268        4     MQL module type                                 (constant) => type of MQL module
   char               moduleName[MAX_PATH];        //       272      260     MQL module name = MQL::WindowExpertName()       (constant) => MQL module name

   LaunchType         launchType;                  //       532        4     launch type                                     (constant) => how was the program started
   RootFunction       rootFunction;                //       536        4     the program's last root function                (variable) => where is it
   BOOL               initCycle;                   //       540        4     whether or not in an init cycle                 (variable) => where is it
   InitializeReason   initReason;                  //       544        4     last initialize reason                          (variable) => where did it come from
   UninitializeReason uninitReason;                //       548        4     last MQL::UninitializeReason()                  (variable) => where did it come from/where does it go to
   BOOL               testing;                     //       552        4     IsTesting() status                              (constant) => does it run in Tester
   BOOL               visualMode;                  //       556        4     IsVisualMode() status                           (constant) => does the Tester run with VisualMode=On
   BOOL               optimization;                //       560        4     IsOptimization() status                         (constant) => does the Tester run with Optimization=On
   TEST*              test;                        //       564        4     test meta-data                                  (constant) => test configuration and data

   DWORD              initFlags;                   //       568        4     init configuration                              (constant) => how is the program initialized
   DWORD              deinitFlags;                 //       572        4     deinit configuration                            (constant) => how is the program deinitialized
   BOOL               logging;                     //       576        4     logging configuration                           (constant) => is logging enabled
   char               customLogFile[MAX_PATH];     //       580      260     custom log filename                             (constant) => where goes logging to

   char               symbol[MAX_SYMBOL_LENGTH+1]; //       840       12     current symbol                                  (variable) => the current chart symbol
   uint               timeframe;                   //       852        4     current chart period                            (variable) => the current chart timeframe
   HWND               hChart;                      //       856        4     chart handle = MQL::WindowHandle()              (constant) => handle of the chart frame
   HWND               hChartWindow;                //       860        4     chart handle with title bar "Symbol,Period"     (constant) => handle of the chart window

   void*              rates;                       //       864        4     current price series = MQL::ArrayCopyRates()    (constant) => RateInfo[]
   uint               bars;                        //       868        4     current number of bars = MQL::Bars              (variable)
   uint               ticks;                       //       872        4     number of received ticks                        (variable)
   datetime           currentTickTime;             //       876        4     server time of the current tick                 (variable)
   datetime           previousTickTime;            //       880        4     server time of the previous tick                (variable)
   DWORD              _alignment1;                 //       884        4     (alignment to the next double)
   double             bid;                         //       888        8     current bid price = MQL::Bid                    (variable)
   double             ask;                         //       896        8     current ask price = MQL::Ask                    (variable)

   EXECUTION_CONTEXT* superContext;                //       904        4     parent/calling EXECUTION_CONTEXT                (constant) => is the program loaded by iCustom()
   uint               threadId;                    //       908        4     current thread                                  (variable) => the executing thread

   int                mqlError;                    //       912        4     last error in MQL (main module and libraries)   (variable)
   int                dllError;                    //       916        4     last error in DLL                               (variable)
   char*              dllErrorMsg;                 //       920        4     DLL error message                               (variable)
   int                dllWarning;                  //       924        4     last DLL warning                                (variable)
   char*              dllWarningMsg;               //       928        4     DLL warning message                             (variable)
};                                                 // ---------------------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //              = 932


// type definition
typedef std::vector<EXECUTION_CONTEXT*> ContextChain;                // all contexts (i.e. MQL modules) of a MQL program


// getters (used by MQL4)
uint               WINAPI ec_ProgramIndex    (const EXECUTION_CONTEXT* ec);
ProgramType        WINAPI ec_ProgramType     (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ProgramName     (const EXECUTION_CONTEXT* ec);
ModuleType         WINAPI ec_ModuleType      (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ModuleName      (const EXECUTION_CONTEXT* ec);
LaunchType         WINAPI ec_LaunchType      (const EXECUTION_CONTEXT* ec);
RootFunction       WINAPI ec_RootFunction    (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_InitCycle       (const EXECUTION_CONTEXT* ec);
InitializeReason   WINAPI ec_InitReason      (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_UninitReason    (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Testing         (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_VisualMode      (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Optimization    (const EXECUTION_CONTEXT* ec);
//                        ec.test
DWORD              WINAPI ec_InitFlags       (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_DeinitFlags     (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Logging         (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_CustomLogFile   (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_Symbol          (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Timeframe       (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChart          (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChartWindow    (const EXECUTION_CONTEXT* ec);
//                        ec.rates
//                        ec.bars
uint               WINAPI ec_Ticks           (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_CurrentTickTime (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_PreviousTickTime(const EXECUTION_CONTEXT* ec);
//                        ec.bid
//                        ec.ask
BOOL               WINAPI ec_SuperContext    (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec);
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext  (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_ThreadId        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_MqlError        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_DllError        (const EXECUTION_CONTEXT* ec);
//                        ...
int                WINAPI ec_DllWarning      (const EXECUTION_CONTEXT* ec);
//                        ...


// validating setters (also used by MQL4)
uint               WINAPI ec_SetProgramIndex      (EXECUTION_CONTEXT* ec, uint               index    );
ProgramType        WINAPI ec_SetProgramType       (EXECUTION_CONTEXT* ec, ProgramType        type     );
const char*        WINAPI ec_SetProgramName       (EXECUTION_CONTEXT* ec, const char*        name     );
ModuleType         WINAPI ec_SetModuleType        (EXECUTION_CONTEXT* ec, ModuleType         type     );
const char*        WINAPI ec_SetModuleName        (EXECUTION_CONTEXT* ec, const char*        name     );
LaunchType         WINAPI ec_SetLaunchType        (EXECUTION_CONTEXT* ec, LaunchType         type     );
RootFunction       WINAPI ec_SetRootFunction      (EXECUTION_CONTEXT* ec, RootFunction       id       );
BOOL               WINAPI ec_SetInitCycle         (EXECUTION_CONTEXT* ec, BOOL               status   );
InitializeReason   WINAPI ec_SetInitReason        (EXECUTION_CONTEXT* ec, InitializeReason   reason   );
UninitializeReason WINAPI ec_SetUninitReason      (EXECUTION_CONTEXT* ec, UninitializeReason reason   );
BOOL               WINAPI ec_SetTesting           (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetVisualMode        (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetOptimization      (EXECUTION_CONTEXT* ec, BOOL               status   );
//                        ec.test
DWORD              WINAPI ec_SetInitFlags         (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetDeinitFlags       (EXECUTION_CONTEXT* ec, DWORD              flags    );
BOOL               WINAPI ec_SetLogging           (EXECUTION_CONTEXT* ec, BOOL               status   );
const char*        WINAPI ec_SetCustomLogFile     (EXECUTION_CONTEXT* ec, const char*        fileName );
const char*        WINAPI ec_SetSymbol            (EXECUTION_CONTEXT* ec, const char*        symbol   );
uint               WINAPI ec_SetTimeframe         (EXECUTION_CONTEXT* ec, uint               timeframe);
HWND               WINAPI ec_SetHChart            (EXECUTION_CONTEXT* ec, HWND               hWnd     );
HWND               WINAPI ec_SetHChartWindow      (EXECUTION_CONTEXT* ec, HWND               hWnd     );
//                        ec.rates
//                        ec.bars
uint               WINAPI ec_SetTicks             (EXECUTION_CONTEXT* ec, uint               count    );
datetime           WINAPI ec_SetCurrentTickTime   (EXECUTION_CONTEXT* ec, datetime           time     );
datetime           WINAPI ec_SetPreviousTickTime  (EXECUTION_CONTEXT* ec, datetime           time     );
//                        ec.bid
//                        ec.ask
EXECUTION_CONTEXT* WINAPI ec_SetSuperContext      (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec      );
uint               WINAPI ec_SetThreadId          (EXECUTION_CONTEXT* ec, uint               id       );
int                WINAPI ec_SetMqlError          (EXECUTION_CONTEXT* ec, int                error    );
int                WINAPI ec_SetDllError          (EXECUTION_CONTEXT* ec, int                error    );
//                        ...
int                WINAPI ec_SetDllWarning        (EXECUTION_CONTEXT* ec, int                error    );
//                        ...


// master context getters
RootFunction       WINAPI mec_RootFunction(const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI mec_UninitReason(const EXECUTION_CONTEXT* ec);
DWORD              WINAPI mec_InitFlags   (const EXECUTION_CONTEXT* ec);


// helpers
const char*        WINAPI EXECUTION_CONTEXT_toStr  (const EXECUTION_CONTEXT* ec, BOOL outputDebug=FALSE);
const char*        WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug=FALSE);
