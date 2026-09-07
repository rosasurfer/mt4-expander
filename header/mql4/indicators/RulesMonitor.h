#pragma once
#include "expander.h"

// container for view data of the status panel
struct VIEW_DATA {
   int    trend;

   color  bgColorUpTrend;
   HBRUSH bgBrushUpTrend;

   color  bgColorDownTrend;
   HBRUSH bgBrushDownTrend;

   color  textColor;
   HFONT  hFont;

   //wchar text[256];
};

LRESULT CALLBACK StatusPanelWindowProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
BOOL    WINAPI   InitViewData(VIEW_DATA* data, HDC hDC);
void    WINAPI   ReleaseViewData(VIEW_DATA* data);
