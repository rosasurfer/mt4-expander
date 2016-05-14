/**
 * Der Expander des Fortschritts
 *
 * @author  Peter Walther
 *
 *
 * Post-Build Event: copy "$(TargetPath)" "$(SolutionDir)..\mt4\mql4\libraries\Expander.$(ConfigurationName)$(TargetExt)"
 */
#include "stdafx.h"
#include "common.h"
#include "Expander.h"


// Loglevel-Status
bool logDebug, logNotice, logInfo, logWarn, logError, logFatal;

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
   //debug("thread=%d %s", GetCurrentThreadId(), IsUIThread() ? "ui":"  ");
   //SetLogLevel(L_WARN);                                     // Default-Loglevel

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
 * Synchronisiert den EXECUTION_CONTEXT eines MQL-Hauptmoduls. Wird nur von den Rootfunktionen der MQL-Hauptmodule aufgerufen.
 * In init() wird der Kontext teilweise in mehreren Schritten initialisiert (jeweils nachdem die entsprechenden Informationen
 * verfügbar sind).
 *
 * Bei Experts und Scripts gibt es während ihrer Laufzeit nur eine Instanz des Hauptmodulkontextes. Bei Indikatoren ändert sich die
 * Instanz mit jedem init()-Cycle, da MetaTrader den Arrayspeicher der Instanz in Indicator::init() jedesmal neu alloziiert.
 *
 * @param  EXECUTION_CONTEXT* ec           - Context des Hauptmoduls eines MQL-Programms
 * @param  ModuleType         moduleType   - Modul-Typ, entspricht Programm-Typ
 * @param  char*              moduleName   - Name des Hauptmoduls, entspricht Programmname (je nach MetaTrader-Version ggf. mit Pfad)
 * @param  RootFunction       rootFunction - MQL-RootFunction, innerhalb der der Aufruf erfolgt
 * @param  UninitializeReason reason       - MQL-UninitializeReason zum Zeitpunkt des Aufrufs
 * @param  char*              symbol       - aktuelles Chart-Symbol
 * @param  int                period       - aktuelle Chart-Periode
 *
 * @return BOOL - Erfolgsstatus
 *
 *
 * Notes:
 * ------
 * • Im Indikator gibt es während eines init()-Cycles in der Zeitspanne vom Verlassen von Indicator::deinit() bis zum Wiedereintritt
 *   in Indicator::init() keinen gültigen Hauptmodulkontext. Der alte Speicherblock wird sofort freigegeben, in init() wird ein neuer
 *   alloziiert. Während dieser Zeitspanne wird der init()-Cycle von bereits geladenen Libraries durchgeführt und es darf nicht auf
 *   den dann ungültigen Hauptmodulkontext zugegriffen werden.
 * • Nach Recompilation oder Crash einer Library wird der Speicherblock ihres Kontexts ungültig und auf ihn darf ebenfalls nicht mehr
 *   zugegriffen werden.
 * • Generell ist es empfehlenswert, lesend und schreibend nur den eigenen und den Master-Kontext zu verwenden. Der Master-Kontext
 *   ist immer gültig.
 *
 *
 *  Ablauf:
 *  --- Laden des Indikators ---------------------------------------------------------------------------------------------
 *  SetMainExecutionContext()  Indicator::init()              programId=0  creating new chain             set: programId=1
 *  SyncExecutionContext()     Indicator::library1::init()    programId=0  loaded by Indicator            set: programId=1
 *  SyncExecutionContext()     Indicator::library2::init()    programId=0  loaded by Indicator            set: programId=1
 *  SyncExecutionContext()     Indicator::library3::init()    programId=0  loaded by Indicator::library1  set: programId=1
 *  --- deinit() ---------------------------------------------------------------------------------------------------------
 *                             Indicator::deinit()            programId=1  Indikator zuerst
 *                             Indicator::library1::deinit()  programId=1  dann die Libraries
 *                             Indicator::library3::deinit()  programId=1  hierarchisch, nicht in Ladereihenfolge
 *                             Indicator::library2::deinit()  programId=1
 *  --- init() -----------------------------------------------------------------------------------------------------------
 *  SyncExecutionContext()     Indicator::library1::init()    programId=1  Libraries zuerst (hierarchisch)
 *  SyncExecutionContext()     Indicator::library3::init()    programId=1  neues Symbol/neuer Timeframe tauchen auf
 *  SyncExecutionContext()     Indicator::library2::init()    programId=1
 *  SetMainExecutionContext()  Indicator::init()              programId=0  zum Schluß der Indikator       set: programId=1
 *  ----------------------------------------------------------------------------------------------------------------------
 */
