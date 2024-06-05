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
 * Return an EXECUTION_CONTEXT's program name.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char* - program name
 */
const char* WINAPI ec_ProgramName(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   return(ec->programName);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the program name of the linked super context (if any).
 *
 * @param  uint pid - pid of the program
 *
 * @return char* - program name
 */
const char* WINAPI ec_SuperProgramName(uint pid) {
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
 * @param  uint pid - pid of the program
 *
 * @return int - loglevel
 */
int WINAPI ec_SuperLoglevel(uint pid) {
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
 * Return the loglevel for the debug output appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the program
 *
 * @return int - loglevel
 */
int WINAPI ec_SuperLoglevelDebug(uint pid) {
   if (pid && g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];
      EXECUTION_CONTEXT* master = chain[0];

      if (master && master->superContext) {
         return(master->superContext->loglevelDebug);
      }
   }
   return(NULL);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the loglevel for the terminal log appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the program
 *
 * @return int - loglevel
 */
int WINAPI ec_SuperLoglevelTerminal(uint pid) {
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
 * @param  uint pid - pid of the program
 *
 * @return int - loglevel
 */
int WINAPI ec_SuperLoglevelAlert(uint pid) {
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
 * Return the loglevel for the custom logfile appender of the linked super context (if any).
 *
 * @param  uint pid - pid of the program
 *
 * @return int - loglevel
 */
int WINAPI ec_SuperLoglevelFile(uint pid) {
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
 * @param  uint pid - pid of the program
 *
 * @return int - loglevel
 */
int WINAPI ec_SuperLoglevelMail(uint pid) {
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
 * @param  uint pid - pid of the program
 *
 * @return int - loglevel
 */
int WINAPI ec_SuperLoglevelSMS(uint pid) {
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
 * Set EXECUTION_CONTEXT.programType and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  ProgramType        type
 *
 * @return ProgramType - the same type or NULL in case of errors
 */
ProgramType WINAPI ec_SetProgramType(EXECUTION_CONTEXT* ec, ProgramType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ProgramType)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (type) {
      case PT_INDICATOR:
      case PT_EXPERT:
      case PT_SCRIPT: break;
      default: return((ProgramType)!error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ProgramType)", type));
   }
   uint pid = ec->pid;
   if (!pid)                         return((ProgramType)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((ProgramType)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->programType = type;
            }
         }
         return(type);
      }
   }
   return((ProgramType)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.programName and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - the same name or a NULL pointer in case of errors
 */
const char* WINAPI ec_SetProgramName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)                    return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)                    return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name || strlen(name) >= sizeof(ec->programName)) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"%s\" (must be 1 to %d chars long)", name, sizeof(ec->programName)-1));

   uint pid = ec->pid;
   if (!pid)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               if (!strcpy(chain[i]->programName, name)) return(NULL);
            }
         }
         return(name);
      }
   }
   return((char*)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.programInitReason and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  InitializeReason   reason
 *
 * @return InitializeReason - the same reason or NULL in case of errors
 */
InitializeReason WINAPI ec_SetProgramInitReason(EXECUTION_CONTEXT* ec, InitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((InitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (reason) {
      case IR_USER:
      case IR_TEMPLATE:
      case IR_PROGRAM:
      case IR_PROGRAM_AFTERTEST:
      case IR_PARAMETERS:
      case IR_TIMEFRAMECHANGE:
      case IR_SYMBOLCHANGE:
      case IR_ACCOUNTCHANGE:
      case IR_RECOMPILE:
      case IR_TERMINAL_FAILURE:
         break;
      default: return((InitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an InitializeReason)", reason));
   }
   uint pid = ec->pid;
   if (!pid)                         return((InitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((InitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->programInitReason = reason;
            }
         }
         return(reason);
      }
   }
   return((InitializeReason)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.programUninitReason and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  UninitializeReason reason
 *
 * @return UninitializeReason - the same reason or NULL in case of errors
 */
UninitializeReason WINAPI ec_SetProgramUninitReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (reason) {
      case UR_UNDEFINED:
      case UR_REMOVE:
      case UR_RECOMPILE:
      case UR_CHARTCHANGE:
      case UR_CHARTCLOSE:
      case UR_PARAMETERS:
      case UR_ACCOUNT:
      case UR_TEMPLATE:
      case UR_CLOSE:
      case UR_INITFAILED: break;
      default: return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an UninitializeReason)", reason));
   }
   uint pid = ec->pid;
   if (!pid)                         return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->programUninitReason = reason;
            }
         }
         return(reason);
      }
   }
   return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.programCoreFunction and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  CoreFunction       id
 *
 * @return CoreFunction - the same id or NULL in case of errors
 */
CoreFunction WINAPI ec_SetProgramCoreFunction(EXECUTION_CONTEXT* ec, CoreFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (id) {
      case CF_INIT:
      case CF_START:
      case CF_DEINIT: break;
      default: return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not a CoreFunction id)", id));
   }
   uint pid = ec->pid;
   if (!pid)                         return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->programCoreFunction = id;
            }
         }
         return(id);
      }
   }
   return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.programInitFlags and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags or NULL in case of errors
 */
