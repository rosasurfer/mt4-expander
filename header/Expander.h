#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <sstream>
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "mql4/include/shared/defines.h"
#include "mql4/include/shared/errors.h"


#pragma warning(push)
#pragma warning(disable: 4060)         // switch statement contains no 'case' or 'default' labels
#pragma warning(disable: 4065)         // switch statement contains 'default' but no 'case' labels
#pragma warning(disable: 4996)         // std::basic_string<>::copy: Function call with parameters that may be unsafe


#ifdef EXPANDER_EXPORTS
 //#define EXPANDER_API extern "C" __declspec(dllexport)
   #define EXPANDER_API                                                             // empty define
   #define EXPORT       comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)     // export unmangled names w/o DEF file
#else
   #define EXPANDER_API extern "C" __declspec(dllimport)
#endif


template <typename T>
std::string to_string(T value) {
   std::ostringstream os;
   os << value;
   return(os.str());
}


#define debug(...)   _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)


/**
 * Schickt eine formatierte Nachricht an die Debugger-Ausgabe. Sollte nicht direkt aufgerufen werden.
 *
 * @param  char*   fileName - Name der Datei, in der der Aufruf erfolgt
 * @param  char*   funcName - Name der Funktion, in der der Aufruf erfolgt
 * @param  int     line     - Zeile, in der der Aufruf erfolgt
 * @param  char*   format   - Formatstring mit Platzhaltern für alle weiteren Parameter
 * @param  va_list args     - weitere Parameter
 *
 * @return int - zur Zeit immer 0
 */
int _debug(char* fileName, char* funcName, int line, const char* format, va_list &args) {
   if (!fileName) fileName = __FILE__;                                           // falls die Funktion direkt aufgerufen wird
   if (!funcName) funcName = __FUNCTION__;                                       // ...
   if (!format  ) return(0);                                                     // falls ein Nullpointer übergeben wurde

   // (1) zuerst alle explizit angegebenen Argumente in einen String transformieren (ab format)
   int size  = _vscprintf(format, args) + 1;                                     // +1 für das terminierende '\0'
   char* msg = (char*) alloca(size);                                             // auf dem Stack
   vsprintf_s(msg, size, format, args);

   // Parameter fileName zerlegen: nur der einfache Dateiname {basename.ext} wird benötigt
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // (2) dann die impliziten Location-Infos vorn anfügen
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;     // +1 für das terminierende '\0'
   char* buffer = (char*) alloca(size);                                          // auf dem Stack
   sprintf_s(buffer, size, locationFormat, baseName, ext, funcName, line, msg);

   OutputDebugString(buffer);
   return(0);
}


/**
 * Schickt einen C-String an die Debugger-Ausgabe. Sollte nicht direkt aufgerufen werden.
 *
 * @return int - zur Zeit immer 0
 */
int _debug(char* fileName, char* funcName, int line, const char* format, ...) {
   va_list args;
   va_start(args, format);
   _debug(fileName, funcName, line, format, args);
   va_end(args);
   return(0);
}


/**
 * Schickt einen C++-String an die Debugger-Ausgabe. Sollte nicht direkt aufgerufen werden.
 *
 * @return int - zur Zeit immer 0
 */
int _debug(char* fileName, char* funcName, int line, const std::string &format, ...) {
   va_list args;
   va_start(args, format);
   _debug(fileName, funcName, line, format.c_str(), args);
   va_end(args);
   return(0);
}


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
   unsigned int time;            // BarOpen-Time
   double       open;
   double       low;
   double       high;
   double       close;
   double       volume;          // immer Ganzzahl
};


// in der DLL aufgetretener Fehler, kann an ein MQL-Programm weitergeleitet werden
struct DLL_ERROR {
   int   code;
   char* message;
};


// Ausführungskontext eines MQL-Programms:
// Laufzeitumgebungsinformationen und Datenaustausch für MQL-Module/-Programme und die DLL
//
struct EXECUTION_CONTEXT {                         // -- size ------- offset ------------------------------------------------------------------------------------------------------------
   unsigned int       id;                          //       4      => ec[ 0]      // eindeutige ID des Context                       (konstant)   => ...
   DWORD              hThreadId;                   //       4      => ec[ 1]      // aktueller Thread, in dem das Programm läuft     (variabel)   => ...

