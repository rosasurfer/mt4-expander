#include "Expander.h"


std::vector<pec_vector> contextChains  (64);                // alle Context-Chains (Index = ProgramID)
std::vector<DWORD>      threads        (64);                // ID's der bekannten Non-UI-Threads
std::vector<uint>       threadsPrograms(64);                // ID's des vom jeweiligen Thread zuletzt ausgeführten MQL-Programms
uint                    lastUIThreadProgram = 0;            // letztes vom UI-Thread ausgeführtes MQL-Programm
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
 * Die EXECUTION_CONTEXTe dienen dem Datenaustausch zwischen mehreren MQL-Programmen, zwischen einzelnen Modulen desselben Programms
 * und zwischen einem Programm und der DLL. Jedes MQL-Modul verfügt über einen eigenen Kontext, alle Kontexte eines MQL-Programms
 * bilden gemeinsam eine Context-Chain. An erster Stelle einer Context-Chain liegt der Master-Context, der in der DLL verwaltet wird.
 * An zweiter Stelle liegt der Context des MQL-Hauptmodules (Expert, Script oder Indikator). Alle weiteren Contexte einer Chain sind
 * Library-Contexte. Über die Kontexte werden wie folgt Daten ausgetauscht:
 *
 *  (1) Datenaustausch vom Hauptmodul zu den Library-Modulen:
 *
 *  (2) Datenaustausch von den Library-Modulen zum Hauptmodul:
 *
 *  (3) Datenaustausch von der DLL zum Hauptmodul:
 *
 *  (4) Datenaustausch vom einem Hauptmodul zu einem anderen Hauptmodul:
 *
 * Kontextgültigkeit: Der Master-Context einer Chain ist immer gültig. Alle anderen Kontexte der Chain können je nach Modul-Typ und
 * Situation ungültig bzw. der Speicher nicht verfügbar sein (dazu später mehr). Von einem MQL-Modul darf generell nur auf den eigenen
 * und auf den Master-Context zugegriffen werden. Ein Zugriff auf den Hauptkontext aus einer Library und umgekehrt ist nur in
 * Ausnahmefällen möglich.
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
 *   den zu dem Zeitpunkt ungültigen Hauptmodulkontext zugegriffen werden.
 * • Nach Recompilation oder Crash einer Library wird der Speicherblock ihres Kontexts ungültig und auf ihn darf ebenfalls nicht mehr
 *   zugegriffen werden.
 *
 *
 *  init-Cycle eines Indikators:
 *  --- erstes Laden: init() --------------------------------------------------------------------------------------------------------
 *  Indicator::init()              programId=0  creating new chain             set: programId=1
 *  Indicator::library1::init()    programId=0  loaded by Indicator            set: programId=1
 *  Indicator::library2::init()    programId=0  loaded by Indicator            set: programId=1
 *  Indicator::library3::init()    programId=0  loaded by Indicator::library1  set: programId=1
 *  --- deinit() --------------------------------------------------------------------------------------------------------------------
 *  Indicator::deinit()            programId=1  Indikator zuerst
 *  Indicator::library1::deinit()  programId=1  dann die Libraries
 *  Indicator::library3::deinit()  programId=1  hierarchisch, nicht in Ladereihenfolge
 *  Indicator::library2::deinit()  programId=1
 *  --- init() ----------------------------------------------------------------------------------------------------------------------
 *  Indicator::library1::init()    programId=1  Libraries zuerst (hierarchisch)
 *  Indicator::library3::init()    programId=1  neues Symbol/neuer Timeframe tauchen auf
 *  Indicator::library2::init()    programId=1
 *  Indicator::init()              programId=0  zum Schluß der Indikator       set: programId=1
 *  ---------------------------------------------------------------------------------------------------------------------------------
 */
