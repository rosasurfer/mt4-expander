#pragma once
#include "expander.h"

void         WINAPI CustomizeTerminal();
char*        WINAPI FindHistoryDirectoryA(const char* filename, BOOL removeFile);
HWND         WINAPI FindInputDialogA(ProgramType programType, const char* programName);
DWORD        WINAPI GetCliOptions();
DWORD        WINAPI GetDebugOptions();
const char*  WINAPI GetExpanderFileNameA();
const wchar* WINAPI GetExpanderFileNameW();
const char*  WINAPI GetHistoryRootPathA();
const wchar* WINAPI GetHistoryRootPathW();
const char*  WINAPI GetMqlDirectoryA();
const wchar* WINAPI GetMqlDirectoryW();
const char*  WINAPI GetMqlSandboxPathA(BOOL inTester);
const wchar* WINAPI GetMqlSandboxPathW(BOOL inTester);
uint         WINAPI GetTerminalBuild();
const char*  WINAPI GetTerminalCommonDataPathA();
const wchar* WINAPI GetTerminalCommonDataPathW();
const char*  WINAPI GetTerminalDataPathA();
const wchar* WINAPI GetTerminalDataPathW();
const char*  WINAPI GetTerminalFileNameA();
const wchar* WINAPI GetTerminalFileNameW();
HWND         WINAPI GetTerminalMainWindow();
const char*  WINAPI GetTerminalPathA();
const wchar* WINAPI GetTerminalPathW();
const char*  WINAPI GetTerminalRoamingDataPathA();
const wchar* WINAPI GetTerminalRoamingDataPathW();
const char*  WINAPI GetTerminalVersion();
BOOL         WINAPI GetTerminalVersionFromFile(VS_FIXEDFILEINFO &fileInfo);
BOOL         WINAPI GetTerminalVersionFromImage(VS_FIXEDFILEINFO &fileInfo);
BOOL         WINAPI IsLockedFile(const string &filename);
BOOL         WINAPI IsPortableMode();
BOOL         WINAPI LoadMqlProgramA(HWND hChart, ProgramType programType, const char* programName);
BOOL         WINAPI LoadMqlProgramW(HWND hChart, ProgramType programType, const wchar* programName);
BOOL         WINAPI ReopenAlertDialog(BOOL sound);