DWORD WINAPI ec_SetProgramInitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->programInitFlags = flags;
            }
         }
         return(flags);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.programDeinitFlags and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags or NULL in case of errors
 */
DWORD WINAPI ec_SetProgramDeinitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->programDeinitFlags = flags;
            }
         }
         return(flags);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.moduleType. If the passed context is the main context also update the master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  ModuleType         type
 *
 * @return ModuleType - the same module type or NULL in case of errors
 */
ModuleType WINAPI ec_SetModuleType(EXECUTION_CONTEXT* ec, ModuleType type) {
   if ((uint)ec < MIN_VALID_POINTER) return((ModuleType)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (type) {
      case MT_INDICATOR:
      case MT_EXPERT:
      case MT_SCRIPT:
      case MT_LIBRARY: break;
      default: return((ModuleType)!error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ModuleType)", type));
   }

   uint pid = ec->pid;
   if (!pid)                         return((ModuleType)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((ModuleType)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         ec->moduleType = type;                       // update context
         if (i==1 && chain[0]) {
            chain[0]->moduleType = type;              // update master
         }
         return(type);
      }
   }
   return((ModuleType)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.moduleName. If the passed context is the main context also update the master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              name
 *
 * @return char* - the same name or a NULL pointer in case of errors
 */
const char* WINAPI ec_SetModuleName(EXECUTION_CONTEXT* ec, const char* name) {
   if ((uint)ec   < MIN_VALID_POINTER)                   return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)name < MIN_VALID_POINTER)                   return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));
   if (!*name || strlen(name) >= sizeof(ec->moduleName)) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter name: \"%s\" (must be 1 to %d chars long)", name, sizeof(ec->moduleName)-1));

   uint pid = ec->pid;
   if (!pid)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                                          // context found
         if (!strcpy(ec->moduleName, name)) return(NULL);            // update context
         if (i==1 && chain[0]) {
            if (!strcpy(chain[0]->moduleName, name)) return(NULL);   // update master
         }
         return(name);
      }
   }
   return((char*)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.moduleUninitReason. If the passed context is the main context also update the master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  UninitializeReason reason
 *
 * @return UninitializeReason - the same reason or NULL in case of errors
 */
UninitializeReason WINAPI ec_SetModuleUninitReason(EXECUTION_CONTEXT* ec, UninitializeReason reason) {
   if ((uint)ec < MIN_VALID_POINTER) return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (reason) {
      case UR_UNDEFINED:
      case UR_REMOVE:
      case UR_RECOMPILE:
      case UR_CHARTCHANGE:
      case UR_CHARTCLOSE:
      case UR_PARAMETERS:
      case UR_ACCOUNT:
      case UR_TEMPLATE:
      case UR_CLOSE:
      case UR_INITFAILED: break;
      default: return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an UninitializeReason)", reason));
   }

   uint pid = ec->pid;
   if (!pid)                         return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         ec->moduleUninitReason = reason;             // update context
         if (i==1 && chain[0]) {
            chain[0]->moduleUninitReason = reason;    // update master
         }
         return(reason);
      }
   }
   return((UninitializeReason)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.moduleCoreFunction. If the passed context is the main context also update the master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  CoreFunction       id
 *
 * @return CoreFunction - the same id or NULL in case of errors
 */
CoreFunction WINAPI ec_SetModuleCoreFunction(EXECUTION_CONTEXT* ec, CoreFunction id) {
   if ((uint)ec < MIN_VALID_POINTER) return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   switch (id) {
      case CF_INIT:
      case CF_START:
      case CF_DEINIT: break;
      default: return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not a CoreFunction id)", id));
   }

   uint pid = ec->pid;
   if (!pid)                         return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         ec->moduleCoreFunction = id;                 // update context
         if (i==1 && chain[0]) {
            chain[0]->moduleCoreFunction = id;        // update master
         }
         return(id);
      }
   }
   return((CoreFunction)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.moduleInitFlags. If the passed context is the main context also update the master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags or NULL in case of errors
 */
DWORD WINAPI ec_SetModuleInitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         ec->moduleInitFlags = flags;                 // update context
         if (i==1 && chain[0]) {
            chain[0]->moduleInitFlags = flags;        // update master
         }
         return(flags);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.moduleDeinitFlags. If the passed context is the main context also update the master context.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              flags
 *
 * @return DWORD - the same flags or NULL in case of errors
 */
DWORD WINAPI ec_SetModuleDeinitFlags(EXECUTION_CONTEXT* ec, DWORD flags) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         ec->moduleDeinitFlags = flags;               // update context
         if (i==1 && chain[0]) {
            chain[0]->moduleDeinitFlags = flags;      // update master
         }
         return(flags);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.symbol and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              symbol
 *
 * @return char* - the same symbol or a NULL pointer in case of errors
 */
const char* WINAPI ec_SetSymbol(EXECUTION_CONTEXT* ec, const char* symbol) {
   if ((uint)ec     < MIN_VALID_POINTER)               return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)symbol < MIN_VALID_POINTER)               return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol));
   if (!*symbol || strlen(symbol) > MAX_SYMBOL_LENGTH) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter symbol: \"%s\" (must be 1 to %d chars long)", symbol, MAX_SYMBOL_LENGTH));

   uint pid = ec->pid;
   if (!pid)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               if (!strcpy(chain[i]->symbol, symbol)) return(NULL);
            }
         }
         return(symbol);
      }
   }
   return((char*)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.timeframe and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               timeframe
 *
 * @return uint - the same timeframe or NULL in case of errors
 */
uint WINAPI ec_SetTimeframe(EXECUTION_CONTEXT* ec, uint timeframe) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)timeframe <= 0)          return(!error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d (must be positive)", timeframe));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->timeframe = timeframe;
            }
         }
         return(timeframe);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.bars and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                bars
 *
 * @return int - the same bars value or NULL in case of errors
 */
