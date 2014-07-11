#include <iostream>
#include <string>
#include "stdafx.h"


using namespace std;


/**
 * DLL entry point 
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
   /*
   switch (reason) {
      case DLL_PROCESS_ATTACH: OutputDebugStringA("MetaTrader::DLL_PROCESS_ATTACH");   break;
      case DLL_THREAD_ATTACH : OutputDebugStringA("MetaTrader::DLL_THREAD_ATTACH");    break;
      case DLL_THREAD_DETACH : OutputDebugStringA("MetaTrader::DLL_THREAD_DETACH");    break;
      case DLL_PROCESS_DETACH: OutputDebugStringA("MetaTrader::DLL_PROCESS_DEATTACH"); break;
   }
   */
   return(TRUE);
}


/**
 * Gibt die Speicheradresse eines MQL-Bool-Arrays zurück.
 *
 * @param  int values[] - MQL-Bool-Array (in MetaTrader als Integer-Array implementiert)
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetBoolsAddress(bool array[]);
 */
int GetBoolsAddress(const int values[]) {
   return((int) values);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zurück.
 *
 * @param  int values[] - MQL-Integer-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetIntsAddress(int array[]);
 */
int GetIntsAddress(const int values[]) {
   return((int) values);

   #pragma EXPORT_FUNCTION
}


/**
 * Alias für GetIntsAddress()
 *
 * MQL-Importdeklaration:  int GetBufferAddress(int buffer[]);
 */
int GetBufferAddress(const int values[]) {
   return(GetIntsAddress(values));

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zurück.
 *
 * @param  double values[] - MQL-Double-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetDoublesAddress(double array[]);
 */
int GetDoublesAddress(const double values[]) {
   return((int) values);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zurück.
 *
 * @param  MqlStr values[] - MQL-String-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetStringsAddress(string values[]);
 */
int GetStringsAddress(const MqlStr values[]) {
   return((int) values);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines Strings zurück.
 *
 * @param  char* value - String (MetaTrader übergibt für einen MQL-String das Element MqlStr.string)
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetStringAddress(string value);
 */
int GetStringAddress(const char* value) {
   /*
   int size = 1000; 
   char* msg = (char*)_alloca(size);
   sprintf_s(msg, size, "MetaTrader::%s::%s(%d)",__FILE__, __FUNCTION__, __LINE__); 
   OutputDebugStringA(msg);
   */
   return((int) value);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt den übergebenen Zeiger auf einen String selbst zurück. Kann in MQL zum Lesen eines Strings von einer 
 * Adresse verwendet werden, da MetaTrader den Zeiger automatisch in einen MQL-String konvertiert.
 *
 * @param  char* value - String, @see struct MqlStr.string
 *
 * @return char* - derselbe Zeiger
 *
 *
 * MQL-Importdeklaration:  string GetString(int address);
 */
char* GetString(const char* value) {
   return((char*) value);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt den letzten aufgetretenen Windows-Fehler des aktuellen Threads zurück. Wrapper für kernel32::GetLastError(), 
 * da MQL eine Funktion desselben Namens definiert.
 *
 * @return int - Fehlercode
 */
int GetLastWin32Error() {
   return(GetLastError());

   #pragma EXPORT_FUNCTION
}


/**
 * Ob die angegebene Timeframe-ID einen eingebauten MetaTrader-Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 */
BOOL IsBuiltinTimeframe(const int timeframe) {
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

   #pragma EXPORT_FUNCTION
}


/**
 * Ob die angegebene Timeframe-ID einen benutzerdefinierten Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 */
 BOOL IsCustomTimeframe(const int timeframe) {
   if (timeframe <= 0)
      return(FALSE);
   return(!IsBuiltinTimeframe(timeframe));

   #pragma EXPORT_FUNCTION
}
