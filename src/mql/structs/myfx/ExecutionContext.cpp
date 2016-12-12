#include "Expander.h"


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
 * Kontextgültigkeit: Der Master-Context einer Chain ist immer gültig. Alle anderen Kontexte der Chain können je nach Modultyp
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
 * Bei Experts und Scripts gibt es während der Laufzeit nur eine Instanz des Hauptmodulkontextes. Bei Indikatoren ändert sich
 * die Instanz mit jedem init()-Cycle, da MetaTrader den Speicher für Variablen in Indicator::init() jeweils neu alloziiert.
 *
 * @param  EXECUTION_CONTEXT* ec              - Context des Hauptmoduls eines MQL-Programms
 * @param  ProgramType        programType     - Programm-Typ
 * @param  char*              programName     - Programmname (je nach MetaTrader-Version ggf. mit Pfad)
 * @param  UninitializeReason uninitReason    - UninitializeReason as passed by the terminal
 * @param  DWORD              initFlags       - Init-Konfiguration
 * @param  DWORD              deinitFlags     - Deinit-Konfiguration
 * @param  char*              symbol          - aktuelles Chart-Symbol
 * @param  uint               period          - aktuelle Chart-Periode
 * @param  EXECUTION_CONTEXT* sec             - super context as passed by the terminal           (possibly invalid)
 * @param  BOOL               isTesting       - IsTesting() flag as passed by the terminal        (possibly incorrect)
 * @param  BOOL               isVisualMode    - IsVisualMode() flag as passed by the terminal     (possibly incorrect)
 * @param  HWND               hChart          - WindowHandle() as passed by the terminal          (possibly incorrect)
 * @param  int                subChartDropped - WindowOnDropped() index as passed by the terminal
 *
 * @return BOOL - Erfolgsstatus
 *
 *
 * Notes:
 * ------
 * • Im Indikator gibt es während eines init()-Cycles in der Zeitspanne vom Verlassen von Indicator::deinit() bis zum Wieder-
 *   eintritt in Indicator::init() keinen gültigen Hauptkontext. Der alte Speicherblock wird sofort freigegeben, in init() wird
 *   ein neuer alloziiert. Während dieser Zeitspanne wird der init()-Cycle von bereits geladenen Libraries durchgeführt und es
 *   darf nicht auf den zu dem Zeitpunkt ungültigen Hauptkontext zugegriffen werden.
 * • Nach Recompilation oder Crash einer Library wird der Speicherblock ihres Kontexts ungültig und auf ihn darf ebenfalls
 *   nicht mehr zugegriffen werden.
 *
 *
 *  Init cycle of single indicator with libraries:
 *  --- first load ------------------------------------------------------------------------------------------------------------
 *  Indicator::init()              REASON_UNDEFINED    programId=0  creating new chain             set programId=1
 *  Indicator::libraryA::init()    REASON_UNDEFINED    programId=0  loaded by indicator            set programId=1
 *  Indicator::libraryB::init()    REASON_UNDEFINED    programId=0  loaded by indicator            set programId=1
 *  Indicator::libraryC::init()    REASON_UNDEFINED    programId=0  loaded by libraryA             set programId=1
 *  --- deinit() --------------------------------------------------------------------------------------------------------------
 *  Indicator::deinit()            REASON_CHARTCHANGE  programId=1  indicator first
 *  Indicator::libraryA::deinit()  REASON_UNDEFINED    programId=1  then libraries
 *  Indicator::libraryC::deinit()  REASON_UNDEFINED    programId=1  hierarchical (not in loading order)
 *  Indicator::libraryB::deinit()  REASON_UNDEFINED    programId=1
 *  --- init() ----------------------------------------------------------------------------------------------------------------
 *  Indicator::libraryA::init()    REASON_UNDEFINED    programId=1  libraries first (new symbol and timeframe show up)
 *  Indicator::libraryC::init()    REASON_UNDEFINED    programId=1  hierarchical (not in loading order)
 *  Indicator::libraryB::init()    REASON_UNDEFINED    programId=1
 *  Indicator::init()              REASON_CHARTCHANGE  programId=0  then indicator                 set programId=1
 *  ---------------------------------------------------------------------------------------------------------------------------
 *
 *
 *  Init cycle of multiple indicators with libraries:
 *  --- first load ------------------------------------------------------------------------------------------------------------
 *  ChartInfos::init()             REASON_UNDEFINED    programId=0  creating new chain             set programId=1
 *  ChartInfos::lib::init()        REASON_UNDEFINED    programId=0  loaded by indicator            set programId=1
 *  SuperBars::init()              REASON_UNDEFINED    programId=0  creating new chain             set programId=2
 *  SuperBars::lib::init()         REASON_UNDEFINED    programId=0  loaded by indicator            set programId=2
 *  --- deinit() --------------------------------------------------------------------------------------------------------------
 *  ChartInfos::deinit()           REASON_CHARTCHANGE  programId=1
 *  ChartInfos::lib::deinit()      REASON_UNDEFINED    programId=1
 *  SuperBars::deinit()            REASON_CHARTCHANGE  programId=2
 *  SuperBars::lib::deinit()       REASON_UNDEFINED    programId=2
 *  --- init() ----------------------------------------------------------------------------------------------------------------
 *  ChartInfos::lib::init()        REASON_UNDEFINED    programId=1
 *  ChartInfos::init()             REASON_CHARTCHANGE  programId=0  first indicator in limbo (1)   set programId=1
 *  SuperBars::lib::init()         REASON_UNDEFINED    programId=2
 *  SuperBars::init()              REASON_CHARTCHANGE  programId=0  next indicator in limbo (1)    set programId=2
 *  ---------------------------------------------------------------------------------------------------------------------------
 *
 *
 * (1) Limbo (latin limbus, edge or boundary, referring to the "edge" of Hell) is a speculative idea about the afterlife
 *     condition of those who die in original sin without being assigned to the Hell of the Damned. Remember "Inception"?
 *     Very hard to escape from.
 *     In Metatrader terms the memory allocated for indicator variables (holding the EXECUTION_CONTEXT, global variables, static
 *     local variables etc.) is released after the indicator leaves deinit(). On re-entry in init() new memory is allocated and
 *     all variables are initialized with zero which is the reason an indicator cannot keep state over an init cycle. Between
 *     deinit() and init() when the indicator enters the state of "limbo" (a mysterious land where the streets have no name
 *     known only to the scammers of MetaQuotes) state is kept in the master execution context which acts as a backup of the
 *     then lost main execution context. On re-entry the master context is copied back to the then newly allocated main context
 *     and state of the context survives. Voilà, it crossed the afterlife.
 */
