/**
 * Empty function stubs for optional MQL functions. These stubs can be overwritten in MQL by custom implementations.
 */
#include "expander.h"
#include "lib/executioncontext.h"
#include "struct/mt4/MqlStr.h"


extern MqlProgramList g_mqlPrograms;            // all MQL programs: vector<ContextChain> with index = program id


/**
 * Initialization functions
 */
int WINAPI onInit() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onInitUser() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onInitParameters() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onInitSymbolChange() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onInitTimeframeChange() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onInitProgram() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onInitProgramAfterTest() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onInitTemplate() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onInitRecompile() {
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
   if (uint pid = GetLastThreadProgram()) {
      EXECUTION_CONTEXT* ec = (*g_mqlPrograms[pid])[0];

      if (ec->programType == PT_EXPERT) {
         error(ERR_ILLEGAL_STATE, "unexpected uninitialize reason UR_ACCOUNT:  ec=%s", EXECUTION_CONTEXT_toStr(ec));
         return(ERR_ILLEGAL_STATE);
      }
      if (ec->programType == PT_INDICATOR) {
         warn(ERR_ILLEGAL_STATE, "unexpected uninitialize reason UR_ACCOUNT:  ec=%s", EXECUTION_CONTEXT_toStr(ec));
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


// builds > 509
int WINAPI onDeinitClose() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


// builds > 509
int WINAPI onDeinitFailed() {
   if (uint pid = GetLastThreadProgram()) {
      EXECUTION_CONTEXT* ec = (*g_mqlPrograms[pid])[0];
      //warn(ERR_ILLEGAL_STATE, "unexpected uninitialize reason UR_INITFAILED:  ec=%s", EXECUTION_CONTEXT_toStr(ec));
   }
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onDeinitParameters() {
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


// builds > 509
int WINAPI onDeinitTemplate() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


int WINAPI onDeinitUndefined() {
   if (uint pid = GetLastThreadProgram()) {
      EXECUTION_CONTEXT* ec = (*g_mqlPrograms[pid])[0];
      if (ec->programType==PT_EXPERT && !ec->testing) {
         warn(ERR_ILLEGAL_STATE, "unexpected uninitialize reason UR_UNDEFINED:  ec=%s", EXECUTION_CONTEXT_toStr(ec));
      }
      if (ec->programType == PT_INDICATOR) {
         warn(ERR_ILLEGAL_STATE, "unexpected uninitialize reason UR_UNDEFINED:  ec=%s", EXECUTION_CONTEXT_toStr(ec));
      }
   }
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
