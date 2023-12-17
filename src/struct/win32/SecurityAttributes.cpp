/**
 * Win32 struct SECURITY_ATTRIBUTES
 *
 * struct SECURITY_ATTRIBUTES {
 *    DWORD  nLength;                     //  4
 *    LPVOID lpSecurityDescriptor;        //  4
 *    BOOL   bInheritHandle;              //  4
 * };                                     // 12 byte
 */
#include "expander.h"


/**
 * Gibt die Gr��e der SECURITY_ATTRIBUTES-Structure in Bytes zur�ck.
 *
 * @param  SECURITY_ATTRIBUTES* sa
 *
 * @return DWORD
 */
DWORD WINAPI sa_Length(const SECURITY_ATTRIBUTES* sa) {
   if ((uint)sa < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter sa: 0x%p (not a valid pointer)", sa));
   return(sa->nLength);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Zeiger auf den SECURITY_DESCRIPTOR der SECURITY_ATTRIBUTES-Structure zur�ck.
 *
 * @param  SECURITY_ATTRIBUTES* sa
 *
 * @return LPVOID
 */
LPVOID WINAPI sa_SecurityDescriptor(const SECURITY_ATTRIBUTES* sa) {
   if ((uint)sa < MIN_VALID_POINTER) return((LPVOID)!error(ERR_INVALID_PARAMETER, "invalid parameter sa: 0x%p (not a valid pointer)", sa));
   return(sa->lpSecurityDescriptor);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Inherited-Status des Handles eines mit diesen SECURITY_ATTRIBUTES erzeugten Prozesses zur�ck.
 *
 * @param  SECURITY_ATTRIBUTES* sa
 *
 * @return BOOL
 */
BOOL WINAPI sa_InheritHandle(const SECURITY_ATTRIBUTES* sa) {
   if ((uint)sa < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter sa: 0x%p (not a valid pointer)", sa));
   return(sa->bInheritHandle);
   #pragma EXPANDER_EXPORT
}
