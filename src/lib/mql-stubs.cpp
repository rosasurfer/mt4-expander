/**
 * Empty function stubs for optional MQL functions. Can be overridden in MQL by custom implementations.
 */
#include "expander.h"


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

int WINAPI afterDeinit() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Logging and status functions
 */
char* WINAPI InputsToStr() {
   return("");
   #pragma EXPANDER_EXPORT
}

int WINAPI ShowStatus(int error) {
   return(error);
   #pragma EXPANDER_EXPORT
}


/**
 * Other
 */
void WINAPI DummyCalls() {
   return;
   #pragma EXPANDER_EXPORT
}
