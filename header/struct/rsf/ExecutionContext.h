#pragma once
#include <vector>

typedef std::vector<string> LogBuffer;


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
   uint               pid;                            //         0        4     MQL program id, starting from 1                           (const)  index in g_mqlPrograms[]
   uint               previousPid;                    //         4        4     previous id of the program or NULL                        (const)
                                                      //
   ProgramType        programType;                    //         8        4     MQL program type                                          (const)  type of MQL program
   char               programName[MAX_FNAME];         //        12      256     MQL program name                                          (const)  MQL program name
   CoreFunction       programCoreFunction;            //       268        4     the program's current core function                       (var)    where is it
   InitializeReason   programInitReason;              //       272        4     last initialize reason                                    (var)
   UninitializeReason programUninitReason;            //       276        4     last MQL::UninitializeReason()                            (var)
   DWORD              programInitFlags;               //       280        4     program init configuration                                (const)  how should it be initialized
   DWORD              programDeinitFlags;             //       284        4     program deinit configuration                              (const)  how should it be deinitialized
                                                      //
   ModuleType         moduleType;                     //       288        4     MQL module type                                           (const)  type of MQL module
   char               moduleName[MAX_FNAME];          //       292      256     MQL module name = MQL::WindowExpertName()                 (const)  MQL module name
   CoreFunction       moduleCoreFunction;             //       548        4     the module's current core function                        (var)    where is it
   UninitializeReason moduleUninitReason;             //       552        4     last MQL::UninitializeReason()                            (var)
   DWORD              moduleInitFlags;                //       556        4     module init configuration                                 (const)  how should it be initialized
   DWORD              moduleDeinitFlags;              //       560        4     module deinit configuration                               (const)  how should it be deinitialized
                                                      //
   char               symbol[MAX_SYMBOL_LENGTH+1];    //       564       12     current chart symbol                                      (var)    MQL::Symbol()
   uint               timeframe;                      //       576        4     current chart timeframe                                   (var)    MQL::Period()
   char               newSymbol[MAX_SYMBOL_LENGTH+1]; //       580       12     new symbol set by Library::init() after IR_CHARTCHANGE    (var)
   uint               newTimeframe;                   //       592        4     new timeframe set by Library::init() after IR_CHARTCHANGE (var)
   const void*        rates;                          //       596        4     current price series from MQL::ArrayCopyRates()           (var)    HistoryBar400[]|HistoryBar401[]
   int                bars;                           //       600        4     current number of bars                                    (var)    MQL::Bars
   int                validBars;                      //       604        4     number of validBars bars                                  (var)    MQL::IndicatorCounted()
   int                changedBars;                    //       608        4     number of changed bars                                    (var)
   uint               ticks;                          //       612        4     number of times start() was called for the instance       (var)
   uint               cycleTicks;                     //       616        4     number of times start() was called since init()           (var)    used for debugging purposes in the DLL only
   time32             currTickTime;                   //       620        4     server time of the currently processed tick               (var)
   time32             prevTickTime;                   //       624        4     server time of the previously processed tick              (var)    used by MQL::IsBarOpen()
   BYTE               _alignment1[4];                 //       628        4     (alignment to the next double)
   double             bid;                            //       632        8     current bid price                                         (var)    MQL::Bid
   double             ask;                            //       640        8     current ask price                                         (var)    MQL::Ask
                                                      //
   uint               digits;                         //       648        4     digits of the symbol                                      (var)    MQL::Digits
   uint               pipDigits;                      //       652        4     digits of a pip                                           (var)
   double             pip;                            //       656        8     size of a pip                                             (var)
   double             point;                          //       664        8     size of a point                                           (var)    MQL::Point
   uint               pipPoints;                      //       672        4     number of points of a pip: 1 or 10                        (var)
                                                      //
   EXECUTION_CONTEXT* superContext;                   //       676        4     indicator host program                                    (const)  if loaded by iCustom()
   uint               threadId;                       //       680        4     current executing thread                                  (var)
   HWND               hChartWindow;                   //       684        4     handle of chart window (title bar "Symbol,Period")        (const)
   HWND               hChart;                         //       688        4     chart handle = MQL::WindowHandle()                        (const)  handle of the embedded chart frame
                                                      //
   BOOL               testing;                        //       692        4     IsTesting() status                                        (const)
   BOOL               visualMode;                     //       696        4     expert IsVisualMode() status                              (const)
   BOOL               optimization;                   //       700        4     expert IsOptimization() status                            (const)
   int                recorder;                       //       704        4     expert recorder mode                                      (var)
                                                      //
   const char*        accountServer;                  //       708        4     MQL::AccountServer()                                      (var)
   int                accountNumber;                  //       712        4     MQL::AccountNumber()                                      (var)
                                                      //
   int                mqlError;                       //       716        4     last MQL error (from all program modules)                 (var)
   int                dllError;                       //       720        4     last DLL error                                            (var)
   char*              dllErrorMsg;                    //       724        4     DLL error message                                         (var)
   int                dllWarning;                     //       728        4     last DLL warning                                          (var)
   char*              dllWarningMsg;                  //       732        4     DLL warning message                                       (var)
                                                      //
   int                loglevel;                       //       736        4     program main loglevel                                     (var)
   int                loglevelTerminal;               //       740        4     loglevel of the terminal log appender                     (var)
   int                loglevelAlert;                  //       744        4     loglevel of the terminal alert appender                   (var)
   int                loglevelDebug;                  //       748        4     loglevel of the debug output appender                     (var)
   int                loglevelFile;                   //       752        4     loglevel of the logfile appender                          (var)
   int                loglevelMail;                   //       756        4     loglevel of the mail appender                             (var)
   int                loglevelSMS;                    //       760        4     loglevel of the SMS appender                              (var)

   std::ofstream*     logger;                         //       764        4     logger instance                                           (var)
   LogBuffer*         logBuffer;                      //       768        4     log buffer                                                (var)
   char               logFilename[MAX_PATH];          //       772      260     logger filename                                           (var)
};                                                    // -------------------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                     //             = 1032


