#include "expander.h"
#include "lib/conversion.h"
#include "lib/format.h"
#include "lib/helper.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "struct/rsf/ExecutionContext.h"


extern MqlProgramList g_mqlPrograms;               // all MQL programs: vector<ContextChain> with index = program id


/**
 * Return an MQL program's id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - program id starting from 1
 */
uint WINAPI ec_Pid(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->pid);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's previous id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - previous program id starting from 1
 */
uint WINAPI ec_PreviousPid(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->previousPid);
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
 * Return the program's core function id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return CoreFunction id or NULL if the program's main module is unloaded from memory
 */
CoreFunction WINAPI ec_ProgramCoreFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((CoreFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programCoreFunction);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a program's initialization reason.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return InitializeReason
 */
InitializeReason WINAPI ec_ProgramInitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programInitReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a program's uninitialization reason.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI ec_ProgramUninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programUninitReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a program's init flags.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - init flags
 */
DWORD WINAPI ec_ProgramInitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programInitFlags);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a program's deinit flags.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - deinit flags
 */
DWORD WINAPI ec_ProgramDeinitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programDeinitFlags);
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
 * Return the current module's core function id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return CoreFunction id or NULL if the module is unloaded from memory
 */
CoreFunction WINAPI ec_ModuleCoreFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((CoreFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->moduleCoreFunction);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a module's uninitialization reason.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return UninitializeReason
 */
UninitializeReason WINAPI ec_ModuleUninitReason(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->moduleUninitReason);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a module's init flags.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - init flags
 */
DWORD WINAPI ec_ModuleInitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->moduleInitFlags);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a module's deinit flags.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return DWORD - deinit flags
 */
DWORD WINAPI ec_ModuleDeinitFlags(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->moduleDeinitFlags);
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
 * Return an EXECUTION_CONTEXT's "Bars" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - bars
 */
int WINAPI ec_Bars(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->bars);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's "ChangedBars" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - changed bars
 */
int WINAPI ec_ChangedBars(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->changedBars);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's "UnchangedBars" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - unchanged bars
 */
int WINAPI ec_UnchangedBars(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->unchangedBars);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the number of times the start() function was called during the program's lifetime.
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
 * Return the number of times the start() function was called during the program's last init() cycle.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint
 */
uint WINAPI ec_CycleTicks(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->cycleTicks);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's last tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - server time
 */
datetime WINAPI ec_LastTickTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->lastTickTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's previous tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return datetime - server time
 */
datetime WINAPI ec_PrevTickTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->prevTickTime);
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
 * Return the current symbol's "Digits" value.
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
 * Return the current symbol's "PipDigits" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - digits of a pip
 */
uint WINAPI ec_PipDigits(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->pipDigits);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current symbol's "SubPipDigits" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - digits of a subpip
 */
uint WINAPI ec_SubPipDigits(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->subPipDigits);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current symbol's "Pip" size.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return double - pip size
 */
double WINAPI ec_Pip(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->pip);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current symbol's "Point" size.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return double - point size
 */
double WINAPI ec_Point(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->point);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current symbol's "PipPoints" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - number of points of a pip
 */
uint WINAPI ec_PipPoints(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->pipPoints);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current symbols standard price format.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - format string
 */
const char* WINAPI ec_PriceFormat(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->priceFormat);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current symbols pip price format (never contains subpips).
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - format string
 */
const char* WINAPI ec_PipPriceFormat(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->pipPriceFormat);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current symbols subpip price format (always contains subpips).
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - format string
 */
const char* WINAPI ec_SubPipPriceFormat(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->subPipPriceFormat);
   #pragma EXPANDER_EXPORT
}


/**
 * Copy an EXECUTION_CONTEXT's super context into the specified target variable.
 *
 * @param  EXECUTION_CONTEXT* ec     - source context
 * @param  EXECUTION_CONTEXT* target - target variable receiving the super context
 *
 * @return BOOL - whether the source context contained a super context and it was successfully copied
 */
