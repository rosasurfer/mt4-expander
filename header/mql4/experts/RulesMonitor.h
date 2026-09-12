#pragma once
#include "expander.h"

// view data for the status panel
struct VIEW_DATA {
   uint   pid;                // MQL program instance
   HWND   hWnd;               // handle of the panel
   BOOL   initialized;        // whether view data is initialized

   int    trend;              // trend direction

   color  bgColorUpTrend;     // background color for uptrends
   HBRUSH bgBrushUpTrend;     // brush for uptrends

   color  bgColorDownTrend;   // background color for downtrends
   HBRUSH bgBrushDownTrend;   // brush for downtrends

   color  textColor;          // text foreground color
   HFONT  hFont;              // handle of the font to be used

   //wchar text[256];
};

LRESULT CALLBACK StatusPanelWindowProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
BOOL    WINAPI   InitViewData(VIEW_DATA* data, HDC hDC);
void    WINAPI   ReleaseViewData(VIEW_DATA* data);
