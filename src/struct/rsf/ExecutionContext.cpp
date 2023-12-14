#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/helper.h"
#include "lib/log.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "struct/rsf/ExecutionContext.h"

#include <fstream>

extern MqlInstanceList g_mqlInstances;             // all MQL program instances


/**
 * Return an MQL program's instance id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - instance id starting from 1
 */
uint WINAPI ec_Pid(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->pid);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's previous instance id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return uint - previous instance id starting from 1
 */
uint WINAPI ec_PreviousPid(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->previousPid);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's start time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return time32 - system time
 */
time32 WINAPI ec_Started(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->started);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's type.
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
 * Return an MQL program's name.
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
 * Return an MQL program's core function id.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return CoreFunction id or NULL if the program's main module is currently unloaded from memory
 */
CoreFunction WINAPI ec_ProgramCoreFunction(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((CoreFunction)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programCoreFunction);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's initialization reason.
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
 * Return an MQL program's uninitialization reason.
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
 * Return an MQL program's init flags.
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
 * Return an MQL program's deinit flags.
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
 * Return an MQL program's current module type.
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
 * Return an MQL program's current module name.
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
 * Return an MQL program's current module core function id.
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
 * Return an MQL program's current module uninitialization reason.
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
 * Return an MQL program's current module init flags.
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
 * Return an MQL program's current module deinit flags.
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
 * Return an MQL program's current symbol.
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
 * Return an MQL program's current timeframe.
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
 * Return an MQL program's current amount of chart bars.
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
 * Return an MQL program's current amount of valid chart bars.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - valid bars
 */
int WINAPI ec_ValidBars(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->validBars);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's current amount of changed chart bars.
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
 * Return the number of times the start() function was called during an MQL program's lifetime.
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
 * Return an MQL program's current tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return time32 - server time
 */
time32 WINAPI ec_CurrTickTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->currTickTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's previous tick time.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return time32 - server time
 */
time32 WINAPI ec_PrevTickTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->prevTickTime);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's current bid price.
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
 * Return an MQL program's current ask price.
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
 * Return an MQL program's current symbol "Digits".
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
 * Return an MQL program's current symbol "PipDigits".
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
 * Return an MQL program's current symbol "Pip" size.
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
 * Return an MQL program's current symbol "Point" size.
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
 * Return an MQL program's current symbol "PipPoints" value.
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
 * Return an MQL program's current symbol standard price format.
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
 * Return an MQL program's current symbol pip price format (doesn't contain subpips).
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
 * Return the MQL program name of the linked super context (if any).
 *
 * @param  uint pid - pid of the current program
 *
 * @return char* - program name
 */
const char* WINAPI ep_SuperProgramName(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->programName);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the main loglevel of the linked super context (if any).
 *
 * @param  uint pid - pid of the current program
 *
 * @return int - loglevel
 */
int WINAPI ep_SuperLoglevel(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->loglevel);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the loglevel for the terminal log appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the current program
 *
 * @return int - loglevel
 */
int WINAPI ep_SuperLoglevelTerminal(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->loglevelTerminal);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the loglevel for the terminal alert appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the current program
 *
 * @return int - loglevel
 */
int WINAPI ep_SuperLoglevelAlert(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->loglevelAlert);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the loglevel for the debug output appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the current program
 *
 * @return int - loglevel
 */
int WINAPI ep_SuperLoglevelDebugger(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->loglevelDebugger);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the loglevel for the separate logfile appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the current program
 *
 * @return int - loglevel
 */
int WINAPI ep_SuperLoglevelFile(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->loglevelFile);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the loglevel for the mail appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the current program
 *
 * @return int - loglevel
 */
int WINAPI ep_SuperLoglevelMail(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->loglevelMail);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the loglevel for the SMS appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the current program
 *
 * @return int - loglevel
 */
int WINAPI ep_SuperLoglevelSMS(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->loglevelSMS);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's current thread id.
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
 * Return an MQL program's chart frame handle.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - handle, same as return value of MQL::WindowHandle()
 */
