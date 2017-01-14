#pragma once

#include "expander.h"
#include "structs/myfx/ExecutionContext.h"


// Context management functions
BOOL             WINAPI SyncMainContext_init  (EXECUTION_CONTEXT* ec, ProgramType type, const char* name, UninitializeReason reason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint period, EXECUTION_CONTEXT* sec, BOOL isTesting, BOOL isVisualMode, BOOL isOptimization, HWND hChart, int subChartDropped);
BOOL             WINAPI SyncMainContext_start (EXECUTION_CONTEXT* ec);
BOOL             WINAPI SyncMainContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason reason);

BOOL             WINAPI SyncLibContext_init  (EXECUTION_CONTEXT* ec, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* name, const char* symbol, uint period, BOOL isOptimization);
BOOL             WINAPI SyncLibContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason);

BOOL             WINAPI LeaveContext(EXECUTION_CONTEXT* ec);

int              WINAPI FindFirstIndicatorInLimbo(HWND hChart, const char* name, UninitializeReason reason);
HWND             WINAPI ProgramFindChart         (HWND hChart, const EXECUTION_CONTEXT* sec, ModuleType moduleType, BOOL isTesting, BOOL isVisualMode, const char* symbol, uint timeframe);
InitializeReason WINAPI ProgramInitReason        (      EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec, const char* programName, ProgramType programType, UninitializeReason uninitializeReason, const char* symbol, BOOL testing, BOOL visualMode, HWND hChart, int subChartDropped);
BOOL             WINAPI ProgramIsLogging         (const EXECUTION_CONTEXT* ec);
BOOL             WINAPI ProgramIsOptimization    (const EXECUTION_CONTEXT* ec, BOOL isOptimization);
BOOL             WINAPI ProgramIsTesting         (const EXECUTION_CONTEXT* ec, BOOL isTesting);
BOOL             WINAPI ProgramIsVisualMode      (const EXECUTION_CONTEXT* ec, BOOL isVisualMode);
const char*      WINAPI ProgramCustomLogFile     (const EXECUTION_CONTEXT* ec);
DWORD            WINAPI StoreThreadAndProgram    (uint programId);
