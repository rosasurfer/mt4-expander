#include "expander.h"
#include "lib/conversion.h"
#include "lib/format.h"
#include "lib/string.h"
#include "struct/xtrade/ExecutionContext.h"


extern std::vector<ContextChain> g_contextChains;                    // all context chains (i.e. MQL programs, index = program id)


/**
 * Return an EXECUTION_CONTEXT's MQL program index.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - program index (starting from 1)
 */
uint WINAPI ec_ProgramIndex(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programIndex);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's MQL program type.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ProgramType
 */
ProgramType WINAPI ec_ProgramType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programType);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's MQL program name.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - program name
 */
const char* WINAPI ec_ProgramName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programName);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's MQL module type.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return ModuleType
 */
ModuleType WINAPI ec_ModuleType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->moduleType);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's MQL module name.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - module name
 */
const char* WINAPI ec_ModuleName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->moduleName);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's program launch type.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return LaunchType
 */
LaunchType WINAPI ec_LaunchType(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->launchType);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's root function.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 */
RootFunction WINAPI ec_RootFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->rootFunction);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not an EXECUTION_CONTEXT's program is currently in an init cycle.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_InitCycle(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->initCycle);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's initialization reason.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return InitializeReason
 */
InitializeReason WINAPI ec_InitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->initReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's uninitialization reason.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI ec_UninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->uninitReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's init flags.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - init flags
 */
DWORD WINAPI ec_InitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->initFlags);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's deinit flags.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - deinit flags
 */
DWORD WINAPI ec_DeinitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->deinitFlags);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's symbol.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - symbol
 */
const char* WINAPI ec_Symbol(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->symbol);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's timeframe.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - timeframe
 */
uint WINAPI ec_Timeframe(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->timeframe);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a program's "Digits" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - digits
 */
uint WINAPI ec_Digits(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->digits);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a program's "Point" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return double - point
 */
double WINAPI ec_Point(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->point);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's bars value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - bars
 */
uint WINAPI ec_Bars(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->bars);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's number of times a program's start() function was called.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint
 */
uint WINAPI ec_Ticks(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->ticks);
   #pragma EXPANDER_EXPORT
}



/**
 * Return an EXECUTION_CONTEXT's previous tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - server time
 */
datetime WINAPI ec_PreviousTickTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->previousTickTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's current tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - server time
 */
datetime WINAPI ec_CurrentTickTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->currentTickTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's current bid price.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return double - bid price
 */
double WINAPI ec_Bid(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->bid);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's current ask price.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return double - ask price
 */
double WINAPI ec_Ask(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->ask);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not an experts input parameter "EA.ExtendedReporting" is activated.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_ExtReporting(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->extReporting);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not an experts input parameter "EA.RecordEquity" is activated.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_RecordEquity(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->recordEquity);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not a program is running in the tester or on a tester chart.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_Testing(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->testing);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten VisualMode-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_VisualMode(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->visualMode);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Optimization-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Optimization(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->optimization);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the id of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - test id
 */
int WINAPI ec_TestId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->id);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the creation time of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - test creation time
 */
datetime WINAPI ec_TestCreated(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->created);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the strategy name of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - strategy name
 */
const char* WINAPI ec_TestStrategy(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->strategy);
   return((char*)NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the reporting id of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - reporting id
 */
int WINAPI ec_TestReportingId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->reportingId);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the reporting symbol of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - reporting symbol
 */
const char* WINAPI ec_TestReportingSymbol(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->reportingSymbol);
   return((char*)NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the instrument symbol of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - instrument symbol
 */
const char* WINAPI ec_TestSymbol(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->symbol);
   return((char*)NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the timeframe of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - test timeframe
 */
uint WINAPI ec_TestTimeframe(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->timeframe);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the start time of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - test start time
 */
datetime WINAPI ec_TestStartTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->startTime);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the end time of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - test end time
 */
datetime WINAPI ec_TestEndTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->endTime);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the bar model used in the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - test bar model
 */
uint WINAPI ec_TestBarModel(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->barModel);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the spread used in the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return double - test spread
 */
double WINAPI ec_TestSpread(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->spread);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the number of bars of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - number of bars of the test
 */
uint WINAPI ec_TestBars(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->bars);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the number of ticks of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - number of ticks of the test
 */
uint WINAPI ec_TestTicks(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->ticks);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the trade directions of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - test trade directions
 */
