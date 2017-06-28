#pragma once

/**
 * XTrade struct LOG_MESSAGE
 *
 * Wrapper für eine nach MQL zu übermittelnde Logmessage.
 *
 *
 * TODO: in EXECUTION_CONTEXT integrieren
 */
#pragma pack(push, 1)
struct LOG_MESSAGE {
   int   level;                                    // Loglevel: L_ERROR, L_WARN oder L_INFO
   int   code;
   char* text;                                     // szchar
};
#pragma pack(pop)


// in EXECUTION_CONTEXT:
// ---------------------
// LOG_MESSAGE**      dllErrors;                   //  4    Array von Logmessages des Typs L_ERROR (variabel)   => welche DLL-Fehler sind aufgetreten
// uint               dllErrorsSize;               //  4    Anzahl von Logmessages (Arraygröße)    (variabel)   => wieviele DLL-Fehler sind aufgetreten
