#pragma once
#include "expander.h"

BOOL WINAPI IsChartTemplatesMenu(HMENU hMenu);
BOOL WINAPI IsChartTemplateSeparatorName(const wstring &label);
BOOL WINAPI ModifyChartTemplateLabel(wstring &label);
BOOL WINAPI RebuildChartTemplatesMenu(HMENU hMenu);

BOOL WINAPI dumpMenu(HMENU hMenu);
