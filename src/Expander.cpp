#include "Expander.h"


std::vector<pec_vector> contextChains  (64);                         // alle Context-Chains (Index = ProgramID)
std::vector<DWORD>      threads        (64);                         // ID's aller bekannten Threads
std::vector<uint>       threadsPrograms(64);                         // ID's des vom Thread zuletzt ausgeführten MQL-Programms
uint                    lastUIThreadProgram;                         // ID des vom UI-Thread zuletzt ausgeführten MQL-Programm
CRITICAL_SECTION        terminalLock;                                // Terminal-weites Lock


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

   threads        .resize(0);
   threadsPrograms.resize(0);
   contextChains  .resize(1);                               // Index[0] wäre keine gültige Programm-ID und bleibt daher frei
   InitializeCriticalSection(&terminalLock);
   return(TRUE);
}


/**
 * DllMain()-Aufruf beim Entladen der DLL
 */
BOOL onProcessDetach() {
   //debug("thread=%d %s", GetCurrentThreadId(), IsUIThread() ? "ui":"  ");

   DeleteCriticalSection(&terminalLock);
   RemoveTickTimers();
   return(TRUE);
}


/**
 * Synchronisiert die EXECUTION_CONTEXTe der Module eines MQL-Programms mit dem Master-Context in der DLL und untereinander.
 *
 * Die EXECUTION_CONTEXTe dienen dem Datenaustausch zwischen mehreren MQL-Programmen, zwischen einzelnen Modulen desselben
 * Programms und zwischen einem Programm und der DLL. Jedes MQL-Modul verfügt über einen eigenen Kontext, alle Kontexte eines
 * MQL-Programms bilden gemeinsam eine Context-Chain. An erster Stelle einer Context-Chain liegt der Master-Context, der in der
 * DLL verwaltet wird. An zweiter Stelle liegt der Context des MQL-Hauptmodules (Expert, Script oder Indikator). Alle weiteren
 * Contexte einer Chain sind Library-Contexte. Über die Kontexte werden wie folgt Daten ausgetauscht:
 *
 *  (1) Datenaustausch vom Hauptmodul zu den Library-Modulen:
 *
 *  (2) Datenaustausch von den Library-Modulen zum Hauptmodul:
 *
 *  (3) Datenaustausch von der DLL zum Hauptmodul:
 *
 *  (4) Datenaustausch vom einem Hauptmodul zu einem anderen Hauptmodul:
 *
 * Kontextgültigkeit: Der Master-Context einer Chain ist immer gültig. Alle anderen Kontexte der Chain können je nach Modul-Typ
 * und Situation ungültig bzw. der Speicher nicht verfügbar sein (dazu später mehr). Von einem MQL-Modul darf generell nur auf
 * den eigenen und auf den Master-Context zugegriffen werden. Ein Zugriff auf den Hauptkontext aus einer Library und umgekehrt
 * ist nur in Ausnahmefällen möglich.
 *
 *
 *
 *
 * Wird von den Rootfunktionen der MQL-Hauptmodule aufgerufen.
 * In init() wird der Kontext in mehreren Schritten initialisiert (nachdem die entsprechenden Informationen verfügbar sind).
 *
 * Bei Experts und Scripts gibt es während der Laufzeit nur eine Instanz des Hauptmodulkontextes. Bei Indikatoren ändert sich die
 * Instanz mit jedem init()-Cycle, da MetaTrader den Speicher für Variablen in Indicator::init() jeweils neu alloziiert.
 *
 * @param  EXECUTION_CONTEXT* ec              - Context des Hauptmoduls eines MQL-Programms
 * @param  ProgramType        programType     - Programm-Typ
 * @param  char*              programName     - Programmname (je nach MetaTrader-Version ggf. mit Pfad)
 * @param  RootFunction       rootFunction    - MQL-RootFunction, innerhalb der der Aufruf erfolgt
 * @param  UninitializeReason uninitReason    - UninitializeReason as passed by the terminal
 * @param  DWORD              initFlags       - Init-Konfiguration
 * @param  DWORD              deinitFlags     - Deinit-Konfiguration
 * @param  char*              symbol          - aktuelles Chart-Symbol
 * @param  uint               period          - aktuelle Chart-Periode
 * @param  EXECUTION_CONTEXT* sec             - supercontext as passed by the terminal            (possibly invalid)
 * @param  BOOL               isTesting       - IsTesting() flag as passed by the terminal        (possibly incorrect)
 * @param  BOOL               isVisualMode    - IsVisualMode() flag as passed by the terminal     (possibly incorrect)
 * @param  HWND               hChart          - WindowHandle() as passed by the terminal          (possibly NULL)
 * @param  int                subChartDropped - WindowOnDropped() index as passed by the terminal
 *
 * @return BOOL - Erfolgsstatus
 *
 *
 * Notes:
 * ------
 * • Im Indikator gibt es während eines init()-Cycles in der Zeitspanne vom Verlassen von Indicator::deinit() bis zum Wiederein-
 *   tritt in Indicator::init() keinen gültigen Hauptkontext. Der alte Speicherblock wird sofort freigegeben, in init() wird ein
 *   neuer alloziiert. Während dieser Zeitspanne wird der init()-Cycle von bereits geladenen Libraries durchgeführt und es darf
 *   nicht auf den zu dem Zeitpunkt ungültigen Hauptkontext zugegriffen werden.
 * • Nach Recompilation oder Crash einer Library wird der Speicherblock ihres Kontexts ungültig und auf ihn darf ebenfalls nicht
 *   mehr zugegriffen werden.
 *
 *
 *  Init cycle of a single indicator examined:
 *  --- first load --------------------------------------------------------------------------------------------------------------
 *  Indicator::init()              programId=0  creating new chain             set programId=1
 *  Indicator::libraryA::init()    programId=0  loaded by indicator            set programId=1
 *  Indicator::libraryB::init()    programId=0  loaded by indicator            set programId=1
 *  Indicator::libraryC::init()    programId=0  loaded by libraryA             set programId=1
 *  --- deinit() ----------------------------------------------------------------------------------------------------------------
 *  Indicator::deinit()            programId=1  indicator first
 *  Indicator::libraryA::deinit()  programId=1  then libraries
 *  Indicator::libraryC::deinit()  programId=1  hierarchical (not loading order)
 *  Indicator::libraryB::deinit()  programId=1
 *  --- init() ------------------------------------------------------------------------------------------------------------------
 *  Indicator::libraryA::init()    programId=1  libraries first (new symbol/timeframe show up)
 *  Indicator::libraryC::init()    programId=1  hierarchical (not loading order)
 *  Indicator::libraryB::init()    programId=1
 *  Indicator::init()              programId=0  then indicator                 set programId=1
 *  -----------------------------------------------------------------------------------------------------------------------------
 *
 *
 *  Init cycle of multiple indicators examined:
 *  --- first load --------------------------------------------------------------------------------------------------------------
 *  ChartInfos::init()             REASON_UNDEFINED    programId=0  creating new chain   set programId=1
 *  ChartInfos::lib::init()        REASON_UNDEFINED    programId=0  loaded by indicator  set programId=1
 *
 *  SuperBars::init()              REASON_UNDEFINED    programId=0  creating new chain   set programId=2
 *  SuperBars::lib::init()         REASON_UNDEFINED    programId=0  loaded by indicator  set programId=2
 *  --- deinit() ----------------------------------------------------------------------------------------------------------------
 *  ChartInfos::deinit()           REASON_CHARTCHANGE  programId=1
 *  ChartInfos::lib::deinit()      REASON_UNDEFINED    programId=1
 *
 *  SuperBars::deinit()            REASON_CHARTCHANGE  programId=2
 *  SuperBars::lib::deinit()       REASON_UNDEFINED    programId=2
 *  --- init() ------------------------------------------------------------------------------------------------------------------
 *  ChartInfos::lib::init()        REASON_UNDEFINED    programId=1  set libMarker
 *  ChartInfos::init()             REASON_CHARTCHANGE  programId=0  libMarker=1          set programId=1
 *
 *  SuperBars::lib::init()         REASON_UNDEFINED    programId=2  set libMarker
 *  SuperBars::init()              REASON_CHARTCHANGE  programId=0  libMarker=1          set programId=2
 *  -----------------------------------------------------------------------------------------------------------------------------
 */
