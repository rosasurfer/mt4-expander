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


// MQL-Rootfunktionen
enum RootFunction {
   RF_INIT      = ROOTFUNCTION_INIT,
   RF_START     = ROOTFUNCTION_START,
   RF_DEINIT    = ROOTFUNCTION_DEINIT
};


// Launchtypen eines MQL-Programms: via Template, via iCustom() oder von Hand
enum LaunchType {
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,
   LT_MANUAL    = LAUNCHTYPE_MANUAL
};


/**
 * MT4-interne Darstellung eines MQL-Strings
 */
struct MqlStr {
   int   size;                                     // Größe des Speicherblocks oder 0, wenn der String ein intern verwaltetes Literal ist
   char* string;
};


/**
 * HistoryFile Header
 */
struct HISTORY_HEADER {                            // -- size ------- offset --- description ----------------------------------------------------------------------------------------
  uint  format;                                    //       4      => hh[ 0]     Barformat, bis Build 509: 400, danach: 401
  char  description[64];                           //      64      => hh[ 1]     Beschreibung (szchar)
  char  symbol[MAX_SYMBOL_LENGTH+1];               //      12      => hh[17]     Symbol       (szchar)
  uint  period;                                    //       4      => hh[20]     Timeframe
  uint  digits;                                    //       4      => hh[21]     Digits
  uint  syncMark;                                  //       4      => hh[22]     Server-SyncMarker (timestamp), wird beim Online-Refresh mit Server-SyncMarker überschrieben
  uint  lastSync;                                  //       4      => hh[23]     LastSync          (unbenutzt), wird beim Online-Refresh *nicht* überschrieben
  uint  timezoneId;     /*zusätzliches Element*/   //       4      => hh[24]     Timezone ID (default: 0 => Server-Timezone)
  BYTE  reserved[48];                              //      48      => hh[25]
};                                                 // -------------------------------------------------------------------------------------------------------------------------------
                                                   //   = 148      = int[37]

/**
 * HistoryFile Barformat v400 (bis Build 509), entspricht dem MT4 struct RateInfo
 */
struct HISTORY_BAR_400 {                           // -- size ------------------ description ----------------------------------------------------------------------------------------
   uint   time;                                    //       4                    Open-Time (timestamp)
   double open;                                    //       8
   double low;                                     //       8
   double high;                                    //       8
   double close;                                   //       8
   double ticks;                                   //       8                    immer Ganzzahl
};                                                 // -------------------------------------------------------------------------------------------------------------------------------
                                                   //    = 44

/**
 * HistoryFile Barformat v401 (ab Build 510), entspricht dem MT4 struct MqlRates
 */
struct HISTORY_BAR_401 {                           // -- size ------------------ description ----------------------------------------------------------------------------------------
   int64  time;                                    //       8                    Open-Time (timestamp)
   double open;                                    //       8
   double high;                                    //       8
   double low;                                     //       8
   double close;                                   //       8
   uint64 ticks;                                   //       8
   int    spread;                                  //       4                    (unbenutzt)
   uint64 volume;                                  //       8                    (unbenutzt)
};                                                 // -------------------------------------------------------------------------------------------------------------------------------
                                                   //    = 60

/**
 * In der DLL aufgetretener Fehler.
 */
struct DLL_ERROR {
   int   code;
   char* message;                                  // szchar
};


/**
 * Ausführungskontext eines MQL-Programms für Laufzeitinformationen und Datenaustausch zwischen MQL-Modulen und DLL
 */
struct EXECUTION_CONTEXT {                         // -- size ------- offset --- description ----------------------------------------------------------------------------------------
   uint               programId;                   //       4      => ec[ 0]     eindeutige Programm-ID (größer 0)               (konstant)   => Index in programs[i]
   ProgramType        programType;                 //       4      => ec[ 1]     Programmtyp                                     (konstant)   => was bin ich
   char               programName[MAX_PATH];       //     260      => ec[ 2]     Programmname (szchar)                           (konstant)   => wie heiße ich
   LaunchType         launchType;                  //       4      => ec[67]     Launchtyp                                       (konstant)   => wie wurde ich gestartet
   EXECUTION_CONTEXT* superContext;                //       4      => ec[68]     übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   uint               initFlags;                   //       4      => ec[69]     init-Flags                                      (konstant)   => wie werde ich initialisiert
   uint               deinitFlags;                 //       4      => ec[70]     deinit-Flags                                    (konstant)   => wie werde ich deinitialisiert
   RootFunction       rootFunction;                //       4      => ec[71]     aktuelle Rootfunktion                           (variabel)   => wo bin ich
   uint               uninitializeReason;          //       4      => ec[72]     letzter Uninitialize-Reason                     (variabel)   => woher komme ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //      12      => ec[73]     aktuelles Symbol (szchar)                       (variabel)   => auf welchem Symbol laufe ich
   uint               timeframe;                   //       4      => ec[76]     aktuelle Bar-Periode                            (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChartWindow;                //       4      => ec[77]     Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen
   HWND               hChart;                      //       4      => ec[78]     Chart-Frame:   MQL->WindowHandle()              (konstant)   => ...
   uint               testFlags;                   //       4      => ec[79]     Test-Flags: Off|On|VisualMode|Optimization      (konstant)   => laufe ich im Tester und wenn ja, wie

   int                lastError;                   //       4      => ec[80]     letzter MQL-Fehler                              (variabel)   => welcher MQL-Fehler ist aufgetreten
   DLL_ERROR**        dllErrors;                   //       4      => ec[81]     Array von DLL-Fehlern                           (variabel)   => welche DLL-Fehler sind aufgetreten
   uint               dllErrorsSize;               //       4      => ec[82]     Anzahl von DLL-Fehlern (Arraygröße)             (variabel)   => wieviele DLL-Fehler sind aufgetreten
   BOOL               logging;                     //       4      => ec[83]     Logstatus                                       (konstant)   => was logge ich
   char               logFile[MAX_PATH];           //     260      => ec[84]     Name der Logdatei (szchar)                      (konstant)   => wohin logge ich
};                                                 // -------------------------------------------------------------------------------------------------------------------------------
                                                   //     596     = int[149]                                                                     warum bin ich nicht auf Ibiza

typedef std::vector<EXECUTION_CONTEXT*> pec_vector;


// Funktionsdeklarationen
BOOL  onProcessAttach();
BOOL  onProcessDetach();

uint  ecc_setProgramId(pec_vector &chain, uint id);

BOOL  WINAPI SetMainExecutionContext(EXECUTION_CONTEXT* ec, const char* name, const char* symbol, int period);
BOOL  WINAPI SyncExecutionContext   (EXECUTION_CONTEXT* ec, const char* name, const char* symbol, int period);

void  WINAPI SetLogLevel(int level);
HWND  WINAPI GetApplicationWindow();
DWORD WINAPI GetUIThreadId();
BOOL  WINAPI IsUIThread();
int   WINAPI GetBoolsAddress  (BOOL   values[]);
int   WINAPI GetIntsAddress   (int    values[]);   int WINAPI GetBufferAddress(int values[]);   // Alias
int   WINAPI GetDoublesAddress(double values[]);
int   WINAPI GetStringsAddress(MqlStr values[]);
int   WINAPI GetStringAddress (char*  value   );
char* WINAPI GetString        (char*  value   );
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