/**
 * Der Expander des Fortschritts
 * 
 *
 * @author  Peter Walther
 */
#include "Expander.h"


/**
 * DLL entry point 
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
   /*
   switch (reason) {
      case DLL_PROCESS_ATTACH: debug("DLL_PROCESS_ATTACH  threadId=%d", GetCurrentThreadId()); break;
      case DLL_THREAD_ATTACH : debug("DLL_THREAD_ATTACH   threadId=%d", GetCurrentThreadId()); break;
      case DLL_THREAD_DETACH : debug("DLL_THREAD_DETACH   threadId=%d", GetCurrentThreadId()); break;
      case DLL_PROCESS_DETACH: debug("DLL_PROCESS_DETACH  threadId=%d", GetCurrentThreadId()); break;
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
 * @mql-import:  int GetBoolsAddress(bool array[]);
 */
int GetBoolsAddress(int values[]) {
   return((int) values);

   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zurück.
 * 
 * @param  int values[] - MQL-Integer-Array
 *
 * @return int - Speicheradresse
 * 
 * 
 * @mql-import:  int GetIntsAddress(int array[]);
 */
int GetIntsAddress(int values[]) {
   return((int) values);

   #pragma EXPORT
}


/**
 * Alias für GetIntsAddress()
 * 
 *
 * @mql-import:  int GetBufferAddress(int buffer[]);
 */
int GetBufferAddress(int values[]) {
   return(GetIntsAddress(values));

   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zurück.
 *
 * @param  double values[] - MQL-Double-Array
 *
 * @return int - Speicheradresse
 *
 *
 * @mql-import:  int GetDoublesAddress(double array[]);
 */
int GetDoublesAddress(double values[]) {
   return((int) values);

   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zurück.
 *
 * @param  MqlStr values[] - MQL-String-Array
 *
 * @return int - Speicheradresse
 *
 *
 * @mql-import:  int GetStringsAddress(string values[]);
 */
int GetStringsAddress(MqlStr values[]) {
   return((int) values);

   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines Strings zurück.
 *
 * @param  char* value - String (MetaTrader übergibt für einen MQL-String das Element MqlStr.string)
 *
 * @return int - Speicheradresse
 *
 *
 * @mql-import:  int GetStringAddress(string value);
 */
int GetStringAddress(char* value) {
   return((int) value);

   #pragma EXPORT
}


/**
 * Gibt den übergebenen Zeiger auf einen String selbst zurück. Kann in MQL zum Lesen eines Strings von einer Adresse 
 * verwendet werden, da MetaTrader den Zeiger mit der entsprechenden Importdeklaration in einen MQL-String konvertiert.
 *
 * @param  char* value - String, @see struct MqlStr.string
 *
 * @return char* - derselbe Zeiger
 *
 *
 * @mql-import:  string GetString(int address);
 */
char* GetString(char* value) {
   return(value);

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
int GetLastWin32Error() {
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
BOOL IsBuiltinTimeframe(int timeframe) {
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
 * @mql-import:  bool pw_IsCustomTimeframe(int timeframe);
 */
 BOOL IsCustomTimeframe(int timeframe) {
   if (timeframe <= 0)
      return(FALSE);
   return(!IsBuiltinTimeframe(timeframe));

   #pragma EXPORT
}


/**
 *
 * @return int
 *
int dll_GetIntValue(int a) {
   int b = a + 666;
   return(b);

   #pragma EXPORT
}*/


/**
 *
 * @return int
 *
int SendReport4(char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, int i1, int i2, double dValues[]) {
/*
s3=file:E:\Trading\MetaTrader\S7\experts\files\mt4ipub-header-9850480.tmp,
        E:\Trading\MetaTrader\S7\experts\files\mt4ipub-current-9850480.tmp,
        E:\Trading\MetaTrader\S7\experts\files\mt4ipub-historic-9850480.tmp
*
   debug("s1=%s", s1);
   debug("s2=%s", s2);
   debug("s3=%s", s3);
   debug("s4=%s", s4);
   debug("s5=%s", s5);
   debug("s6=%s", s6);
   debug("i1=%d", i1);
   debug("i2=%d", i2);
   return(100);

   #pragma EXPORT
}*/


/**
 *
 */
int Test(char* s1, int i1, char* s2) {
   debug("s1=\"%s\"  i1=0x%p  s2=\"%s\"", s1, i1, s2);   
   return(0);

   #pragma EXPORT
}