BOOL WINAPI SyncMainExecutionContext(EXECUTION_CONTEXT* ec, ProgramType programType, const char* programName, RootFunction rootFunction, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint period, EXECUTION_CONTEXT* sec, BOOL isTesting, BOOL isVisualMode, HWND hChart, int subChartDropped) {
   if ((uint)ec          < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)programName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter programName = 0x%p (not a valid pointer)", programName));
   if ((uint)symbol      < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((int)period <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter period = %d", period));
   if (sec && (uint)sec  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));

   //
   // Programmablauf
   // --------------
   // (1) Wenn keine ProgramID gesetzt ist, prüfen, ob Programm ein Indikator im Init-Cycle ist.
   //     • wenn Programm ein Indikator im Init-Cycle ist (kommt nur im UI-Thread vor)
   //       - Hauptkontext aus Master-Context restaurieren
   //     • wenn Programm kein Indikator im Init-Cycle ist
   //       - neuen Master-Context erzeugen
   //       - neue Context-Chain erzeugen und Master- und Hauptkontext darin speichern
   //       - resultierende ProgramID in Master- und Hauptkontext speichern
   //
   // (2) Hauptkontext aktualisieren (aktualisiert automatisch Master-Context)
   //
   // (3) Das vom aktuellen Thread zuletzt ausgeführte Programm (also das aktuelle) in global threadsPrograms[] speichern.
   //
   // (4) Ist der aktuelle Thread der UI-Thread, das aktuelle Programm zusätzlich in global lastUIThreadProgram speichern.
   //
   DWORD              currentThread = GetCurrentThreadId();
   InitializeReason   initReason    = (InitializeReason)NULL;
   EXECUTION_CONTEXT* master        = NULL;


   // (1) Wenn keine ProgramID gesetzt ist, prüfen, ob Programm ein Indikator im Init-Cycle ist.
   if (!ec->programId) {
      initReason                = InitReason(programName, programType, uninitReason, symbol, ec, sec, isTesting, isVisualMode, hChart, subChartDropped);
      BOOL indicatorInInitCycle = programType==PT_INDICATOR && rootFunction==RF_INIT && (initReason==INIT_REASON_PARAMETERS || initReason==INIT_REASON_SYMBOLCHANGE || initReason==INIT_REASON_TIMEFRAMECHANGE);

      if (indicatorInInitCycle) {
         // (1.1) Programm ist Indikator im Init-Cycle (immer im UI-Thread)
         //   - Indikator-Context aus Master-Context restaurieren
         master = contextChains[ec->programId][0];                   // im Init-Cycle setzt InitReason() die gefundene ID
         *ec = *master;                                              // Master-Context kopieren
         contextChains[ec->programId][1] = ec;                       // Context als Hauptkontext speichern
         debug("%s::%s()  programId=0  init-cycle, was id=%d  thread=%s", programName, RootFunctionDescription(rootFunction), ec->programId, IsUIThread() ? "UI": to_string(currentThread).c_str());
      }
      else {
         // (1.2) Programm ist kein Indikator im Init-Cycle
         //   - neue Context-Chain erzeugen
         //   - neuen Master-Context erzeugen
         //   - Master- und Hauptkontext in der Chain speichern
         //   - ProgramID generieren und diese Master- und Hauptkontext zuweisen
         master  = new EXECUTION_CONTEXT;                            // neuen Master-Context erzeugen
         *master = *ec;                                              // Hauptkontext kopieren
         pec_vector chain;                                           // neue Context-Chain erzeugen
         chain.reserve(8);
         chain.push_back(master);                                    // Master- und Hauptkontext in der Chain speichern
         chain.push_back(ec);

         EnterCriticalSection(&terminalLock);
         contextChains.push_back(chain);                             // Chain in der Chain-Liste speichern
         uint size = contextChains.size();                           // contextChains.size ist immer > 1 (index[0] bleibt frei)
         master->programId = ec->programId = size-1;                 // Index = neue ProgramID dem Master- und Hauptkontext zuweisen
         debug("%s::%s()  programId=0  %snew chain => id=%d  thread=%s  hChart=%d", programName, RootFunctionDescription(rootFunction), (IsUIThread() ? "UI  ":""), ec->programId, IsUIThread() ? "UI":to_string(currentThread).c_str(), hChart);
         LeaveCriticalSection(&terminalLock);
      }
   }
   else {
      master     = contextChains[ec->programId][0];
      initReason = ec->initReason;
   }


   // (2.1) Beim Aufruf aus init() zu aktualisieren:
   if (rootFunction == RF_INIT) {
      if (!ec->ticks) {
         ec_SetProgramType(ec,             programType);
         ec_SetProgramName(ec,             programName);
         ec_SetModuleType (ec, (ModuleType)programType);             // im Hauptmodul gilt: ProgramName/Type = ModuleName/Type
         ec_SetModuleName (ec,             programName);
       //ec_SetLaunchType (ec,             launchType );
      }

      ec_SetRootFunction(ec, rootFunction );
      ec_SetInitReason  (ec, initReason   );
      ec_SetUninitReason(ec, uninitReason );
      ec_SetTestFlags   (ec, sec ? sec->testFlags : NULL);           // TODO: ResolveTestFlags()

      ec_SetInitFlags   (ec, initFlags    );
      ec_SetDeinitFlags (ec, deinitFlags  );
      ec_SetLogging     (ec, sec ? sec->logging : FALSE);            // TODO: ResolveLogging()     => benötigt This.IsTesting()
      ec_SetLogFile     (ec, sec ? sec->logFile : NULL );            // TODO: ResolveLogFile()

      ec_SetSymbol      (ec, symbol       );
      ec_SetTimeframe   (ec, period       );
      ec_SetHChart      (ec, sec        ?          sec->hChart  : hChart); // TODO: ResolveHChart()
      ec_SetHChartWindow(ec, ec->hChart ? GetParent(ec->hChart) : NULL  );

      ec_SetSuperContext(ec, sec          );
      ec_SetThreadId    (ec, currentThread);
   }


   // (2.2) Beim Aufruf aus start() zu aktualisieren:
   if (rootFunction == RF_START) {
      ec_SetRootFunction(ec, rootFunction );
      ec_SetThreadId    (ec, currentThread);
      ec_SetTicks       (ec, ec->ticks + 1);
   }


   // (2.3) Beim Aufruf aus deinit() zu aktualisieren:
   if (rootFunction == RF_DEINIT) {
      ec_SetRootFunction(ec, rootFunction );
      ec_SetUninitReason(ec, uninitReason );
      ec_SetThreadId    (ec, currentThread);
   }


   // (3) Das vom aktuellen Thread zuletzt ausgeführte Programm (also das aktuelle) in global threadsPrograms[] speichern.
   int currentThreadIndex=-1, size=threads.size();
   for (int i=0; i < size; i++) {                                    // aktuellen Thread suchen
      if (threads[i] == currentThread) {                             // Thread gefunden
         currentThreadIndex = i;
         threadsPrograms[i] = ec->programId;                         // zuletzt ausgeführtes Programm aktualisieren
         break;
      }
   }
   if (currentThreadIndex == -1) {                                   // Thread nicht gefunden
      EnterCriticalSection(&terminalLock);                           // Thread und Programm zur Verwaltung hinzufügen
      threads        .push_back(currentThread);
      threadsPrograms.push_back(ec->programId);
      if (threads.size() > 128) debug("thread %d added (size=%d)", currentThread, threads.size());
      LeaveCriticalSection(&terminalLock);
   }


   // (4) Ist der aktuelle Thread der UI-Thread, das aktuelle Programm zusätzlich in global lastUIThreadProgram speichern.
   if (IsUIThread())
      lastUIThreadProgram = ec->programId;

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
 * @param  uint               period       - aktuelle Chart-Periode
 *
 * @return BOOL - Erfolgsstatus
 *
 *
 * Notes:
 * ------
 * Ist in Library::init() der übergebene Context bereits initialisiert (ec.programId ist gesetzt), befindet sich das Programm in
 * einem init()-Cycle.
 */
BOOL WINAPI SyncLibExecutionContext(EXECUTION_CONTEXT* ec, const char* moduleName, RootFunction rootFunction, const char* symbol, uint period) {
   if ((uint)ec         < MIN_VALID_POINTER)             return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)moduleName < MIN_VALID_POINTER)             return(error(ERR_INVALID_PARAMETER, "invalid parameter moduleName = 0x%p (not a valid pointer)", moduleName));
   if (rootFunction!=RF_INIT && rootFunction!=RF_DEINIT) return(error(ERR_INVALID_PARAMETER, "illegal library parameter rootFunction = %s", RootFunctionToStr(rootFunction)));
   if ((uint)symbol     < MIN_VALID_POINTER)             return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((int)period <= 0)                                 return(error(ERR_INVALID_PARAMETER, "invalid parameter period = %d", period));

   ec->rootFunction = rootFunction;                         // Context-Daten aktualisieren


   // (1) Index des aktuellen Threads ermitteln
   DWORD currentThread = GetCurrentThreadId();
   int currentThreadIndex=-1, size=threads.size();
   for (int i=0; i < size; i++) {
      if (threads[i] == currentThread) {
         currentThreadIndex = i;                            // Thread gefunden: Index merken
         break;
      }
   }
   if (currentThreadIndex < 0) return(error(ERR_ILLEGAL_STATE, "%s::%s::%s()  programId=%d  thread %d not in known threads", ec->programName, moduleName, RootFunctionDescription(rootFunction), ec->programId, currentThread));


   if (rootFunction == RF_INIT) {
      if (!ec->programId) {
         // (2) Library wird zum ersten mal initialisiert
         int lastProgramId = threadsPrograms[currentThreadIndex];

         pec_vector &chain = contextChains[lastProgramId];
         int size = chain.size(); if (size < 2) return(error(ERR_RUNTIME_ERROR, "???::%s::init()  programId=0  lastProgramid=%d  chain.size=%d", moduleName, lastProgramId, size));

         if (0) debug("%s::%s::init()  übernehme ec von  programId=%d  i-am-module=%d", chain[0]->programName, moduleName, chain[0]->programId, chain.size());
         *ec = *chain[0];                                   // Master-Kontext (Index 0) übernehmen

         ec_SetModuleType(ec, MT_LIBRARY);                  // Context-Daten aktualisieren (Symbol und Timeframe in Libraries ignorieren)
         ec_SetModuleName(ec, moduleName);
         ec->rootFunction = rootFunction;
         ec->mqlError     = NO_ERROR;
         chain.push_back(ec);                               // Context zur Programm-Chain hinzufügen
      }
      else {
         // (3) Library ist in init() und war bereits initialisiert, also init()-Cycle
         ec->mqlError = NO_ERROR;                           // Symbol und Timeframe in Libraries ignorieren

         pec_vector &chain = contextChains[ec->programId];  // die erste Library invalidiert den Zeiger auf den Hauptkontext
         if (chain[1]) {
            warn(ERR_ILLEGAL_STATE, "%s::%s::init()  programId=%d  init cycle,  main-ec is not NULL: %p", ec->programName, moduleName, ec->programId, chain[1]);
            chain[1] = NULL;                                // Hauptkontext auf NULL setzen
         }
         ec_SetSymbol   (chain[0], symbol);                 // Master-Kontext aktualisieren. Dies ist der früheste Zeitpunkt, an dem das neue
         ec_SetTimeframe(chain[0], period);                 // Symbol/der neue Timeframe bekannt sind.
      }
   }


   // (4) Programm-ID des aktuellen Threads aktualisieren
   threadsPrograms[currentThreadIndex] = ec->programId;

   // Ist der aktuelle Thread der UI-Thread, das aktuelle Programm zusätzlich in global lastUIThreadProgram speichern.
   if (IsUIThread())
      lastUIThreadProgram = ec->programId;

   return(TRUE);
   #pragma EXPORT
}