BOOL WINAPI ec_SuperContext(const EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* const target) {
   if ((uint)ec     < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)target < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter target: 0x%p (not a valid pointer)", target));

   if (ec->superContext) {
      *target = *ec->superContext;
      return(TRUE);
   }

   EXECUTION_CONTEXT empty = {};
   *target = empty;
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a pointer to the super context contained in an EXECUTION_CONTEXT. Used by MQL4.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return EXECUTION_CONTEXT* - pointer or NULL if the context contained no super context
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
 * Return an EXECUTION_CONTEXT's chart frame handle.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - handle, equal to the return vale of MQL::WindowHandle()
 */
HWND WINAPI ec_hChart(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->hChart);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's chart window handle.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - handle, identifying the parent window of the chart frame
 */
HWND WINAPI ec_hChartWindow(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->hChartWindow);
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
 * Return the reporting id of the TEST linked to an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - reporting id
 */
int WINAPI ec_TestReportId(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->reportId);
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
const char* WINAPI ec_TestReportSymbol(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->reportSymbol);
   return((char*)NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a program is running in the Strategy Tester or on a test chart.
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
 * Whether a program is running in the Strategy Tester or on a test chart with "VisualMode" on.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_VisualMode(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->visualMode);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a program is running in the Strategy Tester with "Optimization" on.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_Optimization(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->optimization);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether an expert's input parameter "EA.CreateReport" is activated.
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
 * Whether an expert's input parameter "EA.RecordEquity" is activated.
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
 * Return an EXECUTION_CONTEXT's MQL error code.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - error code
 */
int WINAPI ec_MqlError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->mqlError);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's DLL error code.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - error code
 */
int WINAPI ec_DllError(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->dllError);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's DLL warning code.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - warning code
 */
int WINAPI ec_DllWarning(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->dllWarning);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether logging is enabled for a program.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_LogEnabled(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->logEnabled);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an EXECUTION_CONTEXT's log filename.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - filename
 */
const char* WINAPI ec_LogFilename(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->logFilename);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether the program has a separate logfile.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_SeparateLog(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->logEnabled && ec->logFilename && *ec->logFilename);
   #pragma EXPANDER_EXPORT
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programType = type;
   }
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
   if ((uint)ec   < MIN_VALID_POINTER)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name || strlen(name) >= sizeof(ec->programName)) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name: \"%s\" (must be 1 to %d characters)", name, sizeof(ec->programName)-1));

   if (!strcpy(ec->programName, name))
      return(NULL);

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         if (!strcpy(chain[0]->programName, name))
            return(NULL);
   }
   return(name);
}


/**
 * Set a program's InitializeReason.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  InitializeReason   reason
 *
 * @return InitializeReason - the same InitializeReason
 */
InitializeReason WINAPI ec_SetProgramInitReason(EXECUTION_CONTEXT* ec, InitializeReason reason) {
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

      default:
         return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an InitializeReason)", reason));
   }

   ec->programInitReason = reason;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programInitReason = reason;
   }
   return(reason);
}


/**
 * Set a program's UninitializeReason.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  UninitializeReason reason
 *
 * @return UninitializeReason - the same UninitializeReason
 */
UninitializeReason WINAPI ec_SetProgramUninitReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
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
      case UR_CLOSE      :
         break;

      default:
         return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an UninitializeReason)", reason));
   }

   ec->programUninitReason = reason;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programUninitReason = reason;
   }
   return(reason);
}


/**
 * Set the program's CoreFunction id.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  CoreFunction       id
 *
 * @return CoreFunction - the same id
 */
CoreFunction WINAPI ec_SetProgramCoreFunction(EXECUTION_CONTEXT* ec, CoreFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((CoreFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (id) {
      case CF_INIT  :
      case CF_START :
      case CF_DEINIT:
         break;
      default:
         return((CoreFunction)error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not a CoreFunction id)", id));
   }

   ec->programCoreFunction = id;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programCoreFunction = id;
   }
   return(id);
   #pragma EXPANDER_EXPORT
}


/**
 * Set the init flags of a program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags
 */
DWORD WINAPI ec_SetProgramInitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->programInitFlags = flags;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programInitFlags = flags;
   }
   return(flags);
}


