#pragma once
#include "struct/xtrade/Test.h"


/**
 * Framework struct EXECUTION_CONTEXT
 *
 * Execution context of an MQL program. Used for communication and data exchange between MQL modules and DLL.
 *
 * Die EXECUTION_CONTEXTe dienen dem Datenaustausch zwischen mehreren MQL-Programmen, zwischen einzelnen Modulen desselben
 * Programms und zwischen einem Programm und der DLL. Jedes MQL-Modul verfügt über einen eigenen Kontext, alle Kontexte eines
 * MQL-Programms bilden gemeinsam eine Context-Chain. An erster Stelle einer Context-Chain liegt der Master-Context, der in der
 * DLL verwaltet wird. An zweiter Stelle liegt der Context des MQL-Hauptmodules (Expert, Script oder Indikator). Alle weiteren
 * Contexte einer Chain sind Library-Contexte. Über die Kontexte werden wie folgt Daten ausgetauscht:
 *
 *  • Data exchange between MQL program main module and DLL:
 *    The DLL stores DLL error information in the EXECUTION_CONTEXT for further processing by the MQL main module. The main
 *    module signals such errors to the user. On the other hand the main module stores price and market information in the
 *    context to make it available to the DLL.
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
   uint               pid;                         //         0        4     MQL program id starting from 1                      (const) => index in g_contextChains[]
   ProgramType        programType;                 //         4        4     MQL program type                                    (const) => type of MQL program
   char               programName[MAX_PATH];       //         8      260     MQL program name                                    (const) => MQL program name
   ModuleType         moduleType;                  //       268        4     MQL module type                                     (const) => type of MQL module
   char               moduleName[MAX_PATH];        //       272      260     MQL module name = MQL::WindowExpertName()           (const) => MQL module name
                                                   //
   LaunchType         launchType;                  //       532        4     launch type                                         (const) => how was the program started
   CoreFunction       coreFunction;                //       536        4     the program's last core function                    (var  ) => where is it
   BOOL               initCycle;                   //       540        4     whether or not in an init cycle                     (var  )
   InitializeReason   initReason;                  //       544        4     last initialize reason                              (var  ) => where did it come from
   UninitializeReason uninitReason;                //       548        4     last MQL::UninitializeReason()                      (var  ) => where does it go to
   DWORD              initFlags;                   //       552        4     init configuration                                  (const) => how should it be initialized
   DWORD              deinitFlags;                 //       556        4     deinit configuration                                (const) => how should it be deinitialized
                                                   //
   char               symbol[MAX_SYMBOL_LENGTH+1]; //       560       12     current symbol         = MQL::Symbol()              (var  )
   uint               timeframe;                   //       572        4     current chart period   = MQL::Period()              (var  )
   double             point;                       //       576        8     Point of the symbol    = MQL::Point                 (var  )
   uint               digits;                      //       584        4     Digits of the symbol   = MQL::Digits                (var  )
   const void*        rates;                       //       588        4     current price series   = MQL::ArrayCopyRates()      (const) => RateInfo[]
   int                bars;                        //       592        4     current number of bars = MQL::Bars                  (var  )
   int                changedBars;                 //       596        4     number of changed bars                              (var  )
   int                unchangedBars;               //       600        4     number of unchanged bars = MQL::IndicatorCounted()  (var  )
   uint               ticks;                       //       604        4     number of times MQL::start() was called             (var  )
   datetime           lastTickTime;                //       608        4     server time of the last received tick               (var  )
   datetime           prevTickTime;                //       612        4     server time of the previous received tick           (var  )
   double             bid;                         //       616        8     current bid price      = MQL::Bid                   (var  )
   double             ask;                         //       624        8     current ask price      = MQL::Ask                   (var  )
                                                   //
   EXECUTION_CONTEXT* superContext;                //       632        4     indicator host program                              (const) => is the indicator loaded by iCustom()
                                                   //
   BOOL               extReporting;                //       636        4     expert input parameter EA.ExtReporting              (var  )
   BOOL               recordEquity;                //       640        4     expert input parameter EA.RecordEquity              (var  )
   BOOL               optimization;                //       644        4     expert IsOptimization() status                      (const)
   BOOL               visualMode;                  //       648        4     expert IsVisualMode() status                        (const)
   HWND               hChart;                      //       652        4     chart handle = MQL::WindowHandle()                  (const) => handle of the chart frame
   HWND               hChartWindow;                //       656        4     chart handle with title bar "Symbol,Period"         (const) => handle of the chart window
                                                   //
   TEST*              test;                        //       660        4     test configuration and data                         (const)
   BOOL               testing;                     //       664        4     IsTesting() status                                  (const)
                                                   //
   uint               threadId;                    //       668        4     current thread                                      (var  ) => the executing thread
                                                   //
   int                mqlError;                    //       672        4     last error in MQL (main module and libraries)       (var  )
   int                dllError;                    //       676        4     last error in DLL                                   (var  )
   char*              dllErrorMsg;                 //       680        4     DLL error message                                   (var  )
   int                dllWarning;                  //       684        4     last DLL warning                                    (var  )
   char*              dllWarningMsg;               //       688        4     DLL warning message                                 (var  )
   BOOL               logging;                     //       692        4     logging configuration                               (const) => is logging enabled
   char               customLogFile[MAX_PATH];     //       696      260     custom log filename                                 (const) => log location
};                                                 // ---------------------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //              = 956


// type definition
typedef std::vector<EXECUTION_CONTEXT*> ContextChain;                // all contexts of an MQL program (one context per MQL module)


// regular getters (used by MQL4)
uint               WINAPI ec_Pid                (const EXECUTION_CONTEXT* ec);
ProgramType        WINAPI ec_ProgramType        (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ProgramName        (const EXECUTION_CONTEXT* ec);
ModuleType         WINAPI ec_ModuleType         (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ModuleName         (const EXECUTION_CONTEXT* ec);

LaunchType         WINAPI ec_LaunchType         (const EXECUTION_CONTEXT* ec);
CoreFunction       WINAPI ec_CoreFunction       (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_InitCycle          (const EXECUTION_CONTEXT* ec);
InitializeReason   WINAPI ec_InitReason         (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_UninitReason       (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_InitFlags          (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_DeinitFlags        (const EXECUTION_CONTEXT* ec);

const char*        WINAPI ec_Symbol             (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Timeframe          (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Digits             (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Point              (const EXECUTION_CONTEXT* ec);
//                        ec.rates
int                WINAPI ec_Bars               (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_ChangedBars        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_UnchangedBars      (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Ticks              (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_LastTickTime       (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_PrevTickTime       (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Bid                (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Ask                (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_SuperContext       (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* const target);
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext     (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_ExtReporting       (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_RecordEquity       (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Optimization       (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_VisualMode         (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChart             (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChartWindow       (const EXECUTION_CONTEXT* ec);

//                        ec.test
int                WINAPI ec_TestId             (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_TestCreated        (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_TestStrategy       (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_TestSymbol         (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_TestTimeframe      (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_TestStartTime      (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_TestEndTime        (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_TestBarModel       (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_TestBars           (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_TestTicks          (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_TestSpread         (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_TestTradeDirections(const EXECUTION_CONTEXT* ec);
int                WINAPI ec_TestReportId       (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_TestReportSymbol   (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Testing            (const EXECUTION_CONTEXT* ec);

uint               WINAPI ec_ThreadId           (const EXECUTION_CONTEXT* ec);

int                WINAPI ec_MqlError           (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_DllError           (const EXECUTION_CONTEXT* ec);
//                        ...
int                WINAPI ec_DllWarning         (const EXECUTION_CONTEXT* ec);
//                        ...
BOOL               WINAPI ec_Logging            (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_CustomLogging      (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_CustomLogFile      (const EXECUTION_CONTEXT* ec);


// validating setters (may be used by MQL4)
uint               WINAPI ec_SetPid             (EXECUTION_CONTEXT* ec, uint               pid      );
ProgramType        WINAPI ec_SetProgramType     (EXECUTION_CONTEXT* ec, ProgramType        type     );
const char*        WINAPI ec_SetProgramName     (EXECUTION_CONTEXT* ec, const char*        name     );
ModuleType         WINAPI ec_SetModuleType      (EXECUTION_CONTEXT* ec, ModuleType         type     );
const char*        WINAPI ec_SetModuleName      (EXECUTION_CONTEXT* ec, const char*        name     );

LaunchType         WINAPI ec_SetLaunchType      (EXECUTION_CONTEXT* ec, LaunchType         type     );
CoreFunction       WINAPI ec_SetCoreFunction    (EXECUTION_CONTEXT* ec, CoreFunction       id       );
BOOL               WINAPI ec_SetInitCycle       (EXECUTION_CONTEXT* ec, BOOL               status   );
InitializeReason   WINAPI ec_SetInitReason      (EXECUTION_CONTEXT* ec, InitializeReason   reason   );
UninitializeReason WINAPI ec_SetUninitReason    (EXECUTION_CONTEXT* ec, UninitializeReason reason   );
DWORD              WINAPI ec_SetInitFlags       (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetDeinitFlags     (EXECUTION_CONTEXT* ec, DWORD              flags    );

const char*        WINAPI ec_SetSymbol          (EXECUTION_CONTEXT* ec, const char*        symbol   );
uint               WINAPI ec_SetTimeframe       (EXECUTION_CONTEXT* ec, uint               timeframe);
uint               WINAPI ec_SetDigits          (EXECUTION_CONTEXT* ec, uint               digits   );
double             WINAPI ec_SetPoint           (EXECUTION_CONTEXT* ec, double             point    );
//                        ec.rates
int                WINAPI ec_SetBars            (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetChangedBars     (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetUnchangedBars   (EXECUTION_CONTEXT* ec, int                count    );
uint               WINAPI ec_SetTicks           (EXECUTION_CONTEXT* ec, uint               count    );
datetime           WINAPI ec_SetLastTickTime    (EXECUTION_CONTEXT* ec, datetime           time     );
datetime           WINAPI ec_SetPrevTickTime    (EXECUTION_CONTEXT* ec, datetime           time     );
double             WINAPI ec_SetBid             (EXECUTION_CONTEXT* ec, double             price    );
double             WINAPI ec_SetAsk             (EXECUTION_CONTEXT* ec, double             price    );

//                        ec.test
BOOL               WINAPI ec_SetTesting         (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetVisualMode      (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetOptimization    (EXECUTION_CONTEXT* ec, BOOL               status   );

BOOL               WINAPI ec_SetExtReporting    (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetRecordEquity    (EXECUTION_CONTEXT* ec, BOOL               status   );

EXECUTION_CONTEXT* WINAPI ec_SetSuperContext    (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec      );
uint               WINAPI ec_SetThreadId        (EXECUTION_CONTEXT* ec, uint               id       );
HWND               WINAPI ec_SetHChart          (EXECUTION_CONTEXT* ec, HWND               hWnd     );
HWND               WINAPI ec_SetHChartWindow    (EXECUTION_CONTEXT* ec, HWND               hWnd     );

int                WINAPI ec_SetMqlError        (EXECUTION_CONTEXT* ec, int                error    );
int                WINAPI ec_SetDllError        (EXECUTION_CONTEXT* ec, int                error    );
//                        ...
int                WINAPI ec_SetDllWarning      (EXECUTION_CONTEXT* ec, int                error    );
//                        ...
BOOL               WINAPI ec_SetLogging         (EXECUTION_CONTEXT* ec, BOOL               status   );
const char*        WINAPI ec_SetCustomLogFile   (EXECUTION_CONTEXT* ec, const char*        fileName );


// master context getters (used by MQL4)
uint               WINAPI mec_Pid                (const EXECUTION_CONTEXT* ec);
ProgramType        WINAPI mec_ProgramType        (const EXECUTION_CONTEXT* ec);
const char*        WINAPI mec_ProgramName        (const EXECUTION_CONTEXT* ec);
ModuleType         WINAPI mec_ModuleType         (const EXECUTION_CONTEXT* ec);
const char*        WINAPI mec_ModuleName         (const EXECUTION_CONTEXT* ec);

LaunchType         WINAPI mec_LaunchType         (const EXECUTION_CONTEXT* ec);
CoreFunction       WINAPI mec_CoreFunction       (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI mec_InitCycle          (const EXECUTION_CONTEXT* ec);
InitializeReason   WINAPI mec_InitReason         (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI mec_UninitReason       (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI mec_InitFlags          (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI mec_DeinitFlags        (const EXECUTION_CONTEXT* ec);

const char*        WINAPI mec_Symbol             (const EXECUTION_CONTEXT* ec);
uint               WINAPI mec_Timeframe          (const EXECUTION_CONTEXT* ec);
uint               WINAPI mec_Digits             (const EXECUTION_CONTEXT* ec);
double             WINAPI mec_Point              (const EXECUTION_CONTEXT* ec);
//                        mec.rates
int                WINAPI mec_Bars               (const EXECUTION_CONTEXT* ec);
int                WINAPI mec_ChangedBars        (const EXECUTION_CONTEXT* ec);
int                WINAPI mec_UnchangedBars      (const EXECUTION_CONTEXT* ec);
uint               WINAPI mec_Ticks              (const EXECUTION_CONTEXT* ec);
datetime           WINAPI mec_LastTickTime       (const EXECUTION_CONTEXT* ec);
datetime           WINAPI mec_PrevTickTime       (const EXECUTION_CONTEXT* ec);
double             WINAPI mec_Bid                (const EXECUTION_CONTEXT* ec);
double             WINAPI mec_Ask                (const EXECUTION_CONTEXT* ec);

//                        mec.test
BOOL               WINAPI mec_Testing            (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI mec_VisualMode         (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI mec_Optimization       (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI mec_ExtReporting       (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI mec_RecordEquity       (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI mec_SuperContext       (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* const target);
EXECUTION_CONTEXT* WINAPI mec_lpSuperContext     (const EXECUTION_CONTEXT* ec);
uint               WINAPI mec_ThreadId           (const EXECUTION_CONTEXT* ec);
HWND               WINAPI mec_hChart             (const EXECUTION_CONTEXT* ec);
HWND               WINAPI mec_hChartWindow       (const EXECUTION_CONTEXT* ec);

int                WINAPI mec_MqlError           (const EXECUTION_CONTEXT* ec);
int                WINAPI mec_DllError           (const EXECUTION_CONTEXT* ec);
//                        ...
int                WINAPI mec_DllWarning         (const EXECUTION_CONTEXT* ec);
//                        ...
BOOL               WINAPI mec_Logging            (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI mec_CustomLogging      (const EXECUTION_CONTEXT* ec);
const char*        WINAPI mec_CustomLogFile      (const EXECUTION_CONTEXT* ec);


// helpers
const char*        WINAPI EXECUTION_CONTEXT_toStr  (const EXECUTION_CONTEXT* ec, BOOL outputDebug = FALSE);
const char*        WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug = FALSE);