/**
 * Signal leaving of an MQL module's EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - success status
 */
BOOL WINAPI LeaveExecutionContext(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ModuleType type = ec->moduleType;

   if (type==MT_INDICATOR || type==MT_SCRIPT) {
      // leave always
      uint id = ec->programId;
      EXECUTION_CONTEXT* master = contextChains[id][0];
      master->rootFunction = ec->rootFunction = (RootFunction)NULL;  // limbo
      contextChains[id][1]                    = NULL;                // mark main EXECUTION_CONTEXT as released
      return(TRUE);
   }
   if (type==MT_EXPERT || type==MT_LIBRARY) {
      // leave always-except-init-cycle
   }

   return(FALSE);
   #pragma EXPORT
}


/**
 * Resolve the specified program's current init() scenario.
 *
 * @param  char*              programName     - program name
 * @param  ProgramType        programType     - program type
 * @param  UninitializeReason uninitReason    - UninitializeReason as set by the terminal
 * @param  char*              symbol          - current symbol
 * @param  EXECUTION_CONTEXT* ec              - execution context as passed by the terminal       (possibly empty)
 * @param  EXECUTION_CONTEXT* sec             - supercontext as passed by the terminal            (possibly invalid)
 * @param  BOOL               isTesting       - IsTesting() flag as passed by the terminal        (possibly incorrect)
 * @param  BOOL               isVisualMode    - IsVisualMode() flag as passed by the terminal     (possibly incorrect)
 * @param  HWND               hChart          - WindowHandle() as passed by the terminal          (possibly NULL)
 * @param  int                subChartDropped - WindowOnDropped() index as passed by the terminal
 *
 * @return InitializeReason - init reason or NULL if an error occurred
 */