BOOL WINAPI SyncMainContext_init(EXECUTION_CONTEXT* ec, ProgramType programType, const char* programName, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint period, EXECUTION_CONTEXT* sec, BOOL isTesting, BOOL isVisualMode, HWND hChart, int subChartDropped) {
   if ((uint)ec          < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)programName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter programName = 0x%p (not a valid pointer)", programName));
   if ((uint)symbol      < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((int)period <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter period = %d", period));
   if (sec && (uint)sec  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));

   //
   // Ablauf
   // ------
   // (1) Wenn keine ProgramID gesetzt ist, prüfen, ob Programm ein Indikator im Init-Cycle ist.
   //     • wenn Programm ein Indikator im Init-Cycle ist (kommt nur im UI-Thread vor)
   //       - Hauptkontext aus Master-Context restaurieren
   //     • wenn Programm kein Indikator im Init-Cycle ist
   //       - neuen Master-Context erzeugen
   //       - neue Context-Chain erzeugen und Master- und Hauptkontext darin speichern
   //       - resultierende ProgramID in Master- und Hauptkontext speichern
   //
   // (2) Wenn ProgramID gesetzt, dann Expert im Init-Cycle
   //
   // (3) Hauptkontext aktualisieren
   //
   // (4) Wenn Expert im Tester ggf. dessen Libraries aus dem vorherigen Test finden und zuordnen
   //
   EXECUTION_CONTEXT* master        = NULL;
   InitializeReason   initReason    = (InitializeReason)NULL;
   BOOL               isNewExpert   = FALSE;
   uint               lastProgramId = NULL;


   if (!ec->programId) {
      // (1) Wenn keine ProgramID gesetzt ist, prüfen, ob Programm ein Indikator im Init-Cycle ist.
      initReason                = InitReason(programName, programType, uninitReason, symbol, ec, sec, isTesting, isVisualMode, hChart, subChartDropped);
      BOOL indicatorInInitCycle = programType==PT_INDICATOR && (initReason==INIT_REASON_PARAMETERS || initReason==INIT_REASON_SYMBOLCHANGE || initReason==INIT_REASON_TIMEFRAMECHANGE);

      if (indicatorInInitCycle) {
         StoreThreadAndProgram(ec->programId);                       // store last executed program (asap)
         // (1.1) Programm ist Indikator im Init-Cycle (immer im UI-Thread)
         //   - Indikator-Context aus Master-Context restaurieren
         master = contextChains[ec->programId][0];                   // im Init-Cycle setzt InitReason() die gefundene ProgramID
         *ec = *master;                                              // Master-Context kopieren
         contextChains[ec->programId][1] = ec;                       // Context als Hauptkontext speichern
         //debug("%s::init()  programId=0  init-cycle, was id=%d  thread=%s", programName, ec->programId, IsUIThread() ? "UI": to_string(GetCurrentThreadId()).c_str());
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
         //debug("%s::init()  programId=0  %snew chain => id=%d  thread=%s  hChart=%d", programName, (IsUIThread() ? "UI  ":""), ec->programId, IsUIThread() ? "UI":to_string(GetCurrentThreadId()).c_str(), hChart);
         LeaveCriticalSection(&terminalLock);

         // resolve program status and last program executed by the current thread
         isNewExpert = (programType == PT_EXPERT);
         uint index = StoreThreadAndProgram(0);
         lastProgramId = threadsPrograms[index];
         threadsPrograms[index] = ec->programId;                     // store last executed program (asap)
      }
   }
   else {
      // (2) ProgramID gesetzt: Expert im Init-Cycle
      StoreThreadAndProgram(ec->programId);                          // store last executed program (asap)
      initReason = ec->initReason;
   }


   // (3) Nur beim ersten Aufruf zu aktualisieren
   if (!ec->ticks) {
      ec_SetProgramType(ec,             programType);
      ec_SetProgramName(ec,             programName);
      ec_SetModuleType (ec, (ModuleType)programType);                // im Hauptmodul gilt: ProgramName/Type = ModuleName/Type
      ec_SetModuleName (ec,             programName);
    //ec_SetLaunchType (ec,             launchType );
   }

   // Immer zu aktualisieren
   ec_SetRootFunction(ec, RF_INIT     );
   ec_SetInitReason  (ec, initReason  );
   ec_SetUninitReason(ec, uninitReason);
   ec_SetTestFlags   (ec, sec ? sec->testFlags : isTesting);         // TODO: ResolveTestFlags()

   ec_SetInitFlags   (ec, initFlags   );
   ec_SetDeinitFlags (ec, deinitFlags );
   ec_SetLogging     (ec, sec ? sec->logging : FALSE);               // TODO: ResolveLogging()     => benötigt This.IsTesting()
   ec_SetLogFile     (ec, sec ? sec->logFile : NULL );               // TODO: ResolveLogFile()

   ec_SetSymbol      (ec, symbol      );
   ec_SetTimeframe   (ec, period      );
   ec_SetHChart      (ec, sec        ?          sec->hChart  : hChart); // TODO: ResolveHChart()
   ec_SetHChartWindow(ec, ec->hChart ? GetParent(ec->hChart) : NULL  );

   ec_SetSuperContext(ec, sec         );
   ec_SetThreadId    (ec, GetCurrentThreadId());


   // (4) Wenn Expert im Tester, dann ggf. dessen Libraries aus dem vorherigen Test finden und dem Expert zuordnen
   if (isNewExpert && isTesting && lastProgramId) {
      EXECUTION_CONTEXT *lib, *lastMaster=contextChains[lastProgramId][0];

      if (lastMaster && lastMaster->initCycle) {
         pec_vector &currentChain = contextChains[ec->programId];
         pec_vector &lastChain    = contextChains[lastProgramId];
         uint lastSize = lastChain.size();

         for (uint i=2; i < lastSize; i++) {
            lib = lastChain[i];
            if (lib && lib->initCycle) {
               lastChain[i] = NULL;
               lib->programId = ec->programId;                       // TODO: update all relevant library context fields
               lib->initCycle = FALSE;
               currentChain.push_back(lib);
            }
         }
         lastMaster->initCycle = FALSE;
      }
   }
   return(TRUE);
   #pragma EXPORT
}


/**
 * @param  EXECUTION_CONTEXT* ec - Context des Hauptmoduls eines MQL-Programms
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SyncMainContext_start(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (!ec->programId)               return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.programId=%d", ec->programId));

   StoreThreadAndProgram(ec->programId);                             // store last executed program (asap)

   ec_SetRootFunction(ec, RF_START            );                     // update context
   ec_SetTicks       (ec, ec->ticks + 1       );
   ec_SetThreadId    (ec, GetCurrentThreadId());

   return(TRUE);
   #pragma EXPORT
}


/**
 * @param  EXECUTION_CONTEXT* ec           - Context des Hauptmoduls eines MQL-Programms
 * @param  UninitializeReason uninitReason - UninitializeReason as passed by the terminal
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SyncMainContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec));
   if (!ec->programId)               return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.programId=%d", ec->programId));

   StoreThreadAndProgram(ec->programId);                             // store last executed program (asap)

   ec_SetRootFunction(ec, RF_DEINIT           );                     // update context
   ec_SetUninitReason(ec, uninitReason        );
   ec_SetThreadId    (ec, GetCurrentThreadId());

   return(TRUE);
   #pragma EXPORT
}


/**
 * Synchronize a library's EXECUTION_CONTEXT with the context of its main module. Called in Library::init(). If a library is
 * loaded the first time its context is added to the context chain of the executing program.
 *
 * @param  EXECUTION_CONTEXT* ec           - the libray's execution context
 * @param  UninitializeReason uninitReason - UninitializeReason as passed by the terminal
 * @param  char*              moduleName   - the library's name (according to the Metatrader version w/o path)
 * @param  char*              symbol       - current chart symbol
 * @param  uint               period       - current chart period
 *
 * @return BOOL - success status
 *
 *
 * Notes:
 * ------
 * • Libraries loaded by scripts and experts on regular charts do not enter init cycles, libraries in indicators do. However,
 *   libraries loaded by experts in the Strategy Tester enter an init cycle between repetitive tests if the last test was not
 *   manually stopped (by clicking the "Stop" button).
 *
 * • During an init cycle libraries keep state. This is used to distinguish between libraries entering init() the first time
 *   and libraries entering init() as a result of an init cycle.
 *
 * Bugs:
 * -----
 * • After an init cycle in Strategy Tester a library falsely keeps the former state of the IsVisualMode() flag. This happens
 *   even if the tested symbol or the timeframe change.
 *   Workaround: Instead of the function IsVisualMode() libraries and indicators (which can be loaded by libraries) must use
 *   the corresponding flag in the execution context.
 *
 * • After an init cycle in Strategy Tester a library falsely keeps state of the last order context of the former test. Order
 *   functions return wrong results.
 *   Workaround: In "include/core/library::init()" of the MQL framework the order context is manually reset.
 */
BOOL WINAPI SyncLibContext_init(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason, const char* moduleName, const char* symbol, uint period) {
   if ((uint)ec         < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec));
   if ((uint)moduleName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter moduleName=0x%p (not a valid pointer)", moduleName));
   if ((uint)symbol     < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol=0x%p (not a valid pointer)", symbol));
   if ((int)period <= 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter period=%d", period));

   // (1) Wenn keine ProgramID gesetzt ist, wird die Library zum ersten mal geladen und der Context ist undefiniert.
   //     • Master-Context übernehmen und Library-Felder im Context aktualisieren
   //
   // (2) Wenn die ProgramID gesetzt ist, prüfen, ob der Init-Cycle im Indikator (UI-Thread) oder im Expert/Tester statttfindet.
   //     • Init-Cycle im Indikator
   //       -
   //     • Init-Cycle im Expert im Tester
   //       -

   if (!ec->programId) {
      // (1) Library wird zum ersten mal geladen (vom Programm im aktuellen Thread)
      uint index     = StoreThreadAndProgram(0);         // get index of the current thread (last program is already set)
      uint programId = threadsPrograms[index];

      *ec = *contextChains[programId][0];                // copy master context

      ec_SetModuleType(ec, MT_LIBRARY);                  // TODO: update library specific context fields
      ec_SetModuleName(ec, moduleName);
      ec->rootFunction = RF_INIT;
      ec->mqlError     = NO_ERROR;

      contextChains[programId].push_back(ec);            // add context to the program's context chain
   }

   else if (IsUIThread()) {
      // (2) Init-Cycle eines Indikators, called before Indicator::init()
      StoreThreadAndProgram(ec->programId);              // store last executed program (asap)

      ec->rootFunction = RF_INIT;                        // TODO: update library context fields
      ec->mqlError     = NO_ERROR;                       // TODO: check master context for changes
      ec_SetSymbol   (ec, symbol);
      ec_SetTimeframe(ec, period);
   }

   else {
      // (3) Init-Cycle der Library eines Experts im Tester, called before Expert::init()
      StoreThreadAndProgram(ec->programId);              // store last executed program (asap)

      ec->rootFunction = RF_INIT;
      ec->initCycle    = TRUE;                           // mark library context
      contextChains[ec->programId][0]->initCycle = TRUE; // mark master context
   }

   //debug("%s::%s::init()  ec=%s", ec->programName, ec->moduleName, EXECUTION_CONTEXT_toStr(ec));
   return(TRUE);
   #pragma EXPORT
}


/**
 * Synchronize a library's EXECUTION_CONTEXT with the context of its main module. Called in Library::deinit().
 *
 * @param  EXECUTION_CONTEXT* ec           - the libray's execution context
 * @param  UninitializeReason uninitReason - UninitializeReason as passed by the terminal
 *
 * @return BOOL - success status
 */
BOOL WINAPI SyncLibContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec));
   if (!ec->programId)               return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.programId=%d", ec->programId));

   StoreThreadAndProgram(ec->programId);                             // store last executed program (asap)

   ec_SetRootFunction(ec, RF_DEINIT           );                     // update context
   ec_SetUninitReason(ec, uninitReason        );
   ec_SetThreadId    (ec, GetCurrentThreadId());

   //debug("%s::%s::deinit()  ec@%d=%s", ec->programName, ec->moduleName, ec, EXECUTION_CONTEXT_toStr(ec));
   return(TRUE);
   #pragma EXPORT
}