/**
 * Set the deinit flags of a program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags
 */
DWORD WINAPI ec_SetProgramDeinitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->programDeinitFlags = flags;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programDeinitFlags = flags;
   }
   return(flags);
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleType = type;
   }
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
   if ((uint)ec   < MIN_VALID_POINTER)                   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)                   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name || strlen(name) >= sizeof(ec->moduleName)) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name: \"%s\" (must be 1 to %d characters)", name, sizeof(ec->moduleName)-1));

   if (!strcpy(ec->moduleName, name))
      return(NULL);

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         if (!strcpy(chain[0]->moduleName, name))
            return(NULL);
   }
   return(name);
}


/**
 * Set a module's UninitializeReason.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  UninitializeReason reason
 *
 * @return UninitializeReason - the same UninitializeReason
 */
UninitializeReason WINAPI ec_SetModuleUninitReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
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
      case UR_CLOSE      :
         break;

      default:
         return((UninitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an UninitializeReason)", reason));
   }

   ec->moduleUninitReason = reason;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleUninitReason = reason;
   }
   return(reason);
}


/**
 * Set the current module's CoreFunction id.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  CoreFunction       id
 *
 * @return CoreFunction - the same id
 */
CoreFunction WINAPI ec_SetModuleCoreFunction(EXECUTION_CONTEXT* ec, CoreFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((CoreFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (id) {
      case CF_INIT  :
      case CF_START :
      case CF_DEINIT:
         break;
      default:
         return((CoreFunction)error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not a CoreFunction id)", id));
   }

   ec->moduleCoreFunction = id;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleCoreFunction = id;
   }
   return(id);
}


/**
 * Set the init flags of a module.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags
 */
DWORD WINAPI ec_SetModuleInitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->moduleInitFlags = flags;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleInitFlags = flags;
   }
   return(flags);
}


/**
 * Set the deinit flags of a module.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags
 */
DWORD WINAPI ec_SetModuleDeinitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->moduleDeinitFlags = flags;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleDeinitFlags = flags;
   }
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
   if ((uint)ec     < MIN_VALID_POINTER)               return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)symbol < MIN_VALID_POINTER)               return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   if (!*symbol || strlen(symbol) > MAX_SYMBOL_LENGTH) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol: \"%s\" (must be 1 to %d characters)", symbol, MAX_SYMBOL_LENGTH));

   if (!strcpy(ec->symbol, symbol))
      return(NULL);

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         if (!strcpy(chain[0]->symbol, symbol))
            return(NULL);
   }
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->timeframe = timeframe;
   }
   return(timeframe);
}


/**
 * Set an EXECUTION_CONTEXT's "Bars" value (the number of price bars).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                count
 *
 * @return int - the same value
 */
int WINAPI ec_SetBars(EXECUTION_CONTEXT* ec, int count) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (count < 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must be non-negative)", count));

   ec->bars = count;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->bars = count;
   }
   return(count);
}


/**
 * Set an EXECUTION_CONTEXT's "ChangedBars" value (the number of changed indicator values).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                count
 *
 * @return int - the same value
 */
int WINAPI ec_SetChangedBars(EXECUTION_CONTEXT* ec, int count) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (count < -1)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (can't be smaller than -1)", count));

   ec->changedBars = count;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->changedBars = count;
   }
   return(count);
}


/**
 * Set an EXECUTION_CONTEXT's "UnchangedBars" value (the number of unchanged indicator values).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                count
 *
 * @return int - the same value
 */
int WINAPI ec_SetUnchangedBars(EXECUTION_CONTEXT* ec, int count) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (count < -1)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (can't be smaller than -1)", count));

   ec->unchangedBars = count;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->unchangedBars = count;
   }
   return(count);
}


