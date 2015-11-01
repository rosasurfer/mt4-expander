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
std::vector<uint>       threadsProgram(64);                 // das von einem Thread jeweils zuletzt ausgeführte Programm
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
   RemoveTickTimers();
   return(TRUE);
}


/**
 * Wird von den MQL-Rootfunktionen der Hauptmodule (ggf. mehrmals) aufgerufen und setzt den aktuellen EXECUTION_CONTEXT
 * eines Hauptmoduls. Der Hauptmodulkontext von Indikatoren ändert sich mit jedem init()-Cycle, der von Experts und Scripten
 * nicht.
 * Da die Funktion sofort nach RootFunction-Eintritt aufgerufen werden muß (noch bevor evt. Libraries geladen werden), ist
 * der übergebene Context ggf. noch nicht vollständig initialisiert.
 *
 * Bei jedem Aufruf wird die Context-Chain des Programms mit der übergebenen Version aktualisiert.
 *
 *
 * @param  EXECUTION_CONTEXT* ec     - Context des Hauptmoduls eines MQL-Programms
 * @param  char*              name   - Name des Hauptmoduls (je nach MetaTrader-Version ggf. mit Pfad)
 * @param  char*              symbol - aktuelles Chart-Symbol
 * @param  int                period - aktuelle Chart-Periode
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SetMainExecutionContext(EXECUTION_CONTEXT* ec, const char* name, const char* symbol, int period) {
   if ((uint)ec     < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name   < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name));
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (period <= 0)                      return(debug("ERROR:  invalid parameter period = %d", period));


   //if (strcmp(ec->programName, "TestIndicator") == 0) {
   //   debug("ec:   %s::%s()  %s,%s", ec->programName, RootFunctionName(ec->rootFunction), ec->symbol, TimeframeDescription(ec->timeframe));
   //}


   // (1) Prüfen, ob der Context bereits bekannt ist
   int programId = ec->programId;
   if (!programId) {
      // (1.1) neuer Context: neue Context-Chain anlegen und zu den bekannten Chains hinzufügen
      if (logDebug) debug("thread=%d %s  %s::%s()  programId=0  creating new chain", GetCurrentThreadId(), (IsUIThread() ? "ui":"  "), ec->programName, RootFunctionName(ec->rootFunction));

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
      // (1.2) bekannter Context: alle Contexte seiner Context-Chain aktualisieren
      if (logDebug) debug("thread=%d %s  %s::%s()  programId=%d  updating chain", GetCurrentThreadId(), (IsUIThread() ? "ui":"  "), ec->programName, RootFunctionName(ec->rootFunction), programId);
      pec_vector &chain = contextChains[programId];

      // (1.3) Context des Hauptmodules nach Indicator::deinit() durch die übergebene Version ERSETZEN
      int i = 0;
      if (ec != chain[0])                                   // wenn sich der Pointer (= Speicherblock) des Hauptmodulkontext geändert hat
         chain[i++] = ec;

      // (1.4) alle Library-Contexte mit der übergebenen Version überschreiben
      int size = chain.size();
      for (; i < size; i++) {
         *chain[i] = *ec;
      }
   }


   // (2) Thread in den bekannten Threads suchen
   DWORD currentThread = GetCurrentThreadId();
   int size = threads.size();
   for (int i=0; i < size; i++) {                           // Aufwärts, damit der UI-Thread (Index 0 oder 1) schnellstmöglich gefunden wird.
      if (threads[i] == currentThread) {
         threadsProgram[i] = programId;                     // Thread gefunden: ID des laufenden Programms aktualisieren
         return(TRUE);
      }
   }


   // (3) Thread nicht gefunden: Thread und Programm hinzufügen
   EnterCriticalSection(&threadsLock);
   threads       .push_back(currentThread);
   threadsProgram.push_back(programId);
   if (logDebug || threads.size() > 32) debug("thread %d %s  added (size=%d)", currentThread, (IsUIThread() ? "ui":"  "), threads.size());
   LeaveCriticalSection(&threadsLock);

   return(TRUE);
   #pragma EXPORT
}


/**
 * Wird von MQL-Libraries aufgerufen, die ihren lokalen EXECUTION_CONTEXT mit dem des MQL-Hauptmodules synchronisieren
 * wollen. Ist der übergebene Context bereits initialisiert, bricht die Funktion ab. Nach dem Initialisieren wird der
 * Context zur Context-Chain des entsprechenden Programms hinzugefügt.
 *
 * @param  EXECUTION_CONTEXT* ec     - lokaler EXECUTION_CONTEXT einer Library
 * @param  char*              name   - Name der aufrufenden Library (je nach MetaTrader-Version ggf. mit Pfad)
 * @param  char*              symbol - aktuelles Chart-Symbol
 * @param  int                period - aktuelle Chart-Periode
 *
 * @return BOOL - Erfolgsstatus; FALSE, wenn der Context bereits initialisiert war
 *
 *
 * NOTE: letzte Version mit bedingungslosem Überschreiben durch den Main-Context: v1.63
 */
BOOL WINAPI SyncExecutionContext(EXECUTION_CONTEXT* ec, const char* name, const char* symbol, int period) {
   if ((uint)ec     < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name   < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name));
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (period <= 0)                      return(debug("ERROR:  invalid parameter period = %d", period));

   if (ec->programId)
      return(FALSE);                                        // Rückkehr, wenn der Context bereits initialisiert ist

   //debug("%s::init()", name);

   // aktuellen Thread in den bekannten Threads suchen      // Library wird zum ersten mal initialisiert, der Hauptmodul-Context
   DWORD currentThread = GetCurrentThreadId();              // ist immer gültig

   for (int i=threads.size()-1; i >= 0; i--) {
      if (threads[i] == currentThread) {                    // Thread gefunden
         int programId = threadsProgram[i];

         pec_vector &chain = contextChains[programId];
         *ec = *chain[0];                                   // Hauptkontext (Index 0) in den übergebenen Context kopieren
         chain.push_back(ec);                               // Context zur Programm-Chain hinzufügen

         if (strcmp(ec->programName, "TestIndicator") == 0) {
            debug("ec:   überschreibe mit main");
            debug("main: %s::%s()", ec->programName, RootFunctionName(ec->rootFunction));
         }
         return(TRUE);
      }
   }

   // Thread nicht gefunden
   debug("ERROR:  current thread %d not in known threads  [ERR_ILLEGAL_STATE]", currentThread);
   return(FALSE);
   #pragma EXPORT
}


/**
 * Setter ec.programId für alle Elemente einer EXECUTION_CONTEXT*-Chain
 *
 * @param pec_vector chain - Context-Chain eines MQL-Programms
 * @param uint       id    - zu setzende Programm-ID
 *
 * @return uint - dieselbe ID (for chaining)
 */
uint ecc_setProgramId(const pec_vector &chain, uint id) {
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
 * Ermittelt eine eindeutige Message-ID für den String "MetaTrader4_Internal_Message".
 *
 * @return uint - Message ID im Bereich 0xC000 bis 0xFFFF oder 0, falls ein Fehler auftrat.
 */
uint MT4InternalMsg() {
   static uint msgId;
   if (msgId)
      return(msgId);

   msgId = RegisterWindowMessageA("MetaTrader4_Internal_Message");
   if (!msgId)
      debug("ERROR:  RegisterWindowMessageA() failed with [%d]", GetLastError());

   return(msgId);
}


/**
 *
 */
int WINAPI Test() {

   float  f = 1.49999994f;
   double d = 1.49999994;

   //f=1.499999880790710
   //d=1.499999930000000

   debug("f=%.27f  d=%.27lf", f, d);

   return(0);






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
