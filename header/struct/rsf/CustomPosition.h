#pragma once
#include "expander.h"


/**
 * Bin�re Repr�sentation eines einzelnen Konfigurationsterms einer individuellen Position. Die Konfiguration einer CustomPosition besteht
 * aus ein oder mehreren durch Kommas voneinander getrennten Termen.
 *
 * In positions_config[] markiert ein nicht initialisiertes Struct (pct.type=NULL) das Ende einer CustomPosition.
 *
 * @see  MQL: double positions.config[][5];
 */
#pragma pack(push, 1)

struct POSITION_CONFIG_TERM {
   double type;               // [ Ticket | TERM_OPEN_LONG | TERM_OPEN_SHORT | TERM_OPEN_SYMBOL | TERM_OPEN_ALL | TERM_HISTORY_SYMBOL | TERM_HISTORY_ALL | TERM_ADJUSTMENT | TERM_EQUITY ]
   double confValue1;         // [ EMPTY  | LotSize | FromDateTime ]
   double confValue2;         // [ Price  | Amount  | ToDateTime   ]
   double cacheValue1;        // Speicher f�r Zwischenergebnisse bei Berechnung der Kennziffern von TERM_HISTORY_*
   double cacheValue2;        // Speicher f�r Zwischenergebnisse bei Berechnung der Kennziffern von TERM_HISTORY_*
};
#pragma pack(pop)


/**
 * Report-Datensatz einer Position zur Anzeige im Chart.
 *
 * @see  MQL: int    positions.idata[][3];
 *       MQL: double positions.ddata[][9];
 */
#pragma pack(push, 1)

struct POSITION_DATA {
   uint   configType;         // [ CONFIG_AUTO | CONFIG_REAL | CONFIG_VIRTUAL ]
   uint   positionType;       // [ POSITION_LONG | POSITION_SHORT | POSITION_HEDGE | POSITION_HISTORY ]
   uint   commentIndex;       // Index des Positionskommentars in positions.config.comments[]

   double directionalLots;    // ungehedgte Lots
   double hedgedLots;         // gehedgte Lots
   union {
      double breakevenPrice;  // BreakevenPrice, wenn (directionalLots != 0)
      double pipDistance;     // Pip-Distance,   wenn (hedgedLots && !directionalLots)
   };
   double openEquity;         // Equity bei PositionOpen (Ma�stab f�r fullProfitPct)
   double openProfit;         // P/L offener Trades
   double closedProfit;       // P/L geschlossener Trades
   double adjustedProfit;     // P/L-Adjustment
   double fullProfitAbs;      // gesamter P/L absolut
   double fullProfitPct;      // gesamter P/L prozentual
};
#pragma pack(pop)
