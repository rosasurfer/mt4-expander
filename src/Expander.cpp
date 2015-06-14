/**
 * Der Expander des Fortschritts
 *
 * @author  Peter Walther
 *
 *
 * Post-Build Event: copy "$(TargetPath)" "$(SolutionDir)..\mt4\mql4\libraries\Expander.$(ConfigurationName)$(TargetExt)"
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


// Loglevel-Status
bool logDebug, logNotice, logInfo, logWarn, logError, logFatal;

// Thread- und EXECUTION_CONTEXT-Verwaltung
std::vector<DWORD>      threads       (64);                 // alle bekannten Thread-IDs
std::vector<UINT>       threadsProgram(64);                 // das von einem Thread jeweils zuletzt ausgeführte Programm
std::vector<pec_vector> contextChains (64);                 // alle bekannten Context-Chains (Index = ProgramID)
CRITICAL_SECTION        threadsLock;                        // Lock


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fReason, LPVOID lpReserved) {
   BOOL result = TRUE;

   switch (fReason) {
      case DLL_PROCESS_ATTACH: result = onProcessAttach(); break;
      case DLL_THREAD_ATTACH :                             break;
      case DLL_THREAD_DETACH :                             break;
      case DLL_PROCESS_DETACH: result = onProcessDetach(); break;
   }
   return(result);
}


/**
 * DllMain()-Aufruf beim Laden der DLL
 */
BOOL onProcessAttach() {
   SetLogLevel(L_WARN);                                     // Default-Loglevel
   //debug("thread %d %s", GetCurrentThreadId(), IsUIThread() ? "ui":"  ");

   threads       .resize(0);
   threadsProgram.resize(0);
   contextChains .resize(1);                                // Index[0] wäre keine gültige Programm-ID und bleibt daher frei
   InitializeCriticalSection(&threadsLock);
   return(TRUE);
}


/**
 * DllMain()-Aufruf beim Entladen der DLL
 */
BOOL onProcessDetach() {
   if (logDebug) debug("thread %d %s", GetCurrentThreadId(), IsUIThread() ? "ui":"  ");
   DeleteCriticalSection(&threadsLock);
   return(TRUE);
}


