/**
 * Win32 structure SECURITY_ATTRIBUTES
 *
 *
 * struct SECURITY_ATTRIBUTES {
 *    DWORD  nLength;                     //  4
 *    LPVOID lpSecurityDescriptor;        //  4
 *    BOOL   bInheritHandle;              //  4
 * } sa;                                  // 12 byte
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt die Größe der SECURITY_ATTRIBUTES-Structure in Bytes zurück.
 *
 * @param  SECURITY_ATTRIBUTES* sa
 *
 * @return DWORD
 */
DWORD WINAPI sa_Length(const SECURITY_ATTRIBUTES* sa) {
   if ((uint)sa < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter sa = 0x%p (not a valid pointer)", sa));
   return(sa->nLength);
   #pragma EXPORT
}


/**
 * Gibt den Zeiger auf den SECURITY_DESCRIPTOR der SECURITY_ATTRIBUTES-Structure zurück.
 *
 * @param  SECURITY_ATTRIBUTES* sa
 *
 * @return LPVOID
 */
LPVOID WINAPI sa_SecurityDescriptor(const SECURITY_ATTRIBUTES* sa) {
   if ((uint)sa < MIN_VALID_POINTER) return((LPVOID)debug("ERROR:  invalid parameter sa = 0x%p (not a valid pointer)", sa));
   return(sa->lpSecurityDescriptor);
   #pragma EXPORT
}


/**
 * Gibt den Inherited-Status des Handles eines mit diesen SECURITY_ATTRIBUTES erzeugten Prozesses zurück.
 *
 * @param  SECURITY_ATTRIBUTES* sa
 *
 * @return BOOL
 */
BOOL WINAPI sa_InheritHandle(const SECURITY_ATTRIBUTES* sa) {
   if ((uint)sa < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter sa = 0x%p (not a valid pointer)", sa));
   return(sa->bInheritHandle);
   #pragma EXPORT
}