int WINAPI ec_SetBars(EXECUTION_CONTEXT* ec, int bars) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (bars < 0)                     return(!error(ERR_INVALID_PARAMETER, "invalid parameter bars: %d (must be non-negative)", bars));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->bars = bars;
            }
         }
         return(bars);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.validBars and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                validBars
 *
 * @return int - the same validBars value or NULL in case of errors
 */
int WINAPI ec_SetValidBars(EXECUTION_CONTEXT* ec, int validBars) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (validBars < -1)               return(!error(ERR_INVALID_PARAMETER, "invalid parameter validBars: %d (can't be smaller than -1)", validBars));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->validBars = validBars;
            }
         }
         return(validBars);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.changedBars and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                changedBars
 *
 * @return int - the same changedBars value or NULL in case of errors
 */
int WINAPI ec_SetChangedBars(EXECUTION_CONTEXT* ec, int changedBars) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (changedBars < -1)             return(!error(ERR_INVALID_PARAMETER, "invalid parameter changedBars: %d (can't be smaller than -1)", changedBars));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->changedBars = changedBars;
            }
         }
         return(changedBars);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.digits and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               digits
 *
 * @return uint - the same digits value of NULL in case of errors
 */
uint WINAPI ec_SetDigits(EXECUTION_CONTEXT* ec, uint digits) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)digits < 0)              return(!error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d (must be non-negative)", digits));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->digits = digits;
            }
         }
         return(digits);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.pipDigits and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               pipDigits
 *
 * @return uint - the same pipDigits value or NULL in case of errors
 */
uint WINAPI ec_SetPipDigits(EXECUTION_CONTEXT* ec, uint pipDigits) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)pipDigits < 0)           return(!error(ERR_INVALID_PARAMETER, "invalid parameter pipDigits: %d (must be non-negative)", pipDigits));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->pipDigits = pipDigits;
            }
         }
         return(pipDigits);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.pip and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  double             size - pip size
 *
 * @return double - the same pip size value or NULL in case of errors
 */
