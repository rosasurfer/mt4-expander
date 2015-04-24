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
std::vector<DWORD>      threads;                            // alle bekannten Thread-IDs
std::vector<UINT>       threadProgramIds;                   // das von einem Thread jeweils zuletzt ausgeführte Programm
std::vector<pec_vector> contextChains;                      // alle bekannten Context-Chains (Index = ProgramID)
CRITICAL_SECTION        threadsLock;                        // Lock


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fReason, LPVOID lpReserved) {
   switch (fReason) {
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
   threads         .reserve( 8);
   threadProgramIds.reserve( 8);
   contextChains   .reserve(32);
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
               /*
               pec_vector* chain     = &contextChains[threadProgramIds[i]];         // ContextChain eines Scripts löschen
               EXECUTION_CONTEXT* ec = chain->at(0);                                // Da unbekannt ist, wann MetaTrader den Speicher freigibt,
               debug("dropping thread with %s", ProgramTypeToStr(ec->programType)); // sollte nach Script::deinit() nicht mehr auf die Contexte
               if (ec->programType == PROGRAMTYPE_SCRIPT)                           // zugegriffen werden.
                  chain->clear();
               */
               threads         .erase(threads         .begin() + i);
               threadProgramIds.erase(threadProgramIds.begin() + i);
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
   threads         .clear();
   threadProgramIds.clear();
   contextChains   .clear();
   DeleteCriticalSection(&threadsLock);
}


/**
 * Setzt den aktuellen EXECUTION_CONTEXT des Threads. Wird von jeder MQL-Rootfunktion aufgerufen, wodurch MQL-Libraries
 * den EXECUTION_CONTEXT ihres MQL-Hauptmoduls ermitteln können.
 *
 * @param EXECUTION_CONTEXT* pec - Kontext des Hauptmoduls eines MQL-Programms
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SetExecutionContext(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));


   // (1) bei unbekanntem Context neue Chain anlegen und zu den bekannten Chains hinzufügen
   int programId = ec->programId;
   if (!programId) {
      if (logDebug) debug("ec.programId=0,  creating new chain");
      pec_vector chain;
      chain.reserve(8);
      chain.push_back(ec);

      EnterCriticalSection(&threadsLock);
      int size = contextChains.size();
      if (!size) {
         if (logDebug) debug("initializing contextChains");
         contextChains.resize(++size);                      // Index[0] ist keine gültige Programm-ID und bleibt frei
      }
      contextChains.push_back(chain);
      if (logDebug) debug("contextChains.size now %d", contextChains.size());
      ec->programId = programId = size;                     // ecc_setProgramId() unnötig, da chain.size hier immer 1
      LeaveCriticalSection(&threadsLock);
   }


   // (2) ThreadId der Chain aktualisieren (der Thread von Online-EA's wechselt bei jedem Tick)
   DWORD currentThread = GetCurrentThreadId();
   ecc_setHThreadId(contextChains[programId], currentThread);


   // (3) Thread in den bekannten Threads suchen
   for (int i=threads.size()-1; i >= 0; i--) {
      if (threads[i] == currentThread) {
         threadProgramIds[i] = programId;                   // Thread gefunden: laufendes Programm aktualisieren
         return(TRUE);
      }
   }


   // (4) Thread nicht gefunden: Thread und Programm hinzufügen
   EnterCriticalSection(&threadsLock);
   threads         .push_back(currentThread);
   threadProgramIds.push_back(programId);
   if (logDebug || threads.size() > 2)
      debug("thread %d added (now %d threads)", currentThread, threads.size());
   LeaveCriticalSection(&threadsLock);

   return(TRUE);
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
         int programId = threadProgramIds[i];
         *ec = *contextChains[programId][0];                // Hauptkontext kopieren (Index 0)
         contextChains[programId].push_back(ec);            // Context zur Chain des Programms hinzufügen
         return(TRUE);
      }
   }

   // Thread nicht gefunden
   debug("current thread %d not in known threads  [ERR_ILLEGAL_STATE]", currentThread);
   return(FALSE);
   #pragma EXPORT
}


/**
 * Setter ec.hThreadId für alle Elemente einer EXECUTION_CONTEXT*-Chain
 *
 * @param pec_vector chain - Context-Chain eines MQL-Programms
 * @param DWORD      id    - zu setzende Thread-ID
 *
 * @return DWORD - dieselbe ID (for chaining)
 */
DWORD ecc_setHThreadId(pec_vector &chain, DWORD id) {
   int size = chain.size();
   for (int i=0; i < size; i++) {
      chain[i]->hThreadId = id;
   }
   return(id);
}


/**
 * Setter ec.programId für alle Elemente einer EXECUTION_CONTEXT*-Chain
 *
 * @param pec_vector chain - Context-Chain eines MQL-Programms
 * @param uint       id    - zu setzende Programm-ID
 *
 * @return uint - dieselbe ID (for chaining)
 */
uint ecc_setProgramId(pec_vector &chain, uint id) {
   int size = chain.size();
   for (int i=0; i < size; i++) {
      chain[i]->programId = id;
   }
   return(id);
}


/**
 *
 */
void WINAPI SetLogLevel(int level) {
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
   #pragma EXPORT
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
   typedef std::vector<int>                int_vector;

   EXECUTION_CONTEXT ec1;
   EXECUTION_CONTEXT ec2;
   EXECUTION_CONTEXT ec3;

   pec_vector ecChain(0);

   pec_vector* ecChain2 = new std::vector<EXECUTION_CONTEXT*>(2);

   //ecChain      .push_back(ec1);
   //ecChain      .push_back(ec2);
   //ecChain      .push_back(ec3);
   //contextChains.push_back(ecChain);
   return(0);


   int_vector ints(1);

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
