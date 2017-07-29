/**
 * Empty function stubs for MQL init/deinit/main functions. Can be overridden by implementations in MQL.
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

int WINAPI onInit_TimeframeChange() {
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}

int WINAPI onInit_SymbolChange() {
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
