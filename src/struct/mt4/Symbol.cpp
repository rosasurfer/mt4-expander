/**
 * MT4 struct SYMBOL (Dateiformat "symbols.raw")
 *
 * Die Symbole einer Datei sind alphabetisch nach Namen sortiert.
 */
#include "expander.h"
#include "struct/mt4/Symbol.h"

#include <math.h>


/**
 * Gibt den Namen eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - Symbolname
 */
const char* WINAPI symbol_Name(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->name);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Beschreibung eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - Symbolbeschreibung
 */
const char* WINAPI symbol_Description(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->description);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Herkunft eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - ursprünglicher Symbolname oder Leerstring, wenn kein solcher Name gesetzt ist
 */
const char* WINAPI symbol_Origin(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->origin);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den alternativen Namen eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - alternativer Name oder Leerstring, wenn kein solcher Name gesetzt ist
 */
const char* WINAPI symbol_AltName(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->altName);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Basiswährung eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - Basiswährung
 */
const char* WINAPI symbol_BaseCurrency(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->baseCurrency);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Index der Gruppe, in der ein SYMBOL aufgeführt ist, zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Gruppenindex
 */
uint WINAPI symbol_Group(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->group);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Digits eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Digits
 */
uint WINAPI symbol_Digits(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->digits);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den TradeMode eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - TradeMode
 */
uint WINAPI symbol_TradeMode(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->tradeMode);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Hintergrundfarbe eines SYMBOLs im "Market Watch"-Window zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Farbe oder White, wenn keine Hintergrundfarbe gesetzt ist
 *                (CLR_NONE wird vom Terminal als Black interpretiert)
 */
uint WINAPI symbol_BackgroundColor(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol)));
   if (symbol->backgroundColor)
      return(symbol->backgroundColor);
   return(White);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Array-Key eines SYMBOLs zurück. Der Wert kann sich ändern, wenn die Datei "symbols.raw" gespeichert wird.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Array-Key
 */
uint WINAPI symbol_ArrayKey(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol)));
   return(symbol->arrayKey);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die eindeutige ID eines SYMBOLs zurück. Diese ID ist eine feste Eigenschaft, sie ändert sich beim Speichern von "symbols.raw" nicht.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - ID
 */
uint WINAPI symbol_Id(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol)));
   return(symbol->id);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Spreadkonfiguration eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Spread in Points oder 0, wenn für das Symbol ein variabler Spread gilt
 */
uint WINAPI symbol_Spread(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol)));
   return(symbol->spread);
   #pragma EXPANDER_EXPORT
}


/**
 * Ob für ein SYMBOL Swaps berechnet werden.
 *
 * @param  SYMBOL* symbol
 *
 * @return BOOL
 */
BOOL WINAPI symbol_SwapEnabled(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->swapEnabled);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Swaptyp eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - SwapType entsprechend MarketInfo(MODE_SWAPTYPE)
 */
uint WINAPI symbol_SwapType(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->swapType);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Swapbetrag eines SYMBOLs für Long-Positionen zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Swap-Betrag in Einheit entsprechend SYMBOL.swapType
 */
double WINAPI symbol_SwapLongValue(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->swapLongValue);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Swapbetrag eines SYMBOLs für Short-Positionen zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Swap-Betrag in Einheit entsprechend SYMBOL.swapType
 */
double WINAPI symbol_SwapShortValue(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->swapShortValue);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Wochentag des 3-fachen Swaps eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - Wochentag von 0 (SUNDAY) bis 6 (SATURDAY)
 */
uint WINAPI symbol_SwapTripleRolloverDay(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->swapTripleRolloverDay);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die ContractSize eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - ContratSize in Units
 */
double WINAPI symbol_ContractSize(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->contractSize);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die StopDistance eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return uint - StopDistance in Points
 */
uint WINAPI symbol_StopDistance(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->stopDistance);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Margin-Requirements zum Öffnen einer Position von 1 Lot in einem SYMBOL zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Margin-Requirements in Units oder 0 für den Wert der ContractSize
 */
double WINAPI symbol_MarginInit(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginInit);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Margin-Requirements zum Halten einer Position von 1 Lot in einem SYMBOL zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Margin-Requirements in Units oder 0 für den Wert der ContractSize
 */
double WINAPI symbol_MarginMaintenance(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginMaintenance);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Margin-Requirements einer gehedgten Position von 1 Lot in einem SYMBOL zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - Margin-Requirements in Units je Leg des Hedges oder 0 für den Wert der ContractSize
 */
double WINAPI symbol_MarginHedged(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginHedged);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den MarginDivider zur MarginBerechnung eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - MarginDivider
 */
double WINAPI symbol_MarginDivider(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginDivider);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die PointSize eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - PointSize
 */
double WINAPI symbol_PointSize(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->pointSize);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die PointsPerUnit eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return double - PointsPerUnit
 */
