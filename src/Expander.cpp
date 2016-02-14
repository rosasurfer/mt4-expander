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
std::vector<DWORD>      threadIds          (64);            // alle bekannten Thread-IDs
std::vector<uint>       threadIdsProgramIds(64);            // ID des von einem Thread zuletzt ausgeführten MQL-Programms
std::vector<pec_vector> contextChains      (64);            // alle bekannten Context-Chains (Index = ProgramID)
CRITICAL_SECTION        terminalLock;                       // Terminal-weites Lock


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

   threadIds          .resize(0);
   threadIdsProgramIds.resize(0);
   contextChains      .resize(1);                           // Index[0] wäre keine gültige Programm-ID und bleibt daher frei
   InitializeCriticalSection(&terminalLock);
   return(TRUE);
}


/**
 * DllMain()-Aufruf beim Entladen der DLL
 */
BOOL onProcessDetach() {
   if (logDebug) debug("thread %d %s", GetCurrentThreadId(), IsUIThread() ? "ui":"  ");

   DeleteCriticalSection(&terminalLock);
   RemoveTickTimers();
   return(TRUE);
}


/**
 * Setzt den aktuellen EXECUTION_CONTEXT eines MQL-Hauptmoduls und aktualisiert die Context-Chain des Programms mit den Daten der
 * übergebenen Version. Die Funktion wird von den Rootfunktionen der MQL-Hauptmodule aufgerufen. In init() wird der Kontext teilweise
 * in mehreren Schritten initialisiert (jeweils nachdem die entsprechenden Informationen verfügbar sind).
 *
 * Bei Experts und Scripts gibt es während ihrer Laufzeit nur eine Instanz des Hauptmodulkontextes. Bei Indikatoren ändert sich die
 * Instanz mit jedem init()-Cycle, da MetaTrader den Arrayspeicher der Instanz in Indicator::init() jedesmal neu alloziiert.
 *
 * @param  EXECUTION_CONTEXT* ec           - Context des Hauptmoduls eines MQL-Programms
 * @param  char*              programName  - Name des Hauptmoduls (je nach MetaTrader-Version ggf. mit Pfad)
 * @param  RootFunction       rootFunction - MQL-RootFunction, innerhalb der der Aufruf erfolgt
 * @param  char*              symbol       - aktuelles Chart-Symbol
 * @param  int                period       - aktuelle Chart-Periode
 *
 * @return BOOL - Erfolgsstatus
 *
 *
 * Notes:
 * ------
 * Im Indikator gibt es während eines init()-Cycles in der Zeitspanne vom Verlassen von Indicator::deinit() bis zum Wiedereintritt in
 * Indicator::init() keinen gültigen Hauptmodulkontext. Der alte Speicherblock wird sofort freigegeben, in init() wird ein neuer
 * alloziiert. Während dieser Zeitspanne wird der init()-Cycle von bereits geladenen Libraries durchgeführt, also die Funktionen
 * Library::deinit() und Library::init() aufgerufen. In Indikatoren geladene Libraries dürfen daher während ihres init()-Cycles nicht
 * auf den zu diesem Zeitpunkt ungültigen Hauptmodulkontext zugreifen (weder lesend noch schreibend).
 */