/**
 * Set the current symbol's "Digits" value.
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->digits = digits;
   }
   return(digits);
}


/**
 * Set the current symbol's "PipDigits" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               digits
 *
 * @return uint - the same value
 */
uint WINAPI ec_SetPipDigits(EXECUTION_CONTEXT* ec, uint digits) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)digits < 0)              return(error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d (must be non-negative)", digits));

   ec->pipDigits = digits;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->pipDigits = digits;
   }
   return(digits);
}


/**
 * Set the current symbol's "SubPipDigits" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               digits
 *
 * @return uint - the same value
 */
uint WINAPI ec_SetSubPipDigits(EXECUTION_CONTEXT* ec, uint digits) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)digits < 0)              return(error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d (must be non-negative)", digits));

   ec->subPipDigits = digits;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->subPipDigits = digits;
   }
   return(digits);
}


/**
 * Set the current symbol's "Pip" size.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  double             size
 *
 * @return double - the same size
 */
double WINAPI ec_SetPip(EXECUTION_CONTEXT* ec, double size) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (size <= 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %f (must be positive)", size));

   ec->pip = size;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->pip = size;
   }
   return(size);
}


/**
 * Set the current symbol's "Point" size.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  double             size
 *
 * @return double - the same size
 */
double WINAPI ec_SetPoint(EXECUTION_CONTEXT* ec, double size) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (size <= 0)                    return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %f (must be positive)", size));

   ec->point = size;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->point = size;
   }
   return(size);
}


/**
 * Set the current symbol's "PipPoints" value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               points - number of points per pip
 *
 * @return uint - the same value
 */
uint WINAPI ec_SetPipPoints(EXECUTION_CONTEXT* ec, uint points) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (points!=1 && points!=10)      return(error(ERR_INVALID_PARAMETER, "invalid parameter points: %d (must be 1 or 10)", points));

   ec->pipPoints = points;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->pipPoints = points;
   }
   return(points);
}


/**
 * Set a program's super EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec  - a program's execution context
 * @param  EXECUTION_CONTEXT* sec - a super context
 *
 * @return EXECUTION_CONTEXT* - the same super context
 */
EXECUTION_CONTEXT* WINAPI ec_SetSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if (       (uint)ec  < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec));

   ec->superContext = sec;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->superContext = sec;
   }
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
   if ((int)id <= 0)                 return(error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (must be greater than zero)", id));

   ec->threadId = id;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->threadId = id;
   }
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->hChart = hWnd;
   }
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->hChartWindow = hWnd;
   }
   return(hWnd);
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->testing = status;
   }
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->visualMode = status;
   }
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

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->optimization = status;
   }
   return(status);
}


