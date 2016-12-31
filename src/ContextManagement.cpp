#include "header/Expander.h"


/**
 * Synchronisiert den EXECUTION_CONTEXT eines MQL-Hauptmoduls mit dem Master-Context in der DLL. Wird von der init()-Funktion
 * der MQL-Hauptmodule aufgerufen.
 *
 * For a general overview see "ExecutionContext.h".
 *
 * @param  EXECUTION_CONTEXT* ec              - Kontext des Hauptmoduls eines MQL-Programms
 * @param  ProgramType        programType     - Programmtyp
 * @param  char*              programName     - Programmname (je nach Metatrader-Version ggf. mit Pfad)
 * @param  UninitializeReason uninitReason    - uninitialize reason as passed by the terminal
 * @param  DWORD              initFlags       - Init-Konfiguration
 * @param  DWORD              deinitFlags     - Deinit-Konfiguration
 * @param  char*              symbol          - aktuelles Chart-Symbol
 * @param  uint               period          - aktuelle Chart-Periode
 * @param  EXECUTION_CONTEXT* sec             - super context as passed by the terminal           (possibly already released)
 * @param  BOOL               isTesting       - IsTesting() flag as passed by the terminal        (possibly incorrect)
 * @param  BOOL               isVisualMode    - IsVisualMode() flag as passed by the terminal     (possibly incorrect)
 * @param  BOOL               isOptimization  - IsOptimzation() flag as passed by the terminal
 * @param  HWND               hChart          - WindowHandle() as passed by the terminal          (possibly not yet set)
 * @param  int                subChartDropped - WindowOnDropped() index as passed by the terminal
 *
 * @return BOOL - Erfolgsstatus
 *
 *
 *  Init cycle of a single indicator with libraries:
 *  --- first load ------------------------------------------------------------------------------------------------------------
 *  Indicator::init()              UR_UNDEFINED    programId=0  creating new chain             set programId=1
 *  Indicator::libraryA::init()    UR_UNDEFINED    programId=0  loaded by indicator            set programId=1
 *  Indicator::libraryB::init()    UR_UNDEFINED    programId=0  loaded by indicator            set programId=1
 *  Indicator::libraryC::init()    UR_UNDEFINED    programId=0  loaded by libraryA             set programId=1
 *  --- deinit() --------------------------------------------------------------------------------------------------------------
 *  Indicator::deinit()            UR_CHARTCHANGE  programId=1  indicator first
 *  Indicator::libraryA::deinit()  UR_UNDEFINED    programId=1  then libraries
 *  Indicator::libraryC::deinit()  UR_UNDEFINED    programId=1  hierarchical (not in loading order)
 *  Indicator::libraryB::deinit()  UR_UNDEFINED    programId=1
 *  --- init() ----------------------------------------------------------------------------------------------------------------
 *  Indicator::libraryA::init()    UR_UNDEFINED    programId=1  libraries first (new symbol and timeframe show up)
 *  Indicator::libraryC::init()    UR_UNDEFINED    programId=1  hierarchical (not in loading order)
 *  Indicator::libraryB::init()    UR_UNDEFINED    programId=1
 *  Indicator::init()              UR_CHARTCHANGE  programId=0  then indicator                 set programId=1
 *  ---------------------------------------------------------------------------------------------------------------------------
 *
 *
 *  Init cycle of multiple indicators with libraries:
 *  --- first load ------------------------------------------------------------------------------------------------------------
 *  ChartInfos::init()             UR_UNDEFINED    programId=0  creating new chain             set programId=1
 *  ChartInfos::lib::init()        UR_UNDEFINED    programId=0  loaded by indicator            set programId=1
 *  SuperBars::init()              UR_UNDEFINED    programId=0  creating new chain             set programId=2
 *  SuperBars::lib::init()         UR_UNDEFINED    programId=0  loaded by indicator            set programId=2
 *  --- deinit() --------------------------------------------------------------------------------------------------------------
 *  ChartInfos::deinit()           UR_CHARTCHANGE  programId=1
 *  ChartInfos::lib::deinit()      UR_UNDEFINED    programId=1
 *  SuperBars::deinit()            UR_CHARTCHANGE  programId=2
 *  SuperBars::lib::deinit()       UR_UNDEFINED    programId=2
 *  --- init() ----------------------------------------------------------------------------------------------------------------
 *  ChartInfos::lib::init()        UR_UNDEFINED    programId=1
 *  ChartInfos::init()             UR_CHARTCHANGE  programId=0  first indicator in limbo       set programId=1
 *  SuperBars::lib::init()         UR_UNDEFINED    programId=2
 *  SuperBars::init()              UR_CHARTCHANGE  programId=0  next indicator in limbo        set programId=2
 *  ---------------------------------------------------------------------------------------------------------------------------
 */
