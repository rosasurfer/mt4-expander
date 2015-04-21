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


// Thread- und EXECUTION_CONTEXT-Verwaltung
std::vector<DWORD>              threads;                    // alle bekannten Threads
std::vector<EXECUTION_CONTEXT*> lastContexts;               // der letzte Context je Thread
CRITICAL_SECTION                threadsLock;                // Lock


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
 * Aufruf beim Laden der DLL
 */
void onProcessAttach() {
   SetLogLevel(L_WARN);                                     // Default-Loglevel
   threads     .reserve(8);
   lastContexts.reserve(8);
   InitializeCriticalSection(&threadsLock);
}


/**
 * Threaddaten löschen
 */
void onThreadDetach() {
   DWORD currentThread = GetCurrentThreadId();

   // Thread in den bekannten Threads suchen und Daten löschen
   for (int i=threads.size()-1; i >= 0; i--) {
      if (threads[i] == currentThread) {
         EnterCriticalSection(&threadsLock);

         // Suche wiederholen, da die Daten inzwischen modifiziert worden sein können
         for (i=threads.size()-1; i >= 0; i--) {
            if (threads[i] == currentThread) {
               threads     .erase(threads     .begin() + i);
               lastContexts.erase(lastContexts.begin() + i);
               if (logDebug) debug("thread %d at index %d deleted (now %d threads)", currentThread, i, threads.size());
               break;
            }
         }

         LeaveCriticalSection(&threadsLock);
         break;
      }
   }
}


/**
 * Entladen der DLL
 */
void onProcessDetach() {
   onThreadDetach();
   DeleteCriticalSection(&threadsLock);
}


/**
 * Setzt den aktuellen EXECUTION_CONTEXT des Threads. Wird von jeder MQL-Rootfunktion aufgerufen, wodurch MQL-Libraries
 * den EXECUTION_CONTEXT ihres MQL-Hauptmoduls ermitteln können.
 *
 * @param EXECUTION_CONTEXT* ec - Kontext des Hauptmoduls eines MQL-Programms
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SetExecutionContext(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));

   // (1) ThreadId im übergebenen EXECUTION_CONTEXT aktualisieren (der Thread von Online-EA's wechselt bei jedem Tick)
   DWORD currentThread = GetCurrentThreadId();
   ec->hThreadId = currentThread;


   // (2) Thread in den bekannten Threads suchen
   for (int i=threads.size()-1; i >= 0; i--) {
      if (threads[i] == currentThread) {                    // Thread gefunden:
         lastContexts[i] = ec;                              // letzten Context aktualisieren
         return(TRUE);
      }
   }


   // (3) Thread nicht gefunden: Thread und Context hinzufügen
   EnterCriticalSection(&threadsLock);
   threads     .push_back(currentThread);
   lastContexts.push_back(ec           );
   if (threads.size() > 2)
      debug("thread %d added (now %d threads)", currentThread, threads.size());
   LeaveCriticalSection(&threadsLock);

   return(TRUE);


   /*
   // alle bekannten EXECUTION_CONTEXTe
   EXECUTION_CONTEXT** contexts;                            // Array EXECUTION_CONTEXT*[]
   int                 contextsSize;                        // Arraygröße
   CRITICAL_SECTION    contextsLock;                        // Lock

   // EXECUTION_CONTEXT in der Liste der bekannten Contexte suchen
   for (i=0; i < contextsSize; i++) {
      if (contexts[i] == ec) {
         if (logDebug) debug("context %d found in contexts[%d]", ec, i);
         break;
      }
   }

   // wenn Context nicht gefunden: hinzufügen
   if (i >= contextsSize) {
      EnterCriticalSection(&contextsLock);

      for (i=0; i < contextsSize; i++) {
         if (!contexts[i]) break;
      }
      if (i == contextsSize) {                              // kein freier Slot mehr vorhanden, Arrays vergrößern (Startwert: 32)
         int new_size = 2 * (contextsSize ? contextsSize : 1);
         if (logInfo) debug("sizeof(contexts) -> now %d", new_size);

         void** tmp = (void**) realloc(contexts, new_size * sizeof(EXECUTION_CONTEXT*)); if (!tmp) { LeaveCriticalSection(&contextsLock); return(debug("->realloc(contexts) failed")); }

         for (int n=contextsSize; n < new_size; n++) {   // neuen Bereich initialisieren
            tmp[n] = NULL;
         }
         contexts     = (EXECUTION_CONTEXT**) tmp;          // Zuweisung ändern
         contextsSize = new_size;
      }
      if (logDebug) debug("contexts[%d] = %d", i, ec);
      contexts[i] = ec;                                     // Context an Index i speichern

      LeaveCriticalSection(&contextsLock);
   }
   */
   #pragma EXPORT
}


/**
 * Kopiert den aktuellen EXECUTION_CONTEXT des Threads in die übergebene Variable und registriert sie für die
 * automatische Synchronisation. Diese Funktion wird von MQL-Libraries aufgerufen, um den EXECUTION_CONTEXT
 * ihres MQL-Hauptmodules zu ermitteln. Bei Änderungen an einem der verbundenen EXECUTION_CONTEXTe werden alle
 * zugehörigen EXECUTION_CONTEXTe aktualisiert.
 *
 * @param EXECUTION_CONTEXT* ec - Zeiger auf einen EXECUTION_CONTEXT
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI GetExecutionContext(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));

   DWORD currentThread = GetCurrentThreadId();

   // aktuellen Thread in den bekannten Threads suchen
   for (int i=threads.size()-1; i >= 0; i--) {
      if (threads[i] == currentThread) {                    // Thread gefunden
         *ec = *lastContexts[i];                            // Context kopieren

         // TODO: Context zu den synchron zu haltenden Contexten dieses Threads hinzufügen
         return(TRUE);
      }
   }

   // Thread nicht gefunden
   debug("current thread %d not in known threads  [ERR_ILLEGAL_STATE]", currentThread);
   return(FALSE);
   #pragma EXPORT
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
   return(SetExecutionContext(ec));
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI Test_onStart(EXECUTION_CONTEXT* ec, int logLevel) {
   SetLogLevel(logLevel);
   return(SetExecutionContext(ec));
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI Test_onDeinit(EXECUTION_CONTEXT* ec, int logLevel) {
   SetLogLevel(logLevel);
   return(SetExecutionContext(ec));
   #pragma EXPORT
}


/**
 *
 */
int WINAPI Test() {

   int_vector ints(8);

   ints.push_back(1);
   ints.push_back(1);
   ints.push_back(1);
   ints.push_back(1);
   ints.push_back(1);

   int_vector::iterator it = ints.begin();
   ints.erase(ints.begin() + 1);


   debug("capacity(ints)=%d  size(ints)=%d", ints.capacity(), ints.size());

   return(0);


   debug("sizeof(LaunchType)=%d  sizeof(EXECUTION_CONTEXT)=%d", sizeof(LaunchType), sizeof(EXECUTION_CONTEXT));
   return(0);

   std::string str("Hello world");
   int len = str.length();
   char* test = new char[len+1];
   str.copy(test, len);
   test[len] = '\0';
   delete test;
   return(0);

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
