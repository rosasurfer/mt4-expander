#pragma once
#include "expander.h"
#include "struct/rsf/ExecutionContext.h"


int                WINAPI SyncMainContext_init  (EXECUTION_CONTEXT* ec, ProgramType type, const char* name, UninitializeReason reason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint timeframe, uint digits, double point, BOOL extReporting, BOOL recordEquity, BOOL isTesting, BOOL isVisualMode, BOOL isOptimization, EXECUTION_CONTEXT* sec, HWND hChart, int droppedOnChart, int droppedOnPosX, int droppedOnPosY);
int                WINAPI SyncMainContext_start (EXECUTION_CONTEXT* ec, const void* rates, int bars, int changedBars, uint ticks, datetime tickTime, double bid, double ask);
int                WINAPI SyncMainContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason reason);

int                WINAPI SyncLibContext_init   (EXECUTION_CONTEXT* ec, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* name, const char* symbol, uint timeframe, uint digits, double point, BOOL isTesting, BOOL isOptimization);
int                WINAPI SyncLibContext_deinit (EXECUTION_CONTEXT* ec, UninitializeReason uninitReason);

int                WINAPI LeaveContext          (EXECUTION_CONTEXT* ec);

TEST*              WINAPI Expert_InitTest(EXECUTION_CONTEXT* ec, BOOL isTesting);

uint               WINAPI FindModuleInLimbo(ModuleType type, const char* name, UninitializeReason uninitReason, BOOL testing, HWND hChart);
HWND               WINAPI FindWindowHandle(HWND hChart, const EXECUTION_CONTEXT* sec, ModuleType moduleType, const char* symbol, uint timeframe, BOOL isTesting, BOOL isVisualMode);
UninitializeReason WINAPI FixUninitReason(EXECUTION_CONTEXT* ec, ModuleType moduleType, CoreFunction coreFunction, UninitializeReason uninitReason);

uint               WINAPI GetCurrentThreadIndex();
uint               WINAPI GetLastThreadProgram();
int                WINAPI SetLastThreadProgram(uint pid);

InitializeReason   WINAPI GetInitReason          (EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec, ProgramType programType, const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL testing, BOOL visualMode, HWND hChart, int droppedOnChart, int droppedOnPosX, int droppedOnPosY, uint &prevPid);
InitializeReason   WINAPI GetInitReason_indicator(EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec,                          const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL testing, BOOL visualMode, HWND hChart, int droppedOnChart,                                       uint &prevPid);
InitializeReason   WINAPI GetInitReason_expert   (EXECUTION_CONTEXT* ec,                                                        const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL testing,                                                   int droppedOnPosX, int droppedOnPosY);
InitializeReason   WINAPI GetInitReason_script   (EXECUTION_CONTEXT* ec,                                                        const char* programName,                                                                                                                      int droppedOnPosX, int droppedOnPosY);

const char*        WINAPI Program_CustomLogFilename     (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI Program_IsLogEnabled          (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI Program_IsLogToDebugEnabled   (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI Program_IsLogToTerminalEnabled(const EXECUTION_CONTEXT* ec);
BOOL               WINAPI Program_IsLogToCustomEnabled  (const EXECUTION_CONTEXT* ec);
BOOL               WINAPI Program_IsOptimization        (const EXECUTION_CONTEXT* ec, BOOL isOptimization);
BOOL               WINAPI Program_IsPartialTest         (uint pid, const char* programName);
BOOL               WINAPI Program_IsTesting             (const EXECUTION_CONTEXT* ec, BOOL isTesting);
BOOL               WINAPI Program_IsVisualMode          (const EXECUTION_CONTEXT* ec, BOOL isVisualMode);

uint               WINAPI PushProgram(ContextChain* chain);