InitializeReason WINAPI InitReason(const char* programName, ProgramType programType, UninitializeReason uninitReason, const char* symbol, EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec, BOOL isTesting, BOOL isVisualMode, HWND hChart, int subChartDropped) {
   if ((uint)programName < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter programName = 0x%p (not a valid pointer)", programName));
   if ((uint)symbol      < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)ec          < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec  < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));

   if (programType == PT_SCRIPT) return(       INITREASON_USER);
   if (programType == PT_EXPERT) return((InitializeReason)NULL);
   /*
   Init-Szenarien:
   ---------------
   - onInit_User()             - bei Laden durch den User                               -      Input-Dialog
   - onInit_Template()         - bei Laden durch ein Template (auch bei Terminal-Start) - kein Input-Dialog
   - onInit_Program()          - bei Laden durch iCustom()                              - kein Input-Dialog
   - onInit_ProgramAfterTest() - bei Laden durch iCustom() nach Testende                - kein Input-Dialog
   - onInit_Parameters()       - nach Änderung der Indikatorparameter                   -      Input-Dialog
   - onInit_TimeframeChange()  - nach Timeframewechsel des Charts                       - kein Input-Dialog
   - onInit_SymbolChange()     - nach Symbolwechsel des Charts                          - kein Input-Dialog
   - onInit_Recompile()        - bei Reload nach Recompilation                          - kein Input-Dialog

   History:
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build 547-551: onInit_User()             - Broken: Wird zwei mal aufgerufen, beim zweiten mal ist der EXECUTION_CONTEXT ungültig.
   - Build  >= 654: onInit_User()             - UninitializeReason() ist REASON_UNDEFINED.
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build 577-583: onInit_Template()         - Broken: Kein Aufruf bei Terminal-Start, der Indikator wird aber geladen.
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build 556-569: onInit_Program()          - Broken: Wird in- und außerhalb des Testers bei jedem Tick aufgerufen.
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build  <= 229: onInit_ProgramAfterTest() - UninitializeReason() ist REASON_UNDEFINED.
   - Build     387: onInit_ProgramAfterTest() - Broken: Wird nie aufgerufen.
   - Build 388-628: onInit_ProgramAfterTest() - UninitializeReason() ist REASON_REMOVE.
   - Build  <= 577: onInit_ProgramAfterTest() - Wird nur nach einem automatisiertem Test aufgerufen (VisualMode=Off), der Aufruf
                                                erfolgt vorm Start des nächsten Tests.
   - Build  >= 578: onInit_ProgramAfterTest() - Wird auch nach einem manuellen Test aufgerufen (VisualMode=On), nur in diesem Fall
                                                erfolgt der Aufruf sofort nach Testende.
   - Build  >= 633: onInit_ProgramAfterTest() - UninitializeReason() ist REASON_CHARTCLOSE.
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build 577:     onInit_TimeframeChange()  - Broken: Bricht mit Logmessage "WARN: expert stopped" ab.
   ------------------------------------------------------------------------------------------------------------------------------------
   */
   uint build      = GetTerminalBuild();
   BOOL isUIThread = IsUIThread();


   // (1) REASON_PARAMETERS
   if (uninitReason == REASON_PARAMETERS) {
      // innerhalb iCustom(): nie
      if (sec)           return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // außerhalb iCustom(): bei erster Parameter-Eingabe eines neuen Indikators oder Parameter-Wechsel eines vorhandenen Indikators (auch im Tester bei VisualMode=On), Input-Dialog
      BOOL isProgramNew;
      int programId = ec->programId;
      if (programId) {
         isProgramNew = !contextChains[programId][0]->ticks;         // im Master-Context nachschauen
      }
      else {
         programId = FindFirstIndicatorInLimbo(hChart, programName, uninitReason);
         if (programId < 0) return((InitializeReason)NULL);
         isProgramNew = !programId;
         programId && ec_SetProgramId(ec, programId);                // ProgramID on-th-fly speichern
      }
      if (isProgramNew) return(INITREASON_USER      );               // erste Parameter-Eingabe eines manuell neu hinzugefügten Indikators
      else              return(INITREASON_PARAMETERS);               // Parameter-Wechsel eines vorhandenen Indikators
   }


   // (2) REASON_CHARTCHANGE
   if (uninitReason == REASON_CHARTCHANGE) {
      // innerhalb iCustom(): nie
      if (sec)               return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // außerhalb iCustom(): bei Symbol- oder Timeframe-Wechsel eines vorhandenen Indikators, kein Input-Dialog
      int programId = ec->programId;
      if (!programId) {
         programId = FindFirstIndicatorInLimbo(hChart, programName, uninitReason);
         if (programId <= 0) return((InitializeReason)(programId < 0 ? NULL : error(ERR_RUNTIME_ERROR, "no %s indicator during %s in limbo found", programName, UninitializeReasonToStr(uninitReason))));
         programId && ec_SetProgramId(ec, programId);                // ProgramID on-th-fly speichern
      }
      char* masterSymbol = contextChains[programId][0]->symbol;
      if (strcmp(masterSymbol, symbol)) return(INITREASON_TIMEFRAMECHANGE);
      else                              return(INITREASON_SYMBOLCHANGE   );
   }


   // (3) REASON_UNDEFINED
   if (uninitReason == REASON_UNDEFINED) {
      // außerhalb iCustom(): je nach Umgebung
      if (!sec) {
         if (build < 654)          return(INITREASON_TEMPLATE);      // wenn Template mit Indikator geladen wird (auch bei Start und im Tester bei VisualMode=On|Off), kein Input-Dialog
         if (subChartDropped >= 0) return(INITREASON_TEMPLATE);
         else                      return(INITREASON_USER    );      // erste Parameter-Eingabe eines manuell neu hinzugefügten Indikators, Input-Dialog
      }
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (isTesting && !isVisualMode/*Fix*/ && isUIThread) {         // versionsunabhängig
         if (build <= 229)         return(INITREASON_PROGRAM_AFTERTEST);
                                   return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      }
      return(INITREASON_PROGRAM);
   }


   // (4) REASON_REMOVE
   if (uninitReason == REASON_REMOVE) {
      // außerhalb iCustom(): nie
      if (!sec)                                                 return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (!isTesting || !isUIThread)                            return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      if (!isVisualMode/*Fix*/) { if (388<=build && build<=628) return(INITREASON_PROGRAM_AFTERTEST); }
      else                      { if (578<=build && build<=628) return(INITREASON_PROGRAM_AFTERTEST); }
      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
   }


   // (5) REASON_RECOMPILE
   if (uninitReason == REASON_RECOMPILE) {
      // innerhalb iCustom(): nie
      if (sec) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // außerhalb iCustom(): bei Reload nach Recompilation, vorhandener Indikator, kein Input-Dialog
      return(INITREASON_RECOMPILE);
   }


   // (6) REASON_CHARTCLOSE
   if (uninitReason == REASON_CHARTCLOSE) {
      // außerhalb iCustom(): nie
      if (!sec)                      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (!isTesting || !isUIThread) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      if (build >= 633)              return(INITREASON_PROGRAM_AFTERTEST);
      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
   }


   switch (uninitReason) {
      case REASON_ACCOUNT:       // nie
      case REASON_TEMPLATE:      // build > 509
      case REASON_INITFAILED:    // ...
      case REASON_CLOSE:         // ...
         return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
   }
   return((InitializeReason)error(ERR_ILLEGAL_STATE, "unknown UninitializeReason %d  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", uninitReason, sec, isTesting, isVisualMode, isUIThread, build));
}


