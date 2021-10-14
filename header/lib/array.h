#pragma once
#include "expander.h"


BOOL WINAPI InitializeIntArray   (int    values[], int size, int    initValue, int from, int count = INT_MAX);
BOOL WINAPI InitializeDoubleArray(double values[], int size, double initValue, int from, int count = INT_MAX);

BOOL WINAPI ShiftIntIndicatorBuffer   (int    buffer[], int size, int count, int    emptyValue);
BOOL WINAPI ShiftDoubleIndicatorBuffer(double buffer[], int size, int count, double emptyValue);