double WINAPI ec_SetPip(EXECUTION_CONTEXT* ec, double size) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (size <= 0)                    return(!error(ERR_INVALID_PARAMETER, "invalid parameter size: %f (must be > 0)", size));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->pip = size;
            }
         }
         return(size);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.point and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  double             size - MQL point size
 *
 * @return double - the same point size or NULL in case of errors
 */
double WINAPI ec_SetPoint(EXECUTION_CONTEXT* ec, double size) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (size <= 0)                    return(!error(ERR_INVALID_PARAMETER, "invalid parameter size: %f (must be > 0)", size));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->point = size;
            }
         }
         return(size);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.superContext and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec  - a program's execution context
 * @param  EXECUTION_CONTEXT* sec - a program's super context
 *
 * @return EXECUTION_CONTEXT* - the same super context or NULL in case of errors
 */
EXECUTION_CONTEXT* WINAPI ec_SetSuperContext(EXECUTION_CONTEXT* ec, EXECUTION_CONTEXT* sec) {
   if ((uint)ec         < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (sec && (uint)sec < MIN_VALID_POINTER) return((EXECUTION_CONTEXT*)!error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec));

   uint pid = ec->pid;
   if (!pid)                         return((EXECUTION_CONTEXT*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((EXECUTION_CONTEXT*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->superContext = sec;
            }
         }
         return(sec);
      }
   }
   return((EXECUTION_CONTEXT*)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.threadId and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  uint               id - thread id
 *
 * @return uint - the same thread id or NULL in case of errors
 */
uint WINAPI ec_SetThreadId(EXECUTION_CONTEXT* ec, uint id) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)id <= 0)                 return(!error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (must be > 0)", id));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->threadId = id;
            }
         }
         return(id);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.chartWindow and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  HWND               hWnd - handle of chart window with title bar "Symbol,Period"
 *
 * @return HWND - the same handle or NULL in case of errors
 */
HWND WINAPI ec_SetChartWindow(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)hWnd <= 0)               return((HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: %d (not a valid handle)", hWnd));

   uint pid = ec->pid;
   if (!pid)                         return((HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->chartWindow = hWnd;
            }
         }
         return(hWnd);
      }
   }
   return((HWND)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.chart and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  HWND               hWnd - handle of embedded chart AfxFrame; same as MQL::WindowHandle()
 *
 * @return HWND - the same handle or NULL in case of errors
 */
HWND WINAPI ec_SetChart(EXECUTION_CONTEXT* ec, HWND hWnd) {
   if ((uint)ec < MIN_VALID_POINTER) return((HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((int)hWnd <= 0)               return((HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter hWnd: %d (not a valid handle)", hWnd));

   uint pid = ec->pid;
   if (!pid)                         return((HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((HWND)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->chart = hWnd;
            }
         }
         return(hWnd);
      }
   }
   return((HWND)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.testing and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status or FALSE in case of errors
 */
BOOL WINAPI ec_SetTesting(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->testing = status;
            }
         }
         return(status);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.visualMode and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status or FALSE in case of errors
 */
BOOL WINAPI ec_SetVisualMode(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->visualMode = status;
            }
         }
         return(status);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.optimization and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               status
 *
 * @return BOOL - the same status or FALSE in case of errors
 */
BOOL WINAPI ec_SetOptimization(EXECUTION_CONTEXT* ec, BOOL status) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->optimization = status;
            }
         }
         return(status);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.recorder and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                mode
 *
 * @return int - the same mode or NULL in case of errors
 */
int WINAPI ec_SetRecorder(EXECUTION_CONTEXT* ec, int mode) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (mode < 0)                     return(!error(ERR_INVALID_PARAMETER, "invalid parameter mode: %d (must be non-negative)", mode));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->recorder = mode;
            }
         }
         return(mode);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.accountServer and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              server - server name (an empty string is converted to a NULL pointer)
 *
 * @return char* - the same server name or a NULL pointer in case of errors
 */