HWND WINAPI ec_hChart(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->hChart);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's chart window handle.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return HWND - handle identifying the parent window of the chart frame
 */
HWND WINAPI ec_hChartWindow(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->hChartWindow);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's "EA.Recorder" mode (experts only).
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int
 */
int WINAPI ec_RecordMode(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->recordMode);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's TEST id (if any).
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
 * Return an MQL program's test creation time (if any).
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return time32 - test creation time
 */
time32 WINAPI ec_TestCreated(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->created);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's test start time (if any).
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return time32 - test start time
 */
time32 WINAPI ec_TestStartTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->startTime);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's test end time (if any).
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return time32 - test end time
 */
time32 WINAPI ec_TestEndTime(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (ec->test)
      return(ec->test->endTime);
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's bar model used in a test (if any).
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
 * Return an MQL program's number of bars of a test (if any).
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
 * Return an MQL program's number of ticks of a test (if any).
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
 * Return an MQL program's spread used in a test (if any).
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
 * Return an MQL program's trade directions of a test (if any).
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
 * Whether an MQL program is executed in the tester or on a chart in the tester.
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
 * Whether an MQL program is executed in the tester with "VisualMode" on.
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
 * Whether an MQL program is executed in the tester with "Optimization" on.
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
 * Whether an MQL program's input parameter "Test.ExternalReporting" is activated (experts only).
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return BOOL
 */
BOOL WINAPI ec_ExternalReporting(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->externalReporting);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's current MQL error code.
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
 * Return an MQL program's current DLL error code.
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
 * Return an MQL program's current DLL warning code.
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
 * Return an MQL program's main loglevel.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - loglevel
 */
int WINAPI ec_Loglevel(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->loglevel);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's loglevel for the terminal log appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - loglevel
 */
int WINAPI ec_LoglevelTerminal(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->loglevelTerminal);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's loglevel for the terminal alert appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - loglevel
 */
int WINAPI ec_LoglevelAlert(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->loglevelAlert);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's loglevel for the debug output appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - loglevel
 */
int WINAPI ec_LoglevelDebugger(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->loglevelDebugger);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's loglevel for the separate logfile appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - loglevel
 */
int WINAPI ec_LoglevelFile(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->loglevelFile);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's loglevel for the mail appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - loglevel
 */
int WINAPI ec_LoglevelMail(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->loglevelMail);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's loglevel for the SMS appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return int - loglevel
 */
int WINAPI ec_LoglevelSMS(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   return(ec->loglevelSMS);
   #pragma EXPANDER_EXPORT
}


/**
 * Return an MQL program's custom log filename.
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
 * Set an EXECUTION_CONTEXT's programType value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programType = type;
   }
   return(type);
}


/**
 * Set an EXECUTION_CONTEXT's programName value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         if (!strcpy(chain[0]->programName, name))
            return(NULL);
   }
   return(name);
}


/**
 * Set an EXECUTION_CONTEXT's programInitReason value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programInitReason = reason;
   }
   return(reason);
}


/**
 * Set an EXECUTION_CONTEXT's programUninitReason value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programUninitReason = reason;
   }
   return(reason);
}


/**
 * Set an EXECUTION_CONTEXT's programCoreFunction value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programCoreFunction = id;
   }
   return(id);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an EXECUTION_CONTEXT's programInitFlags value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programInitFlags = flags;
   }
   return(flags);
}


/**
 * Set an EXECUTION_CONTEXT's programDeinitFlags value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->programDeinitFlags = flags;
   }
   return(flags);
}


/**
 * Set an EXECUTION_CONTEXT's moduleType value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleType = type;
   }
   return(type);
}


/**
 * Set an EXECUTION_CONTEXT's moduleName value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         if (!strcpy(chain[0]->moduleName, name))
            return(NULL);
   }
   return(name);
}


/**
 * Set an EXECUTION_CONTEXT's moduleUninitReason value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleUninitReason = reason;
   }
   return(reason);
}


/**
 * Set an EXECUTION_CONTEXT's moduleCoreFunction value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleCoreFunction = id;
   }
   return(id);
}


/**
 * Set an EXECUTION_CONTEXT's moduleInitFlags value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleInitFlags = flags;
   }
   return(flags);
}


/**
 * Set an EXECUTION_CONTEXT's moduleDeinitFlags value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->moduleDeinitFlags = flags;
   }
   return(flags);
}


/**
 * Set an EXECUTION_CONTEXT's symbol value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         if (!strcpy(chain[0]->symbol, symbol))
            return(NULL);
   }
   return(symbol);
}


/**
 * Set an EXECUTION_CONTEXT's timeframe value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->timeframe = timeframe;
   }
   return(timeframe);
}


/**
 * Set an EXECUTION_CONTEXT's bars value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->bars = count;
   }
   return(count);
}


/**
 * Set an EXECUTION_CONTEXT's validBars value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                count
 *
 * @return int - the same value
 */
int WINAPI ec_SetValidBars(EXECUTION_CONTEXT* ec, int count) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (count < -1)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (can't be smaller than -1)", count));

   ec->validBars = count;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->validBars = count;
   }
   return(count);
}