BOOL WINAPI SyncMainExecutionContext(EXECUTION_CONTEXT* ec, ModuleType moduleType, const char* moduleName, RootFunction rootFunction, UninitializeReason reason, const char* symbol, int period) {
   if ((uint)ec         < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)moduleName < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter moduleName = 0x%p (not a valid pointer)", moduleName));
   if ((uint)symbol     < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (period <= 0)                          return(debug("ERROR:  invalid parameter period = %d", period));


   // (1) Context aktualisieren
   if (rootFunction == RF_INIT) {
      if (!ec->programType)  ec_SetProgramType(ec, (ProgramType)moduleType);  // im Hauptkontext gilt: Module[Name|Type] = Program[Name|Type]
      if (!*ec->programName) ec_SetProgramName(ec, moduleName);
      if (!ec->moduleType)   ec_SetModuleType (ec, moduleType);
      if (!*ec->moduleName)  ec_SetModuleName (ec, moduleName);
                             ec_SetSymbol     (ec, symbol);
                             ec->timeframe   = period;
   }
   ec->rootFunction       = rootFunction;
   ec->uninitializeReason = reason;


   // (2) das letzte Programm des aktuellen Threads ermitteln
   DWORD currentThreadId = GetCurrentThreadId();
   int currentThreadIndex=-1, lastProgramId=0, size=threadIds.size();
   for (int i=0; i < size; i++) {                           // Aufwärts, damit der UI-Thread (Index 0 oder 1) schnellstmöglich gefunden wird.
      if (threadIds[i] == currentThreadId) {
         currentThreadIndex = i;
         lastProgramId      = threadIdsProgramIds[i];       // Thread gefunden: Index und ID des zuletzt gelaufenen Programms merken, wird
         break;                                             //                  später in (4) aktualisiert
      }
   }


   // (3) Prüfen, ob der Context bereits bekannt ist
   if (!ec->programId) {
      // (3.1) prüfen, ob wir in einem Indikator im init-Cycle sind
      if (IsUIThread() && lastProgramId && ec->programType==PT_INDICATOR && rootFunction==RF_INIT && (reason==REASON_CHARTCHANGE || reason==REASON_PARAMETERS)) {
         pec_vector &chain = contextChains[lastProgramId];
         if (!chain[1]) {
            // (3.2) Context eines Indikators nach init-Cycle
            if (0) debug("thread=%d ui  %s::%s()  programId=0  reason=%s  lastProgramId=%d  my-init-cycle  chain[1]=%p  master=%p", GetCurrentThreadId(), moduleName, RootFunctionName(rootFunction), UninitializeReasonToStr(reason), lastProgramId, chain[1], chain[0]);
            *ec = *chain[0];                                // Master-Kontext übernehmen
            ec_SetSymbol(ec, symbol);
            ec->timeframe          = period;                // Kontext aktualisieren
            ec->rootFunction       = rootFunction;
            ec->uninitializeReason = reason;
            chain[1] = ec;                                  // Kontext als Hauptmodulkontext speichern
         }
         else {
            // - Nach Recompilation eines Indikators werden Indikator und Libraries komplett neugeladen.
            // - Beim ersten Laden eines Indikators wird REASON_PARAMETERS gesetzt, obwohl es keinen init-Cycle gab.
            debug("WARN:  thread=%d ui  %s::init()  programId=0  reason=%s  chain[1]=%p  new-indicator / former-recompilation / indicator-with-no-libraries???", GetCurrentThreadId(), moduleName, UninitializeReasonToStr(reason), chain[1]);
         }
      }

      // (3.3) neuer Context: neue Context-Chain anlegen und zu den bekannten Chains hinzufügen
      if (!ec->programId) {
         if (0) debug("thread=%d %s  %s::%s()  programId=0  reason=%s  creating new chain", GetCurrentThreadId(), (IsUIThread() ? "ui":"  "), moduleName, RootFunctionName(rootFunction), UninitializeReasonToStr(reason));
         EXECUTION_CONTEXT* master = new EXECUTION_CONTEXT;    // Master-Kontext erzeugen
         *master = *ec;                                        // Daten kopieren

         pec_vector chain;
         chain.reserve(8);
         chain.push_back(master);                              // Master-Kontext an Index 0 der Chain speichern
         chain.push_back(ec);                                  // chain.size ist immer mindestens 2

         EnterCriticalSection(&terminalLock);
         contextChains.push_back(chain);
         int size = contextChains.size();                      // contextChains.size ist immer größer 1 (Index[0] bleibt frei)
         master->programId = ec->programId = size-1;           // Die ec.programId entspricht dem Index in contextChains[].
         if (0) debug("sizeof(chains)=%d  programId=%d", size, ec->programId);
         LeaveCriticalSection(&terminalLock);
      }
   }


   // (4) Master-Kontext aktualisieren                         // Unnötig, wenn der Kontext gerade erzeugt wurde. Es ist jedoch besser,
   pec_vector &chain = contextChains[ec->programId];           // diesen Code an nur einer Stelle zu halten.
   if (ec != chain[1]) return(debug("ERROR:  thread=%d %s  %s::%s()  programId=%d   current-ec=%p != chain[1]=%p", GetCurrentThreadId(), (IsUIThread() ? "ui":"  "), moduleName, RootFunctionName(rootFunction), ec->programId, ec, chain[1]));
   if (rootFunction == RF_INIT) {
      ec_SetSymbol(chain[0], symbol);
      chain[0]->timeframe = period;
   }
   chain[0]->rootFunction       = rootFunction;
   chain[0]->uninitializeReason = reason;


   // (5) Programm-ID des aktuellen Threads aktualisieren
   if (currentThreadIndex >= 0) {
      threadIdsProgramIds[currentThreadIndex] = ec->programId;
      return(TRUE);
   }


   // (6) Thread ist neu: Thread und Programm-ID zur Verwaltung hinzufügen
   EnterCriticalSection(&terminalLock);
   threadIds          .push_back(currentThreadId);
   threadIdsProgramIds.push_back(ec->programId);
   if (logDebug || threadIds.size() > 128) debug("thread %d %s  added (size=%d)", currentThreadId, (IsUIThread() ? "ui":"  "), threadIds.size());
   LeaveCriticalSection(&terminalLock);

   return(TRUE);
   #pragma EXPORT
}


/**
 * Synchronisiert den EXECUTION_CONTEXT einer MQL-Library mit dem Kontext ihres Hauptmodules. Wird nur von Libraries in init()
 * oder deinit() aufgerufen. Der Kontext neu geladener Libraries wird zur Context-Chain des MQL-Programms hinzugefügt.
 *
 * @param  EXECUTION_CONTEXT* ec           - lokaler EXECUTION_CONTEXT einer MQL-Library
 * @param  char*              moduleName   - Name der aufrufenden Library (je nach MetaTrader-Version ggf. mit Pfad)
 * @param  RootFunction       rootFunction - MQL-RootFunction der Library, innerhalb der der Aufruf erfolgt
 * @param  char*              symbol       - aktuelles Chart-Symbol
 * @param  int                period       - aktuelle Chart-Periode
 *
 * @return BOOL - Erfolgsstatus
 *
 *
 * Notes:
 * ------
 * Ist in Library::init() der übergebene Context bereits initialisiert (ec.programId ist gesetzt), befindet sich das Programm in
 * einem init()-Cycle. Zum init()-Cycle von Indikatoren: @see SetMainExecutionContext()
 */
BOOL WINAPI SyncLibExecutionContext(EXECUTION_CONTEXT* ec, const char* moduleName, RootFunction rootFunction, const char* symbol, int period) {
   if ((uint)ec         < MIN_VALID_POINTER)             return(debug("ERROR:  invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)moduleName < MIN_VALID_POINTER)             return(debug("ERROR:  invalid parameter moduleName = 0x%p (not a valid pointer)", moduleName));
   if (rootFunction!=RF_INIT && rootFunction!=RF_DEINIT) return(debug("ERROR:  illegal library parameter rootFunction = %s", RootFunctionToStr(rootFunction)));
   if ((uint)symbol     < MIN_VALID_POINTER)             return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (period <= 0)                                      return(debug("ERROR:  invalid parameter period = %d", period));

   ec->rootFunction = rootFunction;                         // Context-Daten aktualisieren


   // (1) Index des aktuellen Threads ermitteln
   DWORD currentThreadId = GetCurrentThreadId();
   int currentThreadIndex=-1, size=threadIds.size();
   for (int i=0; i < size; i++) {                           // Aufwärts, damit der UI-Thread (Index 0 oder 1) schnellstmöglich gefunden wird.
      if (threadIds[i] == currentThreadId) {
         currentThreadIndex = i;                            // Thread gefunden: Index merken
         break;
      }
   }
   if (currentThreadIndex < 0) return(debug("ERROR:  current thread %d not in known threads [ERR_ILLEGAL_STATE]", currentThreadId));


   if (rootFunction == RF_INIT) {
      if (!ec->programId) {
         // (2) Library wird zum ersten mal initialisiert
         int lastProgramId = threadIdsProgramIds[currentThreadIndex];

         pec_vector &chain = contextChains[lastProgramId];
         int size = chain.size(); if (size < 2) return(debug("ERROR:  ???::%s::init()  programId=0  lastProgramid=%d  chain.size=%d", moduleName, lastProgramId, size));

         if (0) debug("%s::%s::init()  übernehme ec von  programId=%d  i-am-module=%d", chain[0]->programName, moduleName, chain[0]->programId, chain.size());
         *ec = *chain[0];                                   // Master-Kontext (Index 0) übernehmen

         ec_SetModuleType(ec, MT_LIBRARY);                  // Context-Daten aktualisieren (Symbol und Timeframe in Libraries ignorieren)
         ec_SetModuleName(ec, moduleName);
         ec->rootFunction = rootFunction;
         ec->lastError    = NO_ERROR;
         chain.push_back(ec);                               // Context zur Programm-Chain hinzufügen
      }
      else {
         // (3) Library ist in init() und war bereits initialisiert, also init()-Cycle
         ec->lastError = NO_ERROR;                          // Symbol und Timeframe in Libraries ignorieren

         pec_vector &chain = contextChains[ec->programId];  // die erste Library invalidiert den Zeiger auf den Hauptkontext
         if (chain[1]) {
            if (0) debug("%s::%s::init()  programId=%d  init cycle,  setting main-ec to NULL, was chain[1]=%p  bak=%p", ec->programName, moduleName, ec->programId, chain[1], chain[0]);
            chain[1] = NULL;                                // Hauptkontext auf NULL setzen
            ec_SetSymbol(chain[0], symbol);                 // Master-Kontext aktualisieren. Dies ist der früheste Zeitpunkt, an dem das neue
            chain[0]->timeframe = period;                   // Symbol/der neue Timeframe bekannt sind. Sie werden nur hier gebraucht.
         }
      }
   }


   // (4) Programm-ID des aktuellen Threads aktualisieren
   threadIdsProgramIds[currentThreadIndex] = ec->programId;

   return(TRUE);
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
uint WINAPI MT4InternalMsg() {
   static uint msgId;
   if (msgId)
      return(msgId);

   msgId = RegisterWindowMessageA("MetaTrader4_Internal_Message");
   if (!msgId)
      debug("ERROR:  RegisterWindowMessageA() failed with [%d]", GetLastError());

   return(msgId);
   #pragma EXPORT
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

   debug("sizeof(EXECUTION_CONTEXT) = %d", sizeof(EXECUTION_CONTEXT));
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