BOOL WINAPI SetMainExecutionContext(EXECUTION_CONTEXT* ec, const char* programName, const RootFunction rootFunction, const char* symbol, int period) {
   if ((uint)ec          < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)programName < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter programName = 0x%p (not a valid pointer)", programName));
   if ((uint)symbol      < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (period <= 0)                           return(debug("ERROR:  invalid parameter period = %d", period));


   // (1) Context-Daten übernehmen
   if (!*ec->programName) {
      ec_setProgramName(ec, programName);
   }
   if (rootFunction == ROOTFUNCTION_INIT) {
      ec_setSymbol(ec, symbol);
      ec->timeframe = period;
   }
   ec->rootFunction = rootFunction;


   // (2) Prüfen, ob der Context bereits bekannt ist
   int programId = ec->programId;
   if (!programId) {
      // (2.1) neuer Context: neue Context-Chain anlegen und zu den bekannten Chains hinzufügen
      if (logDebug) debug("thread=%d %s  %s::%s()  programId=0  creating new chain", GetCurrentThreadId(), (IsUIThread() ? "ui":"  "), programName, RootFunctionName(rootFunction));

      pec_vector chain;
      chain.reserve(8);
      chain.push_back(ec);                                  // chain.size ist hier immer 1

      EnterCriticalSection(&terminalLock);
      contextChains.push_back(chain);
      int size = contextChains.size();                      // contextChains.size ist immer größer 1 (Index[0] bleibt frei)
      ec->programId = programId = size-1;                   // Die programId entspricht dem Index in contextChains[].
      if (logDebug) debug("programId=%d  sizeof(chains) now %d", programId, size);
      LeaveCriticalSection(&terminalLock);
   }
   else {
      // (2.2) bekannter Context: alle Contexte seiner Context-Chain aktualisieren
      if (0 && (rootFunction!=ROOTFUNCTION_START || logDebug)) debug("thread=%d %s  %s::%s()  programId=%d  updating chain", GetCurrentThreadId(), (IsUIThread() ? "ui":"  "), programName, RootFunctionName(rootFunction), programId);
      pec_vector &chain = contextChains[programId];

      // (2.3) Context des Hauptmodules nach Indicator::deinit() durch die übergebene Version ERSETZEN
      int i = 0;
      if (ec != chain[0])                                   // wenn sich der Pointer (= Speicherblock) des Hauptmodulkontext geändert hat
         chain[i++] = ec;

      // (2.4) alle Library-Contexte mit der übergebenen Version überschreiben
      int size = chain.size();
      for (; i < size; i++) {
         *chain[i] = *ec;
      }
   }


   // (3) Thread in den bekannten Threads suchen und ID des laufenden Programms aktualisieren
   DWORD currentThreadId = GetCurrentThreadId();
   int size = threadIds.size();
   for (int i=0; i < size; i++) {                           // Aufwärts, damit der UI-Thread (Index 0 oder 1) schnellstmöglich gefunden wird.
      if (threadIds[i] == currentThreadId) {
         threadIdsProgramIds[i] = programId;                // Thread gefunden: ID des laufenden Programms aktualisieren
         return(TRUE);
      }
   }


   // (4) Thread nicht gefunden: Thread und Programm hinzufügen
   EnterCriticalSection(&terminalLock);
   threadIds          .push_back(currentThreadId);
   threadIdsProgramIds.push_back(programId);
   if (logDebug || threadIds.size() > 128) debug("thread %d %s  added (size=%d)", currentThreadId, (IsUIThread() ? "ui":"  "), threadIds.size());
   LeaveCriticalSection(&terminalLock);

   return(TRUE);
   #pragma EXPORT
}


/**
 * Synchronisiert den EXECUTION_CONTEXT einer MQL-Library mit dem Kontext ihres Hauptmodules. Wird nur von Libraries und nur in
 * Library::init() aufgerufen. Nach dem ersten Synchronisieren (Initialisierung) wird der Library-Context zur Context-Chain des
 * MQL-Programms hinzugefügt.
 *
 * @param  EXECUTION_CONTEXT* ec     - lokaler EXECUTION_CONTEXT einer MQL-Library
 * @param  char*              name   - Name der aufrufenden Library (je nach MetaTrader-Version ggf. mit Pfad)
 * @param  char*              symbol - aktuelles Chart-Symbol
 * @param  int                period - aktuelle Chart-Periode
 *
 * @return BOOL - Erfolgsstatus; FALSE, wenn der Context bereits initialisiert war
 *
 *
 * Notes:
 * ------
 * • Ist der übergebene Context bereits initialisiert (ec.programId ist gesetzt), befindet sich das Programm in einem init()-Cycle.
 * • Die letzte Version, die bei Aufruf fälschlicherweise bedingungslos durch den Main-Context überschrieben wurde, war v1.63.
 */
BOOL WINAPI SyncExecutionContext(EXECUTION_CONTEXT* ec, const char* name, const char* symbol, int period) {
   if ((uint)ec     < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name   < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name));
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (period <= 0)                      return(debug("ERROR:  invalid parameter period = %d", period));

   if (logDebug) debug("%s::init()  programId=%d", name, ec->programId);

   if (ec->programId)
      return(FALSE);                                        // Rückkehr, wenn der Context bereits initialisiert ist

   // aktuellen Thread in den bekannten Threads suchen      // Library wird zum ersten mal initialisiert, der Hauptmodul-Context
   DWORD currentThreadId = GetCurrentThreadId();            // ist immer gültig

   for (int i=threadIds.size()-1; i >= 0; i--) {
      if (threadIds[i] == currentThreadId) {                // Thread gefunden
         int programId = threadIdsProgramIds[i];

         pec_vector &chain = contextChains[programId];
         *ec = *chain[0];                                   // den übergebenen Library-Context mit dem Hauptkontext (Index 0) überschreiben
         chain.push_back(ec);                               // Context zur Programm-Chain hinzufügen

         if (logDebug) debug("überschreibe lib-ec mit main-ec of %s::%s()  programId=%d", ec->programName, RootFunctionName(ec->rootFunction), ec->programId);
         return(TRUE);
      }
   }

   // Thread nicht gefunden
   debug("ERROR:  current thread %d not in known threads  [ERR_ILLEGAL_STATE]", currentThreadId);
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

   typedef std::vector<int> int_vector;
   int_vector ints(1);

   ints.push_back(1);
   ints.push_back(1);
   ints.push_back(1);

   int_vector::iterator it = ints.begin();
   ints.erase(ints.begin() + 1);

   debug("capacity(ints)=%d  size(ints)=%d", ints.capacity(), ints.size());
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
