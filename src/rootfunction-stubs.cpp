/**
 * Globale Stubs für MQL-Init/Deinit/Main-Funktionen. Diese können im MQL-Programm nach Bedarf durch eigene Versionen
 * überschrieben werden.
 */
#include "header/expander.h"


/**
 * Initialisierung
 */
int WINAPI onInit() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onInit_User() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onInit_Template() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onInit_Program() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onInit_ProgramAfterTest() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onInit_Parameters() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onInit_TimeframeChange() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onInit_SymbolChange() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onInit_Recompile() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI afterInit() {
   return(NO_ERROR);
   #pragma EXPORT
}


/**
 * Mainfunktionen
 */
int WINAPI onStart() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI onTick() {
   return(NO_ERROR);
   #pragma EXPORT
}


/**
 * Deinitialisierung
 */
int WINAPI onDeinit() {
   return(NO_ERROR);
   #pragma EXPORT
}

int WINAPI afterDeinit() {
   return(NO_ERROR);
   #pragma EXPORT
}
