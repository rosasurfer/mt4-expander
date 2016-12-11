#pragma once


/**
 * MyFX struct EXECUTION_CONTEXT
 *
 * Ausführungskontext von MQL-Programmen zur Kommunikation zwischen MQL und DLL
 */
struct EXECUTION_CONTEXT {                         // -- offset ---- size --- description ----------------------------------------------------------------------------------------
   uint               programId;                   //         0         4     eindeutige Programm-ID (größer 0)               (konstant)   => Index in programs[i]
   ProgramType        programType;                 //         4         4     Programmtyp                                     (konstant)   => was für ein Programm bin ich
   char               programName[MAX_PATH];       //         8       260     Programmname (szchar)                           (konstant)   => wie heißt es
   ModuleType         moduleType;                  //       268         4     Modultyp                                        (konstant)   => was für ein Modul bin ich
   char               moduleName[MAX_PATH];        //       272       260     Modulname (szchar)                              (konstant)   => wie heißt es

   LaunchType         launchType;                  //       532         4     Launchtyp                                       (konstant)   => wie wurde ich gestartet
   RootFunction       rootFunction;                //       536         4     letzte Rootfunktion des Modules                 (variabel)   => wo bin ich
   InitializeReason   initReason;                  //       540         4     letzter InitializeReason                        (variabel)   => woher komme ich
   UninitializeReason uninitReason;                //       544         4     letzter MQL::UninitializeReason()               (variabel)   => woher komme/wohin gehe ich
   DWORD              testFlags;                   //       548         4     Tester-Flags: Off|On|VisualMode|Optimization    (konstant)   => laufe ich im Tester und wenn ja, wie

   DWORD              initFlags;                   //       552         4     Init-Konfiguration                              (konstant)   => wie werde ich initialisiert
   DWORD              deinitFlags;                 //       556         4     Deinit-Konfiguration                            (konstant)   => wie werde ich deinitialisiert
   BOOL               logging;                     //       560         4     Log-Konfiguration                               (konstant)   => was logge ich
   char               logFile[MAX_PATH];           //       564       260     Name der Logdatei (szchar)                      (konstant)   => wohin logge ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //       824        12     aktuelles Symbol (szchar)                       (variabel)   => auf welchem Symbol laufe ich
   uint               timeframe;                   //       836         4     aktuelle Bar-Periode                            (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChart;                      //       840         4     Chart-Frame:   MQL::WindowHandle()              (konstant)   => ...
   HWND               hChartWindow;                //       844         4     Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen

   EXECUTION_CONTEXT* superContext;                //       848         4     übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   uint               threadId;                    //       852         4     ID des ausführenden Threads                     (variable)   => wer führt mich aus
   uint               ticks;                       //       856         4     Anzahl der start()-Aufrufe                      (variabel)   => wie oft wurde ich ausgeführt

   int                mqlError;                    //       860         4     Error-Code eines aufgetretenen MQL-Fehlers      (variabel)   => welcher MQL-Fehler ist aufgetreten
   int                dllError;                    //       864         4     Error-Code eines aufgetretenen DLL-Fehlers      (variabel)   => welcher DLL-Fehler ist aufgetreten
   char*              dllErrorMsg;                 //       868         4     Text des DLL-Fehlers                            (variabel)   => ...
   int                dllWarning;                  //       872         4     Error-Code einer aufgetretenen DLL-Warnung      (variabel)   => ...
   char*              dllWarningMsg;               //       876         4     Text der DLL-Warnung                            (variabel)   => ...
};                                                 // ----------------------------------------------------------------------------------------------------------------------------
                                                   //               = 880                                                                     warum bin ich nicht auf Ibiza

typedef std::vector<EXECUTION_CONTEXT*> pec_vector;


// Context management functions
BOOL               WINAPI SyncMainContext_init  (EXECUTION_CONTEXT* ec, ProgramType type, const char* name, UninitializeReason reason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint period, EXECUTION_CONTEXT* sec, BOOL isTesting, BOOL isVisualMode, HWND hChart, int subChartDropped);
BOOL               WINAPI SyncMainContext_start (EXECUTION_CONTEXT* ec);
BOOL               WINAPI SyncMainContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason reason);

BOOL               WINAPI SyncLibContext_init   (EXECUTION_CONTEXT* ec, UninitializeReason uninitReason, const char* name, const char* symbol, uint period);
BOOL               WINAPI SyncLibContext_deinit (EXECUTION_CONTEXT* ec, UninitializeReason uninitReason);