/**
 * Signal leaving of an MQL module's execution context. Called at leaving of MQL::deinit().
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - success status
 */
BOOL WINAPI LeaveContext(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER)  return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=%p (not a valid pointer)", ec));
   uint id = ec->programId;
   if ((int)id < 1)                   return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.programId=%d  ec=%s", (int)id, EXECUTION_CONTEXT_toStr(ec)));
   //if (ec->rootFunction != RF_DEINIT) return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.rootFunction not RF_DEINIT  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = contextChains[id][0];

   switch (ec->moduleType) {
      case MT_INDICATOR:
      case MT_SCRIPT:
         if (ec != contextChains[id][1]) return(error(ERR_ILLEGAL_STATE, "%s::%s::deinit()  illegal parameter ec=%d (not stored as main context=%d)  ec=%s", ec->programName, ec->moduleName, ec, contextChains[id][1], EXECUTION_CONTEXT_toStr(ec)));
         master->rootFunction = ec->rootFunction = (RootFunction)NULL;
         contextChains[id][1] = NULL;                                // always mark context as released
         break;

      case MT_EXPERT:
         if (ec != contextChains[id][1]) return(error(ERR_ILLEGAL_STATE, "%s::%s::deinit()  illegal parameter ec=%d (not stored as main context=%d)  ec=%s", ec->programName, ec->moduleName, ec, contextChains[id][1], EXECUTION_CONTEXT_toStr(ec)));
         master->rootFunction = ec->rootFunction = (RootFunction)NULL;
         if (ec->uninitReason!=REASON_CHARTCHANGE && ec->uninitReason!=REASON_PARAMETERS && ec->uninitReason!=REASON_ACCOUNT) {
            contextChains[id][1] = NULL;                             // mark context as released if not in init cycle
         }
         break;

      case MT_LIBRARY:
         if (ec==master || ec==contextChains[id][1]) return(error(ERR_ILLEGAL_STATE, "%s::%s::deinit()  illegal parameter ec=%d (not stored as library context)  ec=%s", ec->programName, ec->moduleName, ec, EXECUTION_CONTEXT_toStr(ec)));
         ec->rootFunction = (RootFunction)NULL;
         return(FALSE);

      default:
         return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.moduleType=%s", ModuleTypeToStr(ec->moduleType)));
   }

   return(TRUE);
   #pragma EXPORT
}