// getters expecting a struct (exported)
uint               WINAPI ec_Pid                 (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PreviousPid         (const EXECUTION_CONTEXT* ec);

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
int                WINAPI ec_ValidBars           (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_ChangedBars         (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Ticks               (const EXECUTION_CONTEXT* ec);
//                        ec.cycleTicks
time32             WINAPI ec_CurrTickTime        (const EXECUTION_CONTEXT* ec);
time32             WINAPI ec_PrevTickTime        (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Bid                 (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Ask                 (const EXECUTION_CONTEXT* ec);

uint               WINAPI ec_Digits              (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PipDigits           (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Pip                 (const EXECUTION_CONTEXT* ec);
double             WINAPI ec_Point               (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_PipPoints           (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_PriceFormat         (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_SuperContext        (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* const target);
uint               WINAPI ec_ThreadId            (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChartWindow        (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChart              (const EXECUTION_CONTEXT* ec);

BOOL               WINAPI ec_Testing             (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_VisualMode          (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Optimization        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_Recorder            (const EXECUTION_CONTEXT* ec);

const char*        WINAPI ec_AccountServer       (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_AccountNumber       (const EXECUTION_CONTEXT* ec);

int                WINAPI ec_MqlError            (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_DllError            (const EXECUTION_CONTEXT* ec);
//                        ec.dllErrorMsg
int                WINAPI ec_DllWarning          (const EXECUTION_CONTEXT* ec);
//                        ec.dllWarningMsg

int                WINAPI ec_Loglevel            (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelTerminal    (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelAlert       (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelDebug       (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelFile        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelMail        (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_LoglevelSMS         (const EXECUTION_CONTEXT* ec);
//                        ec.logger
//                        ec.logBuffer
const char*        WINAPI ec_LogFilename         (const EXECUTION_CONTEXT* ec);


// getters expecting a pid (exported)
const char*        WINAPI ec_SuperProgramName     (uint pid);

int                WINAPI ec_SuperLoglevel        (uint pid);
int                WINAPI ec_SuperLoglevelTerminal(uint pid);
int                WINAPI ec_SuperLoglevelAlert   (uint pid);
int                WINAPI ec_SuperLoglevelDebug   (uint pid);
int                WINAPI ec_SuperLoglevelFile    (uint pid);
int                WINAPI ec_SuperLoglevelMail    (uint pid);
int                WINAPI ec_SuperLoglevelSMS     (uint pid);


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
int                WINAPI ec_SetValidBars           (EXECUTION_CONTEXT* ec, int                count    );
int                WINAPI ec_SetChangedBars         (EXECUTION_CONTEXT* ec, int                count    );

uint               WINAPI ec_SetDigits              (EXECUTION_CONTEXT* ec, uint               digits   );
uint               WINAPI ec_SetPipDigits           (EXECUTION_CONTEXT* ec, uint               digits   );
double             WINAPI ec_SetPip                 (EXECUTION_CONTEXT* ec, double             size     );
double             WINAPI ec_SetPoint               (EXECUTION_CONTEXT* ec, double             size     );
uint               WINAPI ec_SetPipPoints           (EXECUTION_CONTEXT* ec, uint               points   );
//                        ec.priceFormat

EXECUTION_CONTEXT* WINAPI ec_SetSuperContext        (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec     );
uint               WINAPI ec_SetThreadId            (EXECUTION_CONTEXT* ec, uint               id      );
HWND               WINAPI ec_SetHChart              (EXECUTION_CONTEXT* ec, HWND               hWnd    );
HWND               WINAPI ec_SetHChartWindow        (EXECUTION_CONTEXT* ec, HWND               hWnd    );

BOOL               WINAPI ec_SetTesting             (EXECUTION_CONTEXT* ec, BOOL               status  );
BOOL               WINAPI ec_SetVisualMode          (EXECUTION_CONTEXT* ec, BOOL               status  );
BOOL               WINAPI ec_SetOptimization        (EXECUTION_CONTEXT* ec, BOOL               status  );
int                WINAPI ec_SetRecorder            (EXECUTION_CONTEXT* ec, int                mode    );

//                        ec.accountServer
//                        ec.accountNumber

int                WINAPI ec_SetMqlError            (EXECUTION_CONTEXT* ec, int                error   );
int                WINAPI ec_SetDllError            (EXECUTION_CONTEXT* ec, int                error   );
//                        ec.dllErrorMsg
int                WINAPI ec_SetDllWarning          (EXECUTION_CONTEXT* ec, int                error   );
//                        ec.dllWarningMsg

int                WINAPI ec_SetLoglevel            (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelTerminal    (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelAlert       (EXECUTION_CONTEXT* ec, int                level   );
int                WINAPI ec_SetLoglevelDebug       (EXECUTION_CONTEXT* ec, int                level   );
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
typedef std::vector<EXECUTION_CONTEXT*> ContextChain;       // Each chain holds the execution contexts of all modules of a single MQL program.
typedef std::vector<ContextChain*>      MqlInstanceList;    // List of all MQL program instances ever loaded (index = instance id aka pid).