double WINAPI symbol_PointsPerUnit(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->pointsPerUnit);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die Marginwährung eines SYMBOLs zurück.
 *
 * @param  SYMBOL* symbol
 *
 * @return char* - Marginwährung
 */
const char* WINAPI symbol_MarginCurrency(const SYMBOL* symbol) {
   if ((uint)symbol < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   return(symbol->marginCurrency);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Namen eines SYMBOLs in einem Array zurück.
 *
 * @param  SYMBOL symbols[] - Array
 * @param  int    index     - Array-Index
 *
 * @return char* - Gruppenname
 */
const char* WINAPI symbols_Name(const SYMBOL symbols[], int index) {
   if ((uint)symbols < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbols = 0x%p (not a valid pointer)", symbols));
   if (index         < 0)                 return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index));
   return(symbols[index].name);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt den Array-Key eines SYMBOLs in einem Array zurück.
 *
 * @param  SYMBOL symbols[] - Array
 * @param  int    index     - Array-Index
 *
 * @return uint - Array-Key
 */
uint WINAPI symbols_ArrayKey(const SYMBOL symbols[], int index) {
   if ((uint)symbols < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbols = 0x%p (not a valid pointer)", symbols)));
   if (index         < 0)                 return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index)));
   return(symbols[index].arrayKey);
   #pragma EXPANDER_EXPORT
}


/**
 * Gibt die eindeutige ID eines SYMBOLs in einem Array zurück.
 *
 * @param  SYMBOL symbols[] - Array
 * @param  int    index     - Array-Index
 *
 * @return uint - ID
 */
uint WINAPI symbols_Id(const SYMBOL symbols[], int index) {
   if ((uint)symbols < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbols = 0x%p (not a valid pointer)", symbols)));
   if (index         < 0)                 return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d (not a valid index)", index)));
   return(symbols[index].id);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt den Namen eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  char*   name
 *
 * @return char* - derselbe Name
 */
const char* WINAPI symbol_SetName(SYMBOL* symbol, const char* name) {
   if ((uint)symbol < MIN_VALID_POINTER)     return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)name   < MIN_VALID_POINTER)     return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter name = 0x%p (not a valid pointer)", name));
   int len = strlen(name);
   if (!len || len > sizeof(symbol->name)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter name = \"%s\" (must be 1 to %d characters)", name, sizeof(symbol->name)-1));

   if (!strcpy(symbol->name, name))
      return(NULL);
   return(name);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Bechreibung eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  char*   description - eine vorhandene Beschreibung kann mit einem Leerstring gelöscht werden
 *
 * @return char* - dieselbe Beschreibung
 */
const char* WINAPI symbol_SetDescription(SYMBOL* symbol, const char* description) {
   if ((uint)symbol      < MIN_VALID_POINTER)               return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)description < MIN_VALID_POINTER)               return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter description = 0x%p (not a valid pointer)", description));
   if (strlen(description) > sizeof(symbol->description)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter description = \"%s\" (max %d characters)", description, sizeof(symbol->description)-1));

   if (!strcpy(symbol->description, description))
      return(NULL);
   return(description);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Basiswährung eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  char*   currency
 *
 * @return char* - dieselbe Basiswährung
 */
const char* WINAPI symbol_SetBaseCurrency(SYMBOL* symbol, const char* currency) {
   if ((uint)symbol   < MIN_VALID_POINTER)             return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)currency < MIN_VALID_POINTER)             return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter currency = 0x%p (not a valid pointer)", currency));
   int len = strlen(currency);
   if (len!=3 || len > sizeof(symbol->baseCurrency)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter currency = \"%s\" (3 characters)", currency));

   if (!strcpy(symbol->baseCurrency, currency))
      return(NULL);
   return(currency);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt den Gruppenindex eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  int     index
 *
 * @return int - derselbe Gruppenindex oder -1, falls ein Fehler auftrat
 */
int WINAPI symbol_SetGroup(SYMBOL* symbol, int index) {
   if ((uint)symbol < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol)));
   if (index < 0)                        return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d", index)));
   return(symbol->group = index);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Digits eines SYMBOLs und aktualisiert dabei die Felder SYMBOL.pointSize und SYMBOL.pointsPerUnit entsprechend.
 *
 * @param  SYMBOL* symbol
 * @param  int     digits
 *
 * @return int - dieselben Digits
 */
