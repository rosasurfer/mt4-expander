#pragma once
#include "expander.h"
#include "struct/xtrade/ExecutionContext.h"


HWND   WINAPI FindTesterWindow();
int    WINAPI Tester_GetBarModel();
double WINAPI Tester_GetCommissionValue(const char* symbol, uint timeframe, uint barModel, double lots = 1.0);

BOOL   WINAPI Test_StartReporting(EXECUTION_CONTEXT* ec, datetime time, uint bars, int barModel, int reportingId, const char* reportingSymbol);
BOOL   WINAPI Test_StopReporting (EXECUTION_CONTEXT* ec, datetime time, uint bars);
BOOL   WINAPI Test_SaveReport(TEST* test);

BOOL   WINAPI Test_onPositionOpen (EXECUTION_CONTEXT* ec, int ticket, int type, double lots, const char* symbol, double openPrice, datetime openTime, double stopLoss, double takeProfit, double commission, int magicNumber, const char* comment);
BOOL   WINAPI Test_onPositionClose(EXECUTION_CONTEXT* ec, int ticket, double closePrice, datetime closeTime, double swap, double profit);
