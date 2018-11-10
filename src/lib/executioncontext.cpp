#include "expander.h"
#include "lib/conversion.h"
#include "lib/executioncontext.h"
#include "lib/helper.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "struct/mt4/PriceBar400.h"
#include "struct/mt4/PriceBar401.h"

#include <vector>


std::vector<ContextChain> g_contextChains  (128);        // all context chains, i.e. MQL programs (index = program id)
std::vector<DWORD>        g_threads        (128);        // all known threads executing MQL programs
std::vector<uint>         g_threadsPrograms(128);        // the last MQL program executed by a thread
uint                      g_lastUIThreadProgram;         // the last MQL program executed by the UI thread
CRITICAL_SECTION          g_terminalMutex;               // mutex for application-wide locking


/**
 *  Init cycle of a single indicator using single and nested library calls:
 *  --- first load ----------------------------------------------------------------------------------------------------------
 *  Indicator::init()              UR_UNDEFINED    pid=0  creating new chain             set pid=1
 *  Indicator::libraryA::init()    UR_UNDEFINED    pid=0  loaded by indicator            set pid=1
 *  Indicator::libraryB::init()    UR_UNDEFINED    pid=0  loaded by indicator            set pid=1
 *  Indicator::libraryC::init()    UR_UNDEFINED    pid=0  loaded by libraryA             set pid=1
 *  --- deinit() ------------------------------------------------------------------------------------------------------------
 *  Indicator::deinit()            UR_CHARTCHANGE  pid=1  indicator first
 *  Indicator::libraryA::deinit()  UR_UNDEFINED    pid=1  then libraries
 *  Indicator::libraryC::deinit()  UR_UNDEFINED    pid=1  hierarchical (not in loading order)
 *  Indicator::libraryB::deinit()  UR_UNDEFINED    pid=1
 *  --- init() --------------------------------------------------------------------------------------------------------------
 *  Indicator::libraryA::init()    UR_UNDEFINED    pid=1  libraries first (new symbol and timeframe show up)
 *  Indicator::libraryC::init()    UR_UNDEFINED    pid=1  hierarchical (not in loading order)
 *  Indicator::libraryB::init()    UR_UNDEFINED    pid=1
 *  Indicator::init()              UR_CHARTCHANGE  pid=0  then indicator                 set pid=1
 *  -------------------------------------------------------------------------------------------------------------------------
 *
 *
 *  Init cycle of multiple indicators using single library calls:
 *  --- first load ----------------------------------------------------------------------------------------------------------
 *  ChartInfos::init()             UR_UNDEFINED    pid=0  creating new chain             set pid=1
 *  ChartInfos::lib::init()        UR_UNDEFINED    pid=0  loaded by indicator            set pid=1
 *  SuperBars::init()              UR_UNDEFINED    pid=0  creating new chain             set pid=2
 *  SuperBars::lib::init()         UR_UNDEFINED    pid=0  loaded by indicator            set pid=2
 *  --- deinit() ------------------------------------------------------------------------------------------------------------
 *  ChartInfos::deinit()           UR_CHARTCHANGE  pid=1
 *  ChartInfos::lib::deinit()      UR_UNDEFINED    pid=1
 *  SuperBars::deinit()            UR_CHARTCHANGE  pid=2
 *  SuperBars::lib::deinit()       UR_UNDEFINED    pid=2
 *  --- init() --------------------------------------------------------------------------------------------------------------
 *  ChartInfos::lib::init()        UR_UNDEFINED    pid=1
 *  ChartInfos::init()             UR_CHARTCHANGE  pid=0  first indicator in limbo       set pid=1
 *  SuperBars::lib::init()         UR_UNDEFINED    pid=2
 *  SuperBars::init()              UR_CHARTCHANGE  pid=0  next indicator in limbo        set pid=2
 *  -------------------------------------------------------------------------------------------------------------------------
 */


/**
 * Initialize and synchronize an MQL program's execution context with the master context stored in this DLL. Called by the
 * init() functions of the MQL main modules. For a general overview see type EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec             - an MQL program's main module execution context
 * @param  ProgramType        programType    - program type
 * @param  char*              programName    - program name (with or without filepath depending on the terminal version)
 * @param  UninitializeReason uninitReason   - value of UninitializeReason() as returned by the terminal
 * @param  DWORD              initFlags      - init configuration
 * @param  DWORD              deinitFlags    - deinit configuration
 * @param  char*              symbol         - current chart symbol
 * @param  uint               period         - current chart period
 * @param  uint               digits         - the current symbol's "Digits" value (possibly incorrect)
 * @param  double             point          - the current symbol's "Point" value (possibly incorrect)
 * @param  BOOL               extReporting   - value of an Expert's input parameter "EA.ExtReporting"
 * @param  BOOL               recordEquity   - value of an Expert's input parameter "EA.RecordEquity"
 * @param  BOOL               isTesting      - value of IsTesting() as returned by the terminal (possibly incorrect)
 * @param  BOOL               isVisualMode   - value of IsVisualMode() as returned by the terminal (possibly incorrect)
 * @param  BOOL               isOptimization - value of IsOptimzation() as returned by the terminal
 * @param  EXECUTION_CONTEXT* sec            - super context as managed by the terminal (memory possibly already released)
 * @param  HWND               hChart         - value of WindowHandle() as returned by the terminal (possibly not yet set)
 * @param  int                droppedOnChart - value of WindowOnDropped() as returned by the terminal (possibly incorrect)
 * @param  int                droppedOnPosX  - value of WindowXOnDropped() as returned by the terminal (possibly incorrect)
 * @param  int                droppedOnPosY  - value of WindowYOnDropped() as returned by the terminal (possibly incorrect)
 *
 * @return int - error status
 */
