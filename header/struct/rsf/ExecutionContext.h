#pragma once
#include "struct/rsf/Test.h"
#include <vector>


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
 *  • Data exchange between MQL modules and the Expander DLL:
 *    The MQL modules pass MQL runtime state and market information to the DLL, the DLL passes back calculation results and
 *    DLL runtime state (MQL program management, errors).
 *
 *  • Data exchange between MQL modules of the same program.
 *
 *  • Data exchange between multiple MQL programs.
 */
#pragma pack(push, 1)

struct EXECUTION_CONTEXT {                         // -- offset --- size --- description --------------------------------------------------------------------------------------
   uint               pid;                         //         0        4     MQL program id starting from 1                      (const) => index in g_mqlPrograms[]
   uint               previousPid;                 //         4        4     previous pid of the program or NULL                 (const)
                                                   //
   ProgramType        programType;                 //         8        4     MQL program type                                    (const) => type of MQL program
   char               programName[MAX_FNAME];      //        12      256     MQL program name                                    (const) => MQL program name
   CoreFunction       programCoreFunction;         //       268        4     the program's current core function                 (var)   => where is it
   InitializeReason   programInitReason;           //       272        4     last initialize reason                              (var)
   UninitializeReason programUninitReason;         //       276        4     last MQL::UninitializeReason()                      (var)
   DWORD              programInitFlags;            //       280        4     program init configuration                          (const) => how should it be initialized
   DWORD              programDeinitFlags;          //       284        4     program deinit configuration                        (const) => how should it be deinitialized
                                                   //
   ModuleType         moduleType;                  //       288        4     MQL module type                                     (const) => type of MQL module
   char               moduleName[MAX_FNAME];       //       292      256     MQL module name = MQL::WindowExpertName()           (const) => MQL module name
   CoreFunction       moduleCoreFunction;          //       548        4     the module's current core function                  (var)   => where is it
   UninitializeReason moduleUninitReason;          //       552        4     last MQL::UninitializeReason()                      (var)
   DWORD              moduleInitFlags;             //       556        4     module init configuration                           (const) => how should it be initialized
   DWORD              moduleDeinitFlags;           //       560        4     module deinit configuration                         (const) => how should it be deinitialized
                                                   //
   char               symbol[MAX_SYMBOL_LENGTH+1]; //       564       12     current symbol           = MQL::Symbol()            (var)
   uint               timeframe;                   //       576        4     current chart period     = MQL::Period()            (var)
   const void*        rates;                       //       580        4     current price series     = MQL::ArrayCopyRates()    (var)   => RateInfo[] | MqlRates[]
   int                bars;                        //       584        4     current number of bars   = MQL::Bars                (var)
   int                changedBars;                 //       588        4     number of changed bars                              (var)
   int                unchangedBars;               //       592        4     number of unchanged bars = MQL::IndicatorCounted()  (var)
   uint               ticks;                       //       596        4     number of times start() was called for the instance (var)
   uint               cycleTicks;                  //       600        4     number of times start() was called for the cycle    (var)
   datetime           lastTickTime;                //       604        4     server time of the last received tick               (var)
   datetime           prevTickTime;                //       608        4     server time of the previous received tick           (var)
   BYTE               _alignment1[4];              //       612        4     (alignment to the next double)
   double             bid;                         //       616        8     current bid price        = MQL::Bid                 (var)
   double             ask;                         //       624        8     current ask price        = MQL::Ask                 (var)
                                                   //
   uint               digits;                      //       632        4     digits of the symbol     = MQL::Digits              (var)
   uint               pipDigits;                   //       636        4     digits of a pip                                     (var)
   uint               subPipDigits;                //       640        4     digits of a subpip                                  (var)
   BYTE               _alignment2[4];              //       644        4     (alignment to the next double)
   double             pip;                         //       648        8     size of a pip                                       (var)
   double             point;                       //       656        8     size of a point          = MQL::Point               (var)
   uint               pipPoints;                   //       664        4     number of points of a pip: 1 or 10                  (var)
   const char*        priceFormat;                 //       668        4     standard price format                               (var)
   const char*        pipPriceFormat;              //       672        4     pip price format (never subpips)                    (var)
   const char*        subPipPriceFormat;           //       676        4     subpip price format (always subpips)                (var)
                                                   //
   EXECUTION_CONTEXT* superContext;                //       680        4     indicator host program                              (const) => if indicator loaded by iCustom()
   uint               threadId;                    //       684        4     current executing thread                            (var)
   HWND               hChart;                      //       688        4     chart handle             = MQL::WindowHandle()      (const) => handle of the chart frame
   HWND               hChartWindow;                //       692        4     chart handle with title bar "Symbol,Period"         (const) => handle of the chart window
                                                   //
   TEST*              test;                        //       696        4     test configuration, data and results                (const)
   BOOL               testing;                     //       700        4     IsTesting() status                                  (const)
   BOOL               visualMode;                  //       704        4     expert IsVisualMode() status                        (const)
   BOOL               optimization;                //       708        4     expert IsOptimization() status                      (const)
                                                   //
   BOOL               extReporting;                //       712        4     expert input parameter EA.CreateReport              (var)
   BOOL               recordEquity;                //       716        4     expert input parameter EA.RecordEquity              (var)
                                                   //
   int                mqlError;                    //       720        4     last MQL error (from all program modules)           (var)
   int                dllError;                    //       724        4     last DLL error                                      (var)
   char*              dllErrorMsg;                 //       728        4     DLL error message                                   (var)
   int                dllWarning;                  //       732        4     last DLL warning                                    (var)
   char*              dllWarningMsg;               //       736        4     DLL warning message                                 (var)
   BOOL               logging;                     //       740        4     logging configuration                               (var)   => is logging enabled
   char               customLogFile[MAX_PATH];     //       744      260     custom log filename                                 (const) => custom log location
};                                                 // -------------------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //             = 1004


// getters (exported to MQL)
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

uint               WINAPI ec_Digits             (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PipDigits          (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_SubPipDigits       (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Pip                (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Point              (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PipPoints          (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_PriceFormat        (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_PipPriceFormat     (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_SubPipPriceFormat  (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_SuperContext       (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* const target);
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext     (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_ThreadId           (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChart             (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChartWindow       (const EXECUTION_CONTEXT* ec);

//                        ec.test
int                WINAPI ec_TestId             (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_TestCreated        (const EXECUTION_CONTEXT* ec);
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
//                        ec.rates
int                WINAPI ec_SetBars               (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetChangedBars        (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetUnchangedBars      (EXECUTION_CONTEXT* ec, int                count    );

uint               WINAPI ec_SetDigits             (EXECUTION_CONTEXT* ec, uint               digits   );
uint               WINAPI ec_SetPipDigits          (EXECUTION_CONTEXT* ec, uint               digits   );
uint               WINAPI ec_SetSubPipDigits       (EXECUTION_CONTEXT* ec, uint               digits   );
double             WINAPI ec_SetPip                (EXECUTION_CONTEXT* ec, double             size     );
double             WINAPI ec_SetPoint              (EXECUTION_CONTEXT* ec, double             size     );
uint               WINAPI ec_SetPipPoints          (EXECUTION_CONTEXT* ec, uint               points   );
//                        ec.priceFormat
//                        ec.pipPriceFormat
//                        ec.subPipPriceFormat

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


// type definitions
typedef std::vector<EXECUTION_CONTEXT*> ContextChain;       // a chain holds all execution contexts of a sinle MQL program
typedef std::vector<ContextChain*>      MqlProgramList;     // a list of MQL programs with index = pid (program id)
