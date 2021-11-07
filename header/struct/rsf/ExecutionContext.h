#pragma once
#include "struct/rsf/Test.h"
#include <vector>

typedef std::vector<string*> LogBuffer;


#pragma pack(push, 1)
/**
 * Framework struct EXECUTION_CONTEXT
 *
 * Execution context of an MQL program. Used for keeping state, data exchange and communication between MQL modules and DLL.
 *
 * Die EXECUTION_CONTEXTe dienen dem Datenaustausch zwischen mehreren MQL-Programmen, zwischen einzelnen Modulen desselben
 * Programms und zwischen einem Programm und der DLL. Jedes MQL-Modul verfügt über einen eigenen Kontext, alle Kontexte eines
 * MQL-Programms bilden gemeinsam eine Context-Chain. An erster Stelle einer Context-Chain liegt der Master-Context, der in der
 * DLL verwaltet wird. An zweiter Stelle liegt der Context des MQL-Hauptmodules (Expert, Script oder Indikator). Alle weiteren
 * Contexte einer Chain sind Library-Contexte. Über die Kontexte werden wie folgt Daten ausgetauscht:
 *
 *  • Data exchange between MQL modules and the DLL:
 *    The MQL modules pass MQL runtime state and market information to the DLL, the DLL passes back DLL runtime state and
 *    calculation results.
 *  • Data exchange between MQL modules of the same program (i.e. MQL main and library modules).
 *  • Data exchange between different MQL programs (e.g. experts and indicators).
 */
