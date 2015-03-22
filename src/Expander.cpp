/**
 * Der Expander des Fortschritts
 *
 *
 * @author  Peter Walther
 */
#include "stdafx.h"
#include "Expander.h"


// Loglevel-Status
bool logDebug, logNotice, logInfo, logWarn, logError, logFatal;


// Liste der verwalteten EXECUTION_CONTEXTe
EXECUTION_CONTEXT** contexts;                               // Array contexts[]
size_t              contextsSize;                           // Arraygröße
CRITICAL_SECTION    contextsSection;                        // Lock

// Liste des letzten EXECUTION_CONTEXT je Thread
DWORD*              threads;                                // Array threads[]
size_t              threadsSize;                            // Arraygröße
EXECUTION_CONTEXT** threadsLastContext;                     // Array threadsLastContext[]
CRITICAL_SECTION    threadsSection;                         // Lock


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
   switch (reason) {
      case DLL_PROCESS_ATTACH: onProcessAttach(); break;
      case DLL_THREAD_ATTACH : onThreadAttach();  break;
      case DLL_THREAD_DETACH : onThreadDetach();  break;
      case DLL_PROCESS_DETACH: onProcessDetach(); break;
   }
   return(TRUE);
}


/**
 *
 */
void onProcessAttach() {
   SetLogLevel(L_WARN);                                     // Default-Loglevel
   InitializeCriticalSection(&contextsSection);
   InitializeCriticalSection(&threadsSection );
}


/**
 *
 */
void onThreadAttach() {
}


/**
 *
 */
void onThreadDetach() {
   ResetCurrentThreadData();
}


/**
 *
 */
void onProcessDetach() {
   onThreadDetach();
   DeleteCriticalSection(&contextsSection);
   DeleteCriticalSection(&threadsSection );
}