/**
 * Set an EXECUTION_CONTEXT's changedBars value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->changedBars = count;
   }
   return(count);
}


/**
 * Set an EXECUTION_CONTEXT's digits value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->digits = digits;
   }
   return(digits);
}


/**
 * Set an EXECUTION_CONTEXT's pipDigits value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->pipDigits = digits;
   }
   return(digits);
}


/**
 * Set an EXECUTION_CONTEXT' pip value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->pip = size;
   }
   return(size);
}


/**
 * Set an EXECUTION_CONTEXT's point value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->point = size;
   }
   return(size);
}


/**
 * Set an EXECUTION_CONTEXT's pipPoints value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->pipPoints = points;
   }
   return(points);
}


/**
 * Set an EXECUTION_CONTEXT's superContext value.
 *
 * @param  EXECUTION_CONTEXT* ec  - a program's execution context
 * @param  EXECUTION_CONTEXT* sec - a super context
 *
 * @return EXECUTION_CONTEXT* - the same super context
 */
EXECUTION_CONTEXT* WINAPI ec_SetSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if ((uint)ec         < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec));

   ec->superContext = sec;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->superContext = sec;
   }
   return(sec);
}


/**
 * Set an EXECUTION_CONTEXT's threadId value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->threadId = id;
   }
   return(id);
}


/**
 * Set an EXECUTION_CONTEXT's hChart value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  HWND               hWnd - return value of MQL::WindowHandle()
 *
 * @return HWND - the same handle
 */
HWND WINAPI ec_SetHChart(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->hChart = hWnd;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->hChart = hWnd;
   }
   return(hWnd);
}


/**
 * Set an EXECUTION_CONTEXT's hChartWindow value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  HWND               hWnd
 *
 * @return HWND - the same handle
 */
HWND WINAPI ec_SetHChartWindow(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->hChartWindow = hWnd;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->hChartWindow = hWnd;
   }
   return(hWnd);
}


/**
 * Set an EXECUTION_CONTEXT's recordMode value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                mode
 *
 * @return int - the same mode
 */
int WINAPI ec_SetRecordMode(EXECUTION_CONTEXT* ec, int mode) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->recordMode = mode;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->recordMode = mode;
   }
   return(mode);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an EXECUTION_CONTEXT's testing value.
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
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->testing = status;
   }
   return(status);
}


/**
 * Set an EXECUTION_CONTEXT's visualMode value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetVisualMode(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->visualMode = status;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->visualMode = status;
   }
   return(status);
}


/**
 * Set an EXECUTION_CONTEXT's optimization value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetOptimization(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->optimization = status;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->optimization = status;
   }
   return(status);
}


/**
 * Set an EXECUTION_CONTEXT's externalReporting value.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status
 */
BOOL WINAPI ec_SetExternalReporting(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->externalReporting = status;

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (ec==chain[1] && chain[0])
         chain[0]->externalReporting = status;
   }
   return(status);
}