struct EXECUTION_CONTEXT {                            // -- offset --- size --- description --------------------------------------------------------------------------------------
   uint               pid;                            //         0        4     MQL program id starting from 1                            (const) => index in g_mqlPrograms[]
   uint               previousPid;                    //         4        4     previous pid of the program or NULL                       (const)
   datetime           started;                        //         8        4     GMT time the MQL program was started                      (const)
                                                      //
   ProgramType        programType;                    //        12        4     MQL program type                                          (const) => type of MQL program
   char               programName[MAX_FNAME];         //        16      256     MQL program name                                          (const) => MQL program name
   CoreFunction       programCoreFunction;            //       272        4     the program's current core function                       (var)   => where is it
   InitializeReason   programInitReason;              //       276        4     last initialize reason                                    (var)
   UninitializeReason programUninitReason;            //       280        4     last MQL::UninitializeReason()                            (var)
   DWORD              programInitFlags;               //       284        4     program init configuration                                (const) => how should it be initialized
   DWORD              programDeinitFlags;             //       288        4     program deinit configuration                              (const) => how should it be deinitialized
                                                      //
   ModuleType         moduleType;                     //       292        4     MQL module type                                           (const) => type of MQL module
   char               moduleName[MAX_FNAME];          //       296      256     MQL module name = MQL::WindowExpertName()                 (const) => MQL module name
   CoreFunction       moduleCoreFunction;             //       552        4     the module's current core function                        (var)   => where is it
   UninitializeReason moduleUninitReason;             //       556        4     last MQL::UninitializeReason()                            (var)
   DWORD              moduleInitFlags;                //       560        4     module init configuration                                 (const) => how should it be initialized
   DWORD              moduleDeinitFlags;              //       564        4     module deinit configuration                               (const) => how should it be deinitialized
                                                      //
   char               symbol[MAX_SYMBOL_LENGTH+1];    //       568       12     current chart symbol     = MQL::Symbol()                  (var)
   uint               timeframe;                      //       580        4     current chart timeframe  = MQL::Period()                  (var)
   char               newSymbol[MAX_SYMBOL_LENGTH+1]; //       584       12     new symbol set by Library::init() after IR_CHARTCHANGE    (var)
   uint               newTimeframe;                   //       596        4     new timeframe set by Library::init() after IR_CHARTCHANGE (var)
   const void*        rates;                          //       600        4     current price series     = MQL::ArrayCopyRates()          (var)   => HistoryBar400[]|HistoryBar401[]
   int                bars;                           //       604        4     current number of bars   = MQL::Bars                      (var)
   int                changedBars;                    //       608        4     number of changed bars                                    (var)
   int                unchangedBars;                  //       612        4     number of unchanged bars = MQL::IndicatorCounted()        (var)
   uint               ticks;                          //       616        4     number of times start() was called for the instance       (var)
   uint               cycleTicks;                     //       620        4     number of times start() was called for the cycle          (var)
   datetime           prevTickTime;                   //       624        4     server time of the previously processed tick              (var)
   datetime           currTickTime;                   //       628        4     server time of the currently processed tick               (var)
   double             bid;                            //       632        8     current bid price        = MQL::Bid                       (var)
   double             ask;                            //       640        8     current ask price        = MQL::Ask                       (var)
                                                      //
   uint               digits;                         //       648        4     digits of the symbol     = MQL::Digits                    (var)
   uint               pipDigits;                      //       652        4     digits of a pip                                           (var)
   uint               subPipDigits;                   //       656        4     digits of a subpip                                        (var)
   BYTE               _alignment2[4];                 //       660        4     (alignment to the next double)
   double             pip;                            //       664        8     size of a pip                                             (var)
   double             point;                          //       672        8     size of a point          = MQL::Point                     (var)
   uint               pipPoints;                      //       680        4     number of points of a pip: 1 or 10                        (var)
   const char*        priceFormat;                    //       684        4     standard price format                                     (var)
   const char*        pipPriceFormat;                 //       688        4     pip price format (never subpips)                          (var)
   const char*        subPipPriceFormat;              //       692        4     subpip price format (always subpips)                      (var)
                                                      //
   EXECUTION_CONTEXT* superContext;                   //       696        4     indicator host program                                    (const) => if loaded by iCustom()
   uint               threadId;                       //       700        4     current executing thread                                  (var)
   HWND               hChart;                         //       704        4     chart handle             = MQL::WindowHandle()            (const) => handle of the chart frame
   HWND               hChartWindow;                   //       708        4     chart handle with title bar "Symbol,Period"               (const) => handle of the chart window
                                                      //
   TEST*              test;                           //       712        4     test configuration, data and results                      (const)
   BOOL               testing;                        //       716        4     IsTesting() status                                        (const)
   BOOL               visualMode;                     //       720        4     expert IsVisualMode() status                              (const)
   BOOL               optimization;                   //       724        4     expert IsOptimization() status                            (const)
                                                      //
   BOOL               extReporting;                   //       728        4     expert input parameter EA.CreateReport                    (var)
   BOOL               recordEquity;                   //       732        4     expert input parameter EA.RecordEquity                    (var)
                                                      //
   int                mqlError;                       //       736        4     last MQL error (from all program modules)                 (var)
   int                dllError;                       //       740        4     last DLL error                                            (var)
   char*              dllErrorMsg;                    //       744        4     DLL error message                                         (var)
   int                dllWarning;                     //       748        4     last DLL warning                                          (var)
   char*              dllWarningMsg;                  //       752        4     DLL warning message                                       (var)
                                                      //
   int                loglevel;                       //       756        4     program main loglevel                                     (var)
   int                loglevelTerminal;               //       760        4     loglevel of the terminal log appender                     (var)
   int                loglevelAlert;                  //       764        4     loglevel of the terminal alert appender                   (var)
   int                loglevelDebugger;               //       768        4     loglevel of the debug output appender                     (var)
   int                loglevelFile;                   //       772        4     loglevel of the logfile appender                          (var)
   int                loglevelMail;                   //       776        4     loglevel of the mail appender                             (var)
   int                loglevelSMS;                    //       780        4     loglevel of the SMS appender                              (var)

   std::ofstream*     logger;                         //       784        4     logger instance                                           (var)
   LogBuffer*         logBuffer;                      //       788        4     log buffer                                                (var)
   char               logFilename[MAX_PATH];          //       792      260     logger filename                                           (var)
};                                                    // -------------------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                     //             = 1052