BOOL WINAPI SyncMainContext_init(EXECUTION_CONTEXT* ec, ProgramType programType, const char* programName, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint period, EXECUTION_CONTEXT* sec, BOOL isTesting, BOOL isVisualMode, BOOL isOptimization, HWND hChart, int subChartDropped) {
   if ((uint)ec          < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)programName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter programName = 0x%p (not a valid pointer)", programName));
   if ((uint)symbol      < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((int)period <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter period = %d", (int)period));
   if (sec && (uint)sec  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));

   // (1) Wenn keine ProgramID gesetzt ist, prüfen, ob Programm ein Indikator im Init-Cycle ist.
   //     • wenn Programm ein Indikator im Init-Cycle ist (kommt nur im UI-Thread vor)
   //       - Hauptkontext aus Master-Context restaurieren
   //     • wenn Programm kein Indikator im Init-Cycle ist
   //       - neuen Master-Context erzeugen
   //       - neue Context-Chain erzeugen und Master- und Hauptkontext speichern
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

   hChart = ProgramFindChart(hChart, sec, (ModuleType)programType, isTesting, isVisualMode, symbol, period);
   if (hChart == INVALID_HWND) return(error(ERR_RUNTIME_ERROR, "ProgramFindChart() failed"));


   if (!ec->programId) {
      // (1) Wenn keine ProgramID gesetzt ist, prüfen, ob Programm ein Indikator im Init-Cycle oder nach einem Test ist.
      initReason                = ProgramInitReason(ec, sec, programName, programType, uninitReason, symbol, isTesting, isVisualMode, hChart, subChartDropped);
      BOOL indicatorInInitCycle = programType==PT_INDICATOR && (initReason==IR_PARAMETERS || initReason==IR_SYMBOLCHANGE || initReason==IR_TIMEFRAMECHANGE);
      BOOL indicatorAfterTest   = programType==PT_INDICATOR && initReason==IR_PROGRAM_AFTERTEST;

      if (indicatorInInitCycle) {
         StoreThreadAndProgram(ec->programId);                       // store last executed program (asap)
         // (1.1) Programm ist Indikator im Init-Cycle (immer im UI-Thread)
         //   - Indikator-Context aus Master-Context restaurieren
         master = contextChains[ec->programId][0];                   // im Init-Cycle setzt ProgramInitReason() die gefundene ProgramID
         *ec = *master;                                              // Master-Context kopieren
         contextChains[ec->programId][1] = ec;                       // Context als Hauptkontext speichern
         //debug("%s::init()  programId=0  init-cycle, was id=%d  thread=%s", programName, ec->programId, IsUIThread() ? "UI": to_string(GetCurrentThreadId()).c_str());
      }
      else {
         // (1.2) Programm ist kein Indikator im Init-Cycle          // TODO: on IR_PROGRAM_AFTERTEST existiert ein vorheriger Context
         //   - neue Context-Chain erzeugen
         //   - neuen Master-Context erzeugen
         //   - Master- und Hauptkontext in der Chain speichern
         //   - ProgramID generieren und diese Master- und Hauptkontext zuweisen
         master  = new EXECUTION_CONTEXT;                            // neuen Master-Context erzeugen
         *master = *ec;                                              // Hauptkontext hineinkopieren
         ContextChain chain;                                         // neue Context-Chain erzeugen
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
         isNewExpert = (programType==PT_EXPERT);
         uint index = StoreThreadAndProgram(0);
         lastProgramId = threadsPrograms[index];
         threadsPrograms[index] = ec->programId;                     // store last executed program (asap)
      }
      if (indicatorAfterTest) {
         ec_SetSuperContext(ec, sec=NULL);                           // super context (expert) was already released
      }
   }
   else {
      // (2) ProgramID gesetzt: Expert im Init-Cycle
      StoreThreadAndProgram(ec->programId);                          // store last executed program (asap)
      initReason = ec->initReason;                                   // TODO: wrong
   }


   // (3.1) Beim ersten Aufruf von init() zu initialisieren
   if (!ec->ticks) {
      ec_SetProgramType  (ec,             programType);
      ec_SetProgramName  (ec,             programName);
      ec_SetModuleType   (ec, (ModuleType)programType);               // Hauptmodul: ModuleType == ProgramType
      ec_SetModuleName   (ec,             programName);
    //ec_SetLaunchType   (ec,             launchType );

      ec_SetSuperContext (ec, sec   );
      ec_SetHChart       (ec, hChart);
      ec_SetHChartWindow (ec, hChart ? GetParent(hChart) : NULL);

      ec_SetTesting      (ec, isTesting     =ProgramIsTesting     (ec, isTesting     ));
      ec_SetVisualMode   (ec, isVisualMode  =ProgramIsVisualMode  (ec, isVisualMode  ));
      ec_SetOptimization (ec, isOptimization=ProgramIsOptimization(ec, isOptimization));

      ec_SetInitFlags    (ec, initFlags               );
      ec_SetDeinitFlags  (ec, deinitFlags             );
      ec_SetLogging      (ec, ProgramIsLogging    (ec));
      ec_SetCustomLogFile(ec, ProgramCustomLogFile(ec));
   }

   // (3.2) Bei jedem Aufruf von init() zu aktualisieren
   ec_SetRootFunction(ec, RF_INIT     );                             // TODO: wrong for init() calls from start()
 //ec_SetInitCycle   (ec, FALSE       );
   ec_SetInitReason  (ec, initReason  );                             // TODO: wrong for experts and scripts
   ec_SetUninitReason(ec, uninitReason);

   ec_SetSymbol      (ec, symbol      );
   ec_SetTimeframe   (ec, period      );
   ec_SetThreadId    (ec, GetCurrentThreadId());


   // (4) Wenn Expert im Tester, dann ggf. dessen Libraries aus dem vorherigen Test finden und dem Expert zuordnen
   if (isNewExpert && isTesting && lastProgramId) {
      EXECUTION_CONTEXT *lib, *lastMaster=contextChains[lastProgramId][0];

      if (lastMaster && lastMaster->initCycle) {
         ContextChain &currentChain = contextChains[ec->programId];
         ContextChain &lastChain    = contextChains[lastProgramId];
         uint          lastSize     = lastChain.size();

         for (uint i=2; i < lastSize; i++) {                         // skip master and main context
            lib = lastChain[i];
            if (!lib) {
               warn(ERR_ILLEGAL_STATE, "unexpected library context found (lib=chain[%d]=NULL) for lastProgramId=%d", i, lastProgramId);
               continue;
            }
            if (lib->initCycle) {
               lastChain[i] = NULL;

               ec_SetProgramId    (lib, ec->programId    );          // update all relevant library context fields
               ec_SetInitCycle    (lib, FALSE            );
               ec_SetVisualMode   (lib, ec->visualMode   );
               ec_SetOptimization (lib, ec->optimization );          // is this necessary?
               ec_SetLogging      (lib, ec->logging      );
               ec_SetCustomLogFile(lib, ec->customLogFile);
               ec_SetHChart       (lib, ec->hChart       );
               ec_SetHChartWindow (lib, ec->hChartWindow );

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
 * Synchronize a library's EXECUTION_CONTEXT with the context of the executing program's main module. Called in Library::init().
 * If a library is loaded the first time its context is added to the program's context chain.
 *
 * @param  EXECUTION_CONTEXT* ec             - the libray's execution context
 * @param  UninitializeReason uninitReason   - UninitializeReason as passed by the terminal
 * @param  DWORD              initFlags      - init configuration
 * @param  DWORD              deinitFlags    - deinit configuration
 * @param  char*              moduleName     - the library's name w/o path according to the terminal version
 * @param  char*              symbol         - current symbol
 * @param  uint               period         - current period
 * @param  BOOL               isOptimization - MQL::IsOptimization() as passed by the terminaL
 *
 * @return BOOL - success status
 *
 *
 * Notes:
 * ------
 * During init cycles libraries keep state. This can be used to distinguish between first loading and the init cycle.
 * There are two scenarios where libraries execute init cycles.
 *
 * (1) Libraries loaded by indicators during the indicator's regular init cycle.
 *     - Library::deinit() is called after Indicator::deinit()
 *     - Library::init() is called before Indicator::init()
 *
 * (2) Libraries loaded by experts in Strategy Tester between tests if the finished test was not explicitly stopped.
 *     - !!! wann wird Library::deinit() aufgerufen !!!
 *     - Library::init() is called before Expert::init()
 *
 *     - Bug: This init cycle itself is wrong as the library holds state of the former finished test and must not get re-used.
 *            Workaround: On test start library state needs to be explicitly reset (see MQL::core/library::init).
 *            In Expert::init() SyncMainContext_init() removes the library from the former program's context chain and attaches
 *            it to the context chain of the current program.
 *
 *     - Bug: In this scenario libraries also keep state of the last order context and order functions return wrong results.
 *            Workaround: On test start the order context needs to be explicitly reset (see MQL::core/library::init).
 *
 *     - Bug: In this scenario libraries also keep state of the former IsVisualMode() flag. This is true even if tested symbol
 *            or tested timeframe change.
 *            Workaround: Instead of IsVisualMode() use the corresponding flag of the execution context.
 */
BOOL WINAPI SyncLibContext_init(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* moduleName, const char* symbol, uint period, BOOL isOptimization) {
   if ((uint)ec         < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec=0x%p (not a valid pointer)", ec));
   if ((uint)moduleName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter moduleName=0x%p (not a valid pointer)", moduleName));
   if ((uint)symbol     < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol=0x%p (not a valid pointer)", symbol));
   if ((int)period <= 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter period=%d", (int)period));

   // (1) If ec.ProgramID is not set: library is loaded the first time and the context is empty.
   //     - copy master context and update library specific fields
   //
   // (2) If ec.ProgramID is set: check if init cycle in indicator (UI thread) or in expert in Tester (not UI thread)
   //     (2.1) init cycle in indicator
   //     (2.2) init cycle in expert in Tester

   if (!ec->programId) {
      // (1) library is loaded the first time by the current thread's program
      uint index     = StoreThreadAndProgram(0);                     // get the current thread's index (current program is already set)
      uint programId = threadsPrograms[index];                       // get the current program's id (the library loader)

      *ec = *contextChains[programId][0];                            // copy master context

      ec_SetModuleType  (ec, MT_LIBRARY            );                // update library specific fields
      ec_SetModuleName  (ec, moduleName            );
      ec_SetRootFunction(ec, RF_INIT               );
      ec_SetInitCycle   (ec, FALSE                 );
      ec_SetInitReason  (ec, (InitializeReason)NULL);                // in libraries always NULL
      ec_SetUninitReason(ec, uninitReason          );
      ec_SetInitFlags   (ec, initFlags             );
      ec_SetDeinitFlags (ec, deinitFlags           );

      ec_SetTicks       (ec, NULL);                                  // in libraries always NULL
      ec_SetMqlError    (ec, NULL);                                  // in libraries always NULL
      ec_SetDllError    (ec, NULL);
      ec->dllErrorMsg   =    NULL;                                   // TODO: implement g/setter
      ec_SetDllWarning  (ec, NULL);
      ec->dllWarningMsg =    NULL;                                   // TODO: implement g/setter

      contextChains[programId].push_back(ec);                        // add context to the program's context chain
   }

   else if (IsUIThread()) {
      // (2.1) init cycle in indicator called before Indicator::init()
      StoreThreadAndProgram(ec->programId);                          // store last executed program (asap)

      ec_SetRootFunction(ec, RF_INIT     );                          // update library specific fields
      ec_SetInitCycle   (ec, FALSE       );                          // TODO: mark master context ???
      ec_SetUninitReason(ec, uninitReason);
      ec_SetSymbol      (ec, symbol      );
      ec_SetTimeframe   (ec, period      );
   }

   else {
      // (2.2) init cycle in expert in Tester called before Expert::init()
      StoreThreadAndProgram(ec->programId);                          // store last executed program (asap)

      // update library specific fields                              // ec.programId gets updated in Expert::init()
      ec_SetRootFunction (ec, RF_INIT             );
      ec_SetInitCycle    (ec, TRUE                );                 // mark library context
      ec_SetUninitReason (ec, uninitReason        );
      ec_SetVisualMode   (ec, FALSE               );                 // gets updated in Expert::init()
      ec_SetOptimization (ec, isOptimization      );                 // is this value correct?
      ec_SetLogging      (ec, FALSE               );                 // gets updated in Expert::init()
      ec_SetCustomLogFile(ec, NULL                );                 // gets updated in Expert::init()
      ec_SetSymbol       (ec, symbol              );
      ec_SetTimeframe    (ec, period              );
      ec_SetHChart       (ec, NULL                );                 // gets updated in Expert::init()
      ec_SetHChartWindow (ec, NULL                );                 // gets updated in Expert::init()
      ec_SetThreadId     (ec, GetCurrentThreadId());

      contextChains[ec->programId][0]->initCycle = TRUE;             // mark master context
   }

   //debug("%s::%s::init()  ec=%s", ec->programName, ec->moduleName, EXECUTION_CONTEXT_toStr(ec));
   return(TRUE);
   #pragma EXPORT
}


/**
 * Update a library's EXECUTION_CONTEXT. Called in Library::deinit().
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

   ec_SetRootFunction(ec, RF_DEINIT   );                             // update library specific context fields
   ec_SetUninitReason(ec, uninitReason);

   //debug("%s::%s::deinit()  ec@%d=%s", ec->programName, ec->moduleName, ec, EXECUTION_CONTEXT_toStr(ec));
   return(TRUE);
   #pragma EXPORT
}


/**
 * Find the first active matching indicator with a released main EXECUTION_CONTEXT.
 *
 * @param  HWND               hChart - correct value of WindowHandle()
 * @param  const char*        name   - indicator name
 * @param  UninitializeReason reason
 *
 * @return int - The found indicator's program id or NULL if no such indicator was found;
 *               EMPTY (-1) if an error occurred
 *
 * Notes:
 * ------
 * Limbo (latin limbus, edge or boundary, referring to the "edge" of Hell) is a speculative idea about the afterlife condition
 * of those who die in original sin without being assigned to the Hell of the Damned. Remember "Inception"? Very hard to escape
 * from.
 * In Metatrader the memory allocated for indicator variables (holding the EXECUTION_CONTEXT, global variables, static local
 * variables etc.) is released after the indicator leaves deinit(). On re-entry in init() new memory is allocated and all
 * variables are initialized with zero which is the reason an indicator cannot keep state over an init cycle. Between deinit()
 * and init() when the indicator enters the state of "limbo" (a mysterious land where the streets have no name known only to
 * the scammers of MetaQuotes) state is kept in the master execution context which acts as a backup of the then lost main
 * execution context. On re-entry the master context is copied back to the newly allocated main context and state of the context
 * survives. Voilà, it crossed the afterlife.
 */
int WINAPI FindFirstIndicatorInLimbo(HWND hChart, const char* name, UninitializeReason reason) {
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
                        //else debug("i=%d  %s  rootFunction not NULL:  master=%s", i, name, RootFunctionToStr(master->rootFunction));
                     }
                     //else debug("i=%d  %s  uninit reason mis-match:  master=%s  reason=%s", i, name, UninitReasonToStr(master->uninitReason), UninitReasonToStr(reason));
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

   //debug("no matching %s indicator in limbo found: hChart=%d  uninitReason=%s", name, hChart, UninitializeReasonToStr(reason))
   return(NULL);
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
   if ((int)id < 1)                   return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.programId=%d)  ec=%s", (int)id, EXECUTION_CONTEXT_toStr(ec)));
   if (ec->rootFunction != RF_DEINIT) return(error(ERR_INVALID_PARAMETER, "invalid execution context (ec.rootFunction not RF_DEINIT)  ec=%s", EXECUTION_CONTEXT_toStr(ec)));

   switch (ec->moduleType) {
      case MT_INDICATOR:
      case MT_SCRIPT:
         if (ec != contextChains[id][1]) return(error(ERR_ILLEGAL_STATE, "%s::%s::deinit()  illegal parameter ec=%d (not stored as main context=%d)  ec=%s", ec->programName, ec->moduleName, ec, contextChains[id][1], EXECUTION_CONTEXT_toStr(ec)));
         ec_SetRootFunction(ec, (RootFunction)NULL);                 // set main and master context to NULL
         contextChains[id][1] = NULL;                                // mark main context as released
         break;

      case MT_EXPERT:
         if (ec != contextChains[id][1]) return(error(ERR_ILLEGAL_STATE, "%s::%s::deinit()  illegal parameter ec=%d (not stored as main context=%d)  ec=%s", ec->programName, ec->moduleName, ec, contextChains[id][1], EXECUTION_CONTEXT_toStr(ec)));

         if (ec->testing) {
            //debug("%s::deinit()  leaving tester, ec=%s", ec->programName, EXECUTION_CONTEXT_toStr(ec));
         }

         ec_SetRootFunction(ec, (RootFunction)NULL);                 // set main and master context to NULL
         if (ec->uninitReason!=UR_CHARTCHANGE && ec->uninitReason!=UR_PARAMETERS && ec->uninitReason!=UR_ACCOUNT)
            contextChains[id][1] = NULL;                             // mark main context as released if not in init cycle
         break;

      case MT_LIBRARY:
         ec_SetRootFunction(ec, (RootFunction)NULL);                 // set library context to NULL
         return(FALSE);

      default:
         return(error(ERR_INVALID_PARAMETER, "invalid execution context:  ec.moduleType=%s", ModuleTypeToStr(ec->moduleType)));
   }

   return(TRUE);
   #pragma EXPORT
}


/**
 * Find the chart of the current program and return its window handle. Must be called only in SyncMainContext_init().
 *
 * @param  HWND               hChart       - MQL::WindowHandle() as passed by the terminal      (possibly not yet set)
 * @param  EXECUTION_CONTEXT* sec          - super context as passed by the terminal            (possibly already released)
 * @param  ModuleType         moduleType   - module type
 * @param  BOOL               isTesting    - MQL::IsTesting() flag as passed by the terminal    (possibly incorrect)
 * @param  BOOL               isVisualMode - MQL::IsVisualMode() flag as passed by the terminal (possibly incorrect)
 * @param  char*              symbol       - current symbol as passed by the terminal
 * @param  uint               timeframe    - current timeframe as passed by the terminal
 *
 * @return HWND - Window handle or NULL if the program runs in the Strategy Tester with VisualMode=Off;
 *                INVALID_HWND (-1) if an error occurred.
 */
HWND WINAPI ProgramFindChart(HWND hChart, const EXECUTION_CONTEXT* sec, ModuleType moduleType, BOOL isTesting, BOOL isVisualMode, const char* symbol, uint timeframe) {
   if (hChart) return(hChart);                                       // if already defined return WindowHandle() as passed
   if (sec) return(sec->hChart);                                     // if a super context exists return the inherited chart handle
                                                                     // (if hChart is not set the super context is always valid)
   // Wir sind im Hauptmodul
   // - kein SuperContext
   // - WindowHandle() ist NULL

   if (isTesting && !isVisualMode)                                   // Im Tester bei VisualMode=Off gibt es keinen Chart: Rückgabewert NULL
      return(NULL);

   // Wir sind entweder: im Tester bei VisualMode=On              aber: kein Hauptmodul hat VisualMode=On und WindowHandle=NULL
   // oder               außerhalb des Testers

   HWND hWndMain = GetApplicationWindow();
   if (!hWndMain) return(INVALID_HWND);

   HWND hWndMdi  = GetDlgItem(hWndMain, IDC_MDI_CLIENT);
   if (!hWndMdi) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "MDIClient window not found (hWndMain=%p)", hWndMain)));

   HWND hChartWindow = NULL;                                         // chart system window holding the chart AfxFrameOrView


   // (1) Indikator
   if (moduleType == MT_INDICATOR) {
      //
      // Wir sind entweder: normaler Template-Indikator bei Terminalstart oder LoadProfile und WindowHandle() ist noch NULL
      // oder:              Tester-Template-Indikator im Tester bei VisualMode=Off => es gibt keinen Chart
      // Wir sind immer:    im UIThread in init()
      //
      // Wir sind nicht:    in iCustom()
      // und auch nicht:    manuell geladener Indikator im Tester-Chart => WindowHandle() wäre gesetzt
      // und auch nicht:    getesteter Indikator eines neueren Builds   => dito

      // Bis Build 509+ ??? kann WindowHandle() bei Terminalstart oder LoadProfile in init() und in start() 0 zurückgeben,
      // solange das Terminal/der Chart nicht endgültig initialisiert sind. Hat das letzte Chartfenster in Z order noch keinen
      // Titel (es wird gerade erzeugt), ist dies das aktuelle Chartfenster. Existiert kein solches Fenster, wird der Indikator
      // über das Tester-Template in einem Test mit VisualMode=Off geladen und wird keinen Chart haben. Die start()-Funktion
      // wird in diesem Fall nie ausgeführt.
      if (!IsUIThread()) return(_INVALID_HWND(error(ERR_ILLEGAL_STATE, "unknown state, non-ui thread=%d  hChart=%d  sec=%d", GetCurrentThreadId(), hChart, sec)));

      HWND hWndChild = GetWindow(hWndMdi, GW_CHILD);                 // first child window in Z order (top most chart window)
      if (!hWndChild)                                                // MDIClient has no children
         return(NULL);                                               // there is no no chart: Tester with VisualMode=Off

      HWND hWndLast = GetWindow(hWndChild, GW_HWNDLAST);             // last child window in Z order (lowest chart window)
      if (GetWindowTextLength(hWndLast))                             // last child window already has a title
         return(NULL);                                               // there is no chart: Tester with VisualMode=Off

      hChartWindow = hWndLast;                                       // keep chart window (holding the chart AfxFrameOrView)
   }


   // (2) Script
   else if (moduleType == MT_SCRIPT) {
      // Bis Build 509+ ??? kann WindowHandle() bei Terminalstart oder LoadProfile in init() und in start() 0 zurückgeben,
      // solange das Terminal/der Chart nicht endgültig initialisiert sind. Ein laufendes Script wurde in diesem Fall über
      // die Konfiguration in "terminal-start.ini" gestartet und läuft im ersten passenden Chart in absoluter Reihenfolge
      // (CtrlID, nicht Z order).
      HWND hWndChild = GetWindow(hWndMdi, GW_CHILD);                 // first child window in Z order (top most chart window)
      if (!hWndChild) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "MDIClient window has no children in Script::init()  hWndMain=%p", hWndMain)));

      size_t bufferSize = MAX_CHART_DESCRIPTION_LENGTH + 1;
      char* chartDescription = (char*)alloca(bufferSize);            // on the stack
      size_t chars = GetChartDescription(symbol, timeframe, chartDescription, bufferSize);
      if (!chars) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "GetChartDescription() failed")));

      bufferSize = 128;
      char* title = (char*)alloca(bufferSize);
      int id = INT_MAX;

      while (hWndChild) {                                            // iterate over all child windows
         size_t titleLen = GetWindowText(hWndChild, title, bufferSize);
         if (titleLen) {
            if (titleLen >= bufferSize-1) {
               bufferSize <<= 1;
               title = (char*)alloca(bufferSize);
               continue;
            }
            if (StringEndsWith(title, " (offline)"))
               title[titleLen-10] = 0;
            if (StringCompare(title, chartDescription)) {            // find all matching windows
               id = std::min(id, GetDlgCtrlID(hWndChild));           // track the smallest in absolute order
               if (!id) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "MDIClient child window %p has no control id", hWndChild)));
            }
         }
         hWndChild = GetWindow(hWndChild, GW_HWNDNEXT);              // next child in Z order
      }
      if (id == INT_MAX) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "no matching MDIClient child window found for \"%s\"", chartDescription)));

      hChartWindow = GetDlgItem(hWndMdi, id);                        // keep chart window (holding the chart AfxFrameOrView)
   }


   // (3) Expert
   else if (moduleType == MT_EXPERT) {
      return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "MQL::WindowHandle() => 0 in Expert::init()")));
   }
   else {
      return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter moduleType = %d", moduleType)));
   }


   // (4) Das gefundene Chartfenster hat genau ein Child (AfxFrameOrView), welches das gesuchte Metatrader-Handle ist.
   hChart = GetWindow(hChartWindow, GW_CHILD);
   if (!hChart) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "no Metatrader chart window inside of last MDIClient child window %p found", hChartWindow)));

   return(hChart);
}


