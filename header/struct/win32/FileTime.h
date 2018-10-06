#pragma once
#include "stdafx.h"


/**
 * Win32 struct FILETIME
 *
 * struct FILETIME {
 *    DWORD dwLowDateTime;          //  4
 *    DWORD dwHighDateTime;         //  4
 * };                               //  8 byte
 */
DWORD WINAPI ft_LowDateTime (const FILETIME* ft);
DWORD WINAPI ft_HighDateTime(const FILETIME* ft);