int WINAPI SyncMainContext_init(EXECUTION_CONTEXT* ec, ProgramType programType, const char* programName, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint period, uint digits, double point, BOOL extReporting, BOOL recordEquity, BOOL isTesting, BOOL isVisualMode, BOOL isOptimization, EXECUTION_CONTEXT* sec, HWND hChart, int droppedOnChart, int droppedOnPosX, int droppedOnPosY) {
   if ((uint)ec          < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if ((uint)programName < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName)));
   if ((uint)symbol      < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol)));
   if ((int)period <= 0)                      return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter period: %d", (int)period)));
   if ((int)digits <  0)                      return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d", (int)digits)));
   if (sec && (uint)sec  < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec)));

   uint currentPid           = ec->pid;
   BOOL isPid                = (currentPid);
   uint prevPid              = g_threadsPrograms[GetCurrentThreadIndex()]; // pid of the last program executed by the current thread
   uint formerIndPid         = NULL;                                       // old pid of an indicator in init cycle
   BOOL isPartialTest        = FALSE;                                      // whether an partially initialized expert in tester
   EXECUTION_CONTEXT* master = NULL;

   if (isPid) LinkProgramToCurrentThread(currentPid);                      // link the currently executed program asap (error handling)

   // resolve the true InitReason
   InitializeReason initReason  = GetInitReason(ec, sec, programType, programName, uninitReason, symbol, isTesting, isVisualMode, hChart, droppedOnChart, droppedOnPosX, droppedOnPosY, formerIndPid);
   if (!initReason)                       return(ERR_RUNTIME_ERROR);
   if (initReason == IR_TERMINAL_FAILURE) return(_int(ERR_TERMINAL_FAILURE_INIT, debug("%s  InitReason=IR_TERMINAL_FAILURE", programName)));

   // (1) if ec.pid is not set: check if an indicator in init cycle
   //     • (1.1) if indicator in init cycle (only in UI thread):
   //       - restore main context from master context
   //     • (1.2) if not indicator in init cycle: new indicator, new expert or script
   //       - create new master context
   //       - create new context chain and store master and main context
   //       - set generated pid in master and main context
   //
   // (2) ec.pid is set:
   //     • update main context
   //
   // (3) if expert in tester:
   //     • find and reassign reused libraries of the preceeding test

   if (!isPid) {
      if (programType==PT_INDICATOR && formerIndPid) {            // check if indicator in init cycle
         // (1.1) indicator in init cycle (only in UI thread)
         currentPid = formerIndPid;
         LinkProgramToCurrentThread(currentPid);                  // link the currently executed program asap (error handling)

         master = g_contextChains[formerIndPid][0];
         *ec    = *master;                                        // restore main from master context (pid is already set)
         g_contextChains[formerIndPid][1] = ec;                   // overwrite old main context (is already released)
      }
      else {
         // (1.2) not an indicator in init cycle: new indicator, expert or script
         // if an expert in tester check for an existing partially initialized expert of the same name
         if (programType==PT_EXPERT && isTesting && (isPartialTest=Program_IsPartialTest(prevPid, programName))) {
            currentPid = prevPid;
            LinkProgramToCurrentThread(currentPid);               // link the currently executed program asap (error handling)

            // finish initialization of the existing context chain (expert with reused libraries)
            master = g_contextChains[currentPid][0];
            *ec = *master;                // everything????       // overwrite main with master context
            g_contextChains[currentPid][1] = ec;                  // store main context at empty position
         }
         else {
            // create a new context chain                         // TODO: on IR_PROGRAM_AFTERTEST somewhere exists a used context
            master  = new EXECUTION_CONTEXT();                    // create new master context
            *master = *ec;                                        // copy main to master context
            ContextChain chain;
            chain.reserve(8);
            chain.push_back(master);                              // store master and main context in a new context chain
            chain.push_back(ec);

            currentPid = ContextChainsPush(chain);                // store the chain and update master and main context
            master->pid = ec->pid = currentPid;
            LinkProgramToCurrentThread(currentPid);
         }
      }

      BOOL indicatorAfterTest = (programType==PT_INDICATOR && initReason==IR_PROGRAM_AFTERTEST);
      if (indicatorAfterTest) {
         ec_SetSuperContext(ec, sec=NULL);                        // the super context (an expert) has already been released
      }
   }
   else {
      // (2) ec.pid is already set: an expert in init cycle or any program after a repeated init() call
   }


   // (2.1) Beim ersten Aufruf von init() zu initialisieren
   if (!ec->ticks) {                                              // fix an unset chart handle (older terminals)
      hChart = FindWindowHandle(hChart, sec, (ModuleType)programType, symbol, period, isTesting, isVisualMode);
      if (hChart == INVALID_HWND) return(ERR_RUNTIME_ERROR);

      ec_SetProgramType  (ec,             programType);
      ec_SetProgramName  (ec,             programName);
      ec_SetModuleType   (ec, (ModuleType)programType);           // in the main module ProgramType and ModuleType are the same
      ec_SetModuleName   (ec,             programName);

    //ec_SetLaunchType   (ec,             launchType );
      ec_SetInitFlags    (ec, initFlags              );
      ec_SetDeinitFlags  (ec, deinitFlags            );

      ec_SetSuperContext (ec, sec   );
      ec_SetHChart       (ec, hChart);                            // chart handles must be set before ec_SetTesting()
      ec_SetHChartWindow (ec, hChart ? GetParent(hChart) : NULL);

      ec_SetTesting      (ec, isTesting     =Program_IsTesting     (ec, isTesting     ));
      ec_SetVisualMode   (ec, isVisualMode  =Program_IsVisualMode  (ec, isVisualMode  ));
      ec_SetOptimization (ec, isOptimization=Program_IsOptimization(ec, isOptimization));

      ec_SetLogging      (ec, Program_IsLogging    (ec));
      ec_SetCustomLogFile(ec, Program_CustomLogFile(ec));
   }

   // (2.2) Bei jedem Aufruf von init() zu aktualisieren
   ec_SetCoreFunction (ec, CF_INIT     );                         // TODO: wrong for init() calls from start()
 //ec_SetInitCycle    (ec, FALSE       );
   ec_SetInitReason   (ec, initReason  );
   ec_SetUninitReason (ec, uninitReason);

   ec_SetSymbol       (ec, symbol);
   ec_SetTimeframe    (ec, period);
   ec_SetDigits       (ec, digits);
   ec_SetPoint        (ec, point );

   ec_SetBars         (ec,  0);
   ec_SetChangedBars  (ec, -1);
   ec_SetUnchangedBars(ec, -1);

   ec_SetExtReporting (ec, extReporting);
   ec_SetRecordEquity (ec, recordEquity);

   ec_SetThreadId     (ec, GetCurrentThreadId());


   // (3) if a partially initialized expert between tests update the reloaded libaries
   if (programType==PT_EXPERT && isPartialTest) {
      ContextChain& chain = g_contextChains[currentPid];
      uint size = chain.size();

      for (uint i=2; i < size; ++i) {                             // skip master and main context
         EXECUTION_CONTEXT* lib = chain[i];
         if (!lib) {
            warn(ERR_ILLEGAL_STATE, "unexpected library context found at chain[%d]: NULL  ec=%s", i, EXECUTION_CONTEXT_toStr(ec));
            continue;
         }
         EXECUTION_CONTEXT bak = *lib;                            // backup the library on the stack
         *lib = *chain[0];                                        // overwrite it with master context

         ec_SetModuleType (lib, MT_LIBRARY     );                 // restore/update library-specific fields
         ec_SetModuleName (lib, bak.moduleName );

         ec_SetInitCycle  (lib, FALSE          );
         ec_SetInitFlags  (lib, bak.initFlags  );
         ec_SetDeinitFlags(lib, bak.deinitFlags);

         ec_SetMqlError   (lib, NULL);                            // reset all errors
         ec_SetDllError   (lib, NULL);
         ec_SetDllWarning (lib, NULL);
         lib->dllErrorMsg   =   NULL;
         lib->dllWarningMsg =   NULL;
      }
   }
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * @param  EXECUTION_CONTEXT* ec          - main module context of a program
 * @param  void*              rates       - price history of the chart
 * @param  int                bars        - current amount of price bars (chart history)
 * @param  int                changedBars - current amount of changed indicator values
 * @param  uint               ticks       - number of received ticks, i.e. calls of MQL::start()
 * @param  datetime           time        - server time of the current tick
 * @param  double             bid         - bid price of the current tick
 * @param  double             ask         - ask price of the current tick
 *
 * @return int - error status
 */
