#pragma once
/**
 * MQL-Interface zum Zugriff auf ein struct FILETIME.
 *
 *
 * struct FILETIME {
 *    DWORD dwLowDateTime;          //  4
 *    DWORD dwHighDateTime;         //  4
 * };                               //  8 byte
 */
DWORD WINAPI ft_LowDateTime (const FILETIME* ft);
DWORD WINAPI ft_HighDateTime(const FILETIME* ft);