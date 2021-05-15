#pragma once
#include "expander.h"


BOOL WINAPI InitializeDoubleArray(double values[], int size, double initValue, int from, int count = INT_MAX);
BOOL WINAPI ShiftIndicatorBuffer(double buffer[], int size, int count, double emptyValue);