/**
 * Resolve the given program's current init() scenario.
 *
 * @param  EXECUTION_CONTEXT* ec              - execution context as passed by the terminal       (possibly still empty)
 * @param  EXECUTION_CONTEXT* sec             - super context as passed by the terminal           (possibly already released)
 * @param  char*              programName     - program name
 * @param  ProgramType        programType     - program type
 * @param  UninitializeReason uninitReason    - UninitializeReason as set by the terminal
 * @param  char*              symbol          - current symbol
 * @param  BOOL               isTesting       - IsTesting() flag as passed by the terminal        (possibly incorrect)
 * @param  BOOL               isVisualMode    - IsVisualMode() flag as passed by the terminal     (possibly incorrect)
 * @param  HWND               hChart          - correct value of WindowHandle()
 * @param  int                subChartDropped - WindowOnDropped() index as passed by the terminal
 *
 * @return InitializeReason - init reason or NULL if an error occurred
 */
InitializeReason WINAPI ProgramInitReason(EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec, const char* programName, ProgramType programType, UninitializeReason uninitReason, const char* symbol, BOOL isTesting, BOOL isVisualMode, HWND hChart, int subChartDropped) {
   if ((uint)ec          < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec  < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter sec = 0x%p (not a valid pointer)", sec));
   if ((uint)programName < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter programName = 0x%p (not a valid pointer)", programName));
   if ((uint)symbol      < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));

   if (programType == PT_SCRIPT) return(               IR_USER);
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
   - Build  >= 654: onInit_User()             - UninitializeReason() ist UR_UNDEFINED.
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build 577-583: onInit_Template()         - Broken: Kein Aufruf bei Terminal-Start, der Indikator wird aber geladen.
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build 556-569: onInit_Program()          - Broken: Wird in- und außerhalb des Testers bei jedem Tick aufgerufen.
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build  <= 229: onInit_ProgramAfterTest() - UninitializeReason() ist UR_UNDEFINED.
   - Build     387: onInit_ProgramAfterTest() - Broken: Wird nie aufgerufen.
   - Build 388-628: onInit_ProgramAfterTest() - UninitializeReason() ist UR_REMOVE.
   - Build  <= 577: onInit_ProgramAfterTest() - Wird nur nach einem automatisiertem Test aufgerufen (VisualMode=Off), der Aufruf
                                                erfolgt vorm Start des nächsten Tests.
   - Build  >= 578: onInit_ProgramAfterTest() - Wird auch nach einem manuellen Test aufgerufen (VisualMode=On), nur in diesem Fall
                                                erfolgt der Aufruf sofort nach Testende.
   - Build  >= 633: onInit_ProgramAfterTest() - UninitializeReason() ist UR_CHARTCLOSE.
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build 577:     onInit_TimeframeChange()  - Broken: Bricht mit Logmessage "WARN: expert stopped" ab.
   ------------------------------------------------------------------------------------------------------------------------------------
   */
   uint build      = GetTerminalBuild();
   BOOL isUIThread = IsUIThread();


   // (1) UR_PARAMETERS
   if (uninitReason == UR_PARAMETERS) {
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
         if (programId) ec_SetProgramId(ec, programId);              // ProgramID on-the-fly speichern
      }
      if (isProgramNew) return(IR_USER      );                       // erste Parameter-Eingabe eines manuell neu hinzugefügten Indikators
      else              return(IR_PARAMETERS);                       // Parameter-Wechsel eines vorhandenen Indikators
   }


   // (2) UR_CHARTCHANGE
   if (uninitReason == UR_CHARTCHANGE) {
      // innerhalb iCustom(): nie
      if (sec)               return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // außerhalb iCustom(): bei Symbol- oder Timeframe-Wechsel eines vorhandenen Indikators, kein Input-Dialog
      int programId = ec->programId;
      if (!programId) {
         programId = FindFirstIndicatorInLimbo(hChart, programName, uninitReason);
         if (programId <= 0) return((InitializeReason)(programId < 0 ? NULL : error(ERR_RUNTIME_ERROR, "no %s indicator during %s in limbo found", programName, UninitializeReasonToStr(uninitReason))));
         if (programId) ec_SetProgramId(ec, programId);              // ProgramID on-the-fly speichern
      }
      char* masterSymbol = contextChains[programId][0]->symbol;
      if (strcmp(masterSymbol, symbol)) return(IR_TIMEFRAMECHANGE);
      else                              return(IR_SYMBOLCHANGE   );
   }


   // (3) UR_UNDEFINED
   if (uninitReason == UR_UNDEFINED) {
      // außerhalb iCustom(): je nach Umgebung
      if (!sec) {
         if (build < 654)          return(IR_TEMPLATE);              // wenn Template mit Indikator geladen wird (auch bei Start und im Tester bei VisualMode=On|Off), kein Input-Dialog
         if (subChartDropped >= 0) return(IR_TEMPLATE);
         else                      return(IR_USER    );              // erste Parameter-Eingabe eines manuell neu hinzugefügten Indikators, Input-Dialog
      }
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (isTesting && !isVisualMode/*Fix*/ && isUIThread) {         // versionsunabhängig
         if (build <= 229)         return(IR_PROGRAM_AFTERTEST);
                                   return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      }
      return(IR_PROGRAM);
   }


   // (4) UR_REMOVE
   if (uninitReason == UR_REMOVE) {
      // außerhalb iCustom(): nie
      if (!sec)                                                 return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (!isTesting || !isUIThread)                            return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      if (!isVisualMode/*Fix*/) { if (388<=build && build<=628) return(IR_PROGRAM_AFTERTEST); }
      else                      { if (578<=build && build<=628) return(IR_PROGRAM_AFTERTEST); }
      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
   }


   // (5) UR_RECOMPILE
   if (uninitReason == UR_RECOMPILE) {
      // innerhalb iCustom(): nie
      if (sec) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // außerhalb iCustom(): bei Reload nach Recompilation, vorhandener Indikator, kein Input-Dialog
      return(IR_RECOMPILE);
   }


   // (6) UR_CHARTCLOSE
   if (uninitReason == UR_CHARTCLOSE) {
      // außerhalb iCustom(): nie
      if (!sec)                      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (!isTesting || !isUIThread) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      if (build >= 633)              return(IR_PROGRAM_AFTERTEST);
      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
   }


   switch (uninitReason) {
      case UR_ACCOUNT:       // nie
      case UR_TEMPLATE:      // build > 509
      case UR_INITFAILED:    // ...
      case UR_CLOSE:         // ...
         return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
   }
   return((InitializeReason)error(ERR_ILLEGAL_STATE, "unknown UninitializeReason %d  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", uninitReason, sec, isTesting, isVisualMode, isUIThread, build));
}