DWORD WINAPI ec_TestTradeDirections(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->tradeDirections);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not visual mode was enabled for the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_TestVisualMode(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->visualMode);
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the duration of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - test duration in milliseconds
 */
uint WINAPI ec_TestDuration(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->duration);
   return(NULL);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)sec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec));

   if (ec->superContext) {
      *sec = *ec->superContext;
      return(TRUE);
   }

   const EXECUTION_CONTEXT empty = {};
   *sec = empty;
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Zeiger auf den SuperContext eines EXECUTION_CONTEXT zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return EXECUTION_CONTEXT*
 */
EXECUTION_CONTEXT* WINAPI ec_lpSuperContext(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->superContext);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's current thread id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - thread id
 */
uint WINAPI ec_ThreadId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->threadId);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Handle eines Chartframes zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - Handle, entspricht dem Rückgabewert der MQL-Funktion WindowHandle()
 */
HWND WINAPI ec_hChart(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->hChart);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt das in einem EXECUTION_CONTEXT gespeicherte Handle eines Chartfensters zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - Handle
 */
HWND WINAPI ec_hChartWindow(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->hChartWindow);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten MQL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_MqlError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->mqlError);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten DLL-Fehler zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_DllError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->dllError);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die in einem EXECUTION_CONTEXT gespeicherte DLL-Warnung zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - Fehler-Code
 */
int WINAPI ec_DllWarning(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->dllWarning);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Logging-Status zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL - Status
 */
BOOL WINAPI ec_Logging(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->logging);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den in einem EXECUTION_CONTEXT gespeicherten Logdateinamen zurück.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - Dateiname
 */
const char* WINAPI ec_CustomLogFile(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->customLogFile);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the RootFunction of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return RootFunction
 */
RootFunction WINAPI mec_RootFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint programIndex = ec->programIndex;
   if (!programIndex) return((RootFunction)error(ERR_ILLEGAL_STATE, "illegal programIndex %d in ec: %s", programIndex, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = g_contextChains[programIndex][0];
   return(master->rootFunction);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the UninitializeReason of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI mec_UninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint programIndex = ec->programIndex;
   if (!programIndex) return((UninitializeReason)error(ERR_ILLEGAL_STATE, "illegal programIndex %d in ec: %s", programIndex, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = g_contextChains[programIndex][0];
   return(master->uninitReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the init configuration flags of an EXECUTION_CONTEXT's master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - flags
 */
DWORD WINAPI mec_InitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint programIndex = ec->programIndex;
   if (!programIndex) return(error(ERR_ILLEGAL_STATE, "illegal programIndex %d in ec: %s", programIndex, EXECUTION_CONTEXT_toStr(ec)));

   EXECUTION_CONTEXT* master = g_contextChains[programIndex][0];
   return(master->initFlags);
   #pragma EXPANDER_EXPORT
}


/**
 * Set a program's index in the list of all executed programs.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               index - program index (must be greater than zero)
 *
 * @return uint - the same index
 */
uint WINAPI ec_SetProgramIndex(EXECUTION_CONTEXT* ec, uint index) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (index <= 0)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter index: %d (must be greater than zero)", index));

   ec->programIndex = index;                                         // synchronize main and master context
   if (g_contextChains.size() > index && ec==g_contextChains[index][1] && g_contextChains[index][0])
      return(ec_SetProgramIndex(g_contextChains[index][0], index));
   return(index);
}


/**
 * Set a program's type.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  ProgramType        type
 *
 * @return ProgramType - the same type
 */
ProgramType WINAPI ec_SetProgramType(EXECUTION_CONTEXT* ec, ProgramType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (type) {
      case PT_INDICATOR:
      case PT_EXPERT   :
      case PT_SCRIPT   : break;
      default:
         return((ProgramType)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ProgramType)", type));
   }

   ec->programType = type;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetProgramType(g_contextChains[pid][0], type));
   return(type);
}


/**
 * Set a program's name.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - the same name
 */
const char* WINAPI ec_SetProgramName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)          return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)          return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(ec->programName)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name: \"%s\" (must be 1 to %d characters)", name, sizeof(ec->programName)-1));

   if (!strcpy(ec->programName, name))
      return(NULL);

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetProgramName(g_contextChains[pid][0], name));
   return(name);
}


/**
 * Set an EXECUTION_CONTEXT's module type.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  ModuleType         type of the executed module
 *
 * @return ModuleType - the same module type
 */
