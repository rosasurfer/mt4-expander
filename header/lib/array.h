#pragma once
#include "expander.h"


template <typename T>
BOOL WINAPI InitializeArray(T values[], int size, T initValue, int from, int count=INT_MAX);

BOOL WINAPI ShiftIntIndicatorBuffer   (int    buffer[], int size, int count, int    emptyValue);
BOOL WINAPI ShiftDoubleIndicatorBuffer(double buffer[], int size, int count, double emptyValue);