/**
 * Set an EXECUTION_CONTEXT's mqlError value.
 *
 * If called with a library context the error will bubble up to the library's main module. If called with an indicator context
 * loaded by iCustom() the error will bubble up to the loading program. The error code NO_ERROR will never bubble up.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - the same error or EMPTY (-1) in case of errors
 */
int WINAPI ec_SetMqlError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->mqlError = error;

   uint pid = ec->pid;
   if (pid && g_mqlInstances.size() > pid) {
      EXECUTION_CONTEXT* master = (*g_mqlInstances[pid])[0];
      EXECUTION_CONTEXT* main   = (*g_mqlInstances[pid])[1];

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
 * Set an EXECUTION_CONTEXT's dllError value.
 *
 * Zusätzlich wird der DLL-Fehler in den jeweiligen Hauptkontext propagiert (Propagation zum aufrufenden Hauptmodul).
 * Fehler werden nur beim Setzen propagiert, nicht beim Zurücksetzen.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - the same error or EMPTY (-1) in case of errors
 */
int WINAPI ec_SetDllError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->dllError = error;

   uint pid = ec->pid;
   if (pid && g_mqlInstances.size() > pid) {
      EXECUTION_CONTEXT* master = (*g_mqlInstances[pid])[0];
      EXECUTION_CONTEXT* main   = (*g_mqlInstances[pid])[1];

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
 * Set an EXECUTION_CONTEXT's dllWarning value.
 *
 * Zusätzlich wird die DLL-Warnung in den jeweiligen Hauptkontext propagiert (Propagation zum aufrufenden Hauptmodul).
 * Warnungen werden nur beim Setzen propagiert, nicht beim Zurücksetzen.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error
 *
 * @return int - the same error or EMPTY (-1) in case of errors
 */
int WINAPI ec_SetDllWarning(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));

   ec->dllWarning = error;

   uint pid = ec->pid;
   if (pid && g_mqlInstances.size() > pid) {
      EXECUTION_CONTEXT* master = (*g_mqlInstances[pid])[0];
      EXECUTION_CONTEXT* main   = (*g_mqlInstances[pid])[1];

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
 * Set an MQL program's main loglevel.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel
 */
int WINAPI ec_SetLoglevel(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->loglevel = level;

   uint pid = ec->pid;
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (EXECUTION_CONTEXT* master = chain[0]) {
         if (master && master->loglevel != level) {
            master->loglevel = level;                                      // synchronize master context

            if (!master->loglevel || master->loglevel==LOG_OFF) {
               if (master->logger && master->logger->is_open()) {
                  master->logger->close();                                 // close an open logfile if logging was disabled
               }
            }
         }
      }
   }
   return(level);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an MQL program's loglevel for the terminal log appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel
 */
int WINAPI ec_SetLoglevelTerminal(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->loglevelTerminal = level;

   uint pid = ec->pid;                                               // synchronize master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];
      if (master) {
         master->loglevelTerminal = level;
      }
   }
   return(level);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an MQL program's loglevel for the terminal alert appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel
 */
int WINAPI ec_SetLoglevelAlert(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->loglevelAlert = level;

   uint pid = ec->pid;                                               // synchronize master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];
      if (master) {
         master->loglevelAlert = level;
      }
   }
   return(level);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an MQL program's loglevel for the debug output appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel
 */
int WINAPI ec_SetLoglevelDebugger(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->loglevelDebugger = level;

   uint pid = ec->pid;                                               // synchronize master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];
      if (master) {
         master->loglevelDebugger = level;
      }
   }
   return(level);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an MQL program's loglevel for the custom logfile appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel
 */
int WINAPI ec_SetLoglevelFile(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->loglevelFile = level;

   uint pid = ec->pid;
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      if (EXECUTION_CONTEXT* master = chain[0]) {
         if (master && master->loglevelFile != level) {
            master->loglevelFile = level;                                  // synchronize master context

            if (!master->loglevelFile || master->loglevelFile==LOG_OFF) {
               if (master->logger && master->logger->is_open()) {
                  master->logger->close();                                 // close an open logfile if the logfile appender was disabled
               }
            }
         }
      }
   }
   return(level);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an MQL program's loglevel for the mail appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel
 */
int WINAPI ec_SetLoglevelMail(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->loglevelMail = level;

   uint pid = ec->pid;                                               // synchronize master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];
      if (master) {
         master->loglevelMail = level;
      }
   }
   return(level);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an MQL program's loglevel for the SMS appender.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel
 */
int WINAPI ec_SetLoglevelSMS(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   ec->loglevelSMS = level;

   uint pid = ec->pid;                                               // synchronize master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];
      if (master) {
         master->loglevelSMS = level;
      }
   }
   return(level);
   #pragma EXPANDER_EXPORT
}


/**
 * Set an MQL program's custom log filename.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              filename - filename (an empty string or a NULL pointer reset the field)
 *
 * @return char* - the same filename
 */
const char* WINAPI ec_SetLogFilename(EXECUTION_CONTEXT* ec, const char* filename) {
   // note: this setter is not exported

   if (!filename) {
      ec->logFilename[0] = '\0';                                     // convert NULL pointer to an empty string
   }
   else {
      if (strlen(filename) > sizeof(ec->logFilename)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter filename: \"%s\" (max %d characters)", filename, sizeof(ec->logFilename)-1));
      if (!strcpy(ec->logFilename, filename))           return(NULL);
   }

   uint pid = ec->pid;                                               // synchronize main and master context
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];
      if (master && ec==chain[1]) {
         if (!strcpy(master->logFilename, ec->logFilename))
            return(NULL);
      }
   }
   return(filename);
}