ModuleType WINAPI ec_SetModuleType(EXECUTION_CONTEXT* ec, ModuleType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (type) {
      case MT_INDICATOR:
      case MT_EXPERT   :
      case MT_SCRIPT   :
      case MT_LIBRARY  : break;
      default:
         return((ModuleType)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ModuleType)", type));
   }

   ec->moduleType = type;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetModuleType(g_contextChains[pid][0], type));
   return(type);
}


/**
 * Set a module's name
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - the same name
 */
const char* WINAPI ec_SetModuleName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)         return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(ec->moduleName)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name: \"%s\" (must be 1 to %d characters)", name, sizeof(ec->moduleName)-1));

   if (!strcpy(ec->moduleName, name))
      return(NULL);

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetModuleName(g_contextChains[pid][0], name));
   return(name);
}


/**
 * Set a program's launch type.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  LaunchType         type
 *
 * @return LaunchType - the same launch type
 */
LaunchType WINAPI ec_SetLaunchType(EXECUTION_CONTEXT* ec, LaunchType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (type) {
      case LT_TEMPLATE:
      case LT_PROGRAM :
      case LT_MANUAL  : break;
      default:
         return((LaunchType)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a LaunchType)", type));
   }

   ec->launchType = type;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetLaunchType(g_contextChains[pid][0], type));
   return(type);
}


/**
 * Set a program's RootFunction id.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  RootFunction       id
 *
 * @return RootFunction - the same id
 */
RootFunction WINAPI ec_SetRootFunction(EXECUTION_CONTEXT* ec, RootFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (id) {
      case RF_INIT  :
      case RF_START :
      case RF_DEINIT:
      case NULL     :
         break;

      default:
         return((RootFunction)error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not a RootFunction)", id));
   }

   ec->rootFunction = id;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetRootFunction(g_contextChains[pid][0], id));
   return(id);
   #pragma EXPANDER_EXPORT
}


/**
 * Set a program's init cycle status.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               whether or not the program is currently in an init cycle
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetInitCycle(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->initCycle = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetInitCycle(g_contextChains[pid][0], status));
   return(status);
}


/**
 * Set a program's InitializeReason.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  InitializeReason   reason
 *
 * @return InitializeReason - the same InitializeReason
 */
InitializeReason WINAPI ec_SetInitReason(EXECUTION_CONTEXT* ec, InitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   switch (reason) {
      case IR_USER:
      case IR_TEMPLATE:
      case IR_PROGRAM:
      case IR_PROGRAM_AFTERTEST:
      case IR_PARAMETERS:
      case IR_TIMEFRAMECHANGE:
      case IR_SYMBOLCHANGE:
      case IR_RECOMPILE:
         break;

      case NULL:
         if (ec->moduleType==MT_EXPERT ) break;
         if (ec->moduleType==MT_LIBRARY) break;

      default:
         return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an InitializeReason)", reason));
   }

   ec->initReason = reason;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetInitReason(g_contextChains[pid][0], reason));
   return(reason);
}


/**
 * Set a programÄs UninitializeReason.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  UninitializeReason reason
 *
 * @return UninitializeReason - the same UninitializeReason
 */
UninitializeReason WINAPI ec_SetUninitReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   switch (reason) {
      case UR_UNDEFINED  :
      case UR_REMOVE     :
      case UR_RECOMPILE  :
      case UR_CHARTCHANGE:
      case UR_CHARTCLOSE :
      case UR_PARAMETERS :
      case UR_ACCOUNT    :
      // build > 509
      case UR_TEMPLATE   :
      case UR_INITFAILED :
      case UR_CLOSE      : break;
      default:
         return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an UninitializeReason)", reason));
   }

   ec->uninitReason = reason;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetUninitReason(g_contextChains[pid][0], reason));
   return(reason);
}


/**
 * Set the init configuration of an EXECUTION_CONTEXT's module.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags
 */
DWORD WINAPI ec_SetInitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->initFlags = flags;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetInitFlags(g_contextChains[pid][0], flags));
   return(flags);
}


/**
 * Set the deinit configuration of an EXECUTION_CONTEXT's module.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags
 */
DWORD WINAPI ec_SetDeinitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->deinitFlags = flags;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetDeinitFlags(g_contextChains[pid][0], flags));
   return(flags);
}


/**
 * Set a program's chart symbol.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              symbol
 *
 * @return char* - the same symbol
 */
