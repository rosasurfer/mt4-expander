/**
 * Der Expander des Fortschritts
 *
 *
 * @author  Peter Walther
 */
#include "Expander.h"


// Liste des letzten EXECUTION_CONTEXT je Thread
CRITICAL_SECTION    threadsSection;
DWORD*              threads;
size_t              threadsSize;
EXECUTION_CONTEXT** lastContext;


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
   switch (reason) {
      case DLL_PROCESS_ATTACH: //debug("DLL_PROCESS_ATTACH  threadId=%d", GetCurrentThreadId());
                               InitializeCriticalSection(&threadsSection);
                               break;

      case DLL_THREAD_ATTACH : //debug("DLL_THREAD_ATTACH   threadId=%d", GetCurrentThreadId());
                               break;

      case DLL_THREAD_DETACH : //debug("DLL_THREAD_DETACH   threadId=%d", GetCurrentThreadId());
                               ResetCurrentThreadData();
                               break;

      case DLL_PROCESS_DETACH: //debug("DLL_PROCESS_DETACH  threadId=%d", GetCurrentThreadId());
                               ResetCurrentThreadData();
                               DeleteCriticalSection(&threadsSection);
                               break;
   }
   return(TRUE);
}


/**
 *
 */
BOOL WINAPI Expander_onInit(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));

   TrackContext(ec);
   //debug("  thread=%d, ec=%p, ec.self=%p", GetCurrentThreadId(), ec, ec->self);
   //debug("  thread=%d, type=%s, name=%s", GetCurrentThreadId(), ModuleTypeDescription((ModuleType)ec->programType), ec->programName);

   return(TRUE);
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI Expander_onStart(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));

   TrackContext(ec);
   //debug(" thread=%d, ec=%p, ec.self=%p", GetCurrentThreadId(), ec, ec->self);

   return(TRUE);
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI Expander_onDeinit(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));

   TrackContext(ec);
   //debug("thread=%d, ec=%p, ec.self=%p", GetCurrentThreadId(), ec, ec->self);

   return(TRUE);
   #pragma EXPORT
}


/**
 * Aktualisiert den letzten bekannten EXECUTION_CONTEXT eines Threads. Wird indirekt von jeder MQL-Rootfunktion aufgerufen.
 * MQL-Libraries können dadurch den aktuellen EXECUTION_CONTEXT und darüber das sie aufrufende MQL-Programm ermitteln.
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Context eines MQL-Programms
 *
 * @return bool - Erfolgsstaus
 */
BOOL TrackContext(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));

   DWORD currentThread = GetCurrentThreadId();


   // (1) ThreadId im EXECUTION_CONTEXT setzen bzw. aktualisieren
   if (ec->hThreadId != currentThread)
      ec->hThreadId = currentThread;


   // (2) Thread in den bekannten Threads suchen
   size_t i;
   for (i=0; i < threadsSize; i++) {
      if (currentThread == threads[i])
         break;
   }
   if (i < threadsSize) {                                // Thread gefunden: letzten Context aktualisieren
      if (lastContext[i] != ec) {
         lastContext[i] = ec;
         //debug("switching context of thread at index %d", i);
      }
   }
   else {                                                // Thread nicht gefunden
      // synchronize() start
      // EnterCriticalSection(&threadsSection);
      int counter = 1;
      while (!TryEnterCriticalSection(&threadsSection)) { counter++; }
      if (counter > 1) debug("->TryEnterCriticalSection() initially failed but succeeded after %d tries", counter);


      for (i=0; i < threadsSize; i++) {                  // ersten freien Slot suchen
         if (!threads[i])
            break;
      }
      if (i < threadsSize) {
         debug("inserting thread at index %d", i);
      }
      else {                                             // kein freier Slot mehr vorhanden, Arrays vergrößern (Startwert: 8)
         size_t new_size = 2 * (threadsSize ? threadsSize : 8);
         debug("increasing arrays to %d", new_size);

         DWORD* tmp1 = (DWORD*) realloc(threads,     new_size * sizeof(DWORD)             ); if (!tmp1) return(debug("->realloc(threads) failed"));
         void** tmp2 = (void**) realloc(lastContext, new_size * sizeof(EXECUTION_CONTEXT*)); if (!tmp2) return(debug("->realloc(lastContext) failed"));

         for (size_t n=threadsSize; n < new_size; n++) { // neuen Speicherbereich initialisieren
            tmp1[n] =         NULL;
            tmp2[n] = (void*) NULL;
         }
         threads     =                       tmp1;       // und Zuweisung ändern
         lastContext = (EXECUTION_CONTEXT**) tmp2;
         threadsSize = new_size;

         if (!i) debug("adding thread at index %d", i);
         else    debug("adding thread at index %d, thread before was %d", i, threads[i-1]);
      }
      threads    [i] = currentThread;                    // Thread und letzten Context an Index i einfügen
      lastContext[i] = ec;

		LeaveCriticalSection(&threadsSection);
		// synchronize() end
   }

   return(TRUE);
}


/**
 * Setzt die EXECUTION_CONTEXT-Daten des aktuellen Threads zurück. Wird von DllMain() bei DLL_THREAD_DETACH und
 * DLL_PROCESS_DETACH aufgerufen.
 *
 * @return bool - Erfolgsstaus
 */
BOOL ResetCurrentThreadData() {
   DWORD currentThread = GetCurrentThreadId();

   // (1) aktuellen Thread in den Threads mit EXECUTION_CONTEXT suchen
   size_t i;
   for (i=0; i < threadsSize; i++) {
      if (currentThread == threads[i])
         break;
   }

   // (2) bei Sucherfolg Daten löschen, der Slot ist danach wieder verfügbar
   if (i < threadsSize) {
      // synchronize() start
      int counter = 1;
      while (!TryEnterCriticalSection(&threadsSection)) { counter++; }
      if (counter > 1) debug("->TryEnterCriticalSection() initially failed but succeeded after %d tries", counter);

      debug("dropping thread at index %d", i);
      threads    [i] =                      NULL;
      lastContext[i] = (EXECUTION_CONTEXT*) NULL;

		LeaveCriticalSection(&threadsSection);
		// synchronize() end
   }
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
const char* WINAPI GetString(const char* lpValue) {
   if (lpValue && (int)lpValue < MIN_VALID_POINTER)
      return((char*)debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue));
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
   return((char*)debug("unknown module type = "+ to_string(type)));
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
   return((char*)debug("unknown module type = "+ to_string(type)));
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
   return((char*)debug("unknown MQL root function id = "+ to_string(id)));
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
   return((char*)debug("unknown MQL root function id = "+ to_string(id)));
}


/**
 *
 */
int WINAPI Test() {

   debug("sizeof(LaunchType)=%d  sizeof(EXECUTION_CONTEXT)=%d", sizeof(LaunchType), sizeof(EXECUTION_CONTEXT));

   return(0);

   debug(RootFunctionDescription(RF_INIT));

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