/**
 * Stores the specified program as executed by the current thread.
 *
 * @param  uint programId - Program id to store. If this value is 0 (zero) no program information is stored.
 *
 * @return DWORD - index of the current thread in the stored threads or EMPTY (-1) if an error occurred
 */
DWORD WINAPI StoreThreadAndProgram(uint programId) {
   if ((int)programId < 0) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter programId: %d", programId)));

   DWORD currentThread = GetCurrentThreadId();

   // look-up current thread in global threadsPrograms[]
   int currentThreadIndex=-1, size=threads.size();
   for (int i=0; i < size; i++) {
      if (threads[i] == currentThread) {                             // thread found
         currentThreadIndex = i;
         if (programId)
            threadsPrograms[i] = programId;                          // update the last executed program if non-zero
         break;
      }
   }

   if (currentThreadIndex == -1) {                                   // thread not found
      EnterCriticalSection(&terminalLock);
      threads        .push_back(currentThread);                      // add thread to the list
      threadsPrograms.push_back(programId);                          // add program to the list (0 if zero)
      currentThreadIndex = threads.size() - 1;
      if (currentThreadIndex > 128) debug("thread %d added (size=%d)", currentThread, threads.size());
      LeaveCriticalSection(&terminalLock);
   }

   // additionally store the program in global lastUIThreadProgram if the current thread is the UI thread
   if (programId && IsUIThread())
      lastUIThreadProgram = programId;

   return(currentThreadIndex);
}


