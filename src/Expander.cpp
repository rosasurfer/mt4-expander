#include "stdafx.h"


/**
 * DLL entry point 
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
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
 * Gibt den letzten aufgetretenen Windows-Fehler des aktuellen Threads zurück.
 *
 * @return int - Fehlercode
 *
 * NOTE: Wrapper für kernel32::GetLastError(), da MQL eine Funktion desselben Namens definiert.
 */
int GetLastWin32Error() {
   return(GetLastError());

   #pragma EXPORT_FUNCTION
}
