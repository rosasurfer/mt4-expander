/**
 * Der Expander des Fortschritts
 *
 * @author  Peter Walther
 */
#include "stdafx.h"
#include "global.h"
#include "Expander.h"


/**
 * Gibt die Speicheradresse eines MQL-Bool-Arrays zurück.
 *
 * @param  BOOL lpValues[] - MQL-Bool-Array (in MetaTrader als Integer-Array implementiert)
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetBoolsAddress(bool array[]);
 */
int WINAPI GetBoolsAddress(BOOL lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zurück.
 *
 * @param  int lpValues[] - MQL-Integer-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetIntsAddress(int array[]);
 */
int WINAPI GetIntsAddress(int lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Alias
 */
int WINAPI GetBufferAddress(int values[]) {
   return(GetIntsAddress(values));
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zurück.
 *
 * @param  double lpValues[] - MQL-Double-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetDoublesAddress(double array[]);
 */
int WINAPI GetDoublesAddress(double lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zurück.
 *
 * @param  MqlStr lpValues[] - MQL-String-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetStringsAddress(string values[]);
 */
int WINAPI GetStringsAddress(MqlStr lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines Strings zurück.
 *
 * @param  char* lpValue - String (MetaTrader übergibt für einen MQL-String das Element MqlStr.string)
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetStringAddress(string value);
 */
int WINAPI GetStringAddress(const char* lpValue) {
   if (lpValue && (int)lpValue < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue));
   return((int) lpValue);
   #pragma EXPORT
}


/**
 * Gibt den übergebenen Zeiger auf einen String selbst zurück. Kann in MQL zum Lesen eines Strings von einer Adresse
 * verwendet werden, da MetaTrader den Zeiger mit der entsprechenden Importdeklaration in einen MQL-String konvertiert.
 *
 * @param  char* lpValue - String
 *
 * @return char* - derselbe Zeiger oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  string GetString(int address);
 */
char* WINAPI GetString(const char* lpValue) {
   if (lpValue && (int)lpValue < MIN_VALID_POINTER)
      return((char*) debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue));
   return((char*) lpValue);
   #pragma EXPORT
}


/**
 * Gibt den letzten aufgetretenen Windows-Fehler des aktuellen Threads zurück. Wrapper für kernel32::GetLastError(),
 * da MQL eine Funktion desselben Namens definiert.
 *
 * @return int - Fehlercode
 *
 *
 * @mql-import:  int GetLastWin32Error();
 */
int WINAPI GetLastWin32Error() {
   return(GetLastError());
   #pragma EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen eingebauten MetaTrader-Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 *
 *
 * @mql-import:  bool IsBuiltinTimeframe(int timeframe);
 */
BOOL WINAPI IsBuiltinTimeframe(int timeframe) {
   switch (timeframe) {
      case PERIOD_M1 : return(TRUE);
      case PERIOD_M5 : return(TRUE);
      case PERIOD_M15: return(TRUE);
      case PERIOD_M30: return(TRUE);
      case PERIOD_H1 : return(TRUE);
      case PERIOD_H4 : return(TRUE);
      case PERIOD_D1 : return(TRUE);
      case PERIOD_W1 : return(TRUE);
      case PERIOD_MN1: return(TRUE);
   }
   return(FALSE);
   #pragma EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen benutzerdefinierten Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 *
 *
 * @mql-import:  bool IsCustomTimeframe(int timeframe);
 */
 BOOL WINAPI IsCustomTimeframe(int timeframe) {
   if (timeframe <= 0)
      return(FALSE);
   return(!IsBuiltinTimeframe(timeframe));
   #pragma EXPORT
}


/**
 * Gibt die hexadezimale Repräsentation eines Integers zurück.
 * z.B.: IntToHexStr(13465610) => "00CD780A"
 *
 * @param  int value - Integer (4 Byte)
 *
 * @return char* - hexadezimaler String mit 8 Zeichen
 *
 *
 * @mql-import:  string IntToHexStr(int value);
 */
char* WINAPI IntToHexStr(int value) {
   int   size = 9;
   char* str  = new char[size];
   sprintf_s(str, size, "%p", value);

   return(str);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Beschreibung eines ModuleTypes zurück.
 *
 * @param  ModuleType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* ModuleTypeDescription(ModuleType type) {
   switch (type) {
      case MT_EXPERT   : return("Expert"   );
      case MT_SCRIPT   : return("Script"   );
      case MT_INDICATOR: return("Indicator");
      case MT_LIBRARY  : return("Library"  );
   }
   debug("unknown module type = "+ to_string(type));
   return(NULL);
}


/**
 * Gibt die lesbare Konstante eines ModuleTypes zurück.
 *
 * @param  ModuleType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* ModuleTypeToStr(ModuleType type) {
   switch (type) {
      case MT_EXPERT   : return("MT_EXPERT"   );
      case MT_SCRIPT   : return("MT_SCRIPT"   );
      case MT_INDICATOR: return("MT_INDICATOR");
      case MT_LIBRARY  : return("MT_LIBRARY"  );
   }
   debug("unknown module type = "+ to_string(type));
   return(NULL);
}


/**
 * Gibt die lesbare Beschreibung einer RootFunction-ID zurück.
 *
 * @param  RootFunction id
 *
 * @return char* - Beschreibung oder NULL, falls die ID ungültig ist
 */
const char* RootFunctionDescription(RootFunction id) {
   switch (id) {
      case RF_INIT  : return("init()"  );
      case RF_START : return("start()" );
      case RF_DEINIT: return("deinit()");
   }
   debug("unknown MQL root function id = "+ to_string(id));
   return(NULL);
}


/**
 * Gibt die lesbare Konstante einer RootFunction-ID zurück.
 *
 * @param  RootFunction id
 *
 * @return char* - Beschreibung oder NULL, falls die ID ungültig ist
 */
const char* RootFunctionToStr(RootFunction id) {
   switch (id) {
      case RF_INIT  : return("init()"  );
      case RF_START : return("start()" );
      case RF_DEINIT: return("deinit()");
   }
   debug("unknown MQL root function id = "+ to_string(id));
   return(NULL);
}
