#pragma once
#include "expander.h"

// view data for the status panel
struct VIEW_DATA {
   uint   pid;                // MQL program instance
   HWND   hWnd;               // handle of the panel
   BOOL   initialized;        // whether view data is initialized

   color  bgColorUpTrend;     // background color for uptrends
   HBRUSH bgBrushUpTrend;     // brush for uptrends
   color  bgColorDownTrend;   // background color for downtrends
   HBRUSH bgBrushDownTrend;   // brush for downtrends
   color  textColor;          // text foreground color
   HFONT  hFont;              // handle of the font to be used
   HFONT  hSystemFont;        // font handle of system messages

   double price;              // close price
   int    trend;              // trend direction
   double dcw;                // Donchian Channel width
   //wchar text[256];
};

// view status update
struct STATUS_UPDATE {
   double price;
   int    trend;
   double dcw;
};

LRESULT CALLBACK StatusPanelWindowProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
BOOL    WINAPI   InitViewData(VIEW_DATA* data, HDC hDC);
void    WINAPI   ReleaseViewData(VIEW_DATA* data);