/**
 * Gibt die Programm-ID eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Programm-ID
 */
uint WINAPI ec_ProgramId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programId);
   #pragma EXPORT
}


/**
 * Gibt den Programm-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ProgramType
 */
ProgramType WINAPI ec_ProgramType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programType);
   #pragma EXPORT
}


/**
 * Gibt den Programmnamen eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Programmname
 */
const char* WINAPI ec_ProgramName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->programName);
   #pragma EXPORT
}


/**
 * Gibt den Modul-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ModuleType
 */
ModuleType WINAPI ec_ModuleType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->moduleType);
   #pragma EXPORT
}


/**
 * Gibt den Modulnamen eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Modulname
 */
const char* WINAPI ec_ModuleName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->moduleName);
   #pragma EXPORT
}


/**
 * Gibt den Launch-Typ eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return LaunchType
 */
LaunchType WINAPI ec_LaunchType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->launchType);
   #pragma EXPORT
}


/**
 * Gibt die RootFunction eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 */
RootFunction WINAPI ec_RootFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->rootFunction);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten InitCycle-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_InitCycle(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initCycle);
   #pragma EXPORT
}


/**
 * Gibt den InitializeReason eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return InitializeReason
 */
InitializeReason WINAPI ec_InitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initReason);
   #pragma EXPORT
}


/**
 * Gibt den UninitializeReason eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI ec_UninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->uninitReason);
   #pragma EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherten Test-Flags zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - Test-Flags
 */
DWORD WINAPI ec_TestFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->testFlags);
   #pragma EXPORT
}


/**
 * Gibt die Init-Konfiguration eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - Init-Flags
 */
DWORD WINAPI ec_InitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->initFlags);
   #pragma EXPORT
}


/**
 * Gibt die Deinit-Konfiguration eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - Deinit-Flags
 */
DWORD WINAPI ec_DeinitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->deinitFlags);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Logging-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Logging(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->logging);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Logdateinamen zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Dateiname
 */
const char* WINAPI ec_LogFile(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->logFile);
   #pragma EXPORT
}


/**
 * Gibt das Chartsymbol eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Symbol
 */
const char* WINAPI ec_Symbol(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->symbol);
   #pragma EXPORT
}


/**
 * Gibt die Chartperiode eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - Chartperiode
 */
uint WINAPI ec_Timeframe(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->timeframe);
   #pragma EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Handle eines Chartframes zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - Handle, entspricht dem Rückgabewert der MQL-Funktion WindowHandle()
 */
HWND WINAPI ec_hChart(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->hChart);
   #pragma EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Handle eines Chartfensters zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - Handle
 */
HWND WINAPI ec_hChartWindow(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->hChartWindow);
   #pragma EXPORT
}


/**
 * Kopiert den SuperContext eines EXECUTION_CONTEXT in die übergebene Variable.
 *
 * @param  EXECUTION_CONTEXT* ec  - ExecutionContext
 * @param  EXECUTION_CONTEXT* sec - Variable zur Aufnahme des SuperContexts
 *
 * @return BOOL - TRUE,  wenn ein SuperContext kopiert wurde;
 *                FALSE, wenn der ExecutionContext keinen SuperContext enthielt oder ein Fehler auftrat
 */
BOOL WINAPI ec_SuperContext(const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if ((uint)ec  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)sec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));

   if (ec->superContext) {
      *sec = *ec->superContext;
      return(TRUE);
   }

   static const EXECUTION_CONTEXT zeroed = {};
   *sec = zeroed;
   return(FALSE);
   #pragma EXPORT
}


/**
 * Gibt den Zeiger auf den SuperContext eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return EXECUTION_CONTEXT*
 */
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->superContext);
   #pragma EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's current thread id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - thread id
 */
uint WINAPI ec_ThreadId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->threadId);
   #pragma EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's number of times start() was called.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint
 */
uint WINAPI ec_Ticks(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   return(ec->ticks);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten MQL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_MqlError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(-1, error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   return(ec->mqlError);
   #pragma EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten DLL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_DllError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(-1, error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   return(ec->dllError);
   #pragma EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherte DLL-Warnung zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_DllWarning(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(-1, error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));
   return(ec->dllWarning);
   #pragma EXPORT
}


/**
 * Setzt die Programm-ID eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               id - ID größer 0
 *
 * @return uint - dieselbe ID
 */
uint WINAPI ec_SetProgramId(EXECUTION_CONTEXT* ec, uint id) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (id <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter id = %d (must be greater than zero)", id));

   ec->programId = id;                                               // Master-Context synchronisieren
   if (contextChains.size() > id && ec==contextChains[id][1] && contextChains[id][0])
      return(ec_SetProgramId(contextChains[id][0], id));

   return(id);
   #pragma EXPORT
}


/**
 * Setzt den Programm-Typ eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  ProgramType        type
 *
 * @return ProgramType - derselbe ProgramType
 */
ProgramType WINAPI ec_SetProgramType(EXECUTION_CONTEXT* ec, ProgramType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   switch (type) {
      case PT_INDICATOR:
      case PT_EXPERT   :
      case PT_SCRIPT   : break;
      default:
         return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter type = %d (not a ProgramType)", type));
   }

   ec->programType = type;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetProgramType(contextChains[pid][0], type));

   return(type);
   #pragma EXPORT
}