const char* WINAPI ec_SetAccountServer(EXECUTION_CONTEXT* ec, const char* server) {
   if ((uint)ec     < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if ((uint)server < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter server: 0x%p (not a valid pointer)", server));

   uint pid = ec->pid;
   if (!pid)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   const char* _server = (server && *server) ? server : NULL;  // empty string => NULL

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                                    // context found
         if (EXECUTION_CONTEXT* master = chain[0]) {
            // update master
            if (!StrCompare(master->accountServer, _server)) {
               if (master->accountServer) {                    // free an existing string
                  free(master->accountServer);
               }
               master->accountServer = _server ? strdup(_server) : NULL;
            }
            // update all other program modules (ignore existing strings; must not be different anyway)
            for (i=1; i < chainSize; i++) {
               if (chain[i]) {
                  chain[i]->accountServer = master->accountServer;
               }
            }
         }
         return(server);
      }
   }
   return((char*)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.accountNumber and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                number - account number
 *
 * @return int - the same account number or NULL in case of errors
 */
int WINAPI ec_SetAccountNumber(EXECUTION_CONTEXT* ec, int number) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (number < 0)                   return(!error(ERR_INVALID_PARAMETER, "invalid parameter number: %d (must be > 0)", number));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->accountNumber = number;
            }
         }
         return(number);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.dllWarning.
 *
 * - If a main context is passed the warning is propagated to the master context (incl. resetting i.e. ERR_NO_ERROR).
 * - If a library context is passed the warning is propagated to main and master context (except resetting i.e. ERR_NO_ERROR).
 * - If a context of an indicator loaded by iCustom() is passed the warning is propagated to the host program's main and master
 *   context (except resetting i.e. ERR_NO_ERROR).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error - the warning's error code
 *
 * @return int - the same warning or NULL in case of errors
 */
int WINAPI ec_SetDllWarning(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();
   EXECUTION_CONTEXT* master = (chainSize > 0) ? chain[0] : NULL;
   EXECUTION_CONTEXT* main   = (chainSize > 1) ? chain[1] : NULL;

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                                 // context found
         ec->dllWarning = error;                            // update the context itself
         if (ec == main) {
            if (master) master->dllWarning = error;         // update master context
         }
         else if (ec->moduleType==MT_LIBRARY && error) {    // propagation from library to master/main context
            if (master) master->dllWarning = error;
            if (main)   main->dllWarning = error;
         }
         if (ec->superContext && error) {                   // propagation from iCustom() to host program
            ec_SetDllWarning(ec->superContext, error);
         }
         return(error);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.dllError.
 *
 * - If a main context is passed the error is propagated to the master context (incl. resetting i.e. ERR_NO_ERROR).
 * - If a library context is passed the error is propagated to main and master context (except resetting i.e. ERR_NO_ERROR).
 * - If a context of an indicator loaded by iCustom() is passed the error is propagated to the host program's main and master
 *   context (except resetting i.e. ERR_NO_ERROR).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error - error code
 *
 * @return int - the same error or NULL in case of errors
 */
int WINAPI ec_SetDllError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();
   EXECUTION_CONTEXT* master = (chainSize > 0) ? chain[0] : NULL;
   EXECUTION_CONTEXT* main   = (chainSize > 1) ? chain[1] : NULL;

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                                 // context found
         ec->dllError = error;                              // update the context itself
         if (ec == main) {
            if (master) master->dllError = error;           // update master context
         }
         else if (ec->moduleType==MT_LIBRARY && error) {    // propagation from library to master/main context
            if (master) master->dllError = error;
            if (main)   main->dllError = error;
         }
         if (ec->superContext && error) {                   // propagation from iCustom() to host program
            ec_SetDllError(ec->superContext, error);
         }
         return(error);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.mqlError.
 *
 * - If a main context is passed the error is propagated to the master context (incl. resetting i.e. ERR_NO_ERROR).
 * - If a library context is passed the error is propagated to main and master context (except resetting i.e. ERR_NO_ERROR).
 * - If a context of an indicator loaded by iCustom() is passed the error is propagated to the host program's main and master
 *   context (except resetting i.e. ERR_NO_ERROR).
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                error - error code
 *
 * @return int - the same error or NULL in case of errors
 */
int WINAPI ec_SetMqlError(EXECUTION_CONTEXT* ec, int error) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();
   EXECUTION_CONTEXT* master = (chainSize > 0) ? chain[0] : NULL;
   EXECUTION_CONTEXT* main   = (chainSize > 1) ? chain[1] : NULL;

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                                 // context found
         ec->mqlError = error;                              // update the context itself
         if (ec == main) {
            if (master) master->dllError = error;           // update master context
         }
         else if (ec->moduleType==MT_LIBRARY && error) {    // propagation from library to master/main context
            if (master) master->mqlError = error;
            if (main)   main->mqlError = error;
         }
         if (ec->superContext && error) {                   // propagation from iCustom() to host program
            ec_SetMqlError(ec->superContext, error);
         }
         return(error);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.debugOptions and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  DWORD              options
 *
 * @return DWORD - the same options or NULL in case of errors
 */
DWORD WINAPI ec_SetDebugOptions(EXECUTION_CONTEXT* ec, DWORD options) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->debugOptions = options;
            }
         }
         return(options);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Set EXECUTION_CONTEXT.loglevel and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel or NULL in case of errors
 */