/**
 * Wird in init() eines jeden MQL-Programms aufgerufen.
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Kontext
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL WINAPI Expander_onInit(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(SetCurrentExecutionContext(ec));
   #pragma EXPORT
}


/**
 * Wird in start() eines jeden MQL-Programms aufgerufen.
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Kontext
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL WINAPI Expander_onStart(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(SetCurrentExecutionContext(ec));
   #pragma EXPORT
}


/**
 * Wird in deinit() eines jeden MQL-Programms aufgerufen.
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Kontext
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL WINAPI Expander_onDeinit(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(SetCurrentExecutionContext(ec));
   #pragma EXPORT
}


/**
 * Setzt den aktuellen EXECUTION_CONTEXT eines Threads. Wird von jeder MQL-Rootfunktion zu allererst aufgerufen,
 * wodurch Libraries den eigenen EXECUTION_CONTEXT abfragen können.
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Kontext eines MQL-Programms
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL SetCurrentExecutionContext(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));

   DWORD currentThread = GetCurrentThreadId();

   // (1) ThreadId im EXECUTION_CONTEXT aktualisieren
   if (ec->hThreadId != currentThread)
      ec->hThreadId = currentThread;


   // (2) Thread in den verwalteten Threads suchen
   size_t i;
   for (i=0; i < threadsSize; i++) {
      if (threads[i] == currentThread) {                    // Thread gefunden: letzten Context aktualisieren
         threadsLastContext[i] = ec;
         break;
      }
   }


   // (3) wenn Thread nicht gefunden: hinzufügen
   if (i >= threadsSize) {
      EnterCriticalSection(&threadsSection);

      for (i=0; i < threadsSize; i++) {                     // ersten freien Slot suchen
         if (!threads[i]) break;
      }
      if (i == threadsSize) {                               // kein freier Slot mehr vorhanden, Arrays vergrößern (Startwert: 8)
         size_t new_size = 2 * (threadsSize ? threadsSize : 4);
         if (logInfo) debug("sizeof(threads) -> now %d", new_size);

         DWORD* tmp1 = (DWORD*) realloc(threads,            new_size * sizeof(DWORD)             ); if (!tmp1) { LeaveCriticalSection(&threadsSection); return(debug("->realloc(threads) failed"));            }
         void** tmp2 = (void**) realloc(threadsLastContext, new_size * sizeof(EXECUTION_CONTEXT*)); if (!tmp2) { LeaveCriticalSection(&threadsSection); return(debug("->realloc(threadsLastContext) failed")); }

         for (size_t n=threadsSize; n < new_size; n++) {    // neuen Bereich initialisieren
            tmp1[n] = NULL;
            tmp2[n] = NULL;
         }
         threads            =                       tmp1;   // Zuweisung ändern
         threadsLastContext = (EXECUTION_CONTEXT**) tmp2;
         threadsSize        = new_size;
      }
      threads           [i] = currentThread;                // Thread und letzten Context an Index i setzen
      threadsLastContext[i] = ec;

	   LeaveCriticalSection(&threadsSection);
   }


	// (4) EXECUTION_CONTEXT in der Liste der verwalteten Contexte suchen
   for (i=0; i < contextsSize; i++) {
      if (contexts[i] == ec) {
         if (logDebug) debug("context %d found in contexts[%d]", ec, i);
         break;
      }
   }


	// (5) wenn Context nicht gefunden: hinzufügen
   if (i >= contextsSize) {
      EnterCriticalSection(&contextsSection);

      for (i=0; i < contextsSize; i++) {
         if (!contexts[i]) break;
      }
      if (i == contextsSize) {                              // kein freier Slot mehr vorhanden, Arrays vergrößern (Startwert: 32)
         size_t new_size = 2 * (contextsSize ? contextsSize : 1);
         if (logInfo) debug("sizeof(contexts) -> now %d", new_size);

         void** tmp = (void**) realloc(contexts, new_size * sizeof(EXECUTION_CONTEXT*)); if (!tmp) { LeaveCriticalSection(&contextsSection); return(debug("->realloc(contexts) failed")); }

         for (size_t n=contextsSize; n < new_size; n++) {   // neuen Bereich initialisieren
            tmp[n] = NULL;
         }
         contexts     = (EXECUTION_CONTEXT**) tmp;          // Zuweisung ändern
         contextsSize = new_size;
      }
      if (logDebug) debug("contexts[%d] = %d", i, ec);
      contexts[i] = ec;                                     // Context an Index i setzen

	   LeaveCriticalSection(&contextsSection);
   }

   return(TRUE);
}


/**
 * Kopiert den aktuellen EXECUTION_CONTEXT eines Threads in die angegebene Variable und registriert diese Kopie
 * für die automatische Synchronisation von Änderungen mit dem aktuellen EXECUTION_CONTEXT eines Threads.
 *
 * @param EXECUTION_CONTEXT* dest - Zeiger auf einen EXECUTION_CONTEXT
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL WINAPI SyncExecutionContext(EXECUTION_CONTEXT* dest) {
   if ((int)dest < MIN_VALID_POINTER) return(debug("invalid parameter dest = 0x%p (not a valid pointer)", dest));

   DWORD currentThread = GetCurrentThreadId();

   // aktuellen Thread in den bekannten Threads suchen
   for (size_t i=0; i < threadsSize; i++) {
      if (threads[i] == currentThread) {                    // Thread gefunden
         *dest = *threadsLastContext[i];                    // Context kopieren

         // Context zu den synchron zu haltenden Contexten dieses Threads hinzufügen

         return(TRUE);
      }
   }

   // Thread nicht gefunden
   return(debug("thread %d not found - ERR_ILLEGAL_STATE", currentThread));
   #pragma EXPORT
}


/**
 * Setzt die EXECUTION_CONTEXT-Daten des aktuellen Threads zurück.
 *
 * @return bool - Erfolgsstaus
 */
bool ResetCurrentThreadData() {
   DWORD currentThread = GetCurrentThreadId();

   // (1) aktuellen Thread in den Threads mit EXECUTION_CONTEXT suchen
   size_t i;
   for (i=0; i < threadsSize; i++) {
      if (currentThread == threads[i])
         break;
   }

   // (2) bei Sucherfolg Daten löschen, der Slot ist danach wieder verfügbar
   if (i < threadsSize) {
      EnterCriticalSection(&threadsSection);

      threads           [i] = NULL;
      threadsLastContext[i] = NULL;

		LeaveCriticalSection(&threadsSection);
   }
   return(true);
}