int WINAPI SyncMainContext_start(EXECUTION_CONTEXT* ec, const void* rates, int bars, int changedBars, uint ticks, datetime time, double bid, double ask) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (!ec->pid)                     return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context, ec.pid: %d", ec->pid)));

   LinkProgramToCurrentThread(ec->pid);                              // link the currently executed program asap (error handling)

   ec_SetCoreFunction (ec, CF_START);                                // update context
   ec_SetThreadId     (ec, GetCurrentThreadId());
   ec->rates             = rates;
   ec_SetBars         (ec, bars);
   ec_SetChangedBars  (ec, changedBars);
   ec_SetUnchangedBars(ec, changedBars==-1 ? -1 : bars-changedBars);
   ec_SetTicks        (ec, ticks);
   ec_SetPrevTickTime (ec, ec->lastTickTime);
   ec_SetLastTickTime (ec, time);
   ec_SetBid          (ec, bid);
   ec_SetAsk          (ec, ask);

   /*
   if (rates && bars) {
      uint bar=0, shift=bars-1-bar;

      if (GetTerminalBuild() <= 509) {
         RateInfo* rate = (RateInfo*) rates;
         debug("400:  bars: %d  bar[%d]:  time=%d  C=%f  V=%d", bars, bar, rate[shift].time, rate[shift].close, (int)rate[shift].ticks);
      }
      else {
         MqlRates* rate = (MqlRates*) rates;
         debug("401:  bars: %d  bar[%d]:  time=%d  C=%f  V=%d", bars, bar, rate[shift].time, rate[shift].close, rate[shift].ticks);
      }
   }
   */
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * @param  EXECUTION_CONTEXT* ec           - Context des Hauptmoduls eines MQL-Programms
 * @param  UninitializeReason uninitReason - UninitializeReason as passed by the terminal
 *
 * @return int - error status
 */
int WINAPI SyncMainContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (!ec->pid)                     return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context, ec.pid: %d", ec->pid)));

   LinkProgramToCurrentThread(ec->pid);                              // link the currently executed program asap (error handling)

   ec_SetCoreFunction(ec, CF_DEINIT           );                     // update context
   ec_SetUninitReason(ec, uninitReason        );
   ec_SetThreadId    (ec, GetCurrentThreadId());

   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Called only from Library::init().
 *
 * Synchronize a library's EXECUTION_CONTEXT with the context of the program's main module. If a library is loaded the first
 * time its context is added to the program's context chain.
 *
 * @param  EXECUTION_CONTEXT* ec             - the libray's execution context
 * @param  UninitializeReason uninitReason   - UninitializeReason as passed by the terminal
 * @param  DWORD              initFlags      - init configuration
 * @param  DWORD              deinitFlags    - deinit configuration
 * @param  char*              moduleName     - the library's name (may contain a path depending on the terminal version)
 * @param  char*              symbol         - current chart symbol
 * @param  uint               period         - current chart period
 * @param  uint               digits         - the symbol's "Digits" value (possibly incorrect)
 * @param  double             point          - the symbol's "Point" value (possibly incorrect)
 * @param  BOOL               isOptimization - MQL::IsOptimization() as passed by the terminal
 *
 * @return int - error status
 *
 *
 * Notes:
 * ------
 * During init cycles libraries keep state. This is used to distinguish between a freshly loaded and a reused library
 * (init cycle). Libraries perform init cycles in two cases:
 *
 * (1) Libraries loaded by indicators during the indicator's init cycle. Call order:
 *      - Indicator::deinit()
 *      - Library::deinit()    => BUG: global string variables are already destroyed and lost
 *      - Library::init()
 *      - Indicator::init()
 *
 * (2) Libraries loaded by experts between multiple tests of the same strategy. In newer terminals (since when exactly?) this
 *     happens only if the test was not explicitly stopped by using the "Stop" button. In older terminals (e.g. build 500)
 *     this happens for all such tests. Call order:
 *      - Expert::deinit()
 *      - Library::deinit()    => BUG: global string variables are already destroyed and lost
 *      - Library::init()
 *      - Expert::init()
 *
 *     Probably this init cycle is meant to speed up optimizations but the actual implementation must be considered a bug.
 *     Reused libraries should be reset before each new test. Instead they keep state of the previously finished test,
 *     specifically:
 *      - Global variables are not reset and keep former values (except strings).
 *      - The last selected order context is not reset and order functions return wrong results.
 *      - The flag IsVisualMode() is not reset even if symbol or timeframe of the next test change.
 *
 *     Workaround: On start of a test reused libraries need to be manually reset:
 *      - SyncMainContext_init() removes a library from the previously finished expert's context and attaches it to the
 *        context of the newly tested expert.
 *      - MQL::core/library::init() resets a previously selected order context.
 *      - Global array variables must be reset by implementing library::Tester.ResetGlobalLibraryVars().
 *      - The MQL function IsVisualMode() must not be used, instead use the corresponding flag in the execution context.
 *
 *
 * @see  https://github.com/rosasurfer/mt4-expander/blob/master/src/lib/executioncontext.cpp  SyncMainContext_init()
 * @see  https://github.com/rosasurfer/mt4-mql/blob/master/mql4/include/core/library.mqh      init()
 */
