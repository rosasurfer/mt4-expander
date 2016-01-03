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
 * @param  BOOL values[] - MQL-Bool-Array (in MetaTrader als Integer-Array implementiert)
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetBoolsAddress(bool array[]);
 */
int WINAPI GetBoolsAddress(const BOOL values[]) {
   if (values && (uint)values < MIN_VALID_POINTER)
      return(debug("ERROR:  invalid parameter values = 0x%p (not a valid pointer)", values));
   return((int) values);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zurück.
 *
 * @param  int values[] - MQL-Integer-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetIntsAddress(int array[]);
 */
int WINAPI GetIntsAddress(const int values[]) {
   if (values && (uint)values < MIN_VALID_POINTER)
      return(debug("ERROR:  invalid parameter values = 0x%p (not a valid pointer)", values));
   return((int) values);
   #pragma EXPORT
}


/**
 * Alias
 */
int WINAPI GetBufferAddress(const int values[]) {
   return(GetIntsAddress(values));
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zurück.
 *
 * @param  double values[] - MQL-Double-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetDoublesAddress(double array[]);
 */
int WINAPI GetDoublesAddress(const double values[]) {
   if (values && (uint)values < MIN_VALID_POINTER)
      return(debug("ERROR:  invalid parameter values = 0x%p (not a valid pointer)", values));
   return((int) values);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zurück.
 *
 * @param  MqlStr values[] - MQL-String-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetStringsAddress(string values[]);
 */
int WINAPI GetStringsAddress(const MqlStr values[]) {
   if (values && (uint)values < MIN_VALID_POINTER)
      return(debug("ERROR:  invalid parameter values = 0x%p (not a valid pointer)", values));
   return((int) values);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines Strings zurück.
 *
 * @param  char* value - C-String (MetaTrader übergibt für einen MQL-String das Element MqlStr.string)
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetStringAddress(string value);
 */
int WINAPI GetStringAddress(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER)
      return(debug("ERROR:  invalid parameter value = 0x%p (not a valid pointer)", value));
   return((int) value);
   #pragma EXPORT
}


/**
 * Gibt den übergebenen Zeiger auf einen String selbst zurück. Wird in MQL zum Lesen eines C-Strings von
 * einer Adresse verwendet, da MetaTrader einen C-String automatisch in einen MQL-String konvertiert.
 *
 * @param  char* value - String
 *
 * @return char* - derselbe Zeiger oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  string GetString(int address);
 */
char* WINAPI GetString(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER)
      return((char*) debug("ERROR:  invalid parameter value = 0x%p (not a valid pointer)", value));
   return((char*) value);
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
   char* szchar = new char[size];                                    // TODO: Speicherleck schließen
   sprintf_s(szchar, size, "%p", value);

   return(szchar);
   #pragma EXPORT
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
   debug("ERROR:  unknown module type = "+ to_string(type));
   return(NULL);
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
   debug("ERROR:  unknown module type = "+ to_string(type));
   return(NULL);
}


/**
 * Gibt die lesbare Konstante eines ProgramTypes zurück.
 *
 * @param  ProgramType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* ProgramTypeToStr(ProgramType type) {
   switch (type) {
      case PT_EXPERT   : return("PT_EXPERT"   );
      case PT_SCRIPT   : return("PT_SCRIPT"   );
      case PT_INDICATOR: return("PT_INDICATOR");
   }
   debug("ERROR:  unknown program type = "+ to_string(type));
   return(NULL);
}


/**
 * Gibt die lesbare Beschreibung eines ProgramTypes zurück.
 *
 * @param  ProgramType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* ProgramTypeDescription(ProgramType type) {
   switch (type) {
      case PT_EXPERT   : return("Expert"   );
      case PT_SCRIPT   : return("Script"   );
      case PT_INDICATOR: return("Indicator");
   }
   debug("ERROR:  unknown program type = "+ to_string(type));
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
      case RF_INIT  : return("RF_INIT"  );
      case RF_START : return("RF_START" );
      case RF_DEINIT: return("RD_DEINIT");
   }
   debug("ERROR:  unknown MQL root function id = "+ to_string(id));
   return(NULL);
}


/**
 * Gibt den Namen einer RootFunction zurück.
 *
 * @param  RootFunction id
 *
 * @return char* - Name oder NULL, falls die ID ungültig ist
 */
const char* RootFunctionName(RootFunction id) {
   switch (id) {
      case RF_INIT  : return("init"  );
      case RF_START : return("start" );
      case RF_DEINIT: return("deinit");
   }
   debug("ERROR:  unknown MQL root function id = "+ to_string(id));
   return(NULL);
}


/**
 * Gibt die lesbare Konstante einer Timeframe-ID zurück.
 *
 * @param  int period - Timeframe-ID
 *
 * @return char*
 */
const char* PeriodToStr(int period) {
   switch (period) {
      case PERIOD_M1 : return("PERIOD_M1" );     // 1 minute
      case PERIOD_M5 : return("PERIOD_M5" );     // 5 minutes
      case PERIOD_M15: return("PERIOD_M15");     // 15 minutes
      case PERIOD_M30: return("PERIOD_M30");     // 30 minutes
      case PERIOD_H1 : return("PERIOD_H1" );     // 1 hour
      case PERIOD_H4 : return("PERIOD_H4" );     // 4 hour
      case PERIOD_D1 : return("PERIOD_D1" );     // 1 day
      case PERIOD_W1 : return("PERIOD_W1" );     // 1 week
      case PERIOD_MN1: return("PERIOD_MN1");     // 1 month
      case PERIOD_Q1 : return("PERIOD_Q1" );     // 1 quarter
   }
   debug("unknown timeframe id = "+ to_string(period));
   return(NULL);
}


/**
 * Gibt die Beschreibung einer Timeframe-ID zurück.
 *
 * @param  int period - Timeframe-ID bzw. Anzahl der Minuten je Chart-Bar
 *
 * @return char*
 */
const char* PeriodDescription(int period) {
   switch (period) {
      case PERIOD_M1 : return("M1" );     // 1 minute
      case PERIOD_M5 : return("M5" );     // 5 minutes
      case PERIOD_M15: return("M15");     // 15 minutes
      case PERIOD_M30: return("M30");     // 30 minutes
      case PERIOD_H1 : return("H1" );     // 1 hour
      case PERIOD_H4 : return("H4" );     // 4 hour
      case PERIOD_D1 : return("D1" );     // 1 day
      case PERIOD_W1 : return("W1" );     // 1 week
      case PERIOD_MN1: return("MN1");     // 1 month
      case PERIOD_Q1 : return("Q1" );     // 1 quarter
   }

   int size = _scprintf("%d", period) + 1;
   char* szchar = new char[size];                                    // TODO: Speicherleck schließen
   sprintf_s(szchar, size, "%d", period);

   return(szchar);
}


/**
 * Alias
 */
const char* TimeframeToStr(int timeframe) {
   return(PeriodToStr(timeframe));
}


/**
 * Alias
 */
const char* TimeframeDescription(int timeframe) {
   return(PeriodDescription(timeframe));
}


/**
 * Gibt das Handle des Terminal-Hauptfensters zurück.
 *
 * @return HWND - Handle oder 0, falls ein Fehler auftrat
 */
HWND WINAPI GetApplicationWindow() {
   static HWND hWnd;

   if (!hWnd) {
      HWND  hWndNext = GetTopWindow(NULL);
      DWORD processId, myProcessId=GetCurrentProcessId();

      // alle Top-Level-Windows durchlaufen
      while (hWndNext) {
         GetWindowThreadProcessId(hWndNext, &processId);
         if (processId == myProcessId) {

            // ClassName des Fensters ermitteln
            int   size = 255;
            char* className = (char*) alloca(size);            // auf dem Stack
            int   copied = GetClassName(hWndNext, className, size);
            if (!copied) return((HWND)debug("ERROR:  GetClassName() 0 chars copied  [%d] ", GetLastError()));

            while (copied >= size-1) {                         // GetClassName() gibt die Anzahl der kopierten Zeichen zurück (ohne \0).
               size <<= 1;                                     // Bei size-1 ist unklar, ob der String genau in den Buffer paßte oder nicht.
               className = (char*) alloca(size);               // auf dem Stack
               copied    = GetClassName(hWndNext, className, size);
            }
            if (!copied) return((HWND)debug("ERROR:  GetClassName() 0 chars copied  [%d]", GetLastError()));

            // Klasse mit der Klasse des Terminal-Hauptfensters vergleichen
            if (strcmp(className, "MetaQuotes::MetaTrader::4.00") == 0)
               break;
         }
         hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
      }
      if (!hWndNext) debug("ERROR:  cannot find application main window [%d]", GetLastError());
      hWnd = hWndNext;
   }

   return(hWnd);
   #pragma EXPORT
}


/**
 * Gibt die ID des Userinterface-Threads zurück.
 *
 * @return DWORD - Thread-ID (nicht das Thread-Handle) oder 0, falls ein Fehler auftrat
 */
DWORD WINAPI GetUIThreadId() {
   static DWORD uiThreadId;

   if (!uiThreadId) {
      HWND hWnd = GetApplicationWindow();
      if (hWnd)
         uiThreadId = GetWindowThreadProcessId(hWnd, NULL);
   }
   return(uiThreadId);
   #pragma EXPORT
}


/**
 * Ob der aktuelle Thread der UI-Thread ist.
 *
 * @return BOOL
 */
BOOL WINAPI IsUIThread() {
   return(GetCurrentThreadId() == GetUIThreadId());
   #pragma EXPORT
}


/**
 * Gibt die aktuelle GMT-Zeit des Systems zurück (auch im Tester).
 *
 * @return uint - Unix-Timestamp der GMT-Zeit
 */
uint WINAPI GetGmtTime() {
   // @see  MSDN for manual replacement of the non-public function RtlTimeToSecondsSince1970()
   SYSTEMTIME     stNow,    st1970={1970, 1, 4, 1, 0, 0, 0, 0};
   FILETIME       ftNow,    ft1970;
   ULARGE_INTEGER ulintNow, ulint1970;

   GetSystemTime(&stNow);
   if (!SystemTimeToFileTime(&stNow, &ftNow))   return(debug("ERROR:  SystemTimeToFileTime() failed"));
   ulintNow.LowPart   = ftNow.dwLowDateTime;
   ulintNow.HighPart  = ftNow.dwHighDateTime;

   if (!SystemTimeToFileTime(&st1970, &ft1970)) return(debug("ERROR:  SystemTimeToFileTime() failed"));
   ulint1970.LowPart  = ft1970.dwLowDateTime;
   ulint1970.HighPart = ft1970.dwHighDateTime;

   uint secondsSince1970 = (uint) ((ulintNow.QuadPart - ulint1970.QuadPart)/10000000);
   return(secondsSince1970);
   #pragma EXPORT
}


/**
 * Gibt die aktuelle lokale Zeit zurück (auch im Tester).
 *
 * @return uint - Unix-Timestamp der lokalen Zeit
 */
uint WINAPI GetLocalTime() {
   // @see  MSDN for manual replacement of the non-public function RtlTimeToSecondsSince1970()
   SYSTEMTIME     stNow,    st1970={1970, 1, 4, 1, 0, 0, 0, 0};
   FILETIME       ftNow,    ft1970;
   ULARGE_INTEGER ulintNow, ulint1970;

   GetLocalTime(&stNow);
   if (!SystemTimeToFileTime(&stNow, &ftNow))   return(debug("ERROR:  SystemTimeToFileTime() failed"));
   ulintNow.LowPart   = ftNow.dwLowDateTime;
   ulintNow.HighPart  = ftNow.dwHighDateTime;

   if (!SystemTimeToFileTime(&st1970, &ft1970)) return(debug("ERROR:  SystemTimeToFileTime() failed"));
   ulint1970.LowPart  = ft1970.dwLowDateTime;
   ulint1970.HighPart = ft1970.dwHighDateTime;

   uint secondsSince1970 = (uint) ((ulintNow.QuadPart - ulint1970.QuadPart)/10000000);
   return(secondsSince1970);
   #pragma EXPORT
}


/**
 * Konvertiert den angegebenen WCHAR-String in einen CHAR-String.
 *
 * @param  WCHAR wcstr - wide character string
 * @param  int   count - number of wide characters to convert
 *
 * @return char*
 */
char* WINAPI WCharsToStr(const WCHAR* wcstr, size_t count) {
   //size_t t = wcstombs(mbstr, wcstr, size);
   return((char*)NULL);
   //#pragma EXPORT
}


// Verwaltungsdaten eines TickTimers
struct TICK_TIMER_DATA {
   uint  id;                                 // Timer-ID
   HWND  hWnd;                               // Chartfenster, das Ticks empfängt
   DWORD flags;                              // Timer-Flags (Tickkonfiguration)
   DWORD userdata1;
   DWORD userdata2;                          // event-übergreifende User-Daten (Cookies etc.)
   DWORD userdata3;
};
std::vector<TICK_TIMER_DATA> tickTimers;     // Daten aller aktiven TickTimer


/**
 * Callback function for WM_TIMER messages.
 *
 * @param  HWND     hWnd    - Handle to the window associated with the timer.
 * @param  UINT     msg     - Specifies the WM_TIMER message.
 * @param  UINT_PTR timerId - Specifies the timer's identifier.
 * @param  DWORD    time    - Specifies the number of milliseconds that have elapsed since the system was started. This is the value
 *                            returned by the GetTickCount() function.
 */
VOID CALLBACK TimerCallback(HWND hWnd, UINT msg, UINT_PTR timerId, DWORD time) {
   int size = tickTimers.size();

   for (int i=0; i < size; i++) {
      if (tickTimers[i].id == timerId) {
         TICK_TIMER_DATA tt = tickTimers[i];

         if (tt.flags & TICK_OFFLINE_REFRESH) {
            RECT rect;                                                           // ermitteln, ob der Chart mindestens teilweise sichtbar ist
            HDC hDC = GetDC(hWnd);
            int rgn = GetClipBox(hDC, &rect);
            ReleaseDC(hWnd, hDC);

            if (rgn == RGN_ERROR) {
               debug("ERROR:  GetClipBox(hDC=%p) => RGN_ERROR (win32 error %d)", hDC, GetLastError());
            }
            else if (rgn != NULLREGION) {                                        // Tick nur bei sichtbarem Chart verschicken
               PostMessageA(hWnd, WM_COMMAND, ID_CHART_REFRESH, 0);              // refresh tick
            }
         }
         else {
            PostMessageA(hWnd, MT4InternalMsg(), MT4_TICK, TICK_OFFLINE_EA);     // default tick
         }

         if (tt.flags & TICK_PAUSE_ON_WEEKEND) {
         }
         return;
      }
   }

   debug("ERROR:  timer not found, timerId = %d", timerId);
}


/**
 * Installiert einen Timer, der dem angegebenen Fenster synthetische Ticks schickt.
 *
 * @param  HWND  hWnd   - Handle des Fensters, an das die Ticks geschickt werden.
 * @param  int   millis - Zeitperiode der zu generierenden Ticks in Millisekunden
 * @param  DWORD flags  - die Ticks konfigurierende Flags (default: keine)
 *                        TICK_OFFLINE_REFRESH : Mit diesem Flag wird ein die Ticks empfangener Offline-Chart bei jedem Tick refreshed,
 *                                               ohne dieses Flag wird ein gewöhnlicher synthetischer Tick generiert (default).
 *                        TICK_PAUSE_ON_WEEKEND: Mit diesem Flag werden von Samstag, 00:00 FXT bis Montag, 00:00 FXT keine Ticks generiert,
 *                                               ohne dieses Flag werden auch am Wochenende Ticks generiert (default).
 *
 * @return uint - ID des installierten Timers zur Übergabe an RemoveTickTimer() bei Deinstallation des Timers oder 0, falls ein Fehler auftrat.
 */
uint WINAPI SetupTickTimer(HWND hWnd, int millis, DWORD flags=NULL) {
   // Parametervalidierung
   DWORD threadId = GetWindowThreadProcessId(hWnd, NULL);
   if (threadId != GetCurrentThreadId()) {
      if (!threadId)           return(debug("ERROR:  invalid parameter hWnd = %d (not a window)", hWnd));
                               return(debug("ERROR:  window hWnd = %d not owned by the current thread", hWnd));
   }
   if (millis <= 0)            return(debug("ERROR:  invalid parameter millis = %d", millis));
   if (flags & TICK_PAUSE_ON_WEEKEND) debug("flag TICK_PAUSE_ON_WEEKEND not yet supported");

   // neue Timer-ID erzeugen
   static uint timerId = 10000;
   timerId++;

   // Timer setzen
   uint result = SetTimer(hWnd, timerId, millis, (TIMERPROC)TimerCallback);
   if (result != timerId)                             // muß stimmen, da hWnd immer != NULL
      return(debug("ERROR:  SetTimer(hWnd=%d, timerId=%d, millis=%d) failed with %d", hWnd, timerId, millis, result));
   //debug("SetTimer(hWnd=%d, timerId=%d, millis=%d) success", hWnd, timerId, millis);

   // Timerdaten speichern
   TICK_TIMER_DATA tt = {timerId, hWnd, flags};
   tickTimers.push_back(tt);

   return(timerId);
   #pragma EXPORT
}


/**
 * Deinstalliert einen mit SetupTickTimer() installierten Timer.
 *
 * @param  int timerId - ID des Timers, wie von SetupTickTimer() zurückgegeben.
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI RemoveTickTimer(int timerId) {
   if (timerId <= 0) return(debug("ERROR:  invalid parameter timerId = %d", timerId));

   int size = tickTimers.size();

   for (int i=0; i < size; i++) {
      if (tickTimers[i].id == timerId) {
         if (!KillTimer(tickTimers[i].hWnd, timerId))
            return(debug("ERROR:  KillTimer(hWnd=%d, timerId=%d) failed", tickTimers[i].hWnd, timerId));

         //debug("KillTimer(hWnd=%d, timerId=%d) success", tickTimers[i].hWnd, timerId);
         tickTimers.erase(tickTimers.begin() + i);
         return(TRUE);
      }
   }
   return(debug("ERROR:  no such timer with timerId = %d", timerId));
   #pragma EXPORT
}


/**
 * Deinstalliert alle mit SetupTickTimer() installierten Timer, die nicht explizit deinstalliert wurden.
 * Wird in onProcessDetach() aufgerufen.
 */
void RemoveTickTimers() {
   int size = tickTimers.size();

   for (int i=size-1; i>=0; i--) {                 // rückwärts, da der Vector in RemoveTickTimer() modifiziert wird
      uint id = tickTimers[i].id;
      debug("WARN:  removing orphaned tickTimer with id = %d", id);
      RemoveTickTimer(id);
   }
}


/**
 * Wrapper für die Win32-API-Funktion GetProp(). Gibt den Wert einer Window-Property zurück.
 *
 * @param  HWND  hWnd   - Fensterhandle
 * @param  char* lpName - Property-Name
 *
 * @return HANDLE - Property-Value
 */
HANDLE WINAPI GetWindowProperty(const HWND hWnd, const char* lpName) {
   return(GetProp(hWnd, lpName));
   #pragma EXPORT
}


/**
 * Wrapper für die Win32-API-Funktion RemoveProp(). Gibt den Wert einer Window-Property zurück und löscht sie.
 *
 * @param  HWND  hWnd   - Fensterhandle
 * @param  char* lpName - Property-Name
 *
 * @return HANDLE - Property-Value
 */
HANDLE WINAPI RemoveWindowProperty(const HWND hWnd, const char* lpName) {
   return(RemoveProp(hWnd, lpName));
   #pragma EXPORT
}


/**
 * Wrapper für die Win32-API-Funktion SetProp(). Setzt eine Window-Property.
 *
 * @param  HWND   hWnd   - Fensterhandle
 * @param  char*  lpName - Property-Name
 * @param  HANDLE value  - Property-Value
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI SetWindowProperty(const HWND hWnd, const char* lpName, const HANDLE value) {
   return(SetProp(hWnd, lpName, value));
   #pragma EXPORT
}
