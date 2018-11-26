#pragma once
#include "struct/rsf/Test.h"


/**
 * Framework struct EXECUTION_CONTEXT
 *
 * Execution context of an MQL program. Used for communication and data exchange between MQL modules and DLL.
 *
 * Die EXECUTION_CONTEXTe dienen dem Datenaustausch zwischen mehreren MQL-Programmen, zwischen einzelnen Modulen desselben
 * Programms und zwischen einem Programm und der DLL. Jedes MQL-Modul verf�gt �ber einen eigenen Kontext, alle Kontexte eines
 * MQL-Programms bilden gemeinsam eine Context-Chain. An erster Stelle einer Context-Chain liegt der Master-Context, der in der
 * DLL verwaltet wird. An zweiter Stelle liegt der Context des MQL-Hauptmodules (Expert, Script oder Indikator). Alle weiteren
 * Contexte einer Chain sind Library-Contexte. �ber die Kontexte werden wie folgt Daten ausgetauscht:
 *
 *  � Data exchange between MQL program main module and DLL:
 *    The DLL stores DLL error information in the EXECUTION_CONTEXT for further processing by the MQL main module. The main
 *    module signals such errors to the user. On the other hand the main module stores price and market information in the
 *    context to make it available to the DLL.
 *  � Data exchange between MQL main module and MQL libraries
 *  � Data exchange between multiple MQL programs
 */
#pragma pack(push, 1)

struct EXECUTION_CONTEXT {                         // -- offset --- size --- description --------------------------------------------------------------------------------------
   uint               pid;                         //         0        4     MQL program id starting from 1                      (const) => index in g_contextChains[]
   uint               previousPid;                 //         4        4     previous pid of the program or NULL                 (const)
                                                   //
   ProgramType        programType;                 //         8        4     MQL program type                                    (const) => type of MQL program
   char               programName[MAX_PATH];       //        12      260     MQL program name                                    (const) => MQL program name
   CoreFunction       programCoreFunction;         //       272        4     the program's current core function                 (var  ) => where is it
   InitializeReason   programInitReason;           //       276        4     last initialize reason                              (var  )
   UninitializeReason programUninitReason;         //       280        4     last MQL::UninitializeReason()                      (var  )
   DWORD              programInitFlags;            //       284        4     program init configuration                          (const) => how should it be initialized
   DWORD              programDeinitFlags;          //       288        4     program deinit configuration                        (const) => how should it be deinitialized
                                                   //
   ModuleType         moduleType;                  //       292        4     MQL module type                                     (const) => type of MQL module
   char               moduleName[MAX_PATH];        //       296      260     MQL module name = MQL::WindowExpertName()           (const) => MQL module name
   CoreFunction       moduleCoreFunction;          //       556        4     the module's current core function                  (var  ) => where is it
   UninitializeReason moduleUninitReason;          //       560        4     last MQL::UninitializeReason()                      (var  )
   DWORD              moduleInitFlags;             //       564        4     module init configuration                           (const) => how should it be initialized
   DWORD              moduleDeinitFlags;           //       568        4     module deinit configuration                         (const) => how should it be deinitialized
                                                   //
   char               symbol[MAX_SYMBOL_LENGTH+1]; //       572       12     current symbol         = MQL::Symbol()              (var  )
   uint               timeframe;                   //       584        4     current chart period   = MQL::Period()              (var  )
   uint               digits;                      //       588        4     Digits of the symbol   = MQL::Digits                (var  )
   double             point;                       //       592        8     Point of the symbol    = MQL::Point                 (var  )
   const void*        rates;                       //       600        4     current price series   = MQL::ArrayCopyRates()      (var  ) => RateInfo[]
   int                bars;                        //       604        4     current number of bars = MQL::Bars                  (var  )
   int                changedBars;                 //       608        4     number of changed bars                              (var  )
   int                unchangedBars;               //       612        4     number of unchanged bars = MQL::IndicatorCounted()  (var  )
   uint               ticks;                       //       616        4     number of times start() was called for the instance (var  )
   uint               cycleTicks;                  //       620        4     number of times start() was called for the cycle    (var  )
   datetime           lastTickTime;                //       624        4     server time of the last received tick               (var  )
   datetime           prevTickTime;                //       628        4     server time of the previous received tick           (var  )
   double             bid;                         //       632        8     current bid price      = MQL::Bid                   (var  )
   double             ask;                         //       640        8     current ask price      = MQL::Ask                   (var  )
                                                   //
   EXECUTION_CONTEXT* superContext;                //       648        4     indicator host program                              (const) => if indicator loaded by iCustom()
   uint               threadId;                    //       652        4     current executing thread                            (var  )
   HWND               hChart;                      //       656        4     chart handle = MQL::WindowHandle()                  (const) => handle of the chart frame
   HWND               hChartWindow;                //       660        4     chart handle with title bar "Symbol,Period"         (const) => handle of the chart window
                                                   //
   TEST*              test;                        //       664        4     test configuration and data                         (const)
   BOOL               testing;                     //       668        4     IsTesting() status                                  (const)
   BOOL               visualMode;                  //       672        4     expert IsVisualMode() status                        (const)
   BOOL               optimization;                //       676        4     expert IsOptimization() status                      (const)
                                                   //
   BOOL               extReporting;                //       680        4     expert input parameter EA.ExtReporting              (var  )
   BOOL               recordEquity;                //       684        4     expert input parameter EA.RecordEquity              (var  )
                                                   //
   int                mqlError;                    //       688        4     last error in MQL (main module and libraries)       (var  )
   int                dllError;                    //       692        4     last error in DLL                                   (var  )
   char*              dllErrorMsg;                 //       696        4     DLL error message                                   (var  )
   int                dllWarning;                  //       700        4     last DLL warning                                    (var  )
   char*              dllWarningMsg;               //       704        4     DLL warning message                                 (var  )
   BOOL               logging;                     //       708        4     logging configuration                               (var  ) => is logging enabled
   char               customLogFile[MAX_PATH];     //       712      260     custom log filename                                 (const) => custom log location
};                                                 // -------------------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //              = 972


