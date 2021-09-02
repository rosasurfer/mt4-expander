#pragma once
#include "expander.h"


const char*  WINAPI BarModelDescription(int id);
const char*  WINAPI BarModelToStr(int id);
const char*  WINAPI BoolToStr(BOOL value);
const char*  WINAPI CoreFunctionDescription(CoreFunction func);
const char*  WINAPI CoreFunctionToStr(CoreFunction func);
      char*  WINAPI DeinitFlagsToStr(DWORD flags);
const char*  WINAPI ErrorToStrA(int error);
const wchar* WINAPI ErrorToStrW(int error);
      char*  WINAPI InitFlagsToStr(DWORD flags);
const char*  WINAPI InitializeReasonToStr(InitializeReason reason);
const char*  WINAPI InitReasonToStr(InitializeReason reason);
      char*  WINAPI IntToHexStr(int value);
const char*  WINAPI LoglevelDescriptionA(int level, BOOL upperCase = TRUE);
const char*  WINAPI LoglevelToStr(int level);
const char*  WINAPI ModuleTypeDescription(ModuleType type);
const char*  WINAPI ModuleTypeToStr(ModuleType type);
      char*  WINAPI NumberToStr(double value, const char* format);
const char*  WINAPI OperationTypeDescription(int type);
const char*  WINAPI OperationTypeToStr(int type);
const char*  WINAPI OrderTypeDescription(int type);
const char*  WINAPI OrderTypeToStr(int type);
const char*  WINAPI PeriodDescription(int period);
const char*  WINAPI PeriodToStr(int period);
const char*  WINAPI ProgramTypeDescription(ProgramType type);
const char*  WINAPI ProgramTypeToStr(ProgramType type);
const char*  WINAPI ShowWindowCmdToStr(int cmd);
const char*  WINAPI StringToStr(const char* value);
const char*  WINAPI TimeframeDescription(int timeframe);
const char*  WINAPI TimeframeToStr(int timeframe);
const char*  WINAPI TradeDirectionDescription(int direction);
const char*  WINAPI TradeDirectionToStr(int direction);
const char*  WINAPI UninitializeReasonToStr(UninitializeReason reason);
const char*  WINAPI UninitReasonToStr(UninitializeReason reason);