   ProgramType        programType;                 //       4      => ec[ 2]      // Programmtyp                                     (konstant)   => was bin ich
   LPSTR              programName;                 //       4      => ec[ 3]      // Programmname                                    (konstant)   => wie heiße ich
   LaunchType         launchType;                  //       4      => ec[ 4]      // Launchtyp                                       (konstant)   => wie wurde ich gestartet
   EXECUTION_CONTEXT* superContext;                //       4      => ec[ 5]      // übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   int                initFlags;                   //       4      => ec[ 6]      // init-Flags                                      (konstant)   => wie werde ich initialisiert
   int                deinitFlags;                 //       4      => ec[ 7]      // deinit-Flags                                    (konstant)   => wie werde ich deinitialisiert
   RootFunction       whereami;                    //       4      => ec[ 8]      // aktuelle Rootfunktion des Programms             (variabel)   => wo bin ich
   int                uninitializeReason;          //       4      => ec[ 9]      // letzter Uninitialize-Reason                     (variabel)   => woher komme ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //      12      => ec[10]      // aktuelles Symbol                                (variabel)   => auf welchem Symbol laufe ich
   unsigned int       timeframe;                   //       4      => ec[13]      // aktuelle Bar-Periode                            (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChartWindow;                //       4      => ec[14]      // Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen
   HWND               hChart;                      //       4      => ec[15]      // Chart-Frame:   MQL => WindowHandle()            (konstant)   => ...
   int                testFlags;                   //       4      => ec[16]      // Tester-Flags: Off|On|VisualMode|Optimization    (konstant)   => laufe ich im Tester und wenn ja, wie

   int                lastError;                   //       4      => ec[17]      // letzter in MQL aufgetretener Fehler             (variabel)   => welcher MQL-Fehler ist aufgetreten
   DLL_ERROR**        dllErrors;                   //       4      => ec[18]      // Array von in der DLL aufgetretenen Fehlern      (variabel)   => welche DLL-Fehler sind aufgetreten
   size_t             dllErrorsSize;               //       4      => ec[19]      // Anzahl von DLL-Fehlern (Arraygröße)             (variabel)   => ...
   BOOL               logging;                     //       4      => ec[20]      // Logstatus                                       (konstant)   => was logge ich
   LPSTR              logFile;                     //       4      => ec[21]      // vollständiger Name der Logdatei                 (konstant)   => wohin logge ich
};                                                 // ----------------------
                                                   //      88      = int[22]                                                                         warum bin ich nicht auf Ibiza


// Prototype
struct EXECUTION_CONTEXT_proto {
   unsigned int       id;
   DWORD              hThreadId;

 //ProgramType        programType;                 // ok
   LPSTR              programName;
 //LaunchType         launchType;                  // ok
 //EXECUTION_CONTEXT* superContext;                // ok
 //int                initFlags;                   // ok
 //int                deinitFlags;                 // ok
 //RootFunction       whereami;                    // ok
   int                uninitializeReason;

   char               symbol[MAX_SYMBOL_LENGTH+1];
   unsigned int       timeframe;
   HWND               hChartWindow;                // Win32-Chartfenster (mit Titelzeile)
   HWND               hChart;                      // MQL-Chartfenster (Child von hWndChart = AfxFrame)
   int                testFlags;

   DLL_ERROR**        dllErrors;                   // Array von DLL-Fehlern
   size_t             dllErrorsSize;               // Anzahl von DLL-Fehlern (Arraygröße)
   int                logLevel;
   LPSTR              logFile;
};



// Funktionsdeklarationen
BOOL onProcessAttach();
BOOL onThreadAttach();
BOOL onThreadDetach();
BOOL onProcessDetach();

BOOL ResetCurrentThreadData();
BOOL ManageExecutionContext(EXECUTION_CONTEXT* ec);
