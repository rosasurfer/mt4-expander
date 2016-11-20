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

   int                mqlError;                    //       584         4     Code eines aufgetretenen MQL-Fehlers            (variabel)   => welcher MQL-Fehler ist aufgetreten
   int                dllError;                    //       588         4     Code eines aufgetretenen DLL-Fehlers            (variabel)   => welcher DLL-Fehler ist aufgetreten
   char*              dllErrorMsg;                 //       592         4     Text des DLL-Fehlers                            (variabel)   => ...
   int                dllWarning;                  //       596         4     Code einer aufgetretenen DLL-Warnung            (variabel)   => ...
   char*              dllWarningMsg;               //       600         4     Text der DLL-Warnung                            (variabel)   => ...
   int                dllInfo;                     //       604         4     Code einer aufgetretenen DLL-Info               (variabel)   => ...
   char*              dllInfoMsg;                  //       608         4     Text der DLL-Info                               (variabel)   => ...
   BOOL               logging;                     //       612         4     Logstatus                                       (konstant)   => was logge ich
   char               logFile[MAX_PATH];           //       616       260     Name der Logdatei (szchar)                      (konstant)   => wohin logge ich
};                                                 // ----------------------------------------------------------------------------------------------------------------------------
                                                   //               = 876                                                                     warum bin ich nicht auf Ibiza

typedef std::vector<EXECUTION_CONTEXT*> pec_vector;


// Getter
uint               WINAPI ec_ProgramId         (const EXECUTION_CONTEXT* ec);
ProgramType        WINAPI ec_ProgramType       (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ProgramName       (const EXECUTION_CONTEXT* ec);
ModuleType         WINAPI ec_ModuleType        (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ModuleName        (const EXECUTION_CONTEXT* ec);
LaunchType         WINAPI ec_LaunchType        (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_SuperContext      (const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec);
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext    (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_InitFlags         (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_DeinitFlags       (const EXECUTION_CONTEXT* ec);
RootFunction       WINAPI ec_RootFunction      (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_UninitializeReason(const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_Symbol            (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_Timeframe         (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChartWindow      (const EXECUTION_CONTEXT* ec);
HWND               WINAPI ec_hChart            (const EXECUTION_CONTEXT* ec);
uint               WINAPI ec_TestFlags         (const EXECUTION_CONTEXT* ec);
int                WINAPI ec_MqlError          (const EXECUTION_CONTEXT* ec);
//                        ...
BOOL               WINAPI ec_Logging           (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_LogFile           (const EXECUTION_CONTEXT* ec);

// Setter
uint               WINAPI ec_SetProgramId         (EXECUTION_CONTEXT* ec, uint               id       );
uint               WINAPI ecc_SetProgramId        (pec_vector     &chain, uint               id       );
ProgramType        WINAPI ec_SetProgramType       (EXECUTION_CONTEXT* ec, ProgramType        type     );
const char*        WINAPI ec_SetProgramName       (EXECUTION_CONTEXT* ec, const char*        name     );
ModuleType         WINAPI ec_SetModuleType        (EXECUTION_CONTEXT* ec, ModuleType         type     );
const char*        WINAPI ec_SetModuleName        (EXECUTION_CONTEXT* ec, const char*        name     );
LaunchType         WINAPI ec_SetLaunchType        (EXECUTION_CONTEXT* ec, LaunchType         type     );
EXECUTION_CONTEXT* WINAPI ec_SetSuperContext      (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec      );
EXECUTION_CONTEXT* WINAPI ec_SetLpSuperContext    (EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* lpSec    );
uint               WINAPI ec_SetInitFlags         (EXECUTION_CONTEXT* ec, uint               flags    );
uint               WINAPI ec_SetDeinitFlags       (EXECUTION_CONTEXT* ec, uint               flags    );
RootFunction       WINAPI ec_SetRootFunction      (EXECUTION_CONTEXT* ec, RootFunction       id       );
UninitializeReason WINAPI ec_SetUninitializeReason(EXECUTION_CONTEXT* ec, UninitializeReason reason   );
const char*        WINAPI ec_SetSymbol            (EXECUTION_CONTEXT* ec, const char*        symbol   );
uint               WINAPI ec_SetTimeframe         (EXECUTION_CONTEXT* ec, uint               timeframe);
HWND               WINAPI ec_SetHChartWindow      (EXECUTION_CONTEXT* ec, HWND               hWnd     );
HWND               WINAPI ec_SetHChart            (EXECUTION_CONTEXT* ec, HWND               hWnd     );
uint               WINAPI ec_SetTestFlags         (EXECUTION_CONTEXT* ec, uint               flags    );
int                WINAPI ec_SetMqlError          (EXECUTION_CONTEXT* ec, int                error    );
//                        ...
BOOL               WINAPI ec_SetLogging           (EXECUTION_CONTEXT* ec, BOOL               status   );
const char*        WINAPI ec_SetLogFile           (EXECUTION_CONTEXT* ec, const char*        fileName );
