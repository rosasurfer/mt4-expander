#pragma once

#include "expander.h"
#include "structs/myfx/Test.h"


/**
 * MyFX struct EXECUTION_CONTEXT
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
 *    hingegen erhalten nach jedem Init-Cycle eine neue Instanz, da Metatrader den Speicher für Variablen in Indicator::init()
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
struct EXECUTION_CONTEXT {                         // -- offset ---- size --- description ----------------------------------------------------------------------------------------
   uint               programId;                   //         0         4     eindeutige Programm-ID (größer 0)               (konstant)   => Index in programs[i]
   ProgramType        programType;                 //         4         4     Programmtyp                                     (konstant)   => was für ein Programm bin ich
   char               programName[MAX_PATH];       //         8       260     Programmname                                    (konstant)   => wie heißt es
   ModuleType         moduleType;                  //       268         4     Modultyp                                        (konstant)   => was für ein Modul bin ich
   char               moduleName[MAX_PATH];        //       272       260     Modulname                                       (konstant)   => wie heißt es

   LaunchType         launchType;                  //       532         4     Launchtyp                                       (konstant)   => wie wurde ich gestartet
   RootFunction       rootFunction;                //       536         4     letzte Rootfunktion des Modules                 (variabel)   => wo bin ich
   BOOL               initCycle;                   //       540         4
   InitializeReason   initReason;                  //       544         4     letzter InitializeReason                        (variabel)   => woher komme ich
   UninitializeReason uninitReason;                //       548         4     letzter MQL::UninitializeReason()               (variabel)   => woher komme/wohin gehe ich
   BOOL               testing;                     //       552         4     IsTesting()-Status                              (konstant)   => laufe ich im Tester
   BOOL               visualMode;                  //       556         4     IsVisualMode()-Status                           (konstant)   => laufe ich im Tester mit VisualMode=On
   BOOL               optimization;                //       560         4     IsOptimization()-Status                         (konstant)   => laufe ich im Tester mit Optimization=On
   TEST*              test;                        //       564         4     Test-Metadata                                   (konstant)   => Test-Konfiguration

   DWORD              initFlags;                   //       568         4     Init-Konfiguration                              (konstant)   => wie werde ich initialisiert
   DWORD              deinitFlags;                 //       572         4     Deinit-Konfiguration                            (konstant)   => wie werde ich deinitialisiert
   BOOL               logging;                     //       576         4     Log-Konfiguration                               (konstant)   => was logge ich
   char               customLogFile[MAX_PATH];     //       580       260     Name der Logdatei                               (konstant)   => wohin logge ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //       840        12     aktuelles Symbol                                (variabel)   => auf welchem Symbol laufe ich
   uint               timeframe;                   //       852         4     aktuelle Bar-Periode                            (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChart;                      //       856         4     Chart-Frame:   MQL::WindowHandle()              (konstant)   => ...
   HWND               hChartWindow;                //       860         4     Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen

   EXECUTION_CONTEXT* superContext;                //       864         4     übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   uint               threadId;                    //       868         4     ID des ausführenden Threads                     (variable)   => wer führt mich aus
   uint               ticks;                       //       872         4     Anzahl der start()-Aufrufe                      (variabel)   => wie oft wurde ich ausgeführt

   int                mqlError;                    //       876         4     Error-Code eines aufgetretenen MQL-Fehlers      (variabel)   => welcher MQL-Fehler ist aufgetreten
   int                dllError;                    //       880         4     Error-Code eines aufgetretenen DLL-Fehlers      (variabel)   => welcher DLL-Fehler ist aufgetreten
   char*              dllErrorMsg;                 //       884         4     Text des DLL-Fehlers                            (variabel)   => ...
   int                dllWarning;                  //       888         4     Error-Code einer aufgetretenen DLL-Warnung      (variabel)   => ...
   char*              dllWarningMsg;               //       892         4     Text der DLL-Warnung                            (variabel)   => ...
};                                                 // ----------------------------------------------------------------------------------------------------------------------------
                                                   //               = 896                                                                     warum bin ich nicht auf Ibiza

// Getters
uint               WINAPI ec_ProgramId     (const EXECUTION_CONTEXT* ec);
ProgramType        WINAPI ec_ProgramType   (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ProgramName   (const EXECUTION_CONTEXT* ec);
ModuleType         WINAPI ec_ModuleType    (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_ModuleName    (const EXECUTION_CONTEXT* ec);
LaunchType         WINAPI ec_LaunchType    (const EXECUTION_CONTEXT* ec);
RootFunction       WINAPI ec_RootFunction  (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_InitCycle     (const EXECUTION_CONTEXT* ec);
InitializeReason   WINAPI ec_InitReason    (const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI ec_UninitReason  (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Testing       (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_VisualMode    (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Optimization  (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_InitFlags     (const EXECUTION_CONTEXT* ec);
DWORD              WINAPI ec_DeinitFlags   (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI ec_Logging       (const EXECUTION_CONTEXT* ec);
const char*        WINAPI ec_CustomLogFile (const EXECUTION_CONTEXT* ec);
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
BOOL               WINAPI ec_SetInitCycle         (EXECUTION_CONTEXT* ec, BOOL               status   );
InitializeReason   WINAPI ec_SetInitReason        (EXECUTION_CONTEXT* ec, InitializeReason   reason   );
UninitializeReason WINAPI ec_SetUninitReason      (EXECUTION_CONTEXT* ec, UninitializeReason reason   );
BOOL               WINAPI ec_SetTesting           (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetVisualMode        (EXECUTION_CONTEXT* ec, BOOL               status   );
BOOL               WINAPI ec_SetOptimization      (EXECUTION_CONTEXT* ec, BOOL               status   );
DWORD              WINAPI ec_SetInitFlags         (EXECUTION_CONTEXT* ec, DWORD              flags    );
DWORD              WINAPI ec_SetDeinitFlags       (EXECUTION_CONTEXT* ec, DWORD              flags    );
BOOL               WINAPI ec_SetLogging           (EXECUTION_CONTEXT* ec, BOOL               status   );
const char*        WINAPI ec_SetCustomLogFile     (EXECUTION_CONTEXT* ec, const char*        fileName );
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


// Master context getters
RootFunction       WINAPI mec_RootFunction(const EXECUTION_CONTEXT* ec);
UninitializeReason WINAPI mec_UninitReason(const EXECUTION_CONTEXT* ec);
DWORD              WINAPI mec_InitFlags   (const EXECUTION_CONTEXT* ec);


// Helpers
const char*        WINAPI EXECUTION_CONTEXT_toStr  (const EXECUTION_CONTEXT* ec, BOOL outputDebug=FALSE);
const char*        WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug=FALSE);
