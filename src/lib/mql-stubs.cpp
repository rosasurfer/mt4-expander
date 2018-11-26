/**
 * Empty function stubs for optional MQL functions. These stubs can be overwritten in MQL by custom implementations.
 */
#include "expander.h"
#include "lib/executioncontext.h"
#include "struct/mt4/MqlStr.h"


extern std::vector<ContextChain> g_contextChains;     // all context chains, i.e. MQL programs (index = program id)


/**
 * Initialization functions
 */
int WINAPI onInit() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_User() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_Template() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_Program() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_ProgramAfterTest() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_Parameters() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_SymbolChange() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_TimeframeChange() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_Recompile() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI afterInit() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Main functions
 */
int WINAPI onStart() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onTick() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Deinitialization functions
 */
int WINAPI onDeinit() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitAccountChange() {
   uint pid = GetLastThreadProgram();
   if (pid) {
      EXECUTION_CONTEXT* ec = g_contextChains[pid][0];
      if (ec->programType == PT_EXPERT) {
         return(_int(ERR_RUNTIME_ERROR, error(ERR_RUNTIME_ERROR, "unexpected uninitialize reason UR_ACCOUNT:  ec=%s", EXECUTION_CONTEXT_toStr(ec))));
      }
   }
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitChartChange() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitChartClose() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitParameterChange() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitRecompile() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitRemove() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitUndefined() {
   uint pid = GetLastThreadProgram();
   if (pid) {
      EXECUTION_CONTEXT* ec = g_contextChains[pid][0];
      if (ec->programType==PT_EXPERT && ec->testing) {
         return(_int(ERR_RUNTIME_ERROR, error(ERR_RUNTIME_ERROR, "unexpected uninitialize reason UR_UNDEFINED:  ec=%s", EXECUTION_CONTEXT_toStr(ec))));
      }
   }
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

// builds > 509
int WINAPI onDeinitClose() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitFailed() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onDeinitTemplate() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI afterDeinit() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Other
 */
void WINAPI DummyCalls() {
   return;
   #pragma EXPANDER_EXPORT
}

BOOL WINAPI EventListener_ChartCommand(const MqlStr commands[]) {
   return(error(ERR_NOT_IMPLEMENTED, ""));
   #pragma EXPANDER_EXPORT
}

const char* WINAPI InputsToStr() {
   return("");
   #pragma EXPANDER_EXPORT
}

int WINAPI ShowStatus(int error) {
   return(error);
   #pragma EXPANDER_EXPORT
}