int WINAPI SyncLibContext_init(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* moduleName, const char* symbol, uint period, uint digits, double point, BOOL isOptimization) {
   if ((uint)ec         < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if ((uint)moduleName < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter moduleName: 0x%p (not a valid pointer)", moduleName)));
   if ((uint)symbol     < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol)));
   if ((int)period <= 0)                     return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter period: %d", (int)period)));
   if ((int)digits <  0)                     return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d", (int)digits)));
   if (point <= 0)                           return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter point: %f", point)));

   // (1) if ec.pid is not set: library is loaded the first time and the context is empty
   //     - copy master context and update library specific fields
   //
   // (2) if ec.pid is set: check if init cycle of indicator (UI thread) or expert between tests (non UI thread)
   //     (2.1) init cycle of indicator or
   //     (2.2) init cycle of expert between tests

   if (!ec->pid) {
      // (1) library is loaded the first time
      uint index = GetCurrentThreadIndex();                          // the program (which caused "load-library") is already linked
      uint pid = g_threadsPrograms[index];                           // to the thread: get its index

      *ec = *g_contextChains[pid][0];                                // overwrite library context with master context

      ec_SetModuleType (ec, MT_LIBRARY );                            // update library specific fields
      ec_SetModuleName (ec, moduleName );

      ec_SetInitCycle  (ec, FALSE      );
      ec_SetInitFlags  (ec, initFlags  );                            // libraries may have their own init flags
      ec_SetDeinitFlags(ec, deinitFlags);

      ec_SetMqlError   (ec, NULL);                                   // all errors initialized with NULL
      ec_SetDllError   (ec, NULL);
      ec_SetDllWarning (ec, NULL);
      ec->dllErrorMsg   =   NULL;
      ec->dllWarningMsg =   NULL;

      g_contextChains[pid].push_back(ec);                            // add context to the program's context chain
   }

   else if (IsUIThread()) {
      // (2.1) init cycle of indicator: Library::init() is called before Indicator::init()
      LinkProgramToCurrentThread(ec->pid);                           // link the currently executed program asap (error handling)

      ec_SetCoreFunction(ec, CF_INIT     );                          // update library specific fields
      ec_SetInitCycle   (ec, FALSE       );                          // TODO: mark master context ???
      ec_SetUninitReason(ec, uninitReason);
      ec_SetSymbol      (ec, symbol      );
      ec_SetTimeframe   (ec, period      );
      ec_SetDigits      (ec, digits      );
   }

   else {
      // (2.2) init cycle of experts between tests: Library::init() is called before Expert::init()
      // ec.pid points to the previously tested and finished expert
      if (ec->programType!=PT_EXPERT || !ec->testing) return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "unexpected library init cycle (programType=%s  testing=%s)", ProgramTypeToStr(ec->programType), BoolToStr(ec->testing))));

      EXECUTION_CONTEXT* master=NULL, *oldMaster=NULL;               // master of current and old test
      uint threadIndex = GetCurrentThreadIndex();
      uint oldPid      = ec->pid;                                    // pid of the finished test
      uint currentPid  = g_threadsPrograms[threadIndex];             // pid of the current test
      BOOL isPartialChain;

      // check if a partially initialized context chain exists (master->coreFunction=CF_INIT, main=NULL)
      if (!currentPid || currentPid==oldPid) {                       // first library in init cyle: the thread never executed a program
         isPartialChain = FALSE;                                     // or the program is the finished test (probably in an optimization)
      }
      else {                                                         // get the last executed program: it's myself or something else
         ContextChain& chain = g_contextChains[currentPid];          // if partial chain found, it's myself with one more library in same init cyle
         isPartialChain = (chain.size()>1 && (master=chain[0]) && chain[0]->coreFunction==CF_INIT && !chain[1]);
         if (!isPartialChain) debug("unseen library init cycle in tester (the former program seems not to be the former test):  ec=%s", EXECUTION_CONTEXT_toStr(ec));
      }

      if (!isPartialChain) {
         // create a new partially initialized chain
         master    = new EXECUTION_CONTEXT();                        // create new master context
         oldMaster = g_contextChains[oldPid][0];

         ContextChain chain;
         chain.reserve(8);
         chain.push_back(master);                                    // add master to a new chain
         chain.push_back(NULL);                                      // add empty entry for the yet to come main context
         currentPid = ContextChainsPush(chain);                      // store the chain
         LinkProgramToCurrentThread(currentPid);

         master->pid               = currentPid;                     // update master context with the known properties
         master->programType       = PT_EXPERT;
         strcpy(master->programName, ec->programName);
         master->moduleType        = MT_EXPERT;
         strcpy(master->moduleName,  oldMaster->moduleName);

         master->coreFunction = CF_INIT;
         master->initCycle    = FALSE;

         strcpy(master->symbol, symbol);                             // first moment a new symbol/timeframe show up
         master->timeframe    = period;
         master->point        = point;
         master->digits       = digits;

         master->testing      = TRUE;
         master->optimization = isOptimization;

         master->superContext = FALSE;
         master->threadId     = g_threads[threadIndex];
      }

      // re-initialize the library context with the master context
      EXECUTION_CONTEXT bak = *ec;                                   // backup on the stack
      *ec = *master;
      ec->moduleType   =     MT_LIBRARY;                             // update library specifics
      strcpy(ec->moduleName, bak.moduleName);
      ec->uninitReason =     uninitReason;
      ec->initFlags    =     initFlags;
      ec->deinitFlags  =     deinitFlags;

      g_contextChains[currentPid].push_back(ec);                     // add library to the current test's context chain

      ContextChain& oldChain = g_contextChains[oldPid];              // remove the library from the finished test's chain
      uint size = oldChain.size();
      for (uint i=2; i < size; ++i) {
         if (oldChain[i] == ec) {
            oldChain[i] = NULL;
            break;
         }
      }
   }
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Update a library's EXECUTION_CONTEXT. Called in Library::deinit().
 *
 * @param  EXECUTION_CONTEXT* ec           - the libray's execution context
 * @param  UninitializeReason uninitReason - UninitializeReason as passed by the terminal
 *
 * @return int - error status
 */