BOOL               WINAPI LeaveContext          (EXECUTION_CONTEXT* ec);
BOOL               WINAPI StoreThreadProgram(int programId);


// Getters
uint               WINAPI ec_ProgramId     (const EXECUTION_CONTEXT* ec);
ProgramType        WINAPI ec_ProgramType   (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ProgramName   (const EXECUTION_CONTEXT* ec);
ModuleType         WINAPI ec_ModuleType    (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ModuleName    (const EXECUTION_CONTEXT* ec);
LaunchType         WINAPI ec_LaunchType    (const EXECUTION_CONTEXT* ec);
RootFunction       WINAPI ec_RootFunction  (const EXECUTION_CONTEXT* ec);
InitializeReason   WINAPI ec_InitReason    (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_UninitReason  (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_TestFlags     (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_InitFlags     (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_DeinitFlags   (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Logging       (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_LogFile       (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_Symbol        (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Timeframe     (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChart        (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChartWindow  (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_SuperContext  (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec);
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext(const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_ThreadId      (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Ticks         (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_MqlError      (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_DllError      (const EXECUTION_CONTEXT* ec);
//                        ...
int                WINAPI ec_DllWarning    (const EXECUTION_CONTEXT* ec);
//                        ...


// Setters
uint               WINAPI ec_SetProgramId         (EXECUTION_CONTEXT* ec, uint               id       );
ProgramType        WINAPI ec_SetProgramType       (EXECUTION_CONTEXT* ec, ProgramType        type     );
const char*        WINAPI ec_SetProgramName       (EXECUTION_CONTEXT* ec, const char*        name     );
ModuleType         WINAPI ec_SetModuleType        (EXECUTION_CONTEXT* ec, ModuleType         type     );
const char*        WINAPI ec_SetModuleName        (EXECUTION_CONTEXT* ec, const char*        name     );
LaunchType         WINAPI ec_SetLaunchType        (EXECUTION_CONTEXT* ec, LaunchType         type     );
RootFunction       WINAPI ec_SetRootFunction      (EXECUTION_CONTEXT* ec, RootFunction       id       );
InitializeReason   WINAPI ec_SetInitReason        (EXECUTION_CONTEXT* ec, InitializeReason   reason   );
UninitializeReason WINAPI ec_SetUninitReason      (EXECUTION_CONTEXT* ec, UninitializeReason reason   );
DWORD              WINAPI ec_SetTestFlags         (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetInitFlags         (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetDeinitFlags       (EXECUTION_CONTEXT* ec, DWORD              flags    );
BOOL               WINAPI ec_SetLogging           (EXECUTION_CONTEXT* ec, BOOL               status   );
const char*        WINAPI ec_SetLogFile           (EXECUTION_CONTEXT* ec, const char*        fileName );
const char*        WINAPI ec_SetSymbol            (EXECUTION_CONTEXT* ec, const char*        symbol   );
uint               WINAPI ec_SetTimeframe         (EXECUTION_CONTEXT* ec, uint               timeframe);
HWND               WINAPI ec_SetHChart            (EXECUTION_CONTEXT* ec, HWND               hWnd     );
HWND               WINAPI ec_SetHChartWindow      (EXECUTION_CONTEXT* ec, HWND               hWnd     );
EXECUTION_CONTEXT* WINAPI ec_SetSuperContext      (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec      );
EXECUTION_CONTEXT* WINAPI ec_SetLpSuperContext    (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* lpSec    );
uint               WINAPI ec_SetThreadId          (EXECUTION_CONTEXT* ec, uint               id       );
uint               WINAPI ec_SetTicks             (EXECUTION_CONTEXT* ec, uint               count    );
int                WINAPI ec_SetMqlError          (EXECUTION_CONTEXT* ec, int                error    );
int                WINAPI ec_SetDllError          (EXECUTION_CONTEXT* ec, int                error    );
//                        ...
int                WINAPI ec_SetDllWarning        (EXECUTION_CONTEXT* ec, int                error    );
//                        ...


// Helpers
const char*        WINAPI EXECUTION_CONTEXT_toStr  (const EXECUTION_CONTEXT* ec, BOOL outputDebug=FALSE);
const char*        WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug=FALSE);
