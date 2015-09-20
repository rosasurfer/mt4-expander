/**
 * Header von Historydateien (Kursreihen im "history"-Verzeichnis).
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Binäre Representation eines einzelnen Konfigurationsterms einer individuellen Position. Eine Position kann aus ein
 * oder mehreren mit Kommas voneinander getrennten Termen bestehen.
 *
 * In custom_position_conf[] markiert ein nicht initialisiertes Struct (pct.type=NULL) das Ende einer Position.
 *
 * @see  MQL: double custom.position.conf[][5];
 */
struct POSITION_CONFIG_TERM {
   double type;            // [ Ticket | TYPE_OPEN_LONG | TYPE_OPEN_SHORT | TYPE_OPEN_SYMBOL | TYPE_OPEN_ALL | TYPE_HISTORY_SYMBOL | TYPE_HISTORY_ALL | TYPE_REALIZED | TYPE_EQUITY ]
   double value1;          // [ EMPTY  | LotSize | FromDateTime ]
   double value2;          // [ Price  | Amount  | ToDateTime   ]
   double value3;          // Speicher für Zwischenergebnisse bei Berechnung der Kennziffern einer Position
   double value4;          // Speicher für Zwischenergebnisse bei Berechnung der Kennziffern einer Position
} pct;


POSITION_CONFIG_TERM custom_position_conf[];