const char* WINAPI ec_SetSymbol(EXECUTION_CONTEXT* ec, const char* symbol) {
   if ((uint)ec     < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   int len = strlen(symbol);
   if (!len || len > sizeof(ec->symbol)) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol: \"%s\" (must be 1 to %d characters)", symbol, sizeof(ec->symbol)-1));

   if (!strcpy(ec->symbol, symbol))
      return(NULL);

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetSymbol(g_contextChains[pid][0], symbol));
   return(symbol);
}


/**
 * Set a programs chart timeframe.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               timeframe
 *
 * @return uint - the same timeframe
 */
uint WINAPI ec_SetTimeframe(EXECUTION_CONTEXT* ec, uint timeframe) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)timeframe <= 0)          return(error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d (must be greater than zero)", timeframe));

   ec->timeframe = timeframe;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetTimeframe(g_contextChains[pid][0], timeframe));
   return(timeframe);
}


/**
 * Set a program's "Digits" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               digits
 *
 * @return uint - the same value
 */
uint WINAPI ec_SetDigits(EXECUTION_CONTEXT* ec, uint digits) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)digits < 0)              return(error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d (must be non-negative)", digits));

   ec->digits = digits;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetDigits(g_contextChains[pid][0], digits));
   return(digits);
}


/**
 * Set a program's "Point" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  double             point
 *
 * @return double - the same value
 */
double WINAPI ec_SetPoint(EXECUTION_CONTEXT* ec, double point) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (point <= 0)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter point: %f (must be positive)", point));

   ec->point = point;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetPoint(g_contextChains[pid][0], point));
   return(point);
}


/**
 * Set a program's amount of bars (the number of price bars in the chart).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               count
 *
 * @return uint - the same number
 */
uint WINAPI ec_SetBars(EXECUTION_CONTEXT* ec, uint count) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (count < 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must be non-negative)", count));

   ec->bars = count;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetBars(g_contextChains[pid][0], count));
   return(count);
}


/**
 * Set a program's amount of ticks (number of times the start() function was called).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               count
 *
 * @return uint - the same number
 */
uint WINAPI ec_SetTicks(EXECUTION_CONTEXT* ec, uint count) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (count < 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must be non-negative)", count));

   ec->ticks = count;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetTicks(g_contextChains[pid][0], count));
   return(count);
}


/**
 * Set a program's previous tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  datetime           time - server time
 *
 * @return datetime - the same time
 */
datetime WINAPI ec_SetPreviousTickTime(EXECUTION_CONTEXT* ec, datetime time) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (time < 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));

   ec->previousTickTime = time;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetPreviousTickTime(g_contextChains[pid][0], time));
   return(time);
}


/**
 * Set a program's current tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  datetime           time - server time
 *
 * @return datetime - the same time
 */
datetime WINAPI ec_SetCurrentTickTime(EXECUTION_CONTEXT* ec, datetime time) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (time < 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));

   ec->currentTickTime = time;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetCurrentTickTime(g_contextChains[pid][0], time));
   return(time);
}


/**
 * Set a program's current bid price.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  double             price - may be 0 (zero) if no last tick was stored and the server connection is not yet established
 *
 * @return double - the same price
 */
double WINAPI ec_SetBid(EXECUTION_CONTEXT* ec, double price) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (price < 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter price: %f (must be non-negative)", price));

   ec->bid = price;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetBid(g_contextChains[pid][0], price));
   return(price);
}


/**
 * Set a program's current ask price.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  double             price - may be 0 (zero) if no last tick was stored and the server connection is not yet established
 *
 * @return double - the same price
 */
double WINAPI ec_SetAsk(EXECUTION_CONTEXT* ec, double price) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (price < 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter price: %f (must be non-negative)", price));

   ec->ask = price;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetAsk(g_contextChains[pid][0], price));
   return(price);
}


/**
 * Set a program's "ExtendedReporting" status.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetExtReporting(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->extReporting = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetExtReporting(g_contextChains[pid][0], status));
   return(status);
}


/**
 * Set a program's "RecordEquity" status.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetRecordEquity(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->recordEquity = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetRecordEquity(g_contextChains[pid][0], status));
   return(status);
}


/**
 * Set a program's testing status.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetTesting(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->testing = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetTesting(g_contextChains[pid][0], status));
   return(status);
}


/**
 * Setzt den VisualMode-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe Status
 */
BOOL WINAPI ec_SetVisualMode(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->visualMode = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetVisualMode(g_contextChains[pid][0], status));
   return(status);
}