int WINAPI ec_SetLoglevel(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->loglevel = level;
            }
         }
         if (!level || level==LOG_OFF) {
            EXECUTION_CONTEXT* master = chain[0];
            if (master && master->logger && master->logger->is_open()) {
               master->logger->close();               // close an open logfile if logging was disabled
            }
         }
         return(level);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.loglevelDebug and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel or NULL in case of errors
 */
int WINAPI ec_SetLoglevelDebug(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->loglevelDebug = level;
            }
         }
         return(level);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.loglevelTerminal and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel or NULL in case of errors
 */
int WINAPI ec_SetLoglevelTerminal(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->loglevelTerminal = level;
            }
         }
         return(level);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.loglevelAlert and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel or NULL in case of errors
 */
int WINAPI ec_SetLoglevelAlert(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->loglevelAlert = level;
            }
         }
         return(level);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.loglevelFile and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel or NULL in case of errors
 */
int WINAPI ec_SetLoglevelFile(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->loglevelFile = level;
            }
         }
         if (!level || level==LOG_OFF) {
            EXECUTION_CONTEXT* master = chain[0];
            if (master && master->logger && master->logger->is_open()) {
               master->logger->close();               // close an open logfile if the logfile appender was disabled
            }
         }
         return(level);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.loglevelMail and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel or NULL in case of errors
 */
int WINAPI ec_SetLoglevelMail(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->loglevelMail = level;
            }
         }
         return(level);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.loglevelSMS and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  int                level - loglevel
 *
 * @return int - the same loglevel or NULL in case of errors
 */
