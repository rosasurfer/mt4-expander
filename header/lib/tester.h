#pragma once
#include "expander.h"
#include "struct/xtrade/ExecutionContext.h"


HWND   WINAPI FindTesterWindow();
int    WINAPI Tester_GetBarModel();
double WINAPI Tester_GetCommission(const char* symbol, uint timeframe, uint barModel, double lots = 1.0);

BOOL   WINAPI Test_onPositionOpen (const EXECUTION_CONTEXT* ec, int ticket, int type, double lots, const char* symbol, double openPrice, datetime openTime, double stopLoss, double takeProfit, double commission, int magicNumber, const char* comment);
BOOL   WINAPI Test_onPositionClose(const EXECUTION_CONTEXT* ec, int ticket, double closePrice, datetime closeTime, double swap, double profit);

BOOL   WINAPI Test_SaveReport(const TEST* test);
BOOL   WINAPI Test_StartReporting(const EXECUTION_CONTEXT* ec, datetime time, uint bars, int reportId, const char* reportSymbol);
BOOL   WINAPI Test_StopReporting (const EXECUTION_CONTEXT* ec, datetime time, uint bars);