BOOL WINAPI SyncMainExecutionContext(EXECUTION_CONTEXT* ec, ModuleType moduleType, const char* moduleName, RootFunction rootFunction, UninitializeReason reason, const char* symbol, int period) {
   if ((uint)ec         < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)moduleName < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter moduleName = 0x%p (not a valid pointer)", moduleName));
   if ((uint)symbol     < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (period <= 0)                          return(error(ERR_INVALID_PARAMETER, "invalid parameter period = %d", period));

   //
   // Programmablauf
   // --------------
   // (1) Wenn keine ProgramID gesetzt ist, prüfen, ob Programm ein Indikator im Init-Cycle ist.
   //     • wenn Programm ein Indikator im Init-Cycle ist (immer im UI-Thread)
   //       - Indikator-Context aus Master-Context restaurieren
   //     • wenn Programm kein Indikator im Init-Cycle ist
   //       - neue Context-Chain erzeugen
   //       - neuen Master-Context erzeugen
   //       - Master- und Hauptkontext in der Chain speichern
   //       - ProgramID generieren und diese dem Master- und Hauptkontext zuweisen
   //
   // (2) Im Master- und Hauptkontext bei jedem Aufruf zu aktualisieren:
   //     • RootFunction
   //     • UninitializeReason
   //
   // (3) Im Master- und Hauptkontext nur beim Aufruf aus init() zu aktualisieren:
   //     • Symbol
   //     • Timeframe
   //     • alle übrigen übergebenen Werte, sofern nicht schon gesetzt
   //
   // (4) Das vom aktuellen Thread zuletzt ausgeführte Programm (also das aktuelle) in global threadsPrograms[] speichern.
   //
   // (5) Ist der aktuelle Thread der UI-Thread, das aktuelle Programm zusätzlich in global lastUIThreadProgram speichern.
   //
   EXECUTION_CONTEXT* master = NULL;


   // (1) Wenn keine ProgramID gesetzt ist, prüfen, ob Programm ein Indikator im Init-Cycle ist.
   if (!ec->programId) {
      BOOL lastUIThreadProgramMarked = (IsUIThread() && lastUIThreadProgram && !contextChains[lastUIThreadProgram][1]);
      uint initReason           = NULL;// = InitReason();
      BOOL indicatorInInitCycle = (lastUIThreadProgramMarked
                               || (moduleType==MT_INDICATOR && rootFunction==RF_INIT && (initReason==INIT_REASON_PARAMETERS || initReason==INIT_REASON_SYMBOLCHANGE || initReason==INIT_REASON_TIMEFRAMECHANGE)));

      if (indicatorInInitCycle) {
         // (1.1) Programm ist Indikator im Init-Cycle (immer im UI-Thread)
         //   - Indikator-Context aus Master-Context restaurieren
         master = contextChains[lastUIThreadProgram][0];
         *ec = *master;                                                 // Master-Context übernehmen
         contextChains[lastUIThreadProgram][1] = ec;                    // Context als Hauptkontext speichern

         debug("programId=0  lastUIThreadProgramMarked=%d  indicator in init-cycle", lastUIThreadProgramMarked); if (0) debug("thread=%d UI  %s::%s()  init-cycle  programId=0  reason=%s  lastUIThreadProgram=%d", GetCurrentThreadId(), moduleName, RootFunctionName(rootFunction), UninitializeReasonToStr(reason), lastUIThreadProgram);
      }
      else {
         // (1.2) Programm ist kein Indikator im Init-Cycle
         //   - neue Context-Chain erzeugen
         //   - neuen Master-Context erzeugen
         //   - Master- und Hauptkontext in der Chain speichern
         //   - ProgramID generieren und diese Master- und Hauptkontext zuweisen
         master  = new EXECUTION_CONTEXT;                               // neuen Master-Context erzeugen
         *master = *ec;                                                 // Hauptkontext in Master-Context kopieren
         pec_vector chain;                                              // neue Context-Chain erzeugen
         chain.reserve(8);
         chain.push_back(master);                                       // Master- und Hauptkontext in der Chain speichern
         chain.push_back(ec);

         EnterCriticalSection(&terminalLock);
         contextChains.push_back(chain);                                // Chain in der Chain-Liste speichern
         uint size = contextChains.size();                              // contextChains.size ist immer > 1 (index[0] bleibt frei)
         master->programId = ec->programId = size-1;                    // Index = neue ProgramID dem Master- und Hauptkontext zuweisen
         LeaveCriticalSection(&terminalLock);

         debug("programId=0  lastUIThreadProgramMarked=%d  not an indicator in init-cycle", lastUIThreadProgramMarked); if (0) debug("thread=%d %s  %s::%s()  reason=%s  creating new chain for ec=0x%p with sec=0x%p  =>  size(chains)=%d  programId=%d", GetCurrentThreadId(), (IsUIThread() ? "UI":"  "), moduleName, RootFunctionName(rootFunction), UninitializeReasonToStr(reason), ec, ec->superContext, size, ec->programId);
      }
   }


   // (2) Im Master- und Hauptkontext bei jedem Aufruf zu aktualisieren:
   //     - RootFunction
   //     - UninitializeReason
   !master && (master=contextChains[ec->programId][0]);
   master->rootFunction       = ec->rootFunction       = rootFunction;  // ohne Validierung, um im Tester ein paar CPU-Cycles sparen
   master->uninitializeReason = ec->uninitializeReason = reason;


   // (3) Im Master- und Hauptkontext beim Aufruf aus init() zu aktualisieren:
   //     - Symbol
   //     - Timeframe
   //     - alle übrigen übergebenen Werte, sofern nicht schon gesetzt
   if (rootFunction == RF_INIT) {
      ec_SetSymbol   (master, ec_SetSymbol   (ec, symbol));
      ec_SetTimeframe(master, ec_SetTimeframe(ec, period));
                                                                        // im Hauptmodul gilt: ModuleName/Type = ProgramName/Type
      if (!ec->programType)  ec_SetProgramType(master, ec_SetProgramType(ec, (ProgramType)moduleType));
      if (!*ec->programName) ec_SetProgramName(master, ec_SetProgramName(ec,              moduleName));
      if (!ec->moduleType)   ec_SetModuleType (master, ec_SetModuleType (ec,              moduleType));
      if (!*ec->moduleName)  ec_SetModuleName (master, ec_SetModuleName (ec,              moduleName));
   }


   // (4) Das vom aktuellen Thread zuletzt ausgeführte Programm (also das aktuelle) in global threadsPrograms[] speichern.
   DWORD currentThread = GetCurrentThreadId();
   int currentThreadIndex=-1, size=threads.size();
   for (int i=0; i < size; i++) {                                    // aktuellen Thread suchen
      if (threads[i] == currentThread) {                             // gefunden
         currentThreadIndex = i;
         threadsPrograms[i] = ec->programId;                         // zuletzt ausgeführtes Programm aktualisieren
         break;
      }
   }
   if (currentThreadIndex == -1) {                                   // nicht gefunden: Thread und Programm zur Verwaltung hinzufügen
      EnterCriticalSection(&terminalLock);
      threads        .push_back(currentThread);
      threadsPrograms.push_back(ec->programId);
      if (threads.size() > 128) debug("thread %d added (size=%d)", currentThread, threads.size());
      LeaveCriticalSection(&terminalLock);
   }


   // (5) Ist der aktuelle Thread der UI-Thread, das aktuelle Programm zusätzlich in global lastUIThreadProgram speichern.
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
 * @param  int                period       - aktuelle Chart-Periode
 *
 * @return BOOL - Erfolgsstatus
 *
 *
 * Notes:
 * ------
 * Ist in Library::init() der übergebene Context bereits initialisiert (ec.programId ist gesetzt), befindet sich das Programm in
 * einem init()-Cycle.
 */
BOOL WINAPI SyncLibExecutionContext(EXECUTION_CONTEXT* ec, const char* moduleName, RootFunction rootFunction, const char* symbol, int period) {
   if ((uint)ec         < MIN_VALID_POINTER)             return(error(ERR_INVALID_PARAMETER, "invalid parameter ec = 0x%p (not a valid pointer)", ec));
   if ((uint)moduleName < MIN_VALID_POINTER)             return(error(ERR_INVALID_PARAMETER, "invalid parameter moduleName = 0x%p (not a valid pointer)", moduleName));
   if (rootFunction!=RF_INIT && rootFunction!=RF_DEINIT) return(error(ERR_INVALID_PARAMETER, "illegal library parameter rootFunction = %s", RootFunctionToStr(rootFunction)));
   if ((uint)symbol     < MIN_VALID_POINTER)             return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (period <= 0)                                      return(error(ERR_INVALID_PARAMETER, "invalid parameter period = %d", period));

   ec->rootFunction = rootFunction;                         // Context-Daten aktualisieren


   // (1) Index des aktuellen Threads ermitteln
   DWORD currentThread = GetCurrentThreadId();
   int currentThreadIndex=-1, size=threads.size();
   for (int i=0; i < size; i++) {                           // Aufwärts, damit der UI-Thread (Index 0 oder 1) schnellstmöglich gefunden wird.
      if (threads[i] == currentThread) {
         currentThreadIndex = i;                            // Thread gefunden: Index merken
         break;
      }
   }
   if (currentThreadIndex < 0) return(error(ERR_ILLEGAL_STATE, "current thread %d not in known threads", currentThread));


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
            if (0) debug("%s::%s::init()  programId=%d  init cycle,  setting main-ec to NULL, was chain[1]=%p  bak=%p", ec->programName, moduleName, ec->programId, chain[1], chain[0]);
            chain[1] = NULL;                                // Hauptkontext auf NULL setzen
            ec_SetSymbol(chain[0], symbol);                 // Master-Kontext aktualisieren. Dies ist der früheste Zeitpunkt, an dem das neue
            chain[0]->timeframe = period;                   // Symbol/der neue Timeframe bekannt sind. Sie werden nur hier gebraucht.
         }
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
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  %s";
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
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  WARN: %s";
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
   char* locationFormat = "MT4Expander::%s%s::%s(%d)  ERROR: %s  [%s]";
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
         // Wir sind in einer Callback-Funktion oder im letzten im UI-Thread ausgeführten Indikator und dort u.U. in einer Library
         // oder einem via iCustom() geladenen weiteren Indikator.
         //
         // Vorsicht: Der Hauptkontext des letzten Root-Programms kann ungültig sein.
      }
      else {
         // Wir sind im Expert oder Script des Threads und dort u.U. in einer Library oder einem via iCustom() geladenen weiteren Indikator.
         // Auf den Hauptkontext des Root-Programms kann lesend/schreibend zugegriffen werden.
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