int WINAPI ec_SetLoglevelSMS(EXECUTION_CONTEXT* ec, int level) {
   if ((uint)ec < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   uint pid = ec->pid;
   if (!pid)                         return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return(!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                           // context found
         for (i=0; i < chainSize; i++) {              // update all program modules
            if (chain[i]) {
               chain[i]->loglevelSMS = level;
            }
         }
         return(level);
      }
   }
   return(!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
   #pragma EXPANDER_EXPORT
}


/**
 * Set EXECUTION_CONTEXT.logFilename and update all MQL modules of the program.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  char*              filename - filename (an empty string or a NULL pointer reset the field)
 *
 * @return char* - the same filename or a NULL pointer in case of errors
 */
const char* WINAPI ec_SetLogFilename(EXECUTION_CONTEXT* ec, const char* filename) {
   if ((uint)ec < MIN_VALID_POINTER)          return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));
   if (filename) {
      if ((uint)filename < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter filename: 0x%p (not a valid pointer)", filename));
   }

   uint pid = ec->pid;
   if (!pid)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (not a program id)", pid));
   if (g_mqlInstances.size() <= pid) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec.pid: %d (program instance not found)", pid));

   const char* _filename = (filename && *filename) ? filename : NULL;   // empty string => NULL

   ContextChain &chain = *g_mqlInstances[pid];
   size_t chainSize = chain.size();

   for (size_t i=0; i < chainSize; i++) {
      if (chain[i] == ec) {                                             // context found
         if (EXECUTION_CONTEXT* master = chain[0]) {
            // update master
            if (!StrCompare(master->logFilename, _filename)) {
               if (master->logFilename) {                               // free an existing string
                  free(master->logFilename);
               }
               master->logFilename = _filename ? strdup(_filename) : NULL;
            }
            // update all other program modules (ignore existing strings; must not be different anyway)
            for (i=1; i < chainSize; i++) {
               if (chain[i]) {
                  chain[i]->logFilename = master->logFilename;
               }
            }
         }
         return(filename);
      }
   }
   return((char*)!error(ERR_INVALID_PARAMETER, "invalid EXECUTION_CONTEXT: 0x%p (not a context of program instance %d), ec=%s", ec, pid, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Return a human-readable version of an EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char*
 */
const char* WINAPI EXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec));

   std::ostringstream ss;
   EXECUTION_CONTEXT empty = {};

   if (MemCompare(ec, &empty, sizeof(EXECUTION_CONTEXT))) {
      ss << "{}";
   }
   else {
      ss << std::fixed
         <<  "{pid="                  <<                      ec->pid
         << ", previousPid="          <<                      ec->previousPid

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
         << ", rates="                <<                     (ec->rates ? asformat("0x%p", ec->rates) : "(null)")
         << ", bars="                 <<                      ec->bars
         << ", validBars="            <<                      ec->validBars
         << ", changedBars="          <<                      ec->changedBars
         << ", ticks="                <<                      ec->ticks
         << ", cycleTicks="           <<                      ec->cycleTicks
         << ", currTickTime="         <<                     (ec->currTickTime ? GmtTimeFormatA(ec->currTickTime, "\"%Y.%m.%d %H:%M:%S\"") : "0")
         << ", prevTickTime="         <<                     (ec->prevTickTime ? GmtTimeFormatA(ec->prevTickTime, "\"%Y.%m.%d %H:%M:%S\"") : "0")

         << ", digits="               <<                      ec->digits
         << ", pipDigits="            <<                      ec->pipDigits
         << ", pip="   << std::setprecision(ec->pipDigits) << ec->pip
         << ", point=" << std::setprecision(ec->digits)    << ec->point

         << ", superContext="         <<                     (ec->superContext ? asformat("0x%p", ec->superContext) : "(null)")
         << ", threadId="             <<                      ec->threadId << (ec->threadId ? (IsUIThread(ec->threadId) ? " (UI)":" (non-UI)"):"")
         << ", chartWindow="          <<                     (ec->chartWindow ? asformat("0x%p", ec->chartWindow ) : "(null)")
         << ", chart="                <<                     (ec->chart       ? asformat("0x%p", ec->chart       ) : "(null)")

         << ", testing="              <<            BoolToStr(ec->testing)
         << ", visualMode="           <<            BoolToStr(ec->visualMode)
         << ", optimization="         <<            BoolToStr(ec->optimization)
         << ", recorder="             <<                      ec->recorder

         << ", accountServer="        <<       DoubleQuoteStr(ec->accountServer)
         << ", accountNumber="        <<                      ec->accountNumber

         << ", dllWarning="           <<                    (!ec->dllWarning ? "0" : ErrorToStrA(ec->dllWarning))
         << ", dllError="             <<                    (!ec->dllError   ? "0" : ErrorToStrA(ec->dllError  ))
         << ", mqlError="             <<                    (!ec->mqlError   ? "0" : ErrorToStrA(ec->mqlError  ))

         << ", debugOptions="         <<                      ec->debugOptions
         << ", loglevel="             << LoglevelDescriptionA(ec->loglevel)
         << ", loglevelDebug="        << LoglevelDescriptionA(ec->loglevelDebug)
         << ", loglevelTerminal="     << LoglevelDescriptionA(ec->loglevelTerminal)
         << ", loglevelAlert="        << LoglevelDescriptionA(ec->loglevelAlert)
         << ", loglevelFile="         << LoglevelDescriptionA(ec->loglevelFile)
         << ", loglevelMail="         << LoglevelDescriptionA(ec->loglevelMail)
         << ", loglevelSMS="          << LoglevelDescriptionA(ec->loglevelSMS)
         << ", logger="               <<                     (ec->logger    ? asformat("0x%p", ec->logger) : "(null)")
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
 * Alias of EXECUTION_CONTEXT_toStr() with a different MQL signature.
 *
 * @param  EXECUTION_CONTEXT* ec
 *
 * @return char*
 */
const char* WINAPI lpEXECUTION_CONTEXT_toStr(const EXECUTION_CONTEXT* ec) {
   return(EXECUTION_CONTEXT_toStr(ec));
   #pragma EXPANDER_EXPORT
}