/**
 * Setzt den Programmnamen eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - derselbe Name
 */
const char* WINAPI ec_SetProgramName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)          return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)          return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(ec->programName)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name = \"%s\" (must be 1 to %d characters)", name, sizeof(ec->programName)-1));

   if (!strcpy(ec->programName, name))
      return(NULL);

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetProgramName(contextChains[pid][0], name));

   return(name);
   #pragma EXPORT
}


/**
 * Setzt den Modul-Typ eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  ModuleType         type
 *
 * @return ModuleType - derselbe ModuleType
 */
ModuleType WINAPI ec_SetModuleType(EXECUTION_CONTEXT* ec, ModuleType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   switch (type) {
      case MT_INDICATOR:
      case MT_EXPERT   :
      case MT_SCRIPT   :
      case MT_LIBRARY  : break;
      default:
         return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter type = %d (not a ModuleType)", type));
   }

   ec->moduleType = type;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetModuleType(contextChains[pid][0], type));

   return(type);
   #pragma EXPORT
}


/**
 * Setzt den Modulnamen eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - derselbe Name
 */
const char* WINAPI ec_SetModuleName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(ec->moduleName)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name = \"%s\" (must be 1 to %d characters)", name, sizeof(ec->moduleName)-1));

   if (!strcpy(ec->moduleName, name))
      return(NULL);

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetModuleName(contextChains[pid][0], name));

   return(name);
   #pragma EXPORT
}


/**
 * Setzt den Launch-Typ eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  LaunchType         type
 *
 * @return LaunchType - derselbe Launch-Typ
 */
LaunchType WINAPI ec_SetLaunchType(EXECUTION_CONTEXT* ec, LaunchType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   switch (type) {
      case LT_TEMPLATE:
      case LT_PROGRAM :
      case LT_MANUAL  : break;
      default:
         return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter type = %d (not a LaunchType)", type));
   }

   ec->launchType = type;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetLaunchType(contextChains[pid][0], type));

   return(type);
   #pragma EXPORT
}


/**
 * Setzt die RootFunction-ID eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  RootFunction       id
 *
 * @return RootFunction - dieselbe RootFunction-ID
 */
RootFunction WINAPI ec_SetRootFunction(EXECUTION_CONTEXT* ec, RootFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   switch (id) {
      case RF_INIT  :
      case RF_START :
      case RF_DEINIT: break;
      default:
         return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter id = %d (not a RootFunction)", id));
   }

   ec->rootFunction = id;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetRootFunction(contextChains[pid][0], id));

   return(id);
   #pragma EXPORT
}


/**
 * Setzt den InitCycle-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe InitCycle-Status
 */
BOOL WINAPI ec_SetInitCycle(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->initCycle = status;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetInitCycle(contextChains[pid][0], status));

   return(status);
   #pragma EXPORT
}


/**
 * Setzt den InitializeReason eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  InitializeReason reason
 *
 * @return InitializeReason - derselbe InitializeReason
 */
InitializeReason WINAPI ec_SetInitReason(EXECUTION_CONTEXT* ec, InitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (reason) {
      case INIT_REASON_USER             :
      case INIT_REASON_TEMPLATE         :
      case INIT_REASON_PROGRAM          :
      case INIT_REASON_PROGRAM_AFTERTEST:
      case INIT_REASON_PARAMETERS       :
      case INIT_REASON_TIMEFRAMECHANGE  :
      case INIT_REASON_SYMBOLCHANGE     :
      case INIT_REASON_RECOMPILE        :                                break;
      case NULL                         : if (ec->moduleType==MT_EXPERT) break;
      default:
         return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason = %d (not an InitializeReason)", reason));
   }

   ec->initReason = reason;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetInitReason(contextChains[pid][0], reason));

   return(reason);
   #pragma EXPORT
}


/**
 * Setzt den UninitializeReason eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  UninitializeReason reason
 *
 * @return UninitializeReason - derselbe UninitializeReason
 */
UninitializeReason WINAPI ec_SetUninitReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   switch (reason) {
      case REASON_UNDEFINED  :
      case REASON_REMOVE     :
      case REASON_RECOMPILE  :
      case REASON_CHARTCHANGE:
      case REASON_CHARTCLOSE :
      case REASON_PARAMETERS :
      case REASON_ACCOUNT    :
      // build > 509
      case REASON_TEMPLATE   :
      case REASON_INITFAILED :
      case REASON_CLOSE      : break;
      default:
         return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason = %d (not an UninitializeReason)", reason));
   }

   ec->uninitReason = reason;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetUninitReason(contextChains[pid][0], reason));

   return(reason);
   #pragma EXPORT
}


/**
 * Setzt die Test-Flags eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - dieselben Test-Flags
 */
DWORD WINAPI ec_SetTestFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->testFlags = flags;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetTestFlags(contextChains[pid][0], flags));

   return(flags);
   #pragma EXPORT
}


/**
 * Setzt die Init-Konfiguration eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - dieselben Init-Flags
 */
DWORD WINAPI ec_SetInitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->initFlags = flags;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetInitFlags(contextChains[pid][0], flags));

   return(flags);
   #pragma EXPORT
}


/**
 * Setzt die Deinit-Konfiguration eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - dieselben Deinit-Flags
 */
DWORD WINAPI ec_SetDeinitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->deinitFlags = flags;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetDeinitFlags(contextChains[pid][0], flags));

   return(flags);
   #pragma EXPORT
}


/**
 * Setzt den Logging-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe Logging-Status
 */
BOOL WINAPI ec_SetLogging(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->logging = status;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetLogging(contextChains[pid][0], status));

   return(status);
   #pragma EXPORT
}


