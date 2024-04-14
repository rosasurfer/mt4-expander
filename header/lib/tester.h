#pragma once
#include "expander.h"
#include "struct/mt4/FxtHeader.h"
#include "struct/rsf/ExecutionContext.h"


HWND   WINAPI FindTesterWindow();
int    WINAPI Tester_GetBarModel();
time32 WINAPI Tester_GetStartDate();
time32 WINAPI Tester_GetEndDate();
BOOL   WINAPI Tester_ReadFxtHeader(const char* symbol, uint timeframe, uint barModel, FXT_HEADER* fxtHeader);
double WINAPI Test_GetCommission  (const EXECUTION_CONTEXT* ec);