/**
 * Gibt die Speicheradresse eines MQL-Bool-Arrays zurück.
 *
 * @param  BOOL lpValues[] - MQL-Bool-Array (in MetaTrader als Integer-Array implementiert)
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetBoolsAddress(bool array[]);
 */
int WINAPI GetBoolsAddress(BOOL lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zurück.
 *
 * @param  int lpValues[] - MQL-Integer-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetIntsAddress(int array[]);
 */
int WINAPI GetIntsAddress(int lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Alias
 */
int WINAPI GetBufferAddress(int values[]) {
   return(GetIntsAddress(values));
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zurück.
 *
 * @param  double lpValues[] - MQL-Double-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetDoublesAddress(double array[]);
 */
int WINAPI GetDoublesAddress(double lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zurück.
 *
 * @param  MqlStr lpValues[] - MQL-String-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetStringsAddress(string values[]);
 */
int WINAPI GetStringsAddress(MqlStr lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines Strings zurück.
 *
 * @param  char* lpValue - String (MetaTrader übergibt für einen MQL-String das Element MqlStr.string)
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetStringAddress(string value);
 */
int WINAPI GetStringAddress(const char* lpValue) {
   if (lpValue && (int)lpValue < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue));
   return((int) lpValue);
   #pragma EXPORT
}


/**
 * Gibt den übergebenen Zeiger auf einen String selbst zurück. Kann in MQL zum Lesen eines Strings von einer Adresse
 * verwendet werden, da MetaTrader den Zeiger mit der entsprechenden Importdeklaration in einen MQL-String konvertiert.
 *
 * @param  char* lpValue - String
 *
 * @return char* - derselbe Zeiger oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  string GetString(int address);
 */
char* WINAPI GetString(const char* lpValue) {
   if (lpValue && (int)lpValue < MIN_VALID_POINTER)
      return((char*) debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue));
   return((char*) lpValue);
   #pragma EXPORT
}


/**
 * Gibt den letzten aufgetretenen Windows-Fehler des aktuellen Threads zurück. Wrapper für kernel32::GetLastError(),
 * da MQL eine Funktion desselben Namens definiert.
 *
 * @return int - Fehlercode
 *
 *
 * @mql-import:  int GetLastWin32Error();
 */
int WINAPI GetLastWin32Error() {
   return(GetLastError());
   #pragma EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen eingebauten MetaTrader-Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 *
 *
 * @mql-import:  bool IsBuiltinTimeframe(int timeframe);
 */
BOOL WINAPI IsBuiltinTimeframe(int timeframe) {
   switch (timeframe) {
      case PERIOD_M1 : return(TRUE);
      case PERIOD_M5 : return(TRUE);
      case PERIOD_M15: return(TRUE);
      case PERIOD_M30: return(TRUE);
      case PERIOD_H1 : return(TRUE);
      case PERIOD_H4 : return(TRUE);
      case PERIOD_D1 : return(TRUE);
      case PERIOD_W1 : return(TRUE);
      case PERIOD_MN1: return(TRUE);
   }
   return(FALSE);
   #pragma EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen benutzerdefinierten Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 *
 *
 * @mql-import:  bool IsCustomTimeframe(int timeframe);
 */
 BOOL WINAPI IsCustomTimeframe(int timeframe) {
   if (timeframe <= 0)
      return(FALSE);
   return(!IsBuiltinTimeframe(timeframe));
   #pragma EXPORT
}


/**
 * Gibt die hexadezimale Repräsentation eines Integers zurück.
 * z.B.: IntToHexStr(13465610) => "00CD780A"
 *
 * @param  int value - Integer (4 Byte)
 *
 * @return char* - hexadezimaler String mit 8 Zeichen
 *
 *
 * @mql-import:  string IntToHexStr(int value);
 */
char* WINAPI IntToHexStr(int value) {
   int   size = 9;
   char* str  = new char[size];
   sprintf_s(str, size, "%p", value);

   return(str);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Beschreibung eines ModuleTypes zurück.
 *
 * @param  ModuleType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* ModuleTypeDescription(ModuleType type) {
   switch (type) {
      case MT_EXPERT   : return("Expert"   );
      case MT_SCRIPT   : return("Script"   );
      case MT_INDICATOR: return("Indicator");
      case MT_LIBRARY  : return("Library"  );
   }
   debug("unknown module type = "+ to_string(type));
   return(NULL);
}


/**
 * Gibt die lesbare Konstante eines ModuleTypes zurück.
 *
 * @param  ModuleType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* ModuleTypeToStr(ModuleType type) {
   switch (type) {
      case MT_EXPERT   : return("MT_EXPERT"   );
      case MT_SCRIPT   : return("MT_SCRIPT"   );
      case MT_INDICATOR: return("MT_INDICATOR");
      case MT_LIBRARY  : return("MT_LIBRARY"  );
   }
   debug("unknown module type = "+ to_string(type));
   return(NULL);
}


/**
 * Gibt die lesbare Beschreibung einer RootFunction-ID zurück.
 *
 * @param  RootFunction id
 *
 * @return char* - Beschreibung oder NULL, falls die ID ungültig ist
 */
const char* RootFunctionDescription(RootFunction id) {
   switch (id) {
      case RF_INIT  : return("init()"  );
      case RF_START : return("start()" );
      case RF_DEINIT: return("deinit()");
   }
   debug("unknown MQL root function id = "+ to_string(id));
   return(NULL);
}


/**
 * Gibt die lesbare Konstante einer RootFunction-ID zurück.
 *
 * @param  RootFunction id
 *
 * @return char* - Beschreibung oder NULL, falls die ID ungültig ist
 */
const char* RootFunctionToStr(RootFunction id) {
   switch (id) {
      case RF_INIT  : return("init()"  );
      case RF_START : return("start()" );
      case RF_DEINIT: return("deinit()");
   }
   debug("unknown MQL root function id = "+ to_string(id));
   return(NULL);
}


/**
 *
 */
void SetLogLevel(int level) {
   logDebug = logNotice = logInfo = logWarn = logError = logFatal = false;
   switch (level) {
      case L_ALL   :
      case L_DEBUG : logDebug  = true;
      case L_NOTICE: logNotice = true;
      case L_INFO  : logInfo   = true;
      case L_WARN  : logWarn   = true;
      case L_ERROR : logError  = true;
      case L_FATAL : logFatal  = true;
   }
}


/**
 *
 */
BOOL WINAPI Test_onInit(EXECUTION_CONTEXT* ec, int logLevel) {
   SetLogLevel(logLevel);
   return(Expander_onInit(ec));
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI Test_onStart(EXECUTION_CONTEXT* ec, int logLevel) {
   SetLogLevel(logLevel);
   return(Expander_onStart(ec));
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI Test_onDeinit(EXECUTION_CONTEXT* ec, int logLevel) {
   SetLogLevel(logLevel);
   return(Expander_onDeinit(ec));
   #pragma EXPORT
}


/**
 *
 */
int WINAPI Test() {

   debug("sizeof(LaunchType)=%d  sizeof(EXECUTION_CONTEXT)=%d", sizeof(LaunchType), sizeof(EXECUTION_CONTEXT));
   return(0);

   std::string str("Hello world");
   int len = str.length();
   char* test = new char[len+1];
   str.copy(test, len);
   test[len] = '\0';

   debug("sizeof(EXECUTION_CONTEXT) = "+ to_string(sizeof(EXECUTION_CONTEXT)));

   /*
   debug("error.code=%d  error.message=%s", error->code, error->message);
   error->code    = 200;
   char* msg      = "200: OK";
   int   bufSize  = strlen(msg)+1;
   char* buffer   = new char[bufSize];
   strcpy_s(buffer, bufSize, msg);
   error->message = buffer;
   */

   /*
   auto_ptr<char> p(new char(10));
   int len = strlen(p.get());
   */

   return(0);
   #pragma EXPORT
}
