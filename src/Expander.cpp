/**
 * Der Expander des Fortschritts
 *
 * @author  Peter Walther
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


// Loglevel-Status
bool logDebug, logNotice, logInfo, logWarn, logError, logFatal;


// alle bekannten EXECUTION_CONTEXTe
EXECUTION_CONTEXT** contexts;                               // contexts[]           = Array EXECUTION_CONTEXT*[]
size_t              contextsSize;                           // Arraygröße
CRITICAL_SECTION    contextsSection;                        // Lock

// alle bekannten Threads
DWORD*              threads;                                // threads[]            = Array DWORD[]
// der letzte EXECUTION_CONTEXT je Thread
EXECUTION_CONTEXT** threadsLastContext;                     // threadsLastContext[] = Array EXECUTION_CONTEXT*[]
size_t              threadsSize;                            // Arraygröße, es gilt: count_of(threads) == count_of(threadsLastContext)
CRITICAL_SECTION    threadsSection;                         // Lock


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
   switch (reason) {
      case DLL_PROCESS_ATTACH: onProcessAttach(); break;
      case DLL_THREAD_ATTACH :                    break;
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
 * DLL-Hook für MQL::init()
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Context
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL WINAPI Expander_onInit(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(SetExecutionContext(ec));
   #pragma EXPORT
}


/**
 * DLL-Hook für MQL::start()
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Context
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL WINAPI Expander_onStart(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(SetExecutionContext(ec));
   #pragma EXPORT
}


/**
 * DLL-Hook für MQL::deinit()
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Context
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL WINAPI Expander_onDeinit(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(SetExecutionContext(ec));
   #pragma EXPORT
}


/**
 * Schaltet den aktuellen EXECUTION_CONTEXT des Threads um. Wird indirekt von jeder MQL-Rootfunktion aufgerufen,
 * wodurch MQL-Libraries den EXECUTION_CONTEXT ihres MQL-Hauptmodules ermitteln können.
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Kontext eines MQL-Programms
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL SetExecutionContext(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));

   // (1) ThreadId im übergebenen EXECUTION_CONTEXT aktualisieren (der Thread von Online-EA's wechselt bei jedem Tick)
   DWORD currentThread = GetCurrentThreadId();
   ec->hThreadId = currentThread;


   // (2) Thread in den bekannten Threads suchen
   size_t i;
   for (i=0; i < threadsSize; i++) {
      if (threads[i] == currentThread) {                    // wenn Thread gefunden: letzten Context aktualisieren
         threadsLastContext[i] = ec;
         break;
      }
   }


   // (3) wenn Thread nicht gefunden: Thread und Context hinzufügen
   if (i >= threadsSize) {
      EnterCriticalSection(&threadsSection);

      for (i=0; i < threadsSize; i++) {                     // ersten freien Slot suchen
         if (!threads[i]) break;
      }
      if (i >= threadsSize) {                               // kein freier Slot mehr vorhanden, Arrays vergrößern (Startwert: 8)
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


	// (4) EXECUTION_CONTEXT in der Liste der bekannten Contexte suchen
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
 * Kopiert den aktuellen EXECUTION_CONTEXT des Threads in die übergebene Variable und registriert sie für die
 * automatische Synchronisation. Diese Funktion wird von MQL-Libraries aufgerufen, um den EXECUTION_CONTEXT
 * ihres MQL-Hauptmodules zu ermitteln. Bei Änderungen an einem der verbundenen EXECUTION_CONTEXTe werden alle
 * zugehörigen EXECUTION_CONTEXTe aktualisiert.
 *
 * @param EXECUTION_CONTEXT* dest - Zeiger auf einen EXECUTION_CONTEXT
 *
 * @return BOOL - Erfolgsstaus
 */
BOOL WINAPI GetExecutionContext(EXECUTION_CONTEXT* dest) {
   if ((int)dest < MIN_VALID_POINTER) return(debug("invalid parameter dest = 0x%p (not a valid pointer)", dest));

   DWORD currentThread = GetCurrentThreadId();

   // aktuellen Thread in den bekannten Threads suchen
   for (size_t i=0; i < threadsSize; i++) {
      if (threads[i] == currentThread) {                    // Thread gefunden
         *dest = *threadsLastContext[i];                    // Context kopieren

         // TODO: Context zu den synchron zu haltenden Contexten dieses Threads hinzufügen

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

   // (1) aktuellen Thread in den bekannten Threads suchen
   for (size_t i=0; i < threadsSize; i++) {
      if (currentThread == threads[i]) {
         // (2) bei Sucherfolg Daten löschen, der Slot ist danach wieder verfügbar
         EnterCriticalSection(&threadsSection);
         threads           [i] = NULL;
         threadsLastContext[i] = NULL;
		   LeaveCriticalSection(&threadsSection);
         break;
      }
   }
   return(true);
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