/**
 * Setzt den Namen der Logdatei eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              fileName - statt eines NULL-Pointers kann auch ein Leerstring angegeben werden
 *
 * @return char* - derselbe Dateiname
 */
const char* WINAPI ec_SetLogFile(EXECUTION_CONTEXT* ec, const char* fileName) {
   if ((uint)ec < MIN_VALID_POINTER)                return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   if (fileName) {
      // fileName ist kein NULL-Pointer
      if ((uint)fileName < MIN_VALID_POINTER)       return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter fileName = 0x%p (not a valid pointer)", fileName));
      if (strlen(fileName) > sizeof(ec->logFile)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter fileName = \"%s\" (max %d characters)", fileName, sizeof(ec->logFile)-1));

      if (!strcpy(ec->logFile, fileName))
         return(NULL);
   }
   else {
      // fileName ist NULL-Pointer
      ec->logFile[0] = '\0';
   }

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetLogFile(contextChains[pid][0], fileName));

   return(fileName);
   #pragma EXPORT
}


/**
 * Setzt das Symbol eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              symbol
 *
 * @return char* - dasselbe Symbol
 */
const char* WINAPI ec_SetSymbol(EXECUTION_CONTEXT* ec, const char* symbol) {
   if ((uint)ec     < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   int len = strlen(symbol);
   if (!len || len > sizeof(ec->symbol)) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol = \"%s\" (must be 1 to %d characters)", symbol, sizeof(ec->symbol)-1));

   if (!strcpy(ec->symbol, symbol))
      return(NULL);

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetSymbol(contextChains[pid][0], symbol));

   return(symbol);
   #pragma EXPORT
}


/**
 * Setzt den Chart-Timeframe eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               timeframe
 *
 * @return uint - derselbe Timeframe
 */
uint WINAPI ec_SetTimeframe(EXECUTION_CONTEXT* ec, uint timeframe) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (timeframe <= 0)               return(error(ERR_INVALID_PARAMETER, "invalid parameter timeframe = %d (must be greater than zero)", timeframe));

   ec->timeframe = timeframe;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetTimeframe(contextChains[pid][0], timeframe));

   return(timeframe);
   #pragma EXPORT
}


/**
 * Setzt das Handle des Chart-Frames eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  HWND               hWnd - entspricht dem Rückgabewert von WindowHandle()
 *
 * @return HWND - dasselbe Handle
 */
HWND WINAPI ec_SetHChart(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->hChart = hWnd;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetHChart(contextChains[pid][0], hWnd));

   return(hWnd);
   #pragma EXPORT
}


/**
 * Setzt das Handle des Chart-Fensters eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  HWND               hWnd
 *
 * @return HWND - dasselbe Handle
 */
HWND WINAPI ec_SetHChartWindow(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   ec->hChartWindow = hWnd;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetHChartWindow(contextChains[pid][0], hWnd));

   return(hWnd);
   #pragma EXPORT
}


/**
 * Setzt den SuperContext eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec  - zu modifizierender Context
 * @param  EXECUTION_CONTEXT* sec - zu setzender SuperContext
 *
 * @return EXECUTION_CONTEXT* - der gesetzte SuperContext
 */
EXECUTION_CONTEXT* WINAPI ec_SetSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if (       (uint)ec  < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));

   ec->superContext = sec;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetSuperContext(contextChains[pid][0], sec));

   return(sec);
   #pragma EXPORT
}


/**
 * Alias (imported differently in MQL)
 */
EXECUTION_CONTEXT* WINAPI ec_SetLpSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* lpSec) {
   return(ec_SetSuperContext(ec, lpSec));
   #pragma EXPORT
}


/**
 * Set an EXECUTION_CONTEXT's current thread id.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               id - thread id
 *
 * @return uint - the same thread id
 */
uint WINAPI ec_SetThreadId(EXECUTION_CONTEXT* ec, uint id) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (id <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter id = %d (must be greater than zero)", id));

   ec->threadId = id;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetThreadId(contextChains[pid][0], id));

   return(id);
   #pragma EXPORT
}


/**
 * Set an EXECUTION_CONTEXT's number of times start() was called.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               count
 *
 * @return uint - dieselbe Anzahl
 */
uint WINAPI ec_SetTicks(EXECUTION_CONTEXT* ec, uint count) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (count <= 0)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter count = %d (must be greater than zero)", count));

   ec->ticks = count;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      return(ec_SetTicks(contextChains[pid][0], count));

   return(count);
   #pragma EXPORT
}


/**
 * Setzt den MQL-Fehler eines EXECUTION_CONTEXT. Diese Funktion wird nur von MQL::SetLastError() aufgerufen.
 *
 * Zusätzlich wird der MQL-Fehler in den jeweiligen Hauptkontext propagiert (Propagation zum aufrufenden Hauptmodul).
 * Hat der Kontext einen SuperContext, wird der Fehler auch dorthin propagiert (Propagation zum aufrufenden Programm).
 * Fehler werden nur beim Setzen propagiert, nicht beim Zurücksetzen.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - derselbe Fehler oder -1, falls ein Fehler auftrat
 */
int WINAPI ec_SetMqlError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(-1, error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   ec->mqlError = error;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      ec_SetMqlError(contextChains[pid][0], error);

   if (!error)                                                       // keine Propagation beim Zurücksetzen eines Fehlers
      return(error);

   if (pid && ec->moduleType==MT_LIBRARY) {                          // Fehler aus Libraries in den Hauptkontext propagieren
      EXECUTION_CONTEXT* master = contextChains[pid][0];             // (oder den Master-Context, wenn Hauptkontext nicht verfügbar)
      EXECUTION_CONTEXT* main   = contextChains[pid][1];
      if (main) ec_SetMqlError(main,   error);
      else      ec_SetMqlError(master, error);
   }

   if (ec->superContext)
      ec_SetMqlError(ec->superContext, error);                       // Fehler in einen vorhandenen SuperContext propagieren

   return(error);
   #pragma EXPORT
}


