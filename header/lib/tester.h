#pragma once
#include "expander.h"
#include "struct/mt4/FxtHeader.h"
#include "struct/rsf/ExecutionContext.h"


HWND              WINAPI FindTesterWindow();

int               WINAPI Tester_GetBarModel();
datetime          WINAPI Tester_GetStartDate();
datetime          WINAPI Tester_GetEndDate();
const FXT_HEADER* WINAPI Tester_ReadFxtHeader(const char* symbol, uint timeframe, uint barModel);

double            WINAPI Test_GetCommission  (const EXECUTION_CONTEXT* ec, double lots = 1.0);
BOOL              WINAPI Test_onPositionOpen (const EXECUTION_CONTEXT* ec, int ticket, int type, double lots, const char* symbol, datetime openTime, double openPrice, double stopLoss, double takeProfit, double commission, int magicNumber, const char* comment);
BOOL              WINAPI Test_onPositionClose(const EXECUTION_CONTEXT* ec, int ticket, datetime closeTime, double closePrice, double swap, double profit);

BOOL              WINAPI Test_SaveReport(const TEST* test);
BOOL              WINAPI Test_StartReporting(const EXECUTION_CONTEXT* ec, datetime time, uint bars, int reportId, const char* reportSymbol);
BOOL              WINAPI Test_StopReporting (const EXECUTION_CONTEXT* ec, datetime time, uint bars);
