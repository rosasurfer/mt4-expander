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
 * Schickt einen String an den System-Debugger.  Sollte nur über das Makro debug(...) aufgerufen werden.
 *
 * @param  char* fileName  - Name der Datei, in der der Aufruf erfolgt
 * @param  char* funcName  - Name der Funktion, in der der Aufruf erfolgt
 * @param  int   line      - Zeile, in der der Aufruf erfolgt
 * @param  char* msgFormat - Formatstring mit Platzhaltern für alle weiteren Parameter
 * @param  ...             - beliebige weitere Parameter
 *
 * @return void
 */
void _debug(char* fileName, char* funcName, int line, const char* msgFormat, ...) {
   if (!fileName) fileName = __FILE__;                                           // falls die Funktion direkt aufgerufen wird
   if (!funcName) funcName = __FUNCTION__;                                       // ...
   if (!msgFormat) return;                                                       // falls ein Nullpointer übergeben wurde

   // (1) zuerst alle explizit angegebenen Argumente in einen String transformieren (ab msgFormat)
   va_list args;
   va_start(args, msgFormat);
   int size = _vscprintf(msgFormat, args) + 1;                                   // +1 für das terminierende '\0'
   char* msg = (char*) alloca(size);                                             // auf dem Stack
   vsprintf_s(msg, size, msgFormat, args);
   va_end(args);

   // Parameter fileName zerlegen: nur der einfache Dateiname {basename.ext} wird benötigt
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // (2) dann die impliziten Location-Infos vorn anfügen
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;     // +1 für das terminierende '\0'
   char* buffer = (char*) alloca(size);                                          // auf dem Stack
   sprintf_s(buffer, size, locationFormat, baseName, ext, funcName, line, msg);

   OutputDebugString(buffer);
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
   EXECUTION_CONTEXT* self;                        //       4      => ec[ 0]      // Zeiger auf den Context selbst                   (konstant)   => Validierung des Speicherblocks
   ProgramType        programType;                 //       4      => ec[ 1]      // Programmtyp                                     (konstant)   => was bin ich
   LPSTR              programName;                 //       4      => ec[ 2]      // Programmname                                    (konstant)   => wie heiße ich

   HANDLE             hThreadId;                   //       4      => ec[ 3]      // Thread, in dem das Programm läuft               (variabel)   =>
   LaunchType         launchType;                  //       4      => ec[ 4]      // Launchtyp                                       (konstant)   => wie wurde ich gestartet
   EXECUTION_CONTEXT* superContext;                //       4      => ec[ 5]      // übergeordneter Execution-Context                (konstant)   => laufe ich in einem anderen Programm
   int                initFlags;                   //       4      => ec[ 6]      // init-Flags                                      (konstant)   => wie werde ich initialisiert
   int                deinitFlags;                 //       4      => ec[ 7]      // deinit-Flags                                    (konstant)   => wie werde ich deinitialisiert
   RootFunction       whereami;                    //       4      => ec[ 8]      // aktuelle Rootfunktion des Programms             (variabel)   => wo bin ich
   int                uninitializeReason;          //       4      => ec[ 9]      // letzter Uninitialize-Reason                     (variabel)   => woher komme ich

   char               symbol[MAX_SYMBOL_LENGTH+1]; //      12      => ec[10]      // aktuelles Symbol                                (variabel)   => auf welchem Symbol laufe ich
   int                timeframe;                   //       4      => ec[13]      // aktuelle Bar-Periode                            (variabel)   => mit welcher Bar-Periode laufe ich
   HWND               hChartWindow;                //       4      => ec[14]      // Chart-Fenster: mit Titelzeile "Symbol,Period"   (konstant)   => habe ich einen Chart und welchen
   HWND               hChart;                      //       4      => ec[15]      // Chart-Frame:   MQL => WindowHandle()            (konstant)   => ...
   int                testFlags;                   //       4      => ec[16]      // Tester-Flags: Off|On|VisualMode|Optimization    (konstant)   => laufe ich im Tester und wenn ja, wie

   int                lastError;                   //       4      => ec[17]      // letzter aufgetretener MQL-Fehler                (variabel)   => welcher MQL-Fehler ist aufgetreten
   DLL_ERROR**        dllErrors;                   //       4      => ec[18]      // Array von in der DLL aufgetretenen Fehlern      (variabel)   => welche DLL-Fehler sind aufgetreten
   int                dllErrorsSize;               //       4      => ec[19]      // Anzahl von DLL-Fehlern (Arraygröße)             (variabel)   => ...
   BOOL               logging;                     //       4      => ec[20]      // Logstatus                                       (konstant)   => was logge ich
   LPSTR              logFile;                     //       4      => ec[21]      // Zeiger auf Pfad und Namen der Logdatei          (konstant)   => wohin logge ich
};                                                 // ----------------------
                                                   //      88      = int[22]                                                                         warum bin ich nicht auf Ibiza





// Prototype
struct EXECUTION_CONTEXT_neu {
   int                id;
 //ProgramType        programType;                 // ok
   LPSTR              programName;

   HANDLE             hThreadId;
 //LaunchType         launchType;                  // ok
 //EXECUTION_CONTEXT* superContext;                // ok
 //int                initFlags;                   // ok
 //int                deinitFlags;                 // ok
 //RootFunction       whereami;                    // ok
   int                uninitializeReason;

   char               symbol[MAX_SYMBOL_LENGTH+1];
   int                timeframe;
   HWND               hChartWindow;                // Win32-Chartfenster (mit Titelzeile)
   HWND               hChart;                      // MQL-Chartfenster (Child von hWndChart = AfxFrame)
   int                testFlags;

   DLL_ERROR**        errors;                      // Array von DLL-Fehlern
   int                errorsSize;                  // Anzahl von DLL-Fehlern (Arraygröße)
   int                logLevel;
   LPSTR              logFile;
};