/**
 * Set a program's "extended reporting" status.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetExtReporting(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->extReporting = status;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->extReporting = status;
   }
   return(status);
}


/**
 * Set a program's "record equity" status.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetRecordEquity(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->recordEquity = status;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->recordEquity = status;
   }
   return(status);
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

   uint pid = ec->pid;
   if (pid && g_mqlPrograms.size() > pid) {
      EXECUTION_CONTEXT* master = (*g_mqlPrograms[pid])[0];
      EXECUTION_CONTEXT* main   = (*g_mqlPrograms[pid])[1];

      if (ec == main) {
         if (master) master->mqlError = error;                       // synchronize main and master context
      }
      if (error) {                                                   // error propagation
         if (ec->moduleType == MT_LIBRARY) {                         // propagation from library to main module
            if (master) master->mqlError = error;
            if (main)   main->mqlError   = error;                    // whichever is available
         }
         if (ec->superContext) {                                     // propagation to the host program
            ec_SetMqlError(ec->superContext, error);
         }
      }
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

   uint pid = ec->pid;
   if (pid && g_mqlPrograms.size() > pid) {
      EXECUTION_CONTEXT* master = (*g_mqlPrograms[pid])[0];
      EXECUTION_CONTEXT* main   = (*g_mqlPrograms[pid])[1];

      if (ec == main) {                                              // synchronize main and master context
         if (master) master->dllError = error;
      }
      if (error) {                                                   // error propagation
         if (ec->moduleType == MT_LIBRARY) {                         // propagation from library to main module
            if (master) master->dllError = error;
            if (main)   main->dllError   = error;                    // whichever is available
         }
         // No propagation to the host program as DLL errors will be converted to MQL errors and bubble up to the host
         // program as MQL errors.
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

   uint pid = ec->pid;
   if (pid && g_mqlPrograms.size() > pid) {
      EXECUTION_CONTEXT* master = (*g_mqlPrograms[pid])[0];
      EXECUTION_CONTEXT* main   = (*g_mqlPrograms[pid])[1];

      if (ec == main) {                                              // synchronize main and master context
         if (master) master->dllWarning = error;
      }
      if (error) {                                                   // warning propagation
         if (ec->moduleType == MT_LIBRARY) {                         // propagation from library to main module
            if (master) master->dllWarning = error;
            if (main)   main->dllWarning   = error;                  // whichever is available
         }
      }
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
BOOL WINAPI ec_SetLogEnabled(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->logEnabled = status;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->logEnabled = status;
   }
   return(status);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt den Namen der Logdatei eines EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              filename - statt eines NULL-Pointers kann auch ein Leerstring angegeben werden
 *
 * @return char* - derselbe Dateiname
 */
const char* WINAPI ec_SetLogFilename(EXECUTION_CONTEXT* ec, const char* filename) {
   if ((uint)ec < MIN_VALID_POINTER)                    return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   if (filename) {
      // fileName is not a NULL pointer
      if ((uint)filename < MIN_VALID_POINTER)           return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter fileName: 0x%p (not a valid pointer)", filename));
      if (strlen(filename) > sizeof(ec->logFilename)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter fileName: \"%s\" (max %d characters)", filename, sizeof(ec->logFilename)-1));

      if (!strcpy(ec->logFilename, filename))
         return(NULL);
   }
   else {
      // fileName is a NULL pointer, store an empty string
      ec->logFilename[0] = '\0';
   }

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlPrograms.size() > pid) {
      ContextChain &chain = *g_mqlPrograms[pid];
      if (ec==chain[1] && chain[0])
         if (!strcpy(chain[0]->logFilename, ec->logFilename))
            return(NULL);
   }
   return(filename);
}


/**
 * Return a human-readable version of an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               outputDebug [optional] - whether to duplicate the result to OutputDebugString()
 *                                                     (default: no)
 * @return char*
 */
