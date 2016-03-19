/**
 * MQL-Interface zum Zugriff auf ein struct SYMBOL (Dateiformat "symbols.raw").
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt den Namen eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - Symbolname
 */
const char* WINAPI symbol_Name(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->name);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - Symbolbeschreibung
 */
const char* WINAPI symbol_Description(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->description);
   #pragma EXPORT
}


/**
 * Gibt die Herkunft eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - ursprünglicher Symbolname oder Leerstring, wenn kein solcher Name gesetzt ist
 */
const char* WINAPI symbol_Origin(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->origin);
   #pragma EXPORT
}


/**
 * Gibt den alternativen Namen eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - alternativer Name oder Leerstring, wenn kein solcher Name gesetzt ist
 */
const char* WINAPI symbol_AltName(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->altName);
   #pragma EXPORT
}


/**
 * Gibt die Basiswährung eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - Basiswährung
 */
const char* WINAPI symbol_BaseCurrency(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->baseCurrency);
   #pragma EXPORT
}


/**
 * Gibt den Index der Gruppe, in der ein SYMBOL aufgeführt ist, zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Gruppenindex
 */
uint WINAPI symbol_Group(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->group);
   #pragma EXPORT
}


/**
 * Gibt die Digits eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Digits
 */
uint WINAPI symbol_Digits(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->digits);
   #pragma EXPORT
}


/**
 * Gibt den TradeMode eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - TradeMode
 */
uint WINAPI symbol_TradeMode(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->tradeMode);
   #pragma EXPORT
}


/**
 * Gibt die Hintergrundfarbe eines SYMBOLs im "Market Watch"-Window zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Farbe oder CLR_NONE, wenn keine Hintergrundfarbe gesetzt ist
 */
uint WINAPI symbol_BackgroundColor(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (symbol->backgroundColor)
      return(symbol->backgroundColor);
   return(CLR_NONE);
   #pragma EXPORT
}


/**
 * Gibt die eindeutige ID eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - ID
 */
uint WINAPI symbol_Id(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->id);
   #pragma EXPORT
}


/**
 * Gibt die Spreadkonfiguration eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Spread in Points oder 0, wenn für das Symbol ein variabler Spread gilt
 */
uint WINAPI symbol_Spread(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->spread);
   #pragma EXPORT
}


/**
 * Gibt den Swapbetrag eines SYMBOLs für Long-Positionen zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Swap-Betrag in Einheit entsprechend MarketInfo(MODE_SWAPTYPE)
 */
double WINAPI symbol_SwapLong(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->swapLong);
   #pragma EXPORT
}


/**
 * Gibt den Swapbetrag eines SYMBOLs für Short-Positionen zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Swap-Betrag in Einheit entsprechend MarketInfo(MODE_SWAPTYPE)
 */
double WINAPI symbol_SwapShort(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->swapShort);
   #pragma EXPORT
}


/**
 * Gibt die ContractSize eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - ContratSize in Units
 */
double WINAPI symbol_ContractSize(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->contractSize);
   #pragma EXPORT
}


/**
 * Gibt die StopDistance eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - StopDistance in Points
 */
uint WINAPI symbol_StopDistance(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->stopDistance);
   #pragma EXPORT
}


/**
 * Gibt die Margin-Requirements zum Öffnen einer Position von 1 Lot in einem SYMBOL zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Margin-Requirements in Units oder 0 für den Wert der ContractSize
 */
double WINAPI symbol_MarginInit(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginInit);
   #pragma EXPORT
}


/**
 * Gibt die Margin-Requirements zum Halten einer Position von 1 Lot in einem SYMBOL zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Margin-Requirements in Units oder 0 für den Wert der ContractSize
 */
double WINAPI symbol_MarginMaintenance(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginMaintenance);
   #pragma EXPORT
}


/**
 * Gibt die Margin-Requirements einer gehedgten Position von 1 Lot in einem SYMBOL zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Margin-Requirements in Units je Leg des Hedges oder 0 für den Wert der ContractSize
 */
double WINAPI symbol_MarginHedged(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginHedged);
   #pragma EXPORT
}


/**
 * Gibt den MarginDivider zur MarginBerechnung eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - MarginDivider
 */
double WINAPI symbol_MarginDivider(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginDivider);
   #pragma EXPORT
}