/**
 * Setzt den aktuellen EXECUTION_CONTEXT des Threads. Wird von jeder MQL-Rootfunktion aufgerufen, wodurch MQL-Libraries
 * den EXECUTION_CONTEXT ihres MQL-Hauptmoduls abfragen können.
 *
 * @param EXECUTION_CONTEXT* ec - Kontext des Hauptmoduls eines MQL-Programms
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SetExecutionContext(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));


   //debug("%s::%s  %s,%d", ec->programName, RootFunctionDescription(ec->rootFunction), ec->symbol, ec->timeframe);


   // (1) ThreadId des Contexts aktualisieren (der Thread von Online-EA's wechselt bei jedem Tick)
   DWORD currentThreadId = GetCurrentThreadId();
   ec->hThreadId = currentThreadId;


   // (2) Prüfen, ob der Context bereits bekannt ist
   int programId = ec->programId;
   if (!programId) {
      // (2.1) neuer Context: neue Context-Chain anlegen und zu den bekannten Chains hinzufügen
      if (logDebug) debug("thread=%d %s  %s::%s  programId=%d  creating new chain", GetCurrentThreadId(), (IsUIThread() ? "ui":"  "), ec->programName, RootFunctionDescription(ec->rootFunction), programId);

      pec_vector chain;
      chain.reserve(8);
      chain.push_back(ec);                                  // chain.size ist hier immer 1

      EnterCriticalSection(&threadsLock);
      contextChains.push_back(chain);
      int size = contextChains.size();                      // contextChains.size ist immer größer 1 (Index[0] bleibt frei)
      LeaveCriticalSection(&threadsLock);

      if (logDebug) debug("chains.size now %d", size);
      ec->programId = programId = size-1;                   // Die programId entspricht dem Index in contextChains[].
   }
   else {
      // (2.2) bekannter Context: alle Contexte seiner Context-Chain aktualisieren
      if (logDebug) debug("thread=%d %s  %s::%s  programId=%d  updating chain", GetCurrentThreadId(), (IsUIThread() ? "ui":"  "), ec->programName, RootFunctionDescription(ec->rootFunction), programId);
      pec_vector &chain = contextChains[programId];

      // (2.3) EXECUTION_CONTEXT des Hauptmodules nach Indicator::deinit() durch die übergebene Version ERSETZEN
      int i = 0;
      if (ec != chain[0])                                   // wenn sich der Pointer (= Speicherblock) auf den Hauptmodulkontext ändert
         chain[i++] = ec;

      // (2.4) alle weiteren Contexte (Libraries) mit der übergebenen Version überschreiben
      int size = chain.size();
      for (; i < size; i++) {
         *chain[i] = *ec;
      }
   }


   // (3) Thread in den bekannten Threads suchen
   int size = threads.size();
   for (int i=0; i < size; i++) {                           // Aufwärts, damit der UI-Thread (Index 0 oder 1) schnellstmöglich gefunden wird.
      if (threads[i] == currentThreadId) {
         threadsProgram[i] = programId;                     // Thread gefunden: ID des laufenden Programms aktualisieren
         return(TRUE);
      }
   }


   // (4) Thread nicht gefunden: Thread und Programm hinzufügen
   EnterCriticalSection(&threadsLock);
   threads       .push_back(currentThreadId);
   threadsProgram.push_back(programId);
   if (logDebug || threads.size() > 32) debug("thread %d %s  added (now %d threads)", currentThreadId, (IsUIThread() ? "ui":"  "), threads.size());
   LeaveCriticalSection(&threadsLock);

   return(TRUE);
   #pragma EXPORT
}


/**
 * Kopiert den aktuellen EXECUTION_CONTEXT des Threads in die übergebene Variable und registriert sie für die
 * automatische Synchronisation. Diese Funktion wird von MQL-Libraries aufgerufen, um den EXECUTION_CONTEXT
 * ihres MQL-Hauptmodules zu ermitteln. Bei Änderungen eines der verketteten EXECUTION_CONTEXTe werden alle
 * EXECUTION_CONTEXTe der Chain aktualisiert.
 *
 * @param EXECUTION_CONTEXT* ec - Zeiger auf einen EXECUTION_CONTEXT
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI GetExecutionContext(EXECUTION_CONTEXT* ec) {
   if ((int)ec < MIN_VALID_POINTER) return(debug("invalid parameter ec = 0x%p (not a valid pointer)", ec));

   return(FALSE);

   DWORD currentThread = GetCurrentThreadId();

   // aktuellen Thread in den bekannten Threads suchen
   EnterCriticalSection(&threadsLock);
   for (int i=threads.size()-1; i >= 0; i--) {
      if (threads[i] == currentThread) {                    // Thread gefunden
         int programId = threadsProgram[i];
         *ec = *contextChains[programId][0];                // Hauptkontext kopieren (Index 0)
         contextChains[programId].push_back(ec);            // Context zur Chain des Programms hinzufügen
         LeaveCriticalSection(&threadsLock);

         debug("%s::%s", ec->programName, RootFunctionDescription(ec->rootFunction));

         return(TRUE);
      }
   }
   LeaveCriticalSection(&threadsLock);

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
      //if (logDebug) debug("chain.size=%d  programName[%d]=%s", size, i, chain[i]->programName);
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
   pec_vector  ecChain(0);
   pec_vector* ecChain2 = new std::vector<EXECUTION_CONTEXT*>(2);
   delete ecChain2;

   //ecChain      .push_back(ec1);
   //ecChain      .push_back(ec2);
   //ecChain      .push_back(ec3);
   //contextChains.push_back(ecChain);
   return(0);

   typedef std::vector<int> int_vector;
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
