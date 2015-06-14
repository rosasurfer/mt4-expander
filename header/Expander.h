#pragma once

#include "mql4/include/shared/defines.h"
#include "mql4/include/shared/errors.h"
#include <vector>


#pragma pack(1)


// MQL-Programmtypen
enum ProgramType {
   PT_INDICATOR = PROGRAMTYPE_INDICATOR,
   PT_EXPERT    = PROGRAMTYPE_EXPERT,
   PT_SCRIPT    = PROGRAMTYPE_SCRIPT
};


// MQL-Modultypen                            // als Flag implementiert
enum ModuleType {
   MT_INDICATOR = MODULETYPE_INDICATOR,      // 1
   MT_EXPERT    = MODULETYPE_EXPERT,         // 2
   MT_SCRIPT    = MODULETYPE_SCRIPT,         // 4
   MT_LIBRARY   = MODULETYPE_LIBRARY         // 8 - Libraries sind keine eigenständigen Programme
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
   int   length;                 // Größe des verfügbaren Speicherblocks oder 0, wenn der String ein C-Literal ist
   char* string;
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
// Laufzeitumgebungsinformationen und Datenaustausch zwischen MQL-Modulen und DLL
//
struct EXECUTION_CONTEXT {                         // -- size ------- offset --- description ----------------------------------------------------------------------------------------
   unsigned int       programId;                   //       4      => ec[ 0]     eindeutige Programm-ID (größer 0)               (konstant)   => Index in programs[i]
   ProgramType        programType;                 //       4      => ec[ 1]     Programmtyp                                     (konstant)   => was bin ich
   char               programName[MAX_PATH];       //     260      => ec[ 2]     Programmname                                    (konstant)   => wie heiße ich
   LaunchType         launchType;                  //       4      => ec[67]     Launchtyp                                       (konstant)   => wie wurde ich gestartet
   EXECUTION_CONTEXT* superContext;                //       4      => ec[68]     übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   unsigned int       initFlags;                   //       4      => ec[69]     init-Flags                                      (konstant)   => wie werde ich initialisiert
   unsigned int       deinitFlags;                 //       4      => ec[70]     deinit-Flags                                    (konstant)   => wie werde ich deinitialisiert
   RootFunction       rootFunction;                //       4      => ec[71]     aktuelle Rootfunktion des Programms             (variabel)   => wo bin ich
   int                uninitializeReason;          //       4      => ec[72]     letzter Uninitialize-Reason                     (variabel)   => woher komme ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //      12      => ec[73]     aktuelles Symbol, <NUL>-terminiert              (variabel)   => auf welchem Symbol laufe ich
   unsigned int       timeframe;                   //       4      => ec[76]     aktuelle Bar-Periode                            (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChartWindow;                //       4      => ec[77]     Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen
   HWND               hChart;                      //       4      => ec[78]     Chart-Frame: = MQL->WindowHandle()              (konstant)   => ...
   unsigned int       testFlags;                   //       4      => ec[79]     Tester-Flags: Off|On|VisualMode|Optimization    (konstant)   => laufe ich im Tester und wenn ja, wie

   int                lastError;                   //       4      => ec[80]     letzter in MQL aufgetretener Fehler             (variabel)   => welcher MQL-Fehler ist aufgetreten
   DLL_ERROR**        dllErrors;                   //       4      => ec[81]     Array von in der DLL aufgetretenen Fehlern      (variabel)   => welche DLL-Fehler sind aufgetreten
   unsigned int       dllErrorsSize;               //       4      => ec[82]     Anzahl von DLL-Fehlern (Arraygröße)             (variabel)   => wieviele DLL-Fehler sind aufgetreten
   BOOL               logging;                     //       4      => ec[83]     Logstatus                                       (konstant)   => was logge ich
   char               logFile[MAX_PATH];           //     260      => ec[84]     Name der Logdatei, <NUL>-terminiert             (konstant)   => wohin logge ich
};                                                 // -------------------------------------------------------------------------------------------------------------------------------
                                                   //     596     = int[149]                                                                     warum bin ich nicht auf Ibiza

typedef std::vector<EXECUTION_CONTEXT*> pec_vector;


/*
// Prototype
struct EXECUTION_CONTEXT_proto {
   uint               id;
   ...
   LPSTR              programName;
   ...
   int                uninitializeReason;
   ...
   DLL_ERROR**        dllErrors;
   uint               dllErrorsSize;
   int                logLevel;
   ...
};
*/


// Funktionsdeklarationen
BOOL  onProcessAttach();
BOOL  onProcessDetach();

uint  ecc_setProgramId(pec_vector &chain, uint id);

BOOL  WINAPI SetExecutionContext(EXECUTION_CONTEXT* ec);
BOOL  WINAPI GetExecutionContext(EXECUTION_CONTEXT* ec);

void  WINAPI SetLogLevel(int level);
HWND  WINAPI GetApplicationWindow();
DWORD WINAPI GetUIThreadId();
BOOL  WINAPI IsUIThread();
int   WINAPI GetBoolsAddress  (BOOL   values[]);
int   WINAPI GetIntsAddress   (int    values[]);   int WINAPI GetBufferAddress(int values[]);   // Alias
int   WINAPI GetDoublesAddress(double values[]);
int   WINAPI GetStringsAddress(MqlStr values[]);
int   WINAPI GetStringAddress(char* value);
char* WINAPI GetString       (char* value);
int   WINAPI GetLastWin32Error();
BOOL  WINAPI IsBuiltinTimeframe(int timeframe);
BOOL  WINAPI IsCustomTimeframe(int timeframe);
char* WINAPI IntToHexStr(int value);

const char*  ModuleTypeToStr       (ModuleType  type);
const char*  ModuleTypeDescription (ModuleType  type);
const char*  ProgramTypeToStr      (ProgramType type);
const char*  ProgramTypeDescription(ProgramType type);
const char*  RootFunctionToStr     (RootFunction id);
const char*  RootFunctionName      (RootFunction id);

const char*  PeriodToStr      (int period);  const char* TimeframeToStr      (int timeframe);   // Alias
const char*  PeriodDescription(int period);  const char* TimeframeDescription(int timeframe);   // Alias