// getters (exported to MQL)
uint               WINAPI ec_Pid                 (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PreviousPid         (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_Started             (const EXECUTION_CONTEXT* ec);

ProgramType        WINAPI ec_ProgramType         (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ProgramName         (const EXECUTION_CONTEXT* ec);
CoreFunction       WINAPI ec_ProgramCoreFunction (const EXECUTION_CONTEXT* ec);
InitializeReason   WINAPI ec_ProgramInitReason   (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_ProgramUninitReason (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_ProgramInitFlags    (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_ProgramDeinitFlags  (const EXECUTION_CONTEXT* ec);

ModuleType         WINAPI ec_ModuleType          (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ModuleName          (const EXECUTION_CONTEXT* ec);
CoreFunction       WINAPI ec_ModuleCoreFunction  (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_ModuleUninitReason  (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_ModuleInitFlags     (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_ModuleDeinitFlags   (const EXECUTION_CONTEXT* ec);

const char*        WINAPI ec_Symbol              (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Timeframe           (const EXECUTION_CONTEXT* ec);
//                        ec.newSymbol
//                        ec.newTimeframe
//                        ec.rates
int                WINAPI ec_Bars                (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_ChangedBars         (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_UnchangedBars       (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Ticks               (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_cycleTicks          (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_PrevTickTime        (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_CurrTickTime        (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Bid                 (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Ask                 (const EXECUTION_CONTEXT* ec);

uint               WINAPI ec_Digits              (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PipDigits           (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_SubPipDigits        (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Pip                 (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Point               (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PipPoints           (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_PriceFormat         (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_PipPriceFormat      (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_SubPipPriceFormat   (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_SuperContext        (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* const target);
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext      (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_ThreadId            (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChart              (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChartWindow        (const EXECUTION_CONTEXT* ec);

//                        ec.test
int                WINAPI ec_TestId              (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_TestCreated         (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_TestStartTime       (const EXECUTION_CONTEXT* ec);
datetime           WINAPI ec_TestEndTime         (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_TestBarModel        (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_TestBars            (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_TestTicks           (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_TestSpread          (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_TestTradeDirections (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_TestReportId        (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_TestReportSymbol    (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Testing             (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_VisualMode          (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Optimization        (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_ExtReporting        (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_RecordEquity        (const EXECUTION_CONTEXT* ec);

int                WINAPI ec_MqlError            (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_DllError            (const EXECUTION_CONTEXT* ec);
//                        ec.dllErrorMsg
int                WINAPI ec_DllWarning          (const EXECUTION_CONTEXT* ec);
//                        ec.dllWarningMsg

int                WINAPI ec_Loglevel            (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelTerminal    (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelAlert       (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelDebugger    (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelFile        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelMail        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelSMS         (const EXECUTION_CONTEXT* ec);
//                        ec.logger
//                        ec.logBuffer
const char*        WINAPI ec_LogFilename         (const EXECUTION_CONTEXT* ec);


// validating setters
ProgramType        WINAPI ec_SetProgramType         (EXECUTION_CONTEXT* ec, ProgramType        type     );
const char*        WINAPI ec_SetProgramName         (EXECUTION_CONTEXT* ec, const char*        name     );
CoreFunction       WINAPI ec_SetProgramCoreFunction (EXECUTION_CONTEXT* ec, CoreFunction       id       );
InitializeReason   WINAPI ec_SetProgramInitReason   (EXECUTION_CONTEXT* ec, InitializeReason   reason   );
UninitializeReason WINAPI ec_SetProgramUninitReason (EXECUTION_CONTEXT* ec, UninitializeReason reason   );
DWORD              WINAPI ec_SetProgramInitFlags    (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetProgramDeinitFlags  (EXECUTION_CONTEXT* ec, DWORD              flags    );

ModuleType         WINAPI ec_SetModuleType          (EXECUTION_CONTEXT* ec, ModuleType         type     );
const char*        WINAPI ec_SetModuleName          (EXECUTION_CONTEXT* ec, const char*        name     );
CoreFunction       WINAPI ec_SetModuleCoreFunction  (EXECUTION_CONTEXT* ec, CoreFunction       id       );
UninitializeReason WINAPI ec_SetModuleUninitReason  (EXECUTION_CONTEXT* ec, UninitializeReason reason   );
DWORD              WINAPI ec_SetModuleInitFlags     (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetModuleDeinitFlags   (EXECUTION_CONTEXT* ec, DWORD              flags    );

const char*        WINAPI ec_SetSymbol              (EXECUTION_CONTEXT* ec, const char*        symbol   );
uint               WINAPI ec_SetTimeframe           (EXECUTION_CONTEXT* ec, uint               timeframe);
//                        ec.newSymbol
//                        ec.newTimeframe
//                        ec.rates
int                WINAPI ec_SetBars                (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetChangedBars         (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetUnchangedBars       (EXECUTION_CONTEXT* ec, int                count    );

uint               WINAPI ec_SetDigits              (EXECUTION_CONTEXT* ec, uint               digits   );
uint               WINAPI ec_SetPipDigits           (EXECUTION_CONTEXT* ec, uint               digits   );
uint               WINAPI ec_SetSubPipDigits        (EXECUTION_CONTEXT* ec, uint               digits   );
double             WINAPI ec_SetPip                 (EXECUTION_CONTEXT* ec, double             size     );
double             WINAPI ec_SetPoint               (EXECUTION_CONTEXT* ec, double             size     );
uint               WINAPI ec_SetPipPoints           (EXECUTION_CONTEXT* ec, uint               points   );
//                        ec.priceFormat
//                        ec.pipPriceFormat
//                        ec.subPipPriceFormat

EXECUTION_CONTEXT* WINAPI ec_SetSuperContext        (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec     );
uint               WINAPI ec_SetThreadId            (EXECUTION_CONTEXT* ec, uint               id      );
HWND               WINAPI ec_SetHChart              (EXECUTION_CONTEXT* ec, HWND               hWnd    );
HWND               WINAPI ec_SetHChartWindow        (EXECUTION_CONTEXT* ec, HWND               hWnd    );

//                        ec.test
BOOL               WINAPI ec_SetTesting             (EXECUTION_CONTEXT* ec, BOOL               status  );
BOOL               WINAPI ec_SetVisualMode          (EXECUTION_CONTEXT* ec, BOOL               status  );
BOOL               WINAPI ec_SetOptimization        (EXECUTION_CONTEXT* ec, BOOL               status  );

BOOL               WINAPI ec_SetExtReporting        (EXECUTION_CONTEXT* ec, BOOL               status  );
BOOL               WINAPI ec_SetRecordEquity        (EXECUTION_CONTEXT* ec, BOOL               status  );

int                WINAPI ec_SetMqlError            (EXECUTION_CONTEXT* ec, int                error   );
int                WINAPI ec_SetDllError            (EXECUTION_CONTEXT* ec, int                error   );
//                        ec.dllErrorMsg
int                WINAPI ec_SetDllWarning          (EXECUTION_CONTEXT* ec, int                error   );
//                        ec.dllWarningMsg

int                WINAPI ec_SetLoglevel            (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelTerminal    (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelAlert       (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelDebugger    (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelFile        (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelMail        (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelSMS         (EXECUTION_CONTEXT* ec, int                level   );
//                        ec.logger
//                        ec.logBuffer
const char*        WINAPI ec_SetLogFilename         (EXECUTION_CONTEXT* ec, const char*        filename);


// helpers
const char*        WINAPI EXECUTION_CONTEXT_toStr  (const EXECUTION_CONTEXT* ec);
const char*        WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec);


// type definitions
typedef std::vector<EXECUTION_CONTEXT*> ContextChain;       // a chain holds all execution contexts of a single MQL program
typedef std::vector<ContextChain*>      MqlProgramList;     // list of all MQL programs ever loaded (index: program id)