int WINAPI SyncLibContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (!ec->pid)                     return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context, ec.pid: %d", ec->pid)));

   LinkProgramToCurrentThread(ec->pid);                              // link the currently executed program asap (error handling)

   ec_SetCoreFunction(ec, CF_DEINIT   );                             // update library specific context fields
   ec_SetUninitReason(ec, uninitReason);

   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Signal leaving of an MQL module's execution context. Called at leaving of MQL::deinit().
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - error status
 */
int WINAPI LeaveContext(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER)  return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: %p (not a valid pointer)", ec)));
   uint pid = ec->pid;
   if ((int)pid < 1)                  return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=%d)  ec=%s", (int)pid, EXECUTION_CONTEXT_toStr(ec))));
   if (ec->coreFunction != CF_DEINIT) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context (ec.coreFunction not CF_DEINIT)  ec=%s", EXECUTION_CONTEXT_toStr(ec))));

   switch (ec->moduleType) {
      case MT_INDICATOR:
      case MT_SCRIPT:
         if (ec != g_contextChains[pid][1]) return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "%s::%s::deinit()  illegal parameter ec=%d (doesn't match the stored main context=%d)  ec=%s", ec->programName, ec->moduleName, ec, g_contextChains[pid][1], EXECUTION_CONTEXT_toStr(ec))));
         ec_SetCoreFunction(ec, (CoreFunction)NULL);                 // set main and master context to NULL
         g_contextChains[pid][1] = NULL;                             // mark main context as released
         break;

      case MT_EXPERT:
         if (ec != g_contextChains[pid][1]) return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "%s::%s::deinit()  illegal parameter ec=%d (not stored as main context=%d)  ec=%s", ec->programName, ec->moduleName, ec, g_contextChains[pid][1], EXECUTION_CONTEXT_toStr(ec))));

         if (ec->testing) {
            //debug("%s::deinit()  leaving tester, ec=%s", ec->programName, EXECUTION_CONTEXT_toStr(ec));
         }

         ec_SetCoreFunction(ec, (CoreFunction)NULL);                 // set main and master context to NULL
         if (ec->uninitReason!=UR_CHARTCHANGE && ec->uninitReason!=UR_PARAMETERS && ec->uninitReason!=UR_ACCOUNT)
            g_contextChains[pid][1] = NULL;                          // mark main context as released if not in init cycle
         break;

      case MT_LIBRARY:
         // TODO: This could be kind of critical as the main module already called LeaveContext() before.
         ec_SetCoreFunction(ec, (CoreFunction)NULL);                 // set library context to NULL
         break;

      default:
         return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context, ec.moduleType: %s", ModuleTypeToStr(ec->moduleType))));
   }

   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Push the specififed ContextChain onto the end of the list of managed chains.
 *
 * @param  ContextChain& chain
 *
 * @return uint - the list index where the ContextChain is stored
 *
 * TODO: Don't store a copy of the passed instance. Instead store a pointer.
 */
uint WINAPI ContextChainsPush(ContextChain& chain) {
   int index = EMPTY;

   if (!TryEnterCriticalSection(&g_terminalMutex)) {
      debug("waiting to aquire lock on: g_terminalMutex");
      EnterCriticalSection(&g_terminalMutex);
   }
   g_contextChains.push_back(chain);                     // TODO: prevent push_back() from creating a copy
   index = g_contextChains.size()-1;
   LeaveCriticalSection(&g_terminalMutex);

   return(index);
}


/**
 * Find the index of the current thread in the list of known threads. If the thread is not found it is added to the list.
 *
 * @return int - thread index or EMPTY (-1) in case of errors
 */
int WINAPI GetCurrentThreadIndex() {
   DWORD currentThread = GetCurrentThreadId();

   // look-up the current thread
   uint size = g_threads.size();
   for (uint i=0; i < size; i++) {
      if (g_threads[i] == currentThread)                          // thread found
         return(i);
   }

   // thread not found
   if (!TryEnterCriticalSection(&g_terminalMutex)) {
      debug("waiting to aquire lock on: g_terminalMutex");
      EnterCriticalSection(&g_terminalMutex);
   }
   g_threads        .push_back(currentThread);                    // add current thread to the list
   g_threadsPrograms.push_back(0);                                // add empty program index of 0 (zero) to the list
   uint index = g_threads.size() - 1;

   if (index > 511) debug("thread %d added (size=%d)", currentThread, index+1);
   LeaveCriticalSection(&g_terminalMutex);

   return(index);
}


/**
 * Link the specified MQL program to the current thread.
 *
 * @param  uint pid - MQL program id
 *
 * @return int - index of the current thread in the list of known threads or EMPTY (-1) in case of errors
 */
int WINAPI LinkProgramToCurrentThread(uint pid) {
   if ((int)pid < 1) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d", pid)));

   int index = GetCurrentThreadIndex();
   if (index < 0) return(EMPTY);

   g_threadsPrograms[index] = pid;                       // update the thread's last executed program
   if (IsUIThread())
      g_lastUIThreadProgram = pid;                       // update lastUIThreadProgram if the thread is the UI thread

   return(index);
}


/**
 * Find the first matching and still active indicator with a released main EXECUTION_CONTEXT in memory.
 *
 * @param  HWND               hChart - correct value of WindowHandle()
 * @param  const char*        name   - indicator name
 * @param  UninitializeReason reason
 *
 * @return int - the found indicator's program id or NULL if no such indicator was found;
 *               EMPTY (-1) in case of errors
 *
 * Notes:
 * ------
 * Limbo (latin limbus, edge or boundary, referring to the "edge" of Hell) is a speculative idea about the afterlife condition
 * of those who die in original sin without being assigned to the Hell of the Damned. Very hard to escape from.
 *
 * In MetaTrader the memory allocated for global indicator variables (static and non-static, covering the EXECUTION_CONTEXT)
 * is immediately released after the indicator leaves Indicator::deinit(). On re-entry at Indicator::init() new memory is
 * allocated and global variables are initialized with zero. It's for this reason the indicator doesn't keep state over init
 * cycles.
 *
 * Between Indicator::deinit() and Indicator::init() the indicator enters the state of "limbo", an uncharted territory known
 * only to the developers at MetaQuotes. The framework keeps state in the master execution context which acts as a backup of
 * the then lost main execution context. On re-entry the master context is copied back to the newly allocated main context
 * and thus global state of the indicator can survive. Voilà, it crossed the afterlife.
 */
