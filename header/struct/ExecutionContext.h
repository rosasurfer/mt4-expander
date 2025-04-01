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
   uint               previousPid;                    //         4        4     previous pid of an indicator after IR_CHARTCHANGE         (var)
                                                      //
   ProgramType        programType;                    //         8        4     type of the MQL main module                               (const)
   char               programName[MAX_FNAME];         //        12      256     name of the MQL main module                               (const)
   CoreFunction       programCoreFunction;            //       268        4     current core function of the MQL main module              (var)    where is it
   InitializeReason   programInitReason;              //       272        4     last program initialize reason                            (var)
   UninitializeReason programUninitReason;            //       276        4     last program uninitialize reason                          (var)
   DWORD              programInitFlags;               //       280        4     main module init configuration                            (const)  which init tasks to execute
   DWORD              programDeinitFlags;             //       284        4     main module deinit configuration                          (const)  which deinit tasks to execute
                                                      //
   ModuleType         moduleType;                     //       288        4     type of the current MQL module                            (const)
   char               moduleName[MAX_FNAME];          //       292      256     name of the current MQL module                            (const)  MQL::WindowExpertName()
   CoreFunction       moduleCoreFunction;             //       548        4     the module's current core function                        (var)    where is it
   UninitializeReason moduleUninitReason;             //       552        4     last module uninitialize reason                           (var)    MQL::UninitializeReason()
   DWORD              moduleInitFlags;                //       556        4     current module init configuration                         (const)  which init tasks to execute
   DWORD              moduleDeinitFlags;              //       560        4     current module deinit configuration                       (const)  which deinit tasks to execute
                                                      //
   char               symbol[MAX_SYMBOL_LENGTH+1];    //       564       12     current chart symbol                                      (var)    MQL::Symbol()
   uint               timeframe;                      //       576        4     current chart timeframe                                   (var)    MQL::Period()
   const void*        rates;                          //       580        4     current price series from MQL::ArrayCopyRates()           (var)    HistoryBar[400|401][]
   int                bars;                           //       584        4     current number of chart bars                              (var)    MQL::Bars
   int                validBars;                      //       588        4     number of validBars bars                                  (var)    MQL::IndicatorCounted()
   int                changedBars;                    //       592        4     number of changed bars                                    (var)
   uint               ticks;                          //       596        4     number of times start() was called for the instance       (var)
   uint               cycleTicks;                     //       600        4     number of times start() was called since init()           (var)    used for debugging purposes in DLL only
   time32             currTick;                       //       604        4     server time of the current tick                           (var)
   BOOL               currReal;                       //       608        4     whether the current tick is non-virtual                   (var)
   time32             prevTick;                       //       612        4     server time of the previous tick                          (var)
   BOOL               prevReal;                       //       616        4     whether the previous tick was non-virtual                 (var)
   time32             lastRealTick;                   //       620        4     server time of the last non-virtual tick                  (var)    used by IsBarOpen()
                                                      //
   uint               digits;                         //       624        4     digits of the symbol                                      (var)    MQL::Digits
   uint               pipDigits;                      //       628        4     digits of a pip                                           (var)
   double             pip;                            //       632        8     size of a pip                                             (var)
   double             point;                          //       640        8     size of a point                                           (var)    MQL::Point
                                                      //
   EXECUTION_CONTEXT* superContext;                   //       648        4     indicator host program                                    (const)  whether loaded by iCustom()
   uint               threadId;                       //       652        4     current executing thread                                  (var)
   HWND               chartWindow;                    //       656        4     handle of chart window with title bar "Symbol,Period"     (const)
   HWND               chart;                          //       660        4     handle of embedded chart AfxFrame                         (const)  MQL::WindowHandle()
                                                      //
   BOOL               testing;                        //       664        4     IsTesting() status                                        (const)
   BOOL               visualMode;                     //       668        4     expert IsVisualMode() status                              (const)
   BOOL               optimization;                   //       672        4     expert IsOptimization() status                            (const)
   int                recorder;                       //       676        4     expert recorder mode                                      (var)
                                                      //
   char*              accountServer;                  //       680        4     MQL::AccountServer()                                      (var)
   int                accountNumber;                  //       684        4     MQL::AccountNumber()                                      (var)
                                                      //
   int                dllWarning;                     //       688        4     last DLL warning                                          (var)
   char*              dllWarningMsg;                  //       692        4     DLL warning message                                       (var)
   int                dllError;                       //       696        4     last DLL error                                            (var)
   char*              dllErrorMsg;                    //       700        4     DLL error message                                         (var)
   int                mqlError;                       //       704        4     last MQL error (of all MQL modules)                       (var)
                                                      //
   DWORD              debugOptions;                   //       708        4     specified CLI debug options                               (var)
   int                loglevel;                       //       712        4     program main loglevel                                     (var)
   int                loglevelDebug;                  //       716        4     loglevel of the debug output appender                     (var)
   int                loglevelTerminal;               //       720        4     loglevel of the terminal log appender                     (var)
   int                loglevelAlert;                  //       724        4     loglevel of the terminal alert appender                   (var)
   int                loglevelFile;                   //       728        4     loglevel of the custom logfile appender                   (var)
   int                loglevelMail;                   //       732        4     loglevel of the mail appender                             (var)
   int                loglevelSMS;                    //       736        4     loglevel of the SMS appender                              (var)

   std::ofstream*     logger;                         //       740        4     logger instance                                           (var)
   LogBuffer*         logBuffer;                      //       744        4     log buffer                                                (var)
   char*              logFilename;                    //       748        4     log filename                                              (var)
};                                                    // -------------------------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                     //              = 752


