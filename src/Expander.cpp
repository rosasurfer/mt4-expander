
#define WIN32_LEAN_AND_MEAN             
#include <windows.h>

#define EXPORT_FUNCTION comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)


/**
 * DLL entry point 
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	return(true);
}


/**
 * Gibt den übergebenen Zeiger auf einen String zurück. Kann in MQL zum Lesen eines Strings von einer Adresse 
 * verwendet werden.
 *
 * @param  char* str - Zeiger
 *
 * @return char* - derselbe Zeiger
 *
 *
 * MQL-Importdeklaration:  string GetString(int address);
 */
char* GetString(char* str) {
   return(str);

   #pragma EXPORT_FUNCTION
}