/**
 * Gibt die PointSize eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - PointSize
 */
double WINAPI symbol_PointSize(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->pointSize);
   #pragma EXPORT
}


/**
 * Gibt die PointsPerUnit eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - PointsPerUnit
 */
double WINAPI symbol_PointsPerUnit(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->pointsPerUnit);
   #pragma EXPORT
}


/**
 * Gibt die Marginwährung eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - Marginwährung
 */
const char* WINAPI symbol_MarginCurrency(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginCurrency);
   #pragma EXPORT
}


/**
 * Setzt den Namen eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  char*   name
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI symbol_SetName(SYMBOL* symbol, const char* name) {
   if ((uint)symbol < MIN_VALID_POINTER)      return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)name   < MIN_VALID_POINTER)      return(debug("ERROR:  invalid parameter name = 0x%p (not a valid pointer)", name));
   if (strlen(name) > sizeof(symbol->name)-1) return(debug("ERROR:  invalid parameter name = \"%s\" (max %d characters)", name, sizeof(symbol->name)-1));
   return((BOOL)strcpy(symbol->name, name));
   #pragma EXPORT
}


/**
 * Setzt die Bechreibung eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  char*   description
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI symbol_SetDescription(SYMBOL* symbol, const char* description) {
   if ((uint)symbol      < MIN_VALID_POINTER)               return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)description < MIN_VALID_POINTER)               return(debug("ERROR:  invalid parameter description = 0x%p (not a valid pointer)", description));
   if (strlen(description) > sizeof(symbol->description)-1) return(debug("ERROR:  invalid parameter description = \"%s\" (max %d characters)", description, sizeof(symbol->description)-1));
   return((BOOL)strcpy(symbol->description, description));
   #pragma EXPORT
}


/**
 * Setzt die Basiswährung eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  char*   currency
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI symbol_SetBaseCurrency(SYMBOL* symbol, const char* currency) {
   if ((uint)symbol   < MIN_VALID_POINTER)             return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)currency < MIN_VALID_POINTER)             return(debug("ERROR:  invalid parameter currency = 0x%p (not a valid pointer)", currency));
   int len = strlen(currency);
   if (len!=3 || len > sizeof(symbol->baseCurrency)-1) return(debug("ERROR:  invalid parameter currency = \"%s\" (3 characters)", currency));
   return((BOOL)strcpy(symbol->baseCurrency, currency));
   #pragma EXPORT
}


/**
 * Setzt den Gruppenindex eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  int     index
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI symbol_SetGroup(SYMBOL* symbol, int index) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (index < 0)                        return(debug("ERROR:  invalid parameter index = %d", index));
   symbol->group = index;
   return(TRUE);
   #pragma EXPORT
}


/**
 * Setzt die Digits eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  int     digits
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI symbol_SetDigits(SYMBOL* symbol, int digits) {
   if ((uint)symbol < MIN_VALID_POINTER) return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (digits < 0)                       return(debug("ERROR:  invalid parameter digits = %d", digits));
   symbol->digits = digits;
   return(TRUE);
   #pragma EXPORT
}


/**
 * Setzt die Hintergrundfarbe eines SYMBOLs im "Market Watch"-Window.
 *
 * @param  SYMBOL* symbol
 * @param  uint    color
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI symbol_SetBackgroundColor(SYMBOL* symbol, uint color) {
   if ((uint)symbol < MIN_VALID_POINTER)      return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (color!=CLR_NONE && color & 0xFF000000) return(debug("ERROR:  invalid parameter color = 0x%p (not a valid color)", color));
   symbol->backgroundColor = color;
   return(TRUE);
   #pragma EXPORT
}


/**
 * Setzt die Marginwährung eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  char*   currency
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI symbol_SetMarginCurrency(SYMBOL* symbol, const char* currency) {
   if ((uint)symbol   < MIN_VALID_POINTER)               return(debug("ERROR:  invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)currency < MIN_VALID_POINTER)               return(debug("ERROR:  invalid parameter currency = 0x%p (not a valid pointer)", currency));
   int len = strlen(currency);
   if (len!=3 || len > sizeof(symbol->marginCurrency)-1) return(debug("ERROR:  invalid parameter currency = \"%s\" (3 characters)", currency));
   return((BOOL)strcpy(symbol->marginCurrency, currency));
   #pragma EXPORT
}
