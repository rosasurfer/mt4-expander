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
         break;
   }
   */
   return(TRUE);
}


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
   if (lpValues && (int)lpValues < MIN_VALID_POINTER) {
      debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues);
      lpValues = NULL;
   }
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
   if (lpValues && (int)lpValues < MIN_VALID_POINTER) {
      debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues);
      lpValues = NULL;
   }
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
   if (lpValues && (int)lpValues < MIN_VALID_POINTER) {
      debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues);
      lpValues = NULL;
   }
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
   if (lpValues && (int)lpValues < MIN_VALID_POINTER) {
      debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues);
      lpValues = NULL;
   }
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
   if (lpValue && (int)lpValue < MIN_VALID_POINTER) {
      debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue);
      lpValue = NULL;
   }
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
const char* WINAPI GetString(const char* lpValue) {
   if (lpValue && (int)lpValue < MIN_VALID_POINTER) {
      debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue);
      lpValue = NULL;
   }
   return(lpValue);
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
const char* WINAPI DwordToHexStr(DWORD value) {
   int   size = 9;
   char* str  = new char[size];
   sprintf_s(str, size, "%p", value);

   return(str);
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI IntToHexStr(int value) {
   return(DwordToHexStr(value));
   #pragma EXPORT
}


/**
 * Gibt die lesbare Beschreibung einer ModuleType-ID zurück.
 *
 * @param  ModuleType id
 *
 * @return char* - Beschreibung oder NULL, falls die ID ungültig ist
 */
const char* ModuleTypeDescription(ModuleType id) {
   switch (id) {
      case MT_EXPERT   : return("Expert"   );
      case MT_SCRIPT   : return("Script"   );
      case MT_INDICATOR: return("Indicator");
      case MT_LIBRARY  : return("Library"  );
   }
   debug("unknown module type id = "+ to_string(id));
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
 *
 */
BOOL WINAPI expander_onInit(EXECUTION_CONTEXT* context) {
   if (!context) {
      debug("context=%p", context);
      return(FALSE);
   }

   //debug("context  programName=%s, programType=%d, whereami=%s", context->programName, context->programType, RootFunctionDescription(context->whereami));
   return(TRUE);
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI expander_onStart(EXECUTION_CONTEXT* context) {
   if (!context) {
      debug("context=%p", context);
      return(FALSE);
   }

   //debug("context  programName=%s, programType=%d, whereami=%s", context->programName, context->programType, RootFunctionDescription(context->whereami));
   return(TRUE);
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI expander_onDeinit(EXECUTION_CONTEXT* context) {
   if (!context) {
      debug("context=%p", context);
      return(FALSE);
   }

   //debug("context  programName=%s, programType=%d, whereami=%s", context->programName, context->programType, RootFunctionDescription(context->whereami));
   return(TRUE);
   #pragma EXPORT
}


/**
 *
 */
int WINAPI Test() {

   debug(RootFunctionDescription(RF_INIT));
   debug(RootFunctionDescription((RootFunction) 500));

   return(0);


   #pragma warning(push)
   #pragma warning(disable: 4996)   // std::basic_string<>::copy: Function call with parameters that may be unsafe

   std::string str("Hello world");
   int len = str.length();
   char* test = new char[len+1];
   str.copy(test, len);
   test[len] = '\0';

   #pragma warning(pop)

   debug("sizeof(EXECUTION_CONTEXT) = "+ to_string(sizeof(EXECUTION_CONTEXT)));

   /*
   debug("error.code=%d  error.message=%s", error->code, error->message);
   error->code    = 200;
   char* msg      = "200: OK";
   int   bufSize  = strlen(msg)+1;
   char* buffer   = new char[bufSize];
   strcpy_s(buffer, bufSize, msg);
   error->message = buffer;
   */

   /*
   auto_ptr<char> p(new char(10));
   int len = strlen(p.get());
   */

   return(0);
   #pragma EXPORT
}
