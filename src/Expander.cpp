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
int WINAPI GetBoolsAddress(int values[]) {
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
int WINAPI GetIntsAddress(int values[]) {
   return((int) values);

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
 * @param  double values[] - MQL-Double-Array
 *
 * @return int - Speicheradresse
 *
 *
 * @mql-import:  int GetDoublesAddress(double array[]);
 */
int WINAPI GetDoublesAddress(double values[]) {
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
int WINAPI GetStringsAddress(MqlStr values[]) {
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
int WINAPI GetStringAddress(char* value) {
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
char* WINAPI GetString(char* value) {
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
 * Gibt die hexadezimale Repräsentation eines Dwords zurück.
 * z.B.: DwordToHexStr(13465610) => "00CD780A"
 *
 * @param  DWORD value - Dword (4 Byte, entspricht einem MQL-Integer)
 *
 * @return char* - hexadezimaler Wert mit 8 Stellen
 *
 *
 * @mql-import:  string DwordToHexStr(int value);
 */
char* WINAPI DwordToHexStr(DWORD value) {
   int   size = 9;
   char* string = new char[size];
   sprintf_s(string, size, "%p", value);

   return(string);
   #pragma EXPORT
}


/**
 * Alias
 */
char* WINAPI IntToHexStr(int value) {
   return(DwordToHexStr(value));
   #pragma EXPORT
}


/**
 * MetaQuotes-Alias
 */
char* WINAPI IntegerToHexString(int value) {
   return(DwordToHexStr(value));
   #pragma EXPORT
}


/**
 *
 * @return int
 *
int WINAPI dll_GetIntValue(int a) {
   int b = a + 666;
   return(b);

   #pragma EXPORT
}*/


/**
 * MT4i Publisher
 *
 * @return int
 *
int WINAPI SendReport4(char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, int i1, int i2, double dValues[]) {

   //s3=file:E:\Trading\MetaTrader\S7\experts\files\mt4ipub-header-9850480.tmp,
   //        E:\Trading\MetaTrader\S7\experts\files\mt4ipub-current-9850480.tmp,
   //        E:\Trading\MetaTrader\S7\experts\files\mt4ipub-historic-9850480.tmp

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
int WINAPI Expander_Init() {
   return(0);
   #pragma EXPORT
}


/**
 *
 */
int WINAPI Expander_Start() {
   return(0);
   #pragma EXPORT
}


/**
 *
 */
int WINAPI Expander_Deinit() {
   return(0);
   #pragma EXPORT
}


enum ProgramType  { PT_INDICATOR=1, PT_EXPERT=2, PT_SCRIPT=4 };
enum LaunchType   { LT_TEMPLATE=1, LT_PROGRAM, LT_MANUAL };
enum RootFunction { RF_INIT=1, RF_START, RF_DEINIT };


// Laufzeitdaten eines MQL-Programms
 struct EXECUTION_CONTEXT {
    unsigned int       id;

    ProgramType        programType;
    LPSTR              programName;
    unsigned int       initFlags;
    unsigned int       deinitFlags;
    EXECUTION_CONTEXT* superContext;

    LaunchType         launchType;
    unsigned int       uninitializeReason;
    RootFunction       whereami;

    LPSTR              symbol;
    unsigned int       timeframe;
    HWND               hChart;
    HWND               hChartWindow;
    HANDLE             hThreadId;
    unsigned int       testFlags;

    unsigned int       logLevel;
    LPSTR              logFile;

    unsigned int       events;

    unsigned int       lastError;
    LPSTR*             errorMessages;
    unsigned int       sizeErrorMessages;
 };


/**
 *
 */
int WINAPI Test() {

   EXECUTION_CONTEXT ec;
   debug("sizeof(EXECUTION_CONTEXT) = %d", sizeof(ec));

   auto_ptr<char> p2(new char(10));
   strlen(p2.get());

   return(777);
   #pragma EXPORT
}