// type definition
typedef std::vector<EXECUTION_CONTEXT*> ContextChain;                // the contexts of a single MQL program (one context per MQL module)


// regular getters (exported to MQL)
uint               WINAPI ec_Pid                (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PreviousPid        (const EXECUTION_CONTEXT* ec);

ProgramType        WINAPI ec_ProgramType        (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ProgramName        (const EXECUTION_CONTEXT* ec);
CoreFunction       WINAPI ec_ProgramCoreFunction(const EXECUTION_CONTEXT* ec);
InitializeReason   WINAPI ec_ProgramInitReason  (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_ProgramUninitReason(const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_ProgramInitFlags   (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_ProgramDeinitFlags (const EXECUTION_CONTEXT* ec);

ModuleType         WINAPI ec_ModuleType         (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ModuleName         (const EXECUTION_CONTEXT* ec);
CoreFunction       WINAPI ec_ModuleCoreFunction (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_ModuleUninitReason (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_ModuleInitFlags    (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_ModuleDeinitFlags  (const EXECUTION_CONTEXT* ec);

const char*        WINAPI ec_Symbol             (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Timeframe          (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Digits             (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Point              (const EXECUTION_CONTEXT* ec);
//                        ec.rates
int                WINAPI ec_Bars               (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_ChangedBars        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_UnchangedBars      (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Ticks              (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_cycleTicks         (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_LastTickTime       (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_PrevTickTime       (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Bid                (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Ask                (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_SuperContext       (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* const target);
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext     (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_ThreadId           (const EXECUTION_CONTEXT* ec);
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
BOOL               WINAPI ec_VisualMode         (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Optimization       (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_ExtReporting       (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_RecordEquity       (const EXECUTION_CONTEXT* ec);

int                WINAPI ec_MqlError           (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_DllError           (const EXECUTION_CONTEXT* ec);
//                        ec.dllErrorMsg
int                WINAPI ec_DllWarning         (const EXECUTION_CONTEXT* ec);
//                        ec.dllWarningMsg
BOOL               WINAPI ec_Logging            (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_CustomLogging      (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_CustomLogFile      (const EXECUTION_CONTEXT* ec);


// validating setters
ProgramType        WINAPI ec_SetProgramType        (EXECUTION_CONTEXT* ec, ProgramType        type     );
const char*        WINAPI ec_SetProgramName        (EXECUTION_CONTEXT* ec, const char*        name     );
CoreFunction       WINAPI ec_SetProgramCoreFunction(EXECUTION_CONTEXT* ec, CoreFunction       id       );
InitializeReason   WINAPI ec_SetProgramInitReason  (EXECUTION_CONTEXT* ec, InitializeReason   reason   );
UninitializeReason WINAPI ec_SetProgramUninitReason(EXECUTION_CONTEXT* ec, UninitializeReason reason   );
DWORD              WINAPI ec_SetProgramInitFlags   (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetProgramDeinitFlags (EXECUTION_CONTEXT* ec, DWORD              flags    );

ModuleType         WINAPI ec_SetModuleType         (EXECUTION_CONTEXT* ec, ModuleType         type     );
const char*        WINAPI ec_SetModuleName         (EXECUTION_CONTEXT* ec, const char*        name     );
CoreFunction       WINAPI ec_SetModuleCoreFunction (EXECUTION_CONTEXT* ec, CoreFunction       id       );
UninitializeReason WINAPI ec_SetModuleUninitReason (EXECUTION_CONTEXT* ec, UninitializeReason reason   );
DWORD              WINAPI ec_SetModuleInitFlags    (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetModuleDeinitFlags  (EXECUTION_CONTEXT* ec, DWORD              flags    );

const char*        WINAPI ec_SetSymbol             (EXECUTION_CONTEXT* ec, const char*        symbol   );
uint               WINAPI ec_SetTimeframe          (EXECUTION_CONTEXT* ec, uint               timeframe);
uint               WINAPI ec_SetDigits             (EXECUTION_CONTEXT* ec, uint               digits   );
double             WINAPI ec_SetPoint              (EXECUTION_CONTEXT* ec, double             point    );
//                        ec.rates
int                WINAPI ec_SetBars               (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetChangedBars        (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetUnchangedBars      (EXECUTION_CONTEXT* ec, int                count    );

EXECUTION_CONTEXT* WINAPI ec_SetSuperContext       (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec      );
uint               WINAPI ec_SetThreadId           (EXECUTION_CONTEXT* ec, uint               id       );
HWND               WINAPI ec_SetHChart             (EXECUTION_CONTEXT* ec, HWND               hWnd     );
HWND               WINAPI ec_SetHChartWindow       (EXECUTION_CONTEXT* ec, HWND               hWnd     );

//                        ec.test
BOOL               WINAPI ec_SetTesting            (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetVisualMode         (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetOptimization       (EXECUTION_CONTEXT* ec, BOOL               status   );

BOOL               WINAPI ec_SetExtReporting       (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetRecordEquity       (EXECUTION_CONTEXT* ec, BOOL               status   );

int                WINAPI ec_SetMqlError           (EXECUTION_CONTEXT* ec, int                error    );
int                WINAPI ec_SetDllError           (EXECUTION_CONTEXT* ec, int                error    );
//                        ...
int                WINAPI ec_SetDllWarning         (EXECUTION_CONTEXT* ec, int                error    );
//                        ...
BOOL               WINAPI ec_SetLogging            (EXECUTION_CONTEXT* ec, BOOL               status   );
const char*        WINAPI ec_SetCustomLogFile      (EXECUTION_CONTEXT* ec, const char*        fileName );


// helpers
const char*        WINAPI EXECUTION_CONTEXT_toStr  (const EXECUTION_CONTEXT* ec, BOOL outputDebug = FALSE);
const char*        WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug = FALSE);