/**
 * Setzt den Optimization-Status eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - derselbe Status
 */
BOOL WINAPI ec_SetOptimization(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->optimization = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetOptimization(g_contextChains[pid][0], status));
   return(status);
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
   if (       (uint)ec  < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec));

   ec->superContext = sec;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetSuperContext(g_contextChains[pid][0], sec));
   return(sec);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (id <= 0)                      return(error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (must be greater than zero)", id));

   ec->threadId = id;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetThreadId(g_contextChains[pid][0], id));
   return(id);
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
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->hChart = hWnd;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetHChart(g_contextChains[pid][0], hWnd));
   return(hWnd);
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
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->hChartWindow = hWnd;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetHChartWindow(g_contextChains[pid][0], hWnd));
   return(hWnd);
}


/**
 * Set the MQL error code of an EXECUTION_CONTEXT. Called by MQL::SetLastError().
 *
 * If called with a library context the error will bubble up to the library's main module. If called with an indicator context
 * loaded by iCustom() the error will bubble up to the loading program. The error code NO_ERROR will never bubble up.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - same error or EMPTY (-1) in case of errors
 */
int WINAPI ec_SetMqlError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->mqlError = error;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      ec_SetMqlError(g_contextChains[pid][0], error);

   if (error) {                                                      // no propagation for NO_ERROR
      if (ec->moduleType==MT_LIBRARY && pid) {                       // propagation from library to main module
         EXECUTION_CONTEXT* master = g_contextChains[pid][0];
         EXECUTION_CONTEXT* main   = g_contextChains[pid][1];
         if (main) ec_SetMqlError(main,   error);                    // whichever is available
         else      ec_SetMqlError(master, error);
      }

      if (ec->superContext)
         ec_SetMqlError(ec->superContext, error);                    // propagation to parent program
   }
   return(error);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->dllError = error;

   uint pid = ec->programIndex;
   if (pid && g_contextChains.size() > pid) {
      EXECUTION_CONTEXT* master = g_contextChains[pid][0];
      EXECUTION_CONTEXT* main   = g_contextChains[pid][1];

      if (ec==main && master)                                        // synchronize main and master context
         ec_SetDllError(master, error);

      if (error) {                                                   // no propagation for NO_ERROR
         if (ec->moduleType==MT_LIBRARY) {
            if (main) ec_SetDllError(main,   error);                 // Fehler aus Libraries in den Main-Context propagieren
            else      ec_SetDllError(master, error);                 // oder den Master-Context, falls Main-Context nicht verfügbar
         }
      }
   }
   return(error);
   #pragma EXPANDER_EXPORT
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
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->dllWarning = error;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      ec_SetDllWarning(g_contextChains[pid][0], error);

   if (!error)                                                       // keine Propagation beim Zurücksetzen eines Fehlers
      return(error);

   if (pid && ec->moduleType==MT_LIBRARY) {                          // Warnung aus Libraries in den Hauptkontext propagieren
      EXECUTION_CONTEXT* master = g_contextChains[pid][0];           // (oder den Master-Context, wenn Hauptkontext nicht verfügbar)
      EXECUTION_CONTEXT* main   = g_contextChains[pid][1];
      if (main) ec_SetDllWarning(main,   error);
      else      ec_SetDllWarning(master, error);
   }
   return(error);
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
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->logging = status;

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetLogging(g_contextChains[pid][0], status));
   return(status);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt den Namen der Logdatei eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              fileName - statt eines NULL-Pointers kann auch ein Leerstring angegeben werden
 *
 * @return char* - derselbe Dateiname
 */
const char* WINAPI ec_SetCustomLogFile(EXECUTION_CONTEXT* ec, const char* fileName) {
   if ((uint)ec < MIN_VALID_POINTER)                      return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   if (fileName) {
      // fileName ist kein NULL-Pointer
      if ((uint)fileName < MIN_VALID_POINTER)             return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", fileName));
      if (strlen(fileName) > sizeof(ec->customLogFile)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter fileName: \"%s\" (max %d characters)", fileName, sizeof(ec->customLogFile)-1));

      if (!strcpy(ec->customLogFile, fileName))
         return(NULL);
   }
   else {
      // fileName ist NULL-Pointer
      ec->customLogFile[0] = '\0';
   }

   uint pid = ec->programIndex;                                      // synchronize main and master context
   if (pid && g_contextChains.size() > pid && ec==g_contextChains[pid][1] && g_contextChains[pid][0])
      return(ec_SetCustomLogFile(g_contextChains[pid][0], fileName));
   return(fileName);
}