/**
 * Setzt den DLL-Fehler eines EXECUTION_CONTEXT.
 *
 * Zusätzlich wird der DLL-Fehler in den jeweiligen Hauptkontext propagiert (Propagation zum aufrufenden Hauptmodul).
 * Fehler werden nur beim Setzen propagiert, nicht beim Zurücksetzen.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - derselbe Fehler oder -1, falls ein Fehler auftrat
 */
int WINAPI ec_SetDllError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(-1, error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   ec->dllError = error;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      ec_SetDllError(contextChains[pid][0], error);

   if (!error)                                                       // keine Propagation beim Zurücksetzen eines Fehlers
      return(error);

   if (pid && ec->moduleType==MT_LIBRARY) {                          // Fehler aus Libraries in den Hauptkontext propagieren
      EXECUTION_CONTEXT* master = contextChains[pid][0];             // (oder den Master-Context, wenn Hauptkontext nicht verfügbar)
      EXECUTION_CONTEXT* main   = contextChains[pid][1];
      if (main) ec_SetDllError(main,   error);
      else      ec_SetDllError(master, error);
   }
   return(error);
   #pragma EXPORT
}


/**
 * Setzt die DLL-Warnung eines EXECUTION_CONTEXT.
 *
 * Zusätzlich wird die DLL-Warnung in den jeweiligen Hauptkontext propagiert (Propagation zum aufrufenden Hauptmodul).
 * Warnungen werden nur beim Setzen propagiert, nicht beim Zurücksetzen.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - derselbe Fehler oder -1, falls ein Fehler auftrat
 */
int WINAPI ec_SetDllWarning(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(-1, error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec)));

   ec->dllWarning = error;

   uint pid = ec->programId;                                         // Master-Context synchronisieren
   if (pid && contextChains.size() > pid && ec==contextChains[pid][1] && contextChains[pid][0])
      ec_SetDllWarning(contextChains[pid][0], error);

   if (!error)                                                       // keine Propagation beim Zurücksetzen eines Fehlers
      return(error);

   if (pid && ec->moduleType==MT_LIBRARY) {                          // Warnung aus Libraries in den Hauptkontext propagieren
      EXECUTION_CONTEXT* master = contextChains[pid][0];             // (oder den Master-Context, wenn Hauptkontext nicht verfügbar)
      EXECUTION_CONTEXT* main   = contextChains[pid][1];
      if (main) ec_SetDllWarning(main,   error);
      else      ec_SetDllWarning(master, error);
   }
   return(error);
   #pragma EXPORT
}


/**
 * Return a human-readable version of an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               outputDebug - whether or not to duplicate the result to OutputDebugString()
 *                                          (default: no)
 * @return char*
 */
const char* WINAPI EXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug/*=FALSE*/) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));

   char* result = "{(empty)}";
   static const EXECUTION_CONTEXT empty = {};

   if (memcmp(ec, &empty, sizeof(EXECUTION_CONTEXT)) != 0) {
      std::stringstream ss; ss
         <<  "{programId="    <<                   ec->programId
         << ", programType="  <<  ProgramTypeToStr(ec->programType )
         << ", programName="  <<    DoubleQuoteStr(ec->programName )
         << ", moduleType="   <<   ModuleTypeToStr(ec->moduleType  )
         << ", moduleName="   <<    DoubleQuoteStr(ec->moduleName  )
         << ", launchType="   <<                   ec->launchType
         << ", rootFunction=" << RootFunctionToStr(ec->rootFunction)
         << ", initCycle="    <<         BoolToStr(ec->initCycle   )
         << ", initReason="   <<   InitReasonToStr(ec->initReason  )
         << ", uninitReason=" << UninitReasonToStr(ec->uninitReason)
         << ", testFlags="    <<    TestFlagsToStr(ec->testFlags   )
         << ", initFlags="    <<    InitFlagsToStr(ec->initFlags   )
         << ", deinitFlags="  <<  DeinitFlagsToStr(ec->deinitFlags )
         << ", logging="      <<         BoolToStr(ec->logging     )
         << ", logFile="      <<    DoubleQuoteStr(ec->logFile     )
         << ", symbol="       <<    DoubleQuoteStr(ec->symbol      )
         << ", timeframe="    <<       PeriodToStr(ec->timeframe   )
         << ", hChart="       <<             (uint)ec->hChart
         << ", hChartWindow=" <<             (uint)ec->hChartWindow
         << ", superContext=" <<             (uint)ec->superContext
         << ", threadId="     <<                   ec->threadId
         << ", ticks="        <<                   ec->ticks
         << ", mqlError="     <<                 (!ec->mqlError   ? "0" : ErrorToStr(ec->mqlError  ))
         << ", dllError="     <<                 (!ec->dllError   ? "0" : ErrorToStr(ec->dllError  ))
         << ", dllWarning="   <<                 (!ec->dllWarning ? "0" : ErrorToStr(ec->dllWarning))
         << "}";
      std::string str = ss.str();
      result = strcpy(new char[str.size()+1], str.c_str());          // TODO: close memory leak
   }

   if (outputDebug) debug(result);
   return(result);
   #pragma EXPORT
}


/**
 * Alias (different MQL import)
 */
const char* WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug/*=FALSE*/) {
   return(EXECUTION_CONTEXT_toStr(ec, outputDebug));
   #pragma EXPORT
}