/**
 * Whether or not the program is executed in the Strategy Tester or on a Strategy Tester chart.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isTesting - MQL::IsTesting() status as passed by the terminal (possibly wrong)
 *
 * @return BOOL - real IsTesting() status
 */
BOOL WINAPI ProgramIsTesting(const EXECUTION_CONTEXT* ec, BOOL isTesting) {
   if (ec->superContext)
      return(ec->superContext->testing);                             // prefer an inherited status

   switch (ec->programType) {
      // indicators
      case PT_INDICATOR: {
         if (isTesting)                                              // indicator runs in iCustom() in Tester
            return(TRUE);
         // (1) indicator was loaded manually                        // we have no super context
         //     (1.1) not in Tester:                     chart exists, title is set and doesn't end with "(visual)"
         //     (1.2) in Tester:                         chart exists, title is set and does    end with "(visual)"

         // (2) indicator was loaded by template
         //     (2.1) not in Tester:                     chart exists, title is empty or doesn't end with "(visual)"
         //     (2.2) in Tester:                         chart exists, title is set and does     end with "(visual)"
         //     (2.3) in Tester                       or chart doesn't exist with VisualMode=Off
         HWND hWnd = ec->hChartWindow;
         if (!hWnd) return(TRUE);                                    // (2.3) no chart => in Tester with VisualMode=Off

         int titleLen = GetWindowTextLength(hWnd);
         if (!titleLen) return(FALSE);                               // (2.1) title is empty => not in Tester

         char* title = (char*)alloca(titleLen+1);                    // on the stack
         GetWindowText(hWnd, title, titleLen+1);
         return(StringEndsWith(title, "(visual)"));                  // all remaining cases according to "(visual)" in title
      }

      // experts
      case PT_EXPERT:
         return(isTesting);

      // scripts
      case PT_SCRIPT: {
         HWND hWnd = ec->hChartWindow;
         if (hWnd) {
            int bufferSize = 128;
            char* title = (char*)alloca(bufferSize);                 // on the stack
            while (GetWindowText(hWnd, title, bufferSize) >= bufferSize-1) {
               bufferSize <<= 1;
               title = (char*)alloca(bufferSize);
            }
            return(StringEndsWith(title, "(visual)"));
         }
         return(error(ERR_ILLEGAL_STATE, "script without a chart:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
      }
   }

   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType = %d", ec->programType));
}


/**
 * Whether or not the program is executed in the Strategy Tester or on a Strategy Tester chart with VisualMode=On.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isVisualMode - MQL::IsVisualMode() status as passed by the terminal (possibly wrong)
 *
 * @return BOOL - real IsVisualMode() status
 */
BOOL WINAPI ProgramIsVisualMode(const EXECUTION_CONTEXT* ec, BOOL isVisualMode) {
   if (ec->superContext)
      return(ec->superContext->visualMode);                          // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR: return(ec->testing && ec->hChart);
      case PT_EXPERT:    return(isVisualMode);
      case PT_SCRIPT:    return(ec->testing);                        // scripts can only run on visible charts
   }
   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType = %d", ec->programType));
}


/**
 * Whether or not the program is executed in the Strategy Tester with Optimization=On.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isOptimization - MQL::IsOptimization() status as passed by the terminal
 *
 * @return BOOL - real IsOptimization() status
 */
BOOL WINAPI ProgramIsOptimization(const EXECUTION_CONTEXT* ec, BOOL isOptimization) {
   if (ec->superContext)
      return(ec->superContext->optimization);                        // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR:
      case PT_EXPERT:
      case PT_SCRIPT: return(isOptimization);
   }
   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType = %d", ec->programType));
}


/**
 * Whether or not logging is activated for the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ProgramIsLogging(const EXECUTION_CONTEXT* ec) {
   if (ec->superContext)
      return(ec->superContext->logging);                             // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR:
      case PT_EXPERT: //return(IsLogging());                         // TODO: implement IsLogging()
      case PT_SCRIPT: return(TRUE);
   }
   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType = %d", ec->programType));
}


/**
 * Resolve the custom log file of the program (if any)
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char*
 */
const char* WINAPI ProgramCustomLogFile(const EXECUTION_CONTEXT* ec) {
   if (ec->superContext)
      return(ec->superContext->customLogFile);                       // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR:
      case PT_EXPERT:
      case PT_SCRIPT:
         return(NULL);
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid value ec.programType = %d", ec->programType));
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