/**
 * Find the first active matching indicator with a released main EXECUTION_CONTEXT.
 *
 * @param  HWND               hChart - WindowHandle() as passed by the terminal
 * @param  const char*        name   - indicator name
 * @param  UninitializeReason reason
 *
 * @return int - the found indicator's program id or NULL if no such indicator was found;
 *               -1 if an error occurred
 */
int WINAPI FindFirstIndicatorInLimbo(HWND hChart, const char* name, UninitializeReason reason) {
   if (hChart && !IsWindow(hChart))    return(_int(-1, error(ERR_INVALID_PARAMETER, "invalid parameter hChart = 0x%p (not a window)", hChart)));
   if ((uint)name < MIN_VALID_POINTER) return(_int(-1, error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name)));

   if (hChart) {
      EXECUTION_CONTEXT* master;
      uint size=contextChains.size(), uiThreadId=GetUIThreadId();

      for (uint i=1; i < size; i++) {                                // index[0] is never occupied
         master = contextChains[i][0];

         if (master->threadId == uiThreadId) {
            if (master->hChart == hChart) {
               if (master->programType == MT_INDICATOR) {
                  if (!strcmp(master->programName, name)) {
                     if (master->uninitReason == reason) {
                        if (master->rootFunction == NULL) {          // limbo = init cycle
                           //debug("first %s indicator found in limbo: id=%d", name, master->programId);
                           return(master->programId);
                        }
                        else debug("i=%d  %s  rootFunction not NULL:  master=%s", i, name, RootFunctionToStr(master->rootFunction));
                     }
                     else debug("i=%d  %s  uninit reason mis-match:  master=%s  reason=%s", i, name, UninitReasonToStr(master->uninitReason), UninitReasonToStr(reason));
                  }
                  //else debug("i=%d  %s  name mis-match", i, name);
               }
               //else debug("i=%d  %s  no indicator", i, name);
            }
            //else debug("i=%d  %s  chart mis-match  master=%d  hChart=%d", i, name, master->hChart, hChart);
         }
         //else debug("i=%d  %s  thread mis-match  master->threadId=%d  uiThreadId=%d", i, master->programName, master->threadId, uiThreadId);
      }
   }
   return(debug("no matching %s indicator in limbo found: hChart=%d  uninitReason=%s", name, hChart, UninitializeReasonToStr(reason)));
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
   if (!msgId) return(error(ERR_WIN32_ERROR+GetLastError(), "RegisterWindowMessage() failed"));

   return(msgId);
   #pragma EXPORT
}


