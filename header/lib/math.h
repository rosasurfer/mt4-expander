#pragma once
#include "stdafx.h"
#include <cmath>

#define INF    HUGE_VAL                      // positive infinity, negative: -INF
#define NaN    ((INF)-(INF))                 // not-a-number


int    WINAPI DoubleExp(double value);
BOOL   WINAPI IsInfinite(double value);
BOOL   WINAPI IsNaN(double value);
double WINAPI Math_INF();
double WINAPI Math_NaN();
double WINAPI MathLog10(double value);

float  WINAPI round(float  value, int digits = 0);
double WINAPI round(double value, int digits = 0);
