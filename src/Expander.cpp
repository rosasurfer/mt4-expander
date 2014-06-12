
#define WIN32_LEAN_AND_MEAN             
#include <windows.h>

#define EXPORT_FUNCTION comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)


/**
 * DLL entry point 
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	return(TRUE);
}


/**
 * Gibt die Speicheradresse eines MQL-Bool-Arrays zurück.
 *
 * @param  int* values[] - Zeiger auf MQL-Bool-Array (ist in MetaTrader als Integer-Array implementiert)
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetBoolsAddress(bool array[]);
 */
int GetBoolsAddress(int* values[]) {
   return((int) values);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zurück.
 *
 * @param  int* values[] - Zeiger auf MQL-Integer-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetIntsAddress(int array[]);
 */
int GetIntsAddress(int* values[]) {
   return((int) values);

   #pragma EXPORT_FUNCTION
}


/**
 * Alias für GetIntsAddress()
 *
 * MQL-Importdeklaration:  int GetBufferAddress(int buffer[]);
 */
int GetBufferAddress(int* values[]) {
   return(GetIntsAddress(values));

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zurück.
 *
 * @param  double* values[] - Zeiger auf MQL-Double-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetDoublesAddress(double array[]);
 */
int GetDoublesAddress(double* values[]) {
   return((int) values);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zurück.
 *
 * @param  void* values[] - Zeiger auf MQL-String-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetStringsAddress(string array[]);
 */
int GetStringsAddress(void* values[]) {
   return((int) values);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-Strings zurück.
 *
 * @param  char* value - Zeiger auf MQL-String, @see struct MqlStr.string
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetStringAddress(string value);
 */
int GetStringAddress(char* value) {
   return((int) value);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt den übergebenen Zeiger auf einen String selbst zurück. Kann in MQL zum Lesen eines Strings von einer 
 * Adresse verwendet werden, da MetaTrader den Zeiger automatisch in einen MQL-String konvertiert.
 *
 * @param  char* value - Zeiger auf MQL-String, @see struct MqlStr.string
 *
 * @return char* - derselbe Zeiger
 *
 *
 * MQL-Importdeklaration:  string GetString(int address);
 */
char* GetString(char* value) {
   return(value);

   #pragma EXPORT_FUNCTION
}
