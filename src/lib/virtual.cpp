/**
 * Virtual no-ops. Over-written by MQL implementations of the same name.
 */
#include "expander.h"
#include "lib/executioncontext.h"
#include "struct/mt4/MqlString.h"

extern MqlInstanceList g_mqlInstances;          // all MQL program instances


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


int WINAPI onInitAccountChange() {
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
      EXECUTION_CONTEXT* ec = (*g_mqlInstances[pid])[0];
      warn(ERR_UNDEFINED_STATE, "unexpected uninitialize reason UR_ACCOUNT:  ec=%s", EXECUTION_CONTEXT_toStr(ec));
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
      EXECUTION_CONTEXT* ec = (*g_mqlInstances[pid])[0];
      if (ec->programType==PT_INDICATOR || (ec->programType==PT_EXPERT && !ec->testing)) {
         warn(ERR_UNDEFINED_STATE, "unexpected uninitialize reason UR_UNDEFINED:  ec=%s", EXECUTION_CONTEXT_toStr(ec));
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
 * EA recorder related functions
 */
const char* Recorder_GetInput() {
   return("");
   #pragma EXPANDER_EXPORT
}


const char* Recorder_GetNextMetricSymbol() {
   return("A.000");
   #pragma EXPANDER_EXPORT
}


BOOL Recorder_start() {
   return(FALSE);
   #pragma EXPANDER_EXPORT
}


BOOL Recorder_deinit() {
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


int RecordMetrics() {
   return(ERR_NOT_IMPLEMENTED);
   #pragma EXPANDER_EXPORT
}


/**
 * Other
 */
int WINAPI DeleteRegisteredObjects() {
   return(NULL);
   #pragma EXPANDER_EXPORT
}


void WINAPI DummyCalls() {
   return;
   #pragma EXPANDER_EXPORT
}


const char* WINAPI GetStatusFilenameData() {
   return("");
   #pragma EXPANDER_EXPORT
}


const char* WINAPI InputsToStr() {
   return("");
   #pragma EXPANDER_EXPORT
}


BOOL WINAPI RemoveChartLegend() {
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


int WINAPI ShowStatus(int error) {
   return(error);
   #pragma EXPANDER_EXPORT
}
