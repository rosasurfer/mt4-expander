#pragma once
#include "expander.h"


// separate template declarations/definitions to prevent a bloated binary
template <typename T>
BOOL WINAPI InitializeArray(T values[], int size, T initValue, int from, int count = INT_MAX);

template <typename T>
BOOL WINAPI ShiftIndicatorBuffer(T buffer[], int size, int count, T emptyValue);