/**
 * Return a human-readable version of an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char*
 */
const char* WINAPI EXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   std::ostringstream ss;
   EXECUTION_CONTEXT empty = {};

   if (MemCompare(ec, &empty, sizeof(EXECUTION_CONTEXT))) {
      ss << "{}";
   }
   else {
      ss << std::fixed
         <<  "{pid="                  <<                      ec->pid
         << ", previousPid="          <<                      ec->previousPid
         << ", started="              <<                     (ec->started ? LocalTimeFormatA(ec->started, "\"%Y.%m.%d %H:%M:%S\"") : "0")

         << ", programType="          <<     ProgramTypeToStr(ec->programType)
         << ", programName="          <<       DoubleQuoteStr(ec->programName)
         << ", programCoreFunction="  <<    CoreFunctionToStr(ec->programCoreFunction)
         << ", programInitReason="    <<      InitReasonToStr(ec->programInitReason)
         << ", programUninitReason="  <<    UninitReasonToStr(ec->programUninitReason)
         << ", programInitFlags="     <<       InitFlagsToStr(ec->programInitFlags)
         << ", programDeinitFlags="   <<     DeinitFlagsToStr(ec->programDeinitFlags)

         << ", moduleType="           <<      ModuleTypeToStr(ec->moduleType)
         << ", moduleName="           <<       DoubleQuoteStr(ec->moduleName)
         << ", moduleCoreFunction="   <<    CoreFunctionToStr(ec->moduleCoreFunction)
         << ", moduleUninitReason="   <<    UninitReasonToStr(ec->moduleUninitReason)
         << ", moduleInitFlags="      <<       InitFlagsToStr(ec->moduleInitFlags)
         << ", moduleDeinitFlags="    <<     DeinitFlagsToStr(ec->moduleDeinitFlags)

         << ", symbol="               <<       DoubleQuoteStr(ec->symbol)
         << ", timeframe="            <<   PeriodDescriptionA(ec->timeframe)
         << ", newSymbol="            <<       DoubleQuoteStr(ec->newSymbol)
         << ", newTimeframe="         <<   PeriodDescriptionA(ec->newTimeframe)
         << ", rates="                <<                     (ec->rates ? asformat("0x%p", ec->rates) : "NULL")
         << ", bars="                 <<                      ec->bars
         << ", validBars="            <<                      ec->validBars
         << ", changedBars="          <<                      ec->changedBars
         << ", ticks="                <<                      ec->ticks
         << ", cycleTicks="           <<                      ec->cycleTicks
         << ", currTickTime="         <<                     (ec->currTickTime ? GmtTimeFormatA(ec->currTickTime, "\"%Y.%m.%d %H:%M:%S\"") : "0")
         << ", prevTickTime="         <<                     (ec->prevTickTime ? GmtTimeFormatA(ec->prevTickTime, "\"%Y.%m.%d %H:%M:%S\"") : "0")
         << ", bid=" << std::setprecision(ec->bid ? ec->digits : 0) << ec->bid
         << ", ask=" << std::setprecision(ec->ask ? ec->digits : 0) << ec->ask

         << ", digits="               <<                      ec->digits
         << ", pipDigits="            <<                      ec->pipDigits
         << ", pip="   << std::setprecision(ec->pipDigits) << ec->pip
         << ", point=" << std::setprecision(ec->digits)    << ec->point
         << ", pipPoints="            <<                      ec->pipPoints
         << ", priceFormat="          <<       DoubleQuoteStr(ec->priceFormat)
         << ", pipPriceFormat="       <<       DoubleQuoteStr(ec->pipPriceFormat)

         << ", superContext="         <<                     (ec->superContext ? asformat("0x%p", ec->superContext) : "NULL")
         << ", threadId="             <<                      ec->threadId << (ec->threadId ? (IsUIThread(ec->threadId) ? " (UI)":" (non-UI)"):"")
         << ", hChart="               <<                     (ec->hChart       ? asformat("0x%p", ec->hChart       ) : "NULL")
         << ", hChartWindow="         <<                     (ec->hChartWindow ? asformat("0x%p", ec->hChartWindow ) : "NULL")

         << ", recordMode="           <<                      ec->recordMode

         << ", test="                 <<                     (ec->test ? asformat("0x%p", ec->test) : "NULL")
         << ", testing="              <<            BoolToStr(ec->testing)
         << ", visualMode="           <<            BoolToStr(ec->visualMode)
         << ", optimization="         <<            BoolToStr(ec->optimization)
         << ", externalReporting="    <<            BoolToStr(ec->externalReporting)

         << ", mqlError="             <<                    (!ec->mqlError   ? "0" : ErrorToStrA(ec->mqlError  ))
         << ", dllError="             <<                    (!ec->dllError   ? "0" : ErrorToStrA(ec->dllError  ))
         << ", dllWarning="           <<                    (!ec->dllWarning ? "0" : ErrorToStrA(ec->dllWarning))

         << ", loglevel="             << LoglevelDescriptionA(ec->loglevel)
         << ", loglevelTerminal="     << LoglevelDescriptionA(ec->loglevelTerminal)
         << ", loglevelAlert="        << LoglevelDescriptionA(ec->loglevelAlert)
         << ", loglevelDebugger="     << LoglevelDescriptionA(ec->loglevelDebugger)
         << ", loglevelFile="         << LoglevelDescriptionA(ec->loglevelFile)
         << ", loglevelMail="         << LoglevelDescriptionA(ec->loglevelMail)
         << ", loglevelSMS="          << LoglevelDescriptionA(ec->loglevelSMS)
         << ", logger="               <<                     (ec->logger    ? asformat("0x%p", ec->logger) : "NULL")
         << ", logBuffer="            <<              "(" << (ec->logBuffer ? ec->logBuffer->size() : 0) << ")"
         << ", logFilename="          <<       DoubleQuoteStr(ec->logFilename)
         << "}";
   }
   ss << asformat(" (0x%p)", ec);
   char* result = strdup(ss.str().c_str());                             // TODO: add to GC (close memory leak)

   return(result);
   #pragma EXPANDER_EXPORT
}


/**
 * Alias of EXECUTION_CONTEXT_toStr() with a different MQL import signature
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char*
 */
const char* WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec) {
   return(EXECUTION_CONTEXT_toStr(ec));
   #pragma EXPANDER_EXPORT
}
