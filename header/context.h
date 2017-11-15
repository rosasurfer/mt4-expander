#pragma once

#include "expander.h"
#include "struct/xtrade/ExecutionContext.h"


// Context management functions
BOOL             WINAPI SyncMainContext_init  (EXECUTION_CONTEXT* ec, ProgramType type, const char* name, UninitializeReason reason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint period, EXECUTION_CONTEXT* sec, BOOL isTesting, BOOL isVisualMode, BOOL isOptimization, HWND hChart, int droppedOnChart, int droppedOnPosX, int droppedOnPosY);
BOOL             WINAPI SyncMainContext_start (EXECUTION_CONTEXT* ec, datetime time, double bid, double ask, uint volume);
BOOL             WINAPI SyncMainContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason reason);

BOOL             WINAPI SyncLibContext_init   (EXECUTION_CONTEXT* ec, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* name, const char* symbol, uint period, BOOL isOptimization);
BOOL             WINAPI SyncLibContext_deinit (EXECUTION_CONTEXT* ec, UninitializeReason uninitReason);

BOOL             WINAPI LeaveContext          (EXECUTION_CONTEXT* ec);

int              WINAPI FindIndicatorInLimbo(HWND hChart, const char* name, UninitializeReason reason);
HWND             WINAPI FindWindowHandle    (HWND hChart, const EXECUTION_CONTEXT* sec, ModuleType moduleType, const char* symbol, uint timeframe, BOOL isTesting, BOOL isVisualMode);

InitializeReason WINAPI InitReason          (EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec, ProgramType programType, const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL testing, BOOL visualMode, HWND hChart, int droppedOnChart, int droppedOnPosX, int droppedOnPosY, uint& originalProgramId);
InitializeReason WINAPI InitReason_indicator(EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec,                          const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL testing, BOOL visualMode, HWND hChart, int droppedOnChart,                                       uint& originalProgramId);
InitializeReason WINAPI InitReason_expert   (EXECUTION_CONTEXT* ec,                                                                                 UninitializeReason uninitReason, const char* symbol, BOOL testing,                                                   int droppedOnPosX, int droppedOnPosY);
InitializeReason WINAPI InitReason_script();

BOOL             WINAPI ProgramIsLogging      (const EXECUTION_CONTEXT* ec);
BOOL             WINAPI ProgramIsOptimization (const EXECUTION_CONTEXT* ec, BOOL isOptimization);
BOOL             WINAPI ProgramIsTesting      (const EXECUTION_CONTEXT* ec, BOOL isTesting);
BOOL             WINAPI ProgramIsVisualMode   (const EXECUTION_CONTEXT* ec, BOOL isVisualMode);
const char*      WINAPI ProgramCustomLogFile  (const EXECUTION_CONTEXT* ec);
DWORD            WINAPI StoreThreadAndProgram (uint programId);