int WINAPI symbol_SetDigits(SYMBOL* symbol, int digits) {
   if ((uint)symbol < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if (digits < 0)                       return(error(ERR_INVALID_PARAMETER, "invalid parameter digits = %d", digits));
   symbol->digits = digits;

   int pointsPerUnit     = (int)(pow(10., digits) + 0.5);
   symbol->pointSize     = 1./pointsPerUnit;
   symbol->pointsPerUnit = pointsPerUnit;

   /*
   float val = 37.777779f;
   float rounded_down = floorf(val * 100) / 100;      // Result: 37.77
   float nearest      = roundf(val * 100) / 100;      // Result: 37.78
   float rounded_up   = ceilf (val * 100) / 100;      // Result: 37.78

   http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c

   Here's your generic macro that will work for arbitrary decimal places:

   #define ROUND_TO_DECIMAL_PLACES(x, decimal_places) (roundf(x * 1e##decimal_places) / 1e##decimal_places)
   smileyborg Mar 16 at 3:30
   */
   return(symbol->digits);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Hintergrundfarbe eines SYMBOLs im "Market Watch"-Window.
 *
 * @param  SYMBOL* symbol
 * @param  int     color
 *
 * @return int - dieselbe Farbe oder CLR_NONE, falls ein Fehler auftrat
 */
int WINAPI symbol_SetBackgroundColor(SYMBOL* symbol, int color) {
   if ((uint)symbol < MIN_VALID_POINTER) return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol)));
   if (color & 0xFF000000) {
      if (color != CLR_NONE)             return(_CLR_NONE(error(ERR_INVALID_PARAMETER, "invalid parameter color = 0x%p (not a valid color)", color)));
      color = White;                   // CLR_NONE wird vom Terminal als Black interpretiert
   }
   return(symbol->backgroundColor = color);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die eindeutige ID eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  int     id
 *
 * @return int - dieselbe ID oder -1, falls ein Fehler auftrat
 */
int WINAPI symbol_SetId(SYMBOL* symbol, int id) {
   if ((uint)symbol < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol)));
   if (id < 0)                           return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter id = %d", id)));
   return(symbol->id = id);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die Marginwährung eines SYMBOLs.
 *
 * @param  SYMBOL* symbol
 * @param  char*   currency
 *
 * @return char* - dieselbe Marginwährung
 */
const char* WINAPI symbol_SetMarginCurrency(SYMBOL* symbol, const char* currency) {
   if ((uint)symbol   < MIN_VALID_POINTER)               return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter symbol = 0x%p (not a valid pointer)", symbol));
   if ((uint)currency < MIN_VALID_POINTER)               return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter currency = 0x%p (not a valid pointer)", currency));
   int len = strlen(currency);
   if (len!=3 || len > sizeof(symbol->marginCurrency)-1) return((char*)error(ERR_INVALID_PARAMETER, "illegal length of parameter currency = \"%s\" (3 characters)", currency));

   if (!strcpy(symbol->marginCurrency, currency))
      return(NULL);
   return(currency);
   #pragma EXPANDER_EXPORT
}


/**
 * Setzt die eindeutige ID eines SYMBOLs in einem Array.
 *
 * @param  SYMBOL symbols[] - Array
 * @param  int    index     - Array-Index
 * @param  int    id
 *
 * @return int - dieselbe ID oder -1, falls ein Fehler auftrat
 */
int WINAPI symbols_SetId(SYMBOL symbols[], int index, int id) {
   if ((uint)symbols < MIN_VALID_POINTER) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter symbols = 0x%p (not a valid pointer)", symbols)));
   if (index         < 0)                 return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter index = %d", index)));
   return(symbol_SetId(&symbols[index], id));
   #pragma EXPANDER_EXPORT
}


/**
 * Vergleicht zwei Symbole anhand ihres Namens.
 *
 * @param  void* a
 * @param  void* b
 *
 * @return int - positiver Wert, wenn der Name von Symbol a größer als der von Symbol b ist;
 *               negativer Wert, wenn der Name von Symbol a kleiner als der von Symbol b ist;
 *               0, wenn die Namen beider Symbole gleich sind
 */
int __cdecl CompareSymbols(const void* a, const void* b) {
   SYMBOL* symbolA = (SYMBOL*) a;
   SYMBOL* symbolB = (SYMBOL*) b;

   if (symbolA == symbolB) return( 0);
   if (!symbolA)           return(-1);
   if (!symbolB)           return(+1);

   return(strcmp(symbolA->name, symbolB->name));
}


/**
 * Sortiert das übergebene SYMBOL-Array alphabetisch.
 *
 * @param  SYMBOL symbols[] - Array
 * @param  int    size      - Größe des Arrays
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SortSymbols(SYMBOL symbols[], int size) {
   if ((uint)symbols < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbols = 0x%p (not a valid pointer)", symbols));
   if (size <= 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size = %d", size));
   if (size == 1)          // nothing to sort
      return(TRUE);

   qsort(symbols, size, sizeof(SYMBOL), CompareSymbols);

   for (int i=0; i < size; i++) {            // Array-Key's neu zuordnen
      symbols[i].arrayKey = i;
   }
   return(TRUE);
   #pragma EXPANDER_EXPORT
}
