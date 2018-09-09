#pragma once

#include "expander.h"


uint        WINAPI AnsiToWChar(const char* source, WCHAR* target, uint targetSize);
const char* WINAPI BoolToStr(BOOL value);
string      WINAPI doubleQuoteStr(const string& value);
string      WINAPI doubleQuoteStr(const char* value);
const char* WINAPI DoubleQuoteStr(const char* value);
const char* WINAPI ErrorToStr(int error);
const char* WINAPI IntToHexStr(int value);
const char* WINAPI InitFlagsToStr(DWORD flags);
const char* WINAPI DeinitFlagsToStr(DWORD flags);
const char* WINAPI InitReasonToStr(InitializeReason reason);
const char* WINAPI InitializeReasonToStr(InitializeReason reason);
const char* WINAPI UninitReasonToStr(UninitializeReason reason);
const char* WINAPI UninitializeReasonToStr(UninitializeReason reason);
const char* WINAPI ModuleTypeDescription(ModuleType type);
const char* WINAPI ModuleTypeToStr(ModuleType type);
string      WINAPI numberToStr(double value, const char* format);
const char* WINAPI NumberToStr(double value, const char* format);
const char* WINAPI ProgramTypeDescription(ProgramType type);
const char* WINAPI ProgramTypeToStr(ProgramType type);
const char* WINAPI RootFunctionDescription(RootFunction fn);
const char* WINAPI RootFunctionToStr(RootFunction fn);
const char* WINAPI OperationTypeDescription(int type);
const char* WINAPI OperationTypeToStr(int type);
const char* WINAPI OrderTypeDescription(int type);
const char* WINAPI OrderTypeToStr(int type);
const char* WINAPI PeriodDescription(int period);
const char* WINAPI PeriodToStr(int period);
const char* WINAPI ShowWindowCmdToStr(int cmd);
const char* WINAPI StringToStr(const char* value);
const char* WINAPI TimeframeDescription(int timeframe);
const char* WINAPI TimeframeToStr(int timeframe);
const char* WINAPI TradeDirectionDescription(int direction);
const char* WINAPI TradeDirectionToStr(int direction);