int WINAPI FindIndicatorInLimbo(HWND hChart, const char* name, UninitializeReason reason) {
   if (hChart) {
      EXECUTION_CONTEXT* master;
      int size=g_contextChains.size(), uiThreadId=GetUIThreadId();

      for (int i=1; i < size; i++) {                                 // index[0] is never occupied
         master = g_contextChains[i][0];

         if (master->threadId == uiThreadId) {
            if (master->hChart == hChart) {
               if (master->programType == MT_INDICATOR) {
                  if (StrCompare(master->programName, name)) {
                     if (master->uninitReason == reason) {
                        if (master->coreFunction == NULL) {          // limbo = init cycle
                           //debug("first %s indicator found in limbo: pid=%d", name, master->pid);
                           return(master->pid);
                        }
                        //else debug("i=%d  %s  coreFunction not NULL:  master=%s", i, name, CoreFunctionToStr(master->coreFunction));
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

   //debug("no matching %s indicator found in limbo: hChart=%d  uninitReason=%s", name, hChart, UninitializeReasonToStr(reason))
   return(NULL);
}


/**
 * Find the chart of the current program and return its window handle. Replacement for the broken MQL function WindowHandle().
 * Also returns the correct window handle when the MQL function fails.
 *
 * Must be called only in SyncMainContext_init(), after that use the window handle stored in the program's EXECUTION_CONTEXT.
 *
 * @param  HWND               hChart       - value of WindowHandle() as returned by the terminal (possibly not yet set)
 * @param  EXECUTION_CONTEXT* sec          - super context as managed by the terminal (memory possibly already released)
 * @param  ModuleType         moduleType   - module type
 * @param  char*              symbol       - current symbol
 * @param  uint               timeframe    - current timeframe
 * @param  BOOL               isTesting    - value of IsTesting() as returned by the terminal (possibly incorrect)
 * @param  BOOL               isVisualMode - value of IsVisualMode() as returned by the terminal (possibly incorrect)
 *
 * @return HWND - Window handle or NULL if the program runs in the Strategy Tester with VisualMode=Off;
 *                INVALID_HWND (-1) in case of errors.
 */
HWND WINAPI FindWindowHandle(HWND hChart, const EXECUTION_CONTEXT* sec, ModuleType moduleType, const char* symbol, uint timeframe, BOOL isTesting, BOOL isVisualMode) {
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

   HWND hWndMain = GetTerminalMainWindow();
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

      size_t bufferSize = MAX_CHARTDESCRIPTION_LENGTH + 1;
      char* chartDescription = (char*)alloca(bufferSize);            // on the stack
      size_t chars = GetChartDescription(symbol, timeframe, chartDescription, bufferSize);
      if (!chars) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "=> GetChartDescription()")));

      bufferSize = 128;
      char* title = (char*)alloca(bufferSize);
      int id = INT_MAX;
      SetLastError(NO_ERROR);

      while (hWndChild) {                                            // iterate over all child windows
         size_t titleLen = GetWindowText(hWndChild, title, bufferSize);
         if (!titleLen) if (int error=GetLastError()) return(_INVALID_HWND(error(ERR_WIN32_ERROR+error, "GetWindowText()")));

         if (titleLen) {
            if (titleLen >= bufferSize-1) {
               bufferSize <<= 1;
               title = (char*)alloca(bufferSize);
               continue;
            }
            if (StrEndsWith(title, " (offline)"))
               title[titleLen-10] = 0;
            if (StrCompare(title, chartDescription)) {               // find all matching windows
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
      return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter moduleType: %d", moduleType)));
   }


   // (4) Das gefundene Chartfenster hat genau ein Child (AfxFrameOrView), welches das gesuchte MetaTrader-Handle ist.
   hChart = GetWindow(hChartWindow, GW_CHILD);
   if (!hChart) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "no MetaTrader chart window inside of last MDIClient child window %p found", hChartWindow)));

   return(hChart);
}


/**
 * Resolve a program's true init() reason.
 *
 * @param  EXECUTION_CONTEXT* ec             - an MQL program's main module execution context (possibly still empty)
 * @param  EXECUTION_CONTEXT* sec            - super context as managed by the terminal (memory possibly already released)
 * @param  ProgramType        programType    - program type
 * @param  char*              programName    - program name (with or without filepath depending on the terminal version)
 * @param  UninitializeReason uninitReason   - value of UninitializeReason() as returned by the terminal
 * @param  char*              symbol         - current symbol
 * @param  BOOL               isTesting      - value of IsTesting() as returned by the terminal (possibly incorrect)
 * @param  BOOL               isVisualMode   - value of IsVisualMode() as returned by the terminal (possibly incorrect)
 * @param  HWND               hChart         - correct WindowHandle() value
 * @param  int                droppedOnChart - value of WindowOnDropped() as returned by the terminal (possibly incorrect)
 * @param  int                droppedOnPosX  - value of WindowXOnDropped() as returned by the terminal (possibly incorrect)
 * @param  int                droppedOnPosY  - value of WindowYOnDropped() as returned by the terminal (possibly incorrect)
 * @param  uint&              prevIndPid     - variable receiving the previous pid of an indicator in init cycle
 *
 * @return InitializeReason - init reason or NULL in case of errors
 */
InitializeReason WINAPI GetInitReason(EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec, ProgramType programType, const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL isTesting, BOOL isVisualMode, HWND hChart, int droppedOnChart, int droppedOnPosX, int droppedOnPosY, uint& prevIndPid) {

   if (programType == PT_INDICATOR) return(GetInitReason_indicator(ec, sec, programName, uninitReason, symbol, isTesting, isVisualMode, hChart, droppedOnChart, prevIndPid));
   if (programType == PT_EXPERT)    return(GetInitReason_expert   (ec,      programName, uninitReason, symbol, isTesting, droppedOnPosX, droppedOnPosY));
   if (programType == PT_SCRIPT)    return(GetInitReason_script   (ec,      programName,                                  droppedOnPosX, droppedOnPosY));

   return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter programType: %d (unknown)", programType));
}


/**
 * Resolve an indicator's true init() reason.
 *
 * @param  EXECUTION_CONTEXT* ec             - an MQL program's main module execution context (possibly still empty)
 * @param  EXECUTION_CONTEXT* sec            - super context as managed by the terminal (memory possibly already released)
 * @param  char*              programName    - program name (with or without filepath depending on the terminal version)
 * @param  UninitializeReason uninitReason   - value of UninitializeReason() as returned by the terminal
 * @param  char*              symbol         - current symbol
 * @param  BOOL               isTesting      - value of IsTesting() as returned by the terminal (possibly incorrect)
 * @param  BOOL               isVisualMode   - value of IsVisualMode() as returned by the terminal (possibly incorrect)
 * @param  HWND               hChart         - correct WindowHandle() value
 * @param  int                droppedOnChart - value of WindowOnDropped() as returned by the terminal (possibly incorrect)
 * @param  uint&              prevIndPid     - variable receiving the previous pid of an indicator in init cycle
 *
 * @return InitializeReason - init reason or NULL in case of errors
 */
InitializeReason WINAPI GetInitReason_indicator(EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec, const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL isTesting, BOOL isVisualMode, HWND hChart, int droppedOnChart, uint& prevIndPid) {
   /*
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
      int pid = ec->pid;
      if (pid) {
         isProgramNew = !g_contextChains[pid][0]->ticks;             // im Master-Context nachschauen
      }
      else {
         pid = FindIndicatorInLimbo(hChart, programName, uninitReason);
         if (pid < 0) return((InitializeReason)NULL);
         prevIndPid   =  pid;
         isProgramNew = !pid;
      }
      if (isProgramNew) return(IR_USER      );                       // erste Parameter-Eingabe eines manuell neu hinzugefügten Indikators
      else              return(IR_PARAMETERS);                       // Parameter-Wechsel eines vorhandenen Indikators
   }


   // (2) UR_CHARTCHANGE
   if (uninitReason == UR_CHARTCHANGE) {
      // innerhalb iCustom(): nie
      if (sec)               return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (SuperContext=%p  Testing=%d  VisualMode=%d  UIThread=%d  build=%d)", UninitializeReasonToStr(uninitReason), sec, isTesting, isVisualMode, isUIThread, build));
      // außerhalb iCustom(): bei Symbol- oder Timeframe-Wechsel eines vorhandenen Indikators, kein Input-Dialog
      int pid = ec->pid;
      if (!pid) {
         pid = FindIndicatorInLimbo(hChart, programName, uninitReason);
         if (pid <= 0) return((InitializeReason)(pid < 0 ? NULL : error(ERR_RUNTIME_ERROR, "no %s indicator found in limbo during %s", programName, UninitializeReasonToStr(uninitReason))));
         prevIndPid = pid;
      }
      char* masterSymbol = g_contextChains[pid][0]->symbol;
      if (StrCompare(masterSymbol, symbol)) return(IR_TIMEFRAMECHANGE);
      else                                  return(IR_SYMBOLCHANGE   );
   }


   // (3) UR_UNDEFINED
   if (uninitReason == UR_UNDEFINED) {
      // außerhalb iCustom(): je nach Umgebung
      if (!sec) {
         if (build < 654)         return(IR_TEMPLATE);               // wenn Template mit Indikator geladen wird (auch bei Start und im Tester bei VisualMode=On|Off), kein Input-Dialog
         if (droppedOnChart >= 0) return(IR_TEMPLATE);
         else                     return(IR_USER    );               // erste Parameter-Eingabe eines manuell neu hinzugefügten Indikators, Input-Dialog
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
 * Resolve an expert's true init() reason.
 *
 * @param  EXECUTION_CONTEXT* ec            - an MQL program's main module execution context (possibly still empty)
 * @param  char*              programName   - program name (with or without filepath depending on the terminal version)
 * @param  UninitializeReason uninitReason  - value of UninitializeReason() as returned by the terminal
 * @param  char*              symbol        - current symbol
 * @param  BOOL               isTesting     - value of IsTesting() as returned by the terminal
 * @param  int                droppedOnPosX - value of WindowXOnDropped() as returned by the terminal
 * @param  int                droppedOnPosY - value of WindowYOnDropped() as returned by the terminal
 *
 * @return InitializeReason - init reason or NULL in case of errors
 */
InitializeReason WINAPI GetInitReason_expert(EXECUTION_CONTEXT* ec, const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL isTesting, int droppedOnPosX, int droppedOnPosY) {
   uint build = GetTerminalBuild();
   //debug("uninitReason=%s  testing=%d  droppedX=%d  droppedY=%d  build=%d", UninitReasonToStr(uninitReason), isTesting, droppedOnPosX, droppedOnPosY, build);


   // UR_PARAMETERS                                      // input parameters changed
   if (uninitReason == UR_PARAMETERS) {
      return(IR_PARAMETERS);
   }

   // UR_CHARTCHANGE                                     // chart symbol or period changed
   if (uninitReason == UR_CHARTCHANGE) {
      int pid = ec->pid;
      if (!pid) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s (ec.pid=0  Testing=%d  build=%d)", UninitializeReasonToStr(uninitReason), isTesting, build));
      char* masterSymbol = g_contextChains[pid][0]->symbol;
      if (StrCompare(masterSymbol, symbol)) return(IR_TIMEFRAMECHANGE);
      else                                  return(IR_SYMBOLCHANGE);
   }

   // UR_RECOMPILE                                       // re-loaded after recompilation
   if (uninitReason == UR_RECOMPILE) {
      return(IR_RECOMPILE);
   }

   // UR_CHARTCLOSE                                      // loaded into an existing chart after new template was loaded
   if (uninitReason == UR_CHARTCLOSE) {                  // (old builds only, corresponds to UR_TEMPLATE of new builds)
      if (build > 509) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (Testing=%d  build=%d)", UninitializeReasonToStr(uninitReason), isTesting, build));
      return(IR_USER);
   }

   // UR_UNDEFINED                                       // loaded into a new chart (also at terminal start and in Strategy Tester)
   if (uninitReason == UR_UNDEFINED) {
      if (isTesting)          return(IR_USER);
      if (droppedOnPosX >= 0) return(IR_USER);           // TODO: It is rare but possible to manually load an expert with droppedOnPosX = -1.
      HWND hWndDlg = FindInputDialog(PT_EXPERT, programName);
      if (hWndDlg)            return(IR_TERMINAL_FAILURE);
      else                    return(IR_TEMPLATE);
   }

   // UR_REMOVE                                          // loaded into an existing chart after a previously loaded one was removed manually
   if (uninitReason == UR_REMOVE) {
      if (droppedOnPosX >= 0) return(IR_USER);           // TODO: It is rare but possible to manually load an expert with droppedOnPosX = -1.
      HWND hWndDlg = FindInputDialog(PT_EXPERT, programName);
      if (hWndDlg)            return(IR_TERMINAL_FAILURE);
      else                    return(IR_TEMPLATE);
   }

   // UR_TEMPLATE                                        // loaded into an existing chart after a previously loaded one was removed by LoadTemplate()
   if (uninitReason == UR_TEMPLATE) {
      if (build <= 509)       return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (Testing=%d  build=%d)", UninitializeReasonToStr(uninitReason), isTesting, build));
      if (droppedOnPosX >= 0) return(IR_USER);           // TODO: It is rare but possible to manually load an expert with droppedOnPosX = -1.
      HWND hWndDlg = FindInputDialog(PT_EXPERT, programName);
      if (hWndDlg)            return(IR_TERMINAL_FAILURE);
      else                    return(IR_TEMPLATE);
   }

   switch (uninitReason) {
      case UR_ACCOUNT:
      case UR_CLOSE:
      case UR_INITFAILED:
         return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s (Testing=%d  build=%d)", UninitializeReasonToStr(uninitReason), isTesting, build));
   }
   return((InitializeReason)error(ERR_ILLEGAL_STATE, "unknown UninitializeReason %d (Testing=%d  build=%d)", uninitReason, isTesting, build));
}


/**
 * Resolve a script's true init() reason.
 *
 * @param  EXECUTION_CONTEXT* ec            - an MQL program's main module execution context (possibly still empty)
 * @param  char*              programName   - program name (with or without filepath depending on the terminal version)
 * @param  int                droppedOnPosX - value of WindowXOnDropped() as returned by the terminal
 * @param  int                droppedOnPosY - value of WindowYOnDropped() as returned by the terminal
 *
 * @return InitializeReason - init reason or NULL in case of errors
 */
InitializeReason WINAPI GetInitReason_script(EXECUTION_CONTEXT* ec, const char* programName, int droppedOnPosX, int droppedOnPosY) {
   return(IR_USER);
}


/**
 * Resolve the custom log file of the program (if any).
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char*
 */
const char* WINAPI Program_CustomLogFile(const EXECUTION_CONTEXT* ec) {
   if (ec->superContext)
      return(ec->superContext->customLogFile);                       // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR:
      case PT_EXPERT:
      case PT_SCRIPT:
         return(NULL);
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid value ec.programType: %d", ec->programType));
}


/**
 * Whether or not logging is activated for the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI Program_IsLogging(const EXECUTION_CONTEXT* ec) {
   if (ec->superContext)
      return(ec->superContext->logging);                             // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR:
      case PT_EXPERT: //return(IsLogging());                         // TODO: implement IsLogging()
      case PT_SCRIPT: return(TRUE);
   }
   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType: %d", ec->programType));
}


/**
 * Whether or not the program is executed in the Strategy Tester with Optimization=On.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isOptimization - MQL::IsOptimization() status as passed by the terminal
 *
 * @return BOOL - real IsOptimization() status
 */
BOOL WINAPI Program_IsOptimization(const EXECUTION_CONTEXT* ec, BOOL isOptimization) {
   if (ec->superContext)
      return(ec->superContext->optimization);                        // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR:
      case PT_EXPERT:
      case PT_SCRIPT: return(isOptimization);
   }
   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType: %d", ec->programType));
}


/**
 * Whether or not the program with the specified pid is a partially initialized expert in tester, matching the passed name.
 *
 * @param  char* name - program name
 *
 * @return BOOL
 */
BOOL WINAPI Program_IsPartialTest(uint pid, const char* name) {
   if (g_contextChains.size() > pid) {
      ContextChain& chain = g_contextChains[pid];

      if (chain.size() > 1) {
         EXECUTION_CONTEXT* master = chain[0];
         EXECUTION_CONTEXT* main   = chain[1];

         if (master->programType==PT_EXPERT && master->testing && master->coreFunction==CF_INIT) {
            return(!main && StrCompare(master->programName, name));        // name comparison last
         }
      }
   }
   return(FALSE);
}


/**
 * Whether or not the program is executed in the Strategy Tester or on a Strategy Tester chart.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isTesting - MQL::IsTesting() status as passed by the terminal (possibly wrong)
 *
 * @return BOOL - real IsTesting() status
 */
BOOL WINAPI Program_IsTesting(const EXECUTION_CONTEXT* ec, BOOL isTesting) {
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
         return(StrEndsWith(title, "(visual)"));                     // all remaining cases according to "(visual)" in title
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
            return(StrEndsWith(title, "(visual)"));
         }
         return(error(ERR_ILLEGAL_STATE, "script without a chart:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
      }
   }

   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType: %d", ec->programType));
}


/**
 * Whether or not the program is executed in the Strategy Tester or on a Strategy Tester chart with VisualMode=On.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isVisualMode - MQL::IsVisualMode() status as passed by the terminal (possibly wrong)
 *
 * @return BOOL - real IsVisualMode() status
 */
BOOL WINAPI Program_IsVisualMode(const EXECUTION_CONTEXT* ec, BOOL isVisualMode) {
   if (ec->superContext)
      return(ec->superContext->visualMode);                          // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR: return(ec->testing && ec->hChart);
      case PT_EXPERT:    return(isVisualMode);
      case PT_SCRIPT:    return(ec->testing);                        // scripts can only run on visible charts
   }
   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType: %d", ec->programType));
}