/**
 * Process a C string debug message.
 *
 * @return int - 0 (zero)
 */
int _debug(const char* fileName, const char* funcName, int line, const char* format, ...) {
   va_list args;
   va_start(args, format);
   __debug(fileName, funcName, line, format, args);
   va_end(args);
   return(0);
}


/**
 * Process a C++ string debug message.
 *
 * @return int - 0 (zero)
 */
int _debug(const char* fileName, const char* funcName, int line, const std::string &format, ...) {
   va_list args;
   va_start(args, format);
   __debug(fileName, funcName, line, format.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Send a formatted string to the debugger output.
 *
 * @param  char*   fileName - Name der Datei, in der der Aufruf erfolgt
 * @param  char*   funcName - Name der Funktion, in der der Aufruf erfolgt
 * @param  int     line     - Zeile, in der der Aufruf erfolgt
 * @param  char*   format   - Formatstring mit Platzhaltern für alle weiteren Parameter
 * @param  va_list args     - weitere Parameter
 */
void __debug(const char* fileName, const char* funcName, int line, const char* format, const va_list &args) {
   if (!format) format = "(null)";

   // (1) zuerst alle explizit angegebenen Argumente in einen String transformieren (ab format)
   int size  = _vscprintf(format, args) + 1;                                     // +1 für das terminierende '\0'
   char* msg = (char*) alloca(size);                                             // auf dem Stack
   vsprintf_s(msg, size, format, args);

   // Parameter fileName zerlegen: nur der einfache Dateiname {basename.ext} wird benötigt
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else           _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // (2) dann die impliziten Location-Infos vorn anfügen
   char* locationFormat = "Metatrader::%s%s::%s(%d)  %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;     // +1 für das terminierende '\0'
   char* buffer = (char*) alloca(size);                                          // auf dem Stack
   sprintf_s(buffer, size, locationFormat, baseName, ext, funcName, line, msg);

   OutputDebugString(buffer);
}


/**
 * Process a DLL warning with a C string log message.
 *
 * @return int - 0 (zero)
 */
int _warn(const char* fileName, const char* funcName, int line, int error, const char* msgFormat, ...) {
   va_list args;
   va_start(args, msgFormat);
   __warn(fileName, funcName, line, error, msgFormat, args);
   va_end(args);
   return(0);
}


/**
 * Process a DLL warning with a C++ string log message.
 *
 * @return int - 0 (zero)
 */
int _warn(const char* fileName, const char* funcName, int line, int error, const std::string &msgFormat, ...) {
   va_list args;
   va_start(args, msgFormat);
   __warn(fileName, funcName, line, error, msgFormat.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Process a DLL warning and send it to OutputDebugString(). If the warning occurred during a call from MQL the error
 * is stored in the program's EXECUTION_CONTEXT. Must be called via the <tt>warn(...)</tt> macro.
 *
 * @param  char*   fileName  - file where the macro was called
 * @param  char*   funcName  - function where the macro was called
 * @param  int     line      - line where the macro was called
 * @param  int     error     - error code
 * @param  char*   msgFormat - message with format codes for further parameters
 * @param  va_list msgArgs   - further message parameters
 */
void __warn(const char* fileName, const char* funcName, int line, int error, const char* msgFormat, const va_list &msgArgs) {
   if (!msgFormat) msgFormat = "(null)";

   // create message with the specified parameters
   int size  = _vscprintf(msgFormat, msgArgs) + 1;                                     // +1 for the terminating '\0'
   char* msg = (char*) alloca(size);                                                   // on the stack
   vsprintf_s(msg, size, msgFormat, msgArgs);

   // extract {baseName}.{ext} from fileName
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // add location infos
   char* locationFormat = "Metatrader::%s%s::%s(%d)  WARN: %s";
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg) + 1;           // +1 for the terminating '\0'
   char* locationMsg = (char*) alloca(size);                                           // on the stack
   sprintf_s(locationMsg, size, locationFormat, baseName, ext, funcName, line, msg);

   char* fullMsg;

   // add error details (if any)
   if (error) {
      char* errorFormat  = "%s  [%s]";
      const char* sError = ErrorToStr(error);
      size = _scprintf(errorFormat, locationMsg, sError) + 1;                          // +1 for the terminating '\0'
      fullMsg = (char*) alloca(size);                                                  // on the stack
      sprintf_s(fullMsg, size, errorFormat, locationMsg, sError);
   }
   else {
      fullMsg = locationMsg;
   }

   BOOL inMqlCall = FALSE;
   if (inMqlCall) {
      //ec_SetDllWarning   (ec, error  );
      //ec_SetDllWarningMsg(ec, fullMsg);
   }
   OutputDebugString(fullMsg);
}


/**
 * Process a DLL error with a C string log message.
 *
 * @return int - 0 (zero)
 */
int _error(const char* fileName, const char* funcName, int line, int error, const char* msgFormat, ...) {
   va_list args;
   va_start(args, msgFormat);
   __error(fileName, funcName, line, error, msgFormat, args);
   va_end(args);
   return(0);
}


/**
 * Process a DLL error with a C++ string log message.
 *
 * @return int - 0 (zero)
 */
int _error(const char* fileName, const char* funcName, int line, int error, const std::string &msgFormat, ...) {
   va_list args;
   va_start(args, msgFormat);
   __error(fileName, funcName, line, error, msgFormat.c_str(), args);
   va_end(args);
   return(0);
}


/**
 * Process a DLL error and send it to OutputDebugString(). If the error occurred during a call from MQL the error
 * is stored in the program's EXECUTION_CONTEXT. Must be called via the <tt>error(...)</tt> macro.
 *
 * @param  char*   fileName  - file where the macro was called
 * @param  char*   funcName  - function where the macro was called
 * @param  int     line      - line where the macro was called
 * @param  int     error     - error code
 * @param  char*   msgFormat - message with format codes for further parameters
 * @param  va_list msgArgs   - further message parameters
 */
void __error(const char* fileName, const char* funcName, int line, int error, const char* msgFormat, const va_list &msgArgs) {
   if (!error) return;
   if (!msgFormat) msgFormat = "(null)";

   // create message with the specified parameters
   int size  = _vscprintf(msgFormat, msgArgs) + 1;                                     // +1 for the terminating '\0'
   char* msg = (char*) alloca(size);                                                   // on the stack
   vsprintf_s(msg, size, msgFormat, msgArgs);

   // extract {baseName}.{ext} from fileName
   char baseName[_MAX_FNAME], ext[_MAX_EXT];
   if (!fileName) baseName[0] = ext[0] = '\0';
   else _splitpath_s(fileName, NULL, 0, NULL, 0, baseName, _MAX_FNAME, ext, _MAX_EXT);

   // add location infos and error details
   char* locationFormat = "Metatrader::%s%s::%s(%d)  ERROR: %s  [%s]";
   const char* sError   = ErrorToStr(error);
   size = _scprintf(locationFormat, baseName, ext, funcName, line, msg, sError) + 1;   // +1 for the terminating '\0'
   char* fullMsg = (char*) alloca(size);                                               // on the stack
   sprintf_s(fullMsg, size, locationFormat, baseName, ext, funcName, line, msg, sError);

   // look-up the current thread's last associated MQL program
   DWORD currentThread = GetCurrentThreadId();
   int currentThreadIndex=-1, currentThreadLastProgram=0;
   size = threads.size();
   for (int i=0; i < size; i++) {
      if (threads[i] == currentThread) {                             // thread found
         currentThreadIndex       = i;                               // keep thread index and last MQL program
         currentThreadLastProgram = threadsPrograms[i];
         break;
      }
   }

   if (currentThreadIndex == -1) {
      // Thread unknown/not found: Not much we can do. We could just have entered SyncMainExecutionContext() or
      //                           we could be in a function callback called from a new thread (if that's possible).
   }
   else {
      // Thread executed MQL before
      if (IsUIThread()) {
         // Wir sind in einer Callback-Funktion oder im letzten im UI-Thread ausgeführten Indikator und dort u.U. in einer
         // Library oder einem via iCustom() geladenen weiteren Indikator.
         //
         // Vorsicht: Der Hauptkontext des letzten Root-Programms kann ungültig sein.
      }
      else {
         // Wir sind im Expert oder Script des Threads und dort u.U. in einer Library oder einem via iCustom() geladenen weiteren
         // Indikator. Auf den Hauptkontext des Root-Programms kann lesend/schreibend zugegriffen werden.
      }

      if (BOOL inMqlCall=FALSE) {
         //ec_SetDllError   (ec, error  );
         //ec_SetDllErrorMsg(ec, fullMsg);
      }
   }

   OutputDebugString(fullMsg);
}


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende feste Werte zurückzugeben.
 *
 * @param  ... all parameters are ignored
 */
int  _CLR_NONE(...) { return(CLR_NONE); }
int  _NULL    (...) { return(NULL    ); }
bool _true    (...) { return(true    ); }
bool _false   (...) { return(false   ); }
BOOL _TRUE    (...) { return(TRUE    ); }
BOOL _FALSE   (...) { return(FALSE   ); }


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende variable Werte zurückzugeben.
 * Außer dem ersten werden alle übergebenen Parameter ignoriert.
 */
bool   _bool  (bool   value, ...) { return(value); }
char   _char  (char   value, ...) { return(value); }
int    _int   (int    value, ...) { return(value); }
float  _float (float  value, ...) { return(value); }
double _double(double value, ...) { return(value); }
BOOL   _BOOL  (BOOL   value, ...) { return(value); }


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
   delete[] test;
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
