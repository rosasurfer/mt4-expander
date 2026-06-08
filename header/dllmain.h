#pragma once
#include "expander.h"

static BOOL  WINAPI onProcessAttach();
static BOOL  WINAPI onProcessDetach(BOOL isTerminating);
static DWORD WINAPI ExpanderStartThread(void* lpParam);

BOOL WINAPI PinDllToMemory();
