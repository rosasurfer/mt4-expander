#pragma once
#include "expander.h"

// struct holding view model and data of the status panel
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
   double donchianWidth;      // Donchian Channel width
   double donchianLegsize;    // Donchian Channel leg size
   //wchar text[256];
};

// struct holding a view model update
struct VIEW_UPDATE {
   double price;
   int    trend;
   double donchianWidth;
   double donchianLegsize;
};

BOOL    WINAPI   StatusPanel_InitView(VIEW_DATA* data, HDC hDC);
void    WINAPI   StatusPanel_ReleaseView(VIEW_DATA* data);

LRESULT CALLBACK StatusPanelWindowProc       (HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI   StatusPanel_onCommand       (HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI   StatusPanel_onMT4ExpanderMsg(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI   StatusPanel_onPaint         (HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