// exported getters
const char*        WINAPI ec_ProgramName(const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_SuperProgramName     (uint pid);
int                WINAPI ec_SuperLoglevel        (uint pid);
int                WINAPI ec_SuperLoglevelDebug   (uint pid);
int                WINAPI ec_SuperLoglevelTerminal(uint pid);
int                WINAPI ec_SuperLoglevelAlert   (uint pid);
int                WINAPI ec_SuperLoglevelFile    (uint pid);
int                WINAPI ec_SuperLoglevelMail    (uint pid);
int                WINAPI ec_SuperLoglevelSMS     (uint pid);


// validating setters
ProgramType        WINAPI ec_SetProgramType         (EXECUTION_CONTEXT* ec, ProgramType        type  );
const char*        WINAPI ec_SetProgramName         (EXECUTION_CONTEXT* ec, const char*        name  );
CoreFunction       WINAPI ec_SetProgramCoreFunction (EXECUTION_CONTEXT* ec, CoreFunction       id    );
InitializeReason   WINAPI ec_SetProgramInitReason   (EXECUTION_CONTEXT* ec, InitializeReason   reason);
UninitializeReason WINAPI ec_SetProgramUninitReason (EXECUTION_CONTEXT* ec, UninitializeReason reason);
DWORD              WINAPI ec_SetProgramInitFlags    (EXECUTION_CONTEXT* ec, DWORD              flags );
DWORD              WINAPI ec_SetProgramDeinitFlags  (EXECUTION_CONTEXT* ec, DWORD              flags );

ModuleType         WINAPI ec_SetModuleType          (EXECUTION_CONTEXT* ec, ModuleType         type  );
const char*        WINAPI ec_SetModuleName          (EXECUTION_CONTEXT* ec, const char*        name  );
CoreFunction       WINAPI ec_SetModuleCoreFunction  (EXECUTION_CONTEXT* ec, CoreFunction       id    );
UninitializeReason WINAPI ec_SetModuleUninitReason  (EXECUTION_CONTEXT* ec, UninitializeReason reason);
DWORD              WINAPI ec_SetModuleInitFlags     (EXECUTION_CONTEXT* ec, DWORD              flags );
DWORD              WINAPI ec_SetModuleDeinitFlags   (EXECUTION_CONTEXT* ec, DWORD              flags );

const char*        WINAPI ec_SetSymbol              (EXECUTION_CONTEXT* ec, const char* symbol);
uint               WINAPI ec_SetTimeframe           (EXECUTION_CONTEXT* ec, uint timeframe);
//                        ec.rates
int                WINAPI ec_SetBars                (EXECUTION_CONTEXT* ec, int bars);
int                WINAPI ec_SetValidBars           (EXECUTION_CONTEXT* ec, int validBars);
int                WINAPI ec_SetChangedBars         (EXECUTION_CONTEXT* ec, int changedBars);

uint               WINAPI ec_SetDigits              (EXECUTION_CONTEXT* ec, uint digits);
uint               WINAPI ec_SetPipDigits           (EXECUTION_CONTEXT* ec, uint digits);
double             WINAPI ec_SetPip                 (EXECUTION_CONTEXT* ec, double size);
double             WINAPI ec_SetPoint               (EXECUTION_CONTEXT* ec, double size);

EXECUTION_CONTEXT* WINAPI ec_SetSuperContext        (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec);
uint               WINAPI ec_SetThreadId            (EXECUTION_CONTEXT* ec, uint id);
HWND               WINAPI ec_SetChartWindow         (EXECUTION_CONTEXT* ec, HWND hWnd);
HWND               WINAPI ec_SetChart               (EXECUTION_CONTEXT* ec, HWND hWnd);

BOOL               WINAPI ec_SetTesting             (EXECUTION_CONTEXT* ec, BOOL status);
BOOL               WINAPI ec_SetVisualMode          (EXECUTION_CONTEXT* ec, BOOL status);
BOOL               WINAPI ec_SetOptimization        (EXECUTION_CONTEXT* ec, BOOL status);
int                WINAPI ec_SetRecorder            (EXECUTION_CONTEXT* ec, int mode);

const char*        WINAPI ec_SetAccountServer       (EXECUTION_CONTEXT* ec, const char* server);
int                WINAPI ec_SetAccountNumber       (EXECUTION_CONTEXT* ec, int number);

int                WINAPI ec_SetDllWarning          (EXECUTION_CONTEXT* ec, int error);
//                        ec.dllWarningMsg
int                WINAPI ec_SetDllError            (EXECUTION_CONTEXT* ec, int error);
//                        ec.dllErrorMsg
int                WINAPI ec_SetMqlError            (EXECUTION_CONTEXT* ec, int error);

DWORD              WINAPI ec_SetDebugOptions        (EXECUTION_CONTEXT* ec, DWORD options);
int                WINAPI ec_SetLoglevel            (EXECUTION_CONTEXT* ec, int level);
int                WINAPI ec_SetLoglevelDebug       (EXECUTION_CONTEXT* ec, int level);
int                WINAPI ec_SetLoglevelTerminal    (EXECUTION_CONTEXT* ec, int level);
int                WINAPI ec_SetLoglevelAlert       (EXECUTION_CONTEXT* ec, int level);
int                WINAPI ec_SetLoglevelFile        (EXECUTION_CONTEXT* ec, int level);
int                WINAPI ec_SetLoglevelMail        (EXECUTION_CONTEXT* ec, int level);
int                WINAPI ec_SetLoglevelSMS         (EXECUTION_CONTEXT* ec, int level);
//                        ec.logger
//                        ec.logBuffer
const char*        WINAPI ec_SetLogFilename         (EXECUTION_CONTEXT* ec, const char* filename);


// helpers
const char*        WINAPI EXECUTION_CONTEXT_toStr  (const EXECUTION_CONTEXT* ec);
const char*        WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec);


// type definitions
typedef std::vector<EXECUTION_CONTEXT*> ContextChain;       // A chain holds the execution contexts of all modules of a single MQL program.
typedef std::vector<ContextChain*>      MqlInstanceList;    // List of all MQL program instances ever loaded (index = instance id aka pid).