const char* WINAPI EXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec, BOOL outputDebug/*=FALSE*/) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   std::stringstream ss;
   EXECUTION_CONTEXT empty = {};

   if (MemCompare(ec, &empty, sizeof(EXECUTION_CONTEXT))) {
      ss << "{}";
   }
   else {
      ss << std::fixed
         <<  "{pid="                 <<                     ec->pid
         << ", previousPid="         <<                     ec->previousPid

         << ", programType="         <<    ProgramTypeToStr(ec->programType)
         << ", programName="         <<      DoubleQuoteStr(ec->programName)
         << ", programCoreFunction=" <<   CoreFunctionToStr(ec->programCoreFunction)
         << ", programInitReason="   <<     InitReasonToStr(ec->programInitReason)
         << ", programUninitReason=" <<   UninitReasonToStr(ec->programUninitReason)
         << ", programInitFlags="    <<      InitFlagsToStr(ec->programInitFlags)
         << ", programDeinitFlags="  <<    DeinitFlagsToStr(ec->programDeinitFlags)

         << ", moduleType="          <<     ModuleTypeToStr(ec->moduleType)
         << ", moduleName="          <<      DoubleQuoteStr(ec->moduleName)
         << ", moduleCoreFunction="  <<   CoreFunctionToStr(ec->moduleCoreFunction)
         << ", moduleUninitReason="  <<   UninitReasonToStr(ec->moduleUninitReason)
         << ", moduleInitFlags="     <<      InitFlagsToStr(ec->moduleInitFlags)
         << ", moduleDeinitFlags="   <<    DeinitFlagsToStr(ec->moduleDeinitFlags)

         << ", symbol="              <<      DoubleQuoteStr(ec->symbol)
         << ", timeframe="           <<         PeriodToStr(ec->timeframe)
         << ", rates="               <<                    (ec->rates ? StrFormat("0x%p", ec->rates) : "NULL")
         << ", bars="                <<                     ec->bars
         << ", changedBars="         <<                     ec->changedBars
         << ", unchangedBars="       <<                     ec->unchangedBars
         << ", ticks="               <<                     ec->ticks
         << ", cycleTicks="          <<                     ec->cycleTicks
         << ", lastTickTime="        <<                    (ec->lastTickTime ? GmtTimeFormat(ec->lastTickTime, "\"%Y.%m.%d %H:%M:%S\"") : "0")
         << ", prevTickTime="        <<                    (ec->prevTickTime ? GmtTimeFormat(ec->prevTickTime, "\"%Y.%m.%d %H:%M:%S\"") : "0")
         << ", bid=" << std::setprecision(ec->bid ? ec->digits : 0) << ec->bid
         << ", ask=" << std::setprecision(ec->ask ? ec->digits : 0) << ec->ask

         << ", digits="              <<                     ec->digits
         << ", pipDigits="           <<                     ec->pipDigits
         << ", subPipDigits="        <<                     ec->subPipDigits
         << ", pip=" << std::setprecision(ec->pipDigits) << ec->pip
         << ", point=" << std::setprecision(ec->digits)  << ec->point
         << ", pipPoints="           <<                     ec->pipPoints
         << ", priceFormat="         <<      DoubleQuoteStr(ec->priceFormat)
         << ", pipPriceFormat="      <<      DoubleQuoteStr(ec->pipPriceFormat)
         << ", subPipPriceFormat="   <<      DoubleQuoteStr(ec->subPipPriceFormat)

         << ", superContext="        <<                  (ec->superContext ? StrFormat("0x%p", ec->superContext) : "NULL")
         << ", threadId="            <<                   ec->threadId << (ec->threadId ? (IsUIThread(ec->threadId) ? " (UI)":" (non-UI)"):"")
         << ", hChart="              <<                  (ec->hChart       ? StrFormat("0x%p", ec->hChart       ) : "NULL")
         << ", hChartWindow="        <<                  (ec->hChartWindow ? StrFormat("0x%p", ec->hChartWindow ) : "NULL")

         << ", test="                <<                  (ec->test ? StrFormat("0x%p", ec->test) : "NULL")
         << ", testing="             <<         BoolToStr(ec->testing)
         << ", visualMode="          <<         BoolToStr(ec->visualMode)
         << ", optimization="        <<         BoolToStr(ec->optimization)

         << ", extReporting="        <<         BoolToStr(ec->extReporting)
         << ", recordEquity="        <<         BoolToStr(ec->recordEquity)

         << ", mqlError="            <<                 (!ec->mqlError   ? "0" : ErrorToStr(ec->mqlError  ))
         << ", dllError="            <<                 (!ec->dllError   ? "0" : ErrorToStr(ec->dllError  ))
         << ", dllWarning="          <<                 (!ec->dllWarning ? "0" : ErrorToStr(ec->dllWarning))
         << ", logEnabled="          <<         BoolToStr(ec->logEnabled)
         << ", logFilename="         <<    DoubleQuoteStr(ec->logFilename)
         << "}";
   }
   ss << StrFormat(" (0x%p)", ec);
   char* result = strdup(ss.str().c_str());                             // TODO: add to GC (close memory leak)

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
