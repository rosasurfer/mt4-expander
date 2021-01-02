#pragma once


/**
 * Framework struct LOG_MESSAGE
 *
 * Wrapper f�r eine nach MQL zu �bermittelnde Logmessage.
 *
 * TODO: in EXECUTION_CONTEXT integrieren
 */
#pragma pack(push, 1)

struct LOG_MESSAGE {
   int   level;                  // log level
   int   code;
   char* text;
};
#pragma pack(pop)


// in EXECUTION_CONTEXT:
// ---------------------
// LOG_MESSAGE**      dllErrors;                   //  4    Array von Logmessages des Typs LOG_ERROR (variabel)   => welche DLL-Fehler sind aufgetreten
// uint               dllErrorsSize;               //  4    Anzahl von Logmessages (Arraygr��e)      (variabel)   => wieviele DLL-Fehler sind aufgetreten
