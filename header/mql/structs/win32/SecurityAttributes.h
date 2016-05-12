#pragma once

/**
 * Win32 struct SECURITY_ATTRIBUTES
 *
 *
 * struct SECURITY_ATTRIBUTES {
 *    DWORD  nLength;                     //  4
 *    LPVOID lpSecurityDescriptor;        //  4
 *    BOOL   bInheritHandle;              //  4
 * };                                     // 12 byte
 */
DWORD  WINAPI sa_Length            (const SECURITY_ATTRIBUTES* sa);
LPVOID WINAPI sa_SecurityDescriptor(const SECURITY_ATTRIBUTES* sa);
BOOL   WINAPI sa_InheritHandle     (const SECURITY_ATTRIBUTES* sa);
