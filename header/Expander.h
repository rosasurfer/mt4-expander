#pragma once

#include "mql4/include/shared/defines.h"
#include "mql4/include/shared/errors.h"


#include <vector>
typedef std::vector<int> int_vector;


#pragma pack(1)

// MQL-Moduletypen
enum ModuleType {
   MT_INDICATOR = MODULETYPE_INDICATOR,
   MT_EXPERT    = MODULETYPE_EXPERT,
   MT_SCRIPT    = MODULETYPE_SCRIPT,
   MT_LIBRARY   = MODULETYPE_LIBRARY
};


// MQL-Programmtypen (Library-Module sind keine eigenständigen Programme)
enum ProgramType {
   PT_INDICATOR = MODULETYPE_INDICATOR,
   PT_EXPERT    = MODULETYPE_EXPERT,
   PT_SCRIPT    = MODULETYPE_SCRIPT
};


// MQL-Rootfunktionen: init(), start() oder deinit()
enum RootFunction {
   RF_INIT      = ROOTFUNCTION_INIT,
   RF_START     = ROOTFUNCTION_START,
   RF_DEINIT    = ROOTFUNCTION_DEINIT
};


// Launchtypen eines MQL-Programms: per Template, per iCustom() oder von Hand
enum LaunchType {
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,
   LT_MANUAL    = LAUNCHTYPE_MANUAL
};


// MT4-interne Darstellung eines MQL-Strings
struct MqlStr {
   int   length;                 // 0, wenn der String ein C-Literal ist;
   char* string;                 // andererseits die Größe des verfügbaren Speicherblocks
};


// MT4-interne Darstellung einer Preis-Bar
struct RateInfo {
   int    time;                  // BarOpen-Time
   double open;
   double low;
   double high;
   double close;
   double volume;                // immer Ganzzahl
};


// in der DLL aufgetretener Fehler, kann an ein MQL-Programm weitergereicht werden
struct DLL_ERROR {
   int   code;
   char* message;
};


// Ausführungskontext eines MQL-Programms:
// Laufzeitumgebungsinformationen und Datenaustausch für MQL-Module/-Programme und die DLL
//
struct EXECUTION_CONTEXT {                         // -- size ------- offset --- description ----------------------------------------------------------------------------------------
   int                id;                          //       4      => ec[ 0]     eindeutige ID                                   (konstant)   => ...
   DWORD              hThreadId;                   //       4      => ec[ 1]     aktueller Thread, in dem das Programm läuft     (variabel)   => ...

   ProgramType        programType;                 //       4      => ec[ 2]     Programmtyp                                     (konstant)   => was bin ich
   LPSTR              programName;                 //       4      => ec[ 3]     Programmname                                    (konstant)   => wie heiße ich
   LaunchType         launchType;                  //       4      => ec[ 4]     Launchtyp                                       (konstant)   => wie wurde ich gestartet
   EXECUTION_CONTEXT* superContext;                //       4      => ec[ 5]     übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   int                initFlags;                   //       4      => ec[ 6]     init-Flags                                      (konstant)   => wie werde ich initialisiert
   int                deinitFlags;                 //       4      => ec[ 7]     deinit-Flags                                    (konstant)   => wie werde ich deinitialisiert
   RootFunction       rootFunction;                //       4      => ec[ 8]     aktuelle Rootfunktion des Programms             (variabel)   => wo bin ich
   int                uninitializeReason;          //       4      => ec[ 9]     letzter Uninitialize-Reason                     (variabel)   => woher komme ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //      12      => ec[10]     aktuelles Symbol                                (variabel)   => auf welchem Symbol laufe ich
   int                timeframe;                   //       4      => ec[13]     aktuelle Bar-Periode                            (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChartWindow;                //       4      => ec[14]     Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen
   HWND               hChart;                      //       4      => ec[15]     Chart-Frame:   MQL - WindowHandle()             (konstant)   => ...
   int                testFlags;                   //       4      => ec[16]     Tester-Flags: Off|On|VisualMode|Optimization    (konstant)   => laufe ich im Tester und wenn ja, wie

   int                lastError;                   //       4      => ec[17]     letzter in MQL aufgetretener Fehler             (variabel)   => welcher MQL-Fehler ist aufgetreten
   DLL_ERROR**        dllErrors;                   //       4      => ec[18]     Array von in der DLL aufgetretenen Fehlern      (variabel)   => welche DLL-Fehler sind aufgetreten
   unsigned int       dllErrorsSize;               //       4      => ec[19]     Anzahl von DLL-Fehlern (Arraygröße)             (variabel)   => ...
   BOOL               logging;                     //       4      => ec[20]     Logstatus                                       (konstant)   => was logge ich
   LPSTR              logFile;                     //       4      => ec[21]     vollständiger Name der Logdatei                 (konstant)   => wohin logge ich
};                                                 // -------------------------------------------------------------------------------------------------------------------------------
                                                   //      88      = int[22]                                                                     und warum bin ich nicht auf Ibiza

/*
// Prototype
struct EXECUTION_CONTEXT_proto {
   uint               id;
   ...
   LPSTR              programName;
   ...
   int                uninitializeReason;

   char               symbol[MAX_SYMBOL_LENGTH+1];
   uint               timeframe;
   ...

   DLL_ERROR**        dllErrors;
   uint               dllErrorsSize;
   int                logLevel;
   LPSTR              logFile;
};
*/



// Funktionsdeklarationen
void onProcessAttach();
void onThreadDetach ();
void onProcessDetach();

void SetLogLevel(int level);
BOOL SetExecutionContext(EXECUTION_CONTEXT* ec);
bool DropCurrentThread();
