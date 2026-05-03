#pragma once
#include "expander.h"

BOOL  WINAPI onProcessAttach();
BOOL  WINAPI onProcessDetach(BOOL isTerminating);
DWORD WINAPI onExpanderStart(void* lpParam);