/**
 * Return a human-readable version of an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               outputDebug [optional] - whether or not to duplicate the result to OutputDebugString()
 *                                                     (default: no)
 * @return char*
 */
const char* WINAPI EXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug/*=FALSE*/) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   char* result = "{(empty)}";
   EXECUTION_CONTEXT empty = {};

   if (memcmp(ec, &empty, sizeof(EXECUTION_CONTEXT))) {
      std::stringstream ss; ss
         <<  "{programIndex="     <<                   ec->programIndex
         << ", programType="      <<  ProgramTypeToStr(ec->programType)
         << ", programName="      <<    doubleQuoteStr(ec->programName)
         << ", moduleType="       <<   ModuleTypeToStr(ec->moduleType )
         << ", moduleName="       <<    doubleQuoteStr(ec->moduleName )

         << ", launchType="       <<                   ec->launchType
         << ", rootFunction="     << RootFunctionToStr(ec->rootFunction)
         << ", initCycle="        <<         BoolToStr(ec->initCycle   )
         << ", initReason="       <<   InitReasonToStr(ec->initReason  )
         << ", uninitReason="     << UninitReasonToStr(ec->uninitReason)
         << ", initFlags="        <<    InitFlagsToStr(ec->initFlags   )
         << ", deinitFlags="      <<  DeinitFlagsToStr(ec->deinitFlags )

         << ", symbol="           <<    doubleQuoteStr(ec->symbol      )
         << ", timeframe="        <<       PeriodToStr(ec->timeframe   )
         << ", digits="           <<                   ec->digits
         << ", point="            <<                   ec->point
         << ", rates="            <<                  (ec->rates ? string("0x").append(IntToHexStr((uint)ec->rates)) : "NULL")
         << ", bars="             <<                   ec->bars
         << ", ticks="            <<                   ec->ticks
         << ", previousTickTime=" <<                  (ec->previousTickTime ? doubleQuoteStr(gmTimeFormat(ec->previousTickTime, "%Y.%m.%d %H:%M:%S")) : "0")
         << ", currentTickTime="  <<                  (ec->currentTickTime  ? doubleQuoteStr(gmTimeFormat(ec->currentTickTime,  "%Y.%m.%d %H:%M:%S")) : "0")
         << ", bid="              <<                   ec->bid
         << ", ask="              <<                   ec->ask

         << ", extReporting="     <<         BoolToStr(ec->extReporting)
         << ", recordEquity="     <<         BoolToStr(ec->recordEquity)

         << ", testing="          <<         BoolToStr(ec->testing     )
         << ", visualMode="       <<         BoolToStr(ec->visualMode  )
         << ", optimization="     <<         BoolToStr(ec->optimization)
         << ", test="             <<                  (ec->test         ? string("0x").append(IntToHexStr((uint)ec->test))         : "NULL")

         << ", superContext="     <<                  (ec->superContext ? string("0x").append(IntToHexStr((uint)ec->superContext)) : "NULL")
         << ", threadId="         <<                   ec->threadId
         << ", hChart="           <<                  (ec->hChart       ? string("0x").append(IntToHexStr((uint)ec->hChart))       : "NULL")
         << ", hChartWindow="     <<                  (ec->hChartWindow ? string("0x").append(IntToHexStr((uint)ec->hChartWindow)) : "NULL")

         << ", mqlError="         <<                 (!ec->mqlError   ? "0" : ErrorToStr(ec->mqlError  ))
         << ", dllError="         <<                 (!ec->dllError   ? "0" : ErrorToStr(ec->dllError  ))
         << ", dllWarning="       <<                 (!ec->dllWarning ? "0" : ErrorToStr(ec->dllWarning))
         << ", logging="          <<         BoolToStr(ec->logging      )
         << ", customLogFile="    <<    doubleQuoteStr(ec->customLogFile)
         << "} (0x"               << IntToHexStr((uint)ec) << ")";
      result = strdup(ss.str().c_str());                             // TODO: close memory leak
   }

   if (outputDebug) debug(result);
   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Alias of EXECUTION_CONTEXT_toStr() with a different MQL import signature
 */
const char* WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug/*=FALSE*/) {
   return(EXECUTION_CONTEXT_toStr(ec, outputDebug));
   #pragma EXPANDER_EXPORT
}
