#pragma once
#include "expander.h"


BOOL WINAPI InitializeDoubleArray(double values[], int size, double initValue, int from, int count = WHOLE_ARRAY);
BOOL WINAPI ShiftIndicatorBuffer(double buffer[], int bufferSize, int elements, double emptyValue);
