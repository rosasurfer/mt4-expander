#include "Expander.h"


/**
 * Gibt die Speicheradresse eines MQL-Bool-Arrays zurück.
 *
 * @param  BOOL values[] - MQL-Bool-Array (in MetaTrader als Integer-Array implementiert)
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 */
uint WINAPI GetBoolsAddress(const BOOL values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values = 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zurück.
 *
 * @param  int values[] - MQL-Integer-Array
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 */
uint WINAPI GetIntsAddress(const int values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values = 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zurück.
 *
 * @param  double values[] - MQL-Double-Array
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 */
uint WINAPI GetDoublesAddress(const double values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values = 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zurück.
 *
 * @param  MqlStr values[] - MQL-String-Array
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 */
uint WINAPI GetStringsAddress(const MqlStr values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values = 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines C-Strings zurück.
 *
 * @param  char* value - C-String: MetaTrader übergibt für einen MQL-String das Element MqlStr.string
 *
 * @return uint - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 * Achtung: GetStringAddress() darf in MQL nur mit Array-Elementen verwendet werden. Ist der Parameter ein einfacher String,
 *          wird an die DLL eine Kopie dieses Strings übergeben. Diese Kopie wird u.U. sofort nach Rückkehr freigegeben und
 *          die erhaltene Adresse ist ungültig (z.B. im Tester bei mehrfachen Tests).
 */
uint WINAPI GetStringAddress(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter value = 0x%p (not a valid pointer)", value));
   return((uint) value);
   #pragma EXPORT
}


/**
 * Gibt den übergebenen Zeiger auf einen C-String selbst zurück. Wird in MQL zum Lesen eines Strings von
 * einer Adresse verwendet, da MetaTrader einen C-String automatisch in einen MQL-String konvertiert.
 *
 * @param  char* value - C-String
 *
 * @return char* - derselbe Zeiger oder NULL, falls ein Fehler auftrat
 */
const char* WINAPI GetString(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter value = 0x%p (not a valid pointer)", value));
   return((char*) value);
   #pragma EXPORT
}


/**
 * Gibt den letzten aufgetretenen Windows-Fehler des aktuellen Threads zurück. Wrapper für kernel32::GetLastError(),
 * da MQL eine Funktion desselben Namens definiert.
 *
 * @return int - Fehlercode
 */
int WINAPI GetLastWin32Error() {
   return(GetLastError());
   #pragma EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen MetaTrader-Standard-Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 */
BOOL WINAPI IsStdTimeframe(int timeframe) {
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
 */
BOOL WINAPI IsCustomTimeframe(int timeframe) {
   if (timeframe <= 0)
      return(FALSE);
   return(!IsStdTimeframe(timeframe));
   #pragma EXPORT
}


/**
 * Gibt die hexadezimale Repräsentation eines Integers zurück.
 * z.B.: IntToHexStr(13465610) => "00CD780A"
 *
 * @param  int value - Integer (4 Byte)
 *
 * @return char* - hexadezimaler String mit 8 Zeichen
 */
const char* WINAPI IntToHexStr(int value) {
   uint size = 9;
   char* szchar = new char[size];                                    // TODO: Speicherleck schließen
   sprintf_s(szchar, size, "%p", value);

   return(szchar);
   #pragma EXPORT
}


/**
 * Gibt die hexadezimale Repräsentation eines Zeigers zurück.
 * z.B.: IntToHexStr(13465610) => "00CD780A"
 *
 * @param  void* value - beliebiger Zeiger
 *
 * @return char* - hexadezimaler String mit 8 Zeichen
 */
const char* WINAPI IntToHexStr(void* value) {
   return(IntToHexStr((int)value));
}


/**
 * Return a readable version of an INIT_REASON_* constant.
 *
 * @param  InitializeReason reason - init reason
 *
 * @return char* - readable version or NULL if the reason is invalid
 */
const char* WINAPI InitReasonToStr(InitializeReason reason) {
   switch (reason) {
      case NULL                         : return("NULL"                         );
      case INIT_REASON_USER             : return("INIT_REASON_USER"             );
      case INIT_REASON_TEMPLATE         : return("INIT_REASON_TEMPLATE"         );
      case INIT_REASON_PROGRAM          : return("INIT_REASON_PROGRAM"          );
      case INIT_REASON_PROGRAM_AFTERTEST: return("INIT_REASON_PROGRAM_AFTERTEST");
      case INIT_REASON_PARAMETERS       : return("INIT_REASON_PARAMETERS"       );
      case INIT_REASON_TIMEFRAMECHANGE  : return("INIT_REASON_TIMEFRAMECHANGE"  );
      case INIT_REASON_SYMBOLCHANGE     : return("INIT_REASON_SYMBOLCHANGE"     );
      case INIT_REASON_RECOMPILE        : return("INIT_REASON_RECOMPILE"        );
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter reason = %d", reason);
   return(NULL);
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI InitializeReasonToStr(InitializeReason reason) {
   return(InitReasonToStr(reason));
   #pragma EXPORT
}


/**
 * Gibt die lesbare Konstante eines ModuleTypes zurück.
 *
 * @param  ModuleType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* WINAPI ModuleTypeToStr(ModuleType type) {
   switch (type) {
      case NULL        : return("NULL"        );
      case MT_EXPERT   : return("MT_EXPERT"   );
      case MT_SCRIPT   : return("MT_SCRIPT"   );
      case MT_INDICATOR: return("MT_INDICATOR");
      case MT_LIBRARY  : return("MT_LIBRARY"  );
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter type = %d", type);
   return(NULL);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Beschreibung eines ModuleTypes zurück.
 *
 * @param  ModuleType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* WINAPI ModuleTypeDescription(ModuleType type) {
   switch (type) {
      case NULL        : return("NULL"     );
      case MT_EXPERT   : return("Expert"   );
      case MT_SCRIPT   : return("Script"   );
      case MT_INDICATOR: return("Indicator");
      case MT_LIBRARY  : return("Library"  );
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter type = %d", type);
   return(NULL);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Konstante eines ProgramTypes zurück.
 *
 * @param  ProgramType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* WINAPI ProgramTypeToStr(ProgramType type) {
   switch (type) {
      case NULL        : return("NULL"        );
      case PT_EXPERT   : return("PT_EXPERT"   );
      case PT_SCRIPT   : return("PT_SCRIPT"   );
      case PT_INDICATOR: return("PT_INDICATOR");
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter type = %d", type);
   return(NULL);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Beschreibung eines ProgramTypes zurück.
 *
 * @param  ProgramType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ungültig ist
 */
const char* WINAPI ProgramTypeDescription(ProgramType type) {
   switch (type) {
      case NULL        : return("NULL"     );
      case PT_EXPERT   : return("Expert"   );
      case PT_SCRIPT   : return("Script"   );
      case PT_INDICATOR: return("Indicator");
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter type = %d", type);
   return(NULL);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Konstante einer RootFunction-ID zurück.
 *
 * @param  RootFunction id
 *
 * @return char* - Beschreibung oder ein Null-Pointer, falls die ID ungültig ist
 */
const char* WINAPI RootFunctionToStr(RootFunction id) {
   switch (id) {
      case NULL     : return("NULL"     );
      case RF_INIT  : return("RF_INIT"  );
      case RF_START : return("RF_START" );
      case RF_DEINIT: return("RF_DEINIT");
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter id = %d", id);
   return(NULL);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung einer RootFunction zurück (den Namen).
 *
 * @param  RootFunction id
 *
 * @return char* - Name oder NULL, falls die ID ungültig ist
 */
const char* WINAPI RootFunctionDescription(RootFunction id) {
   switch (id) {
      case NULL     : return("(null)");
      case RF_INIT  : return("init"  );
      case RF_START : return("start" );
      case RF_DEINIT: return("deinit");
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter id = %d", id);
   return(NULL);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Konstante einer Timeframe-ID zurück.
 *
 * @param  int period - Timeframe-ID
 *
 * @return char*
 */
const char* WINAPI PeriodToStr(int period) {
   switch (period) {
      case NULL      : return("NULL"      );
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
   error(ERR_INVALID_PARAMETER, "invalid parameter period = %d", period);
   return(NULL);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung einer Timeframe-ID zurück.
 *
 * @param  int period - Timeframe-ID bzw. Anzahl der Minuten je Chart-Bar
 *
 * @return char*
 */
const char* WINAPI PeriodDescription(int period) {
   switch (period) {
      case NULL      : return("NULL");
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
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI TimeframeToStr(int timeframe) {
   return(PeriodToStr(timeframe));
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI TimeframeDescription(int timeframe) {
   return(PeriodDescription(timeframe));
   #pragma EXPORT
}


/**
 * Gibt die lesbare Konstante eines UninitializeReason-Codes zurück.
 *
 * @param  UninitializeReason reason - Code
 *
 * @return char*
 */
const char* WINAPI UninitReasonToStr(UninitializeReason reason) {
   switch (reason) {
      case REASON_UNDEFINED  : return("REASON_UNDEFINED"  );
      case REASON_REMOVE     : return("REASON_REMOVE"     );
      case REASON_RECOMPILE  : return("REASON_RECOMPILE"  );
      case REASON_CHARTCHANGE: return("REASON_CHARTCHANGE");
      case REASON_CHARTCLOSE : return("REASON_CHARTCLOSE" );
      case REASON_PARAMETERS : return("REASON_PARAMETERS" );
      case REASON_ACCOUNT    : return("REASON_ACCOUNT"    );
      case REASON_TEMPLATE   : return("REASON_TEMPLATE"   );            // ab Build > 509
      case REASON_INITFAILED : return("REASON_INITFAILED" );            // ...
      case REASON_CLOSE      : return("REASON_CLOSE"      );            // ...
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter reason = %d", reason);
   return(NULL);
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI UninitializeReasonToStr(UninitializeReason reason) {
   return(UninitReasonToStr(reason));
   #pragma EXPORT
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
            if (!copied) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetClassName() 0 chars copied"));

            while (copied >= size-1) {                         // GetClassName() gibt die Anzahl der kopierten Zeichen zurück
               size <<= 1;                                     // (ohne \0). Bei size-1 ist unklar, ob der String genau in den
               className = (char*) alloca(size);               // Buffer paßte oder nicht.
               copied    = GetClassName(hWndNext, className, size);
            }
            if (!copied) return((HWND)error(ERR_WIN32_ERROR+GetLastError(), "GetClassName() 0 chars copied"));

            // Klasse mit der Klasse des Terminal-Hauptfensters vergleichen
            if (strcmp(className, "MetaQuotes::MetaTrader::4.00") == 0)
               break;
         }
         hWndNext = GetWindow(hWndNext, GW_HWNDNEXT);
      }
      if (!hWndNext) error(ERR_RUNTIME_ERROR, "cannot find application main window");
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
   if (!SystemTimeToFileTime(&stNow, &ftNow))   return(error(ERR_WIN32_ERROR+GetLastError(), "SystemTimeToFileTime() failed"));
   ulintNow.LowPart   = ftNow.dwLowDateTime;
   ulintNow.HighPart  = ftNow.dwHighDateTime;

   if (!SystemTimeToFileTime(&st1970, &ft1970)) return(error(ERR_WIN32_ERROR+GetLastError(), "SystemTimeToFileTime() failed"));
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
   if (!SystemTimeToFileTime(&stNow, &ftNow))   return(error(ERR_WIN32_ERROR+GetLastError(), "SystemTimeToFileTime() failed"));
   ulintNow.LowPart   = ftNow.dwLowDateTime;
   ulintNow.HighPart  = ftNow.dwHighDateTime;

   if (!SystemTimeToFileTime(&st1970, &ft1970)) return(error(ERR_WIN32_ERROR+GetLastError(), "SystemTimeToFileTime() failed"));
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
 * @param  DWORD    time    - Specifies the number of milliseconds that have elapsed since the system was started. This is the
 *                            value returned by the GetTickCount() function.
 */
VOID CALLBACK TimerCallback(HWND hWnd, UINT msg, UINT_PTR timerId, DWORD time) {
   int size = tickTimers.size();

   for (int i=0; i < size; i++) {
      if (tickTimers[i].id == timerId) {
         TICK_TIMER_DATA ttd = tickTimers[i];

         if (ttd.flags & TICK_IF_VISIBLE) {
            // skip timer event if chart not visible
            RECT rect;                                               // ermitteln, ob der Chart mindestens teilweise sichtbar ist
            HDC hDC = GetDC(hWnd);
            int rgn = GetClipBox(hDC, &rect);
            ReleaseDC(hWnd, hDC);

            if (rgn == NULLREGION)                                   // Chart ist nicht sichtbar
               return;
            if (rgn == RGN_ERROR) {
               warn(ERR_WIN32_ERROR+GetLastError(), "GetClipBox(hDC=%p) => RGN_ERROR", hDC);
               return;
            }
         }
         if (ttd.flags & TICK_PAUSE_ON_WEEKEND) {
            // skip timer event if on weekend: not yet implemented
         }

         if (ttd.flags & TICK_CHART_REFRESH) {
            PostMessageA(hWnd, WM_COMMAND, ID_CHART_REFRESH, 0);
         }
         else if (ttd.flags & TICK_TESTER) {
            PostMessageA(hWnd, WM_COMMAND, ID_CHART_STEPFORWARD, 0);
         }
         else {
            PostMessageA(hWnd, MT4InternalMsg(), MT4_TICK, TICK_OFFLINE_EA);     // default tick
         }
         return;
      }
   }

   warn(ERR_RUNTIME_ERROR, "timer not found, timerId = %d", timerId);
}


/**
 * Installiert einen Timer, der dem angegebenen Fenster synthetische Ticks schickt.
 *
 * @param  HWND  hWnd   - Handle des Fensters, an das die Ticks geschickt werden.
 * @param  int   millis - Zeitperiode der zu generierenden Ticks in Millisekunden
 * @param  DWORD flags  - die Ticks konfigurierende Flags (default: keine)
 *                        TICK_CHART_REFRESH:    Statt eines regulären Ticks wird das Command ID_CHART_REFRESH an den Chart
 *                                               geschickt (für Offline- und synthetische Charts).
 *                        TICK_TESTER:           Statt eines regulären Ticks wird das Command ID_CHART_STEPFORWARD an den Chart
 *                                               geschickt (für Strategy Tester).
 *                        TICK_IF_VISIBLE:       Ticks werden nur verschickt, wenn der Chart mindestens teilweise sichtbar ist.
 *                        TICK_PAUSE_ON_WEEKEND: Ticks werden nur zu regulären Forex-Handelszeiten verschickt (not implemented).
 *
 * @return uint - ID des installierten Timers zur Übergabe an RemoveTickTimer() bei Deinstallation des Timers oder 0, falls ein
 *                Fehler auftrat.
 */
uint WINAPI SetupTickTimer(HWND hWnd, int millis, DWORD flags=NULL) {
   // Parametervalidierung
   DWORD wndThreadId = GetWindowThreadProcessId(hWnd, NULL);
   if (wndThreadId != GetCurrentThreadId()) {
      if (!wndThreadId)                                   return(error(ERR_INVALID_PARAMETER, "invalid parameter hWnd = %p (not a window)", hWnd));
                                                          return(error(ERR_INVALID_PARAMETER, "window hWnd = %p not owned by the current thread", hWnd));
   }
   if (millis <= 0)                                       return(error(ERR_INVALID_PARAMETER, "invalid parameter millis = %d", millis));
   if (flags & TICK_CHART_REFRESH && flags & TICK_TESTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter flags combination: TICK_CHART_REFRESH & TICK_TESTER"));
   if (flags & TICK_PAUSE_ON_WEEKEND)                     warn(ERR_NOT_IMPLEMENTED, "flag TICK_PAUSE_ON_WEEKEND not yet implemented");

   // neue Timer-ID erzeugen
   static uint timerId = 10000;                       // ID's sind mindestens 5-stellig und beginnen bei 10000
   timerId++;

   // Timer setzen
   uint result = SetTimer(hWnd, timerId, millis, (TIMERPROC)TimerCallback);
   if (result != timerId)                             // muß stimmen, da hWnd immer != NULL
      return(error(ERR_WIN32_ERROR+GetLastError(), "SetTimer(hWnd=%p, timerId=%d, millis=%d) failed with %d", hWnd, timerId, millis, result));
   //debug("SetTimer(hWnd=%d, timerId=%d, millis=%d) success", hWnd, timerId, millis);

   // Timerdaten speichern
   TICK_TIMER_DATA ttd = {timerId, hWnd, flags};
   tickTimers.push_back(ttd);

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
   if (timerId <= 0) return(error(ERR_INVALID_PARAMETER, "invalid parameter timerId = %d", timerId));

   int size = tickTimers.size();

   for (int i=0; i < size; i++) {
      if (tickTimers[i].id == timerId) {
         if (!KillTimer(tickTimers[i].hWnd, timerId))
            return(error(ERR_WIN32_ERROR+GetLastError(), "KillTimer(hWnd=%p, timerId=%d) failed", tickTimers[i].hWnd, timerId));
         tickTimers.erase(tickTimers.begin() + i);
         return(TRUE);
      }
   }

   return(error(ERR_RUNTIME_ERROR, "timer not found: id = %d", timerId));
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
      warn(NO_ERROR, "removing orphaned tickTimer with id = %d", id);
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


/**
 * Prüft, ob ein C-String initialisiert ist oder ein NULL-Pointer ist.
 *
 * @param  char* value - zu prüfender String
 *
 * @return BOOL
 */
BOOL WINAPI StringIsNull(const char* value) {
   return(value == NULL);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Repräsentation eines C-Strings zurück. Für einen initialisierten String ist dies der String selbst,
 * für einen nicht initialisierten String (NULL-Pointer) der String "NULL" (ohne Anführungszeichen).
 *
 * @param  char* value - String
 *
 * @return char* - lesbare Repräsentation
 *
 *
 * NOTE: Wird diese Funktion in MQL implementiert, setzt MetaTrader bei einem NULL-Pointer den Fehler ERR_NOT_INITIALIZED_STRING
 *       und füllt das Log mit überflüssigen "warn: not initialized string"-Einträgen.
 */
const char* WINAPI StringToStr(const char* value) {
   if (!value)
      return("NULL");                        // C-Literal: Speicher muß nicht extra verwaltet werden
   return(value);
   #pragma EXPORT
}


/**
 * Vergleicht zwei C-Strings mit Berücksichtigung von Groß-/Kleinschreibung.
 *
 * @param  char* s1
 * @param  char* s2
 *
 * @return BOOL
 */
BOOL WINAPI StringCompare(const char* s1, const char* s2) {
   if ( s1 ==  s2) return(TRUE);             // Sind die Pointer gleich, ist es der Inhalt auch.
   if (!s1 || !s2) return(FALSE);            // Ist einer der beiden ein NULL-Pointer, kann der andere keiner sein.

   // beide sind keine NULL-Pointer
   size_t len1 = strlen(s1);
   size_t len2 = strlen(s2);
   if (len1 != len2) return(FALSE);          // beide sind unterschiedlich lang

   // beide sind gleich lang
   for (size_t i=0; i < len1; i++) {
      if (s1[i] != s2[i])
         return(FALSE);                      // beide sind unterschiedlich
   }

   // beide sind gleich
   return(TRUE);
   #pragma EXPORT
}


/**
 * Shifted die Werte eines IndicatorBuffers um eine Anzahl von Bars nach hinten. Die ältesten Werte verfallen.
 *
 * @param  double buffer[]   - MQL-Double-Array (IndicatorBuffer)
 * @param  int    bufferSize - Größe des Arrays
 * @param  int    bars       - Anzahl der zu shiftenden Bars
 * @param  double emptyValue - Initialisierungswert für freiwerdende Bufferelemente
 *
 * @return BOOL - Erfolgsstatus
 */
BOOL WINAPI ShiftIndicatorBuffer(double buffer[], int bufferSize, int bars, double emptyValue) {
   if (buffer && (uint)buffer < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer = 0x%p (not a valid pointer)", buffer));
   if (bufferSize < 0)                             return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize = %d", bufferSize));
   if (bars < 0)                                   return(error(ERR_INVALID_PARAMETER, "invalid parameter bars = %d", bars));
   if (!bufferSize || !bars) return(TRUE);

   MoveMemory((void*)&buffer[0], &buffer[bars], (bufferSize-bars)*sizeof(buffer[0]));

   for (int i=bufferSize-bars; i < bufferSize; i++) {
      buffer[i] = emptyValue;
   }
   return(TRUE);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Konstante eines ShowWindow()-Commands zurück.
 *
 * @param  int cmdShow - ShowWindow()-Command
 *
 * @return char*
 */
const char* WINAPI ShowWindowCmdToStr(int cmdShow) {
   switch (cmdShow) {
      case SW_HIDE           : return("SW_HIDE"           );
      case SW_SHOWNORMAL     : return("SW_SHOWNORMAL"     );
      case SW_SHOWMINIMIZED  : return("SW_SHOWMINIMIZED"  );
      case SW_SHOWMAXIMIZED  : return("SW_SHOWMAXIMIZED"  );
      case SW_SHOWNOACTIVATE : return("SW_SHOWNOACTIVATE" );
      case SW_SHOW           : return("SW_SHOW"           );
      case SW_MINIMIZE       : return("SW_MINIMIZE"       );
      case SW_SHOWMINNOACTIVE: return("SW_SHOWMINNOACTIVE");
      case SW_SHOWNA         : return("SW_SHOWNA"         );
      case SW_RESTORE        : return("SW_RESTORE"        );
      case SW_SHOWDEFAULT    : return("SW_SHOWDEFAULT"    );
      case SW_FORCEMINIMIZE  : return("SW_FORCEMINIMIZE"  );
   }
   error(ERR_INVALID_PARAMETER, "invalid parameter cmdShow = %d", cmdShow);
   return(NULL);
   #pragma EXPORT
}


/**
 * Read the terminal's version numbers.
 *
 * @param  _Out_ uint* major  - variable holding the major version number after return
 * @param  _Out_ uint* minor  - variable holding the minor version number after return
 * @param  _Out_ uint* hotfix - variable holding the hotfix number after return
 * @param  _Out_ uint* build  - variable holding the build number after return
 *
 * @return BOOL - success status
 */
BOOL WINAPI GetTerminalVersionNumbers(uint* major, uint* minor, uint* hotfix, uint* build) {
   if (major  && (uint)major  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter major = 0x%p (not a valid pointer)", major));
   if (minor  && (uint)minor  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter minor = 0x%p (not a valid pointer)", minor));
   if (hotfix && (uint)hotfix < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter hotfix = 0x%p (not a valid pointer)", hotfix));
   if (build  && (uint)build  < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter build = 0x%p (not a valid pointer)", build));

   static uint resultMajor, resultMinor, resultHotfix, resultBuild;

   if (!resultMajor) {
      // resolve the executable's full file name
      char* fileName;
      uint size = MAX_PATH >> 1, length=size;
      while (length >= size) {
         size   <<= 1;
         fileName = (char*) alloca(size);                               // on stack
         length   = GetModuleFileName(NULL, fileName, size);
      }
      if (!length) return(error(ERR_WIN32_ERROR+GetLastError(), "GetModuleFileName() 0 chars copied"));

      // get the file's version info
      DWORD infoSize = GetFileVersionInfoSize(fileName, &infoSize);
      if (!infoSize) return(error(ERR_WIN32_ERROR+GetLastError(), "GetFileVersionInfoSize() returned 0"));

      char* infoBuffer = (char*) alloca(infoSize);                      // on stack
      BOOL result = GetFileVersionInfo(fileName, NULL, infoSize, infoBuffer);
      if (!result) return(error(ERR_WIN32_ERROR+GetLastError(), "GetFileVersionInfo() returned FALSE"));

      // query the version root values
      VS_FIXEDFILEINFO* fileInfo;
      uint len;
      result = VerQueryValue(infoBuffer, "\\", (LPVOID*)&fileInfo, &len);
      if (!result) return(error(ERR_WIN32_ERROR+GetLastError(), "VerQueryValue() returned FALSE"));

      // parse the version numbers
      resultMajor  = (fileInfo->dwFileVersionMS >> 16) & 0xffff;
      resultMinor  = (fileInfo->dwFileVersionMS      ) & 0xffff;
      resultHotfix = (fileInfo->dwFileVersionLS >> 16) & 0xffff;
      resultBuild  = (fileInfo->dwFileVersionLS      ) & 0xffff;
   }

   // assign results to parameters
   if (major)  *major  = resultMajor;
   if (minor)  *minor  = resultMinor;
   if (hotfix) *hotfix = resultHotfix;
   if (build)  *build  = resultBuild;

   return(TRUE);
   #pragma EXPORT
}


/**
 * Return the terminal's version string.
 *
 * @return char* - version or NULL pointer if an error occurred
 */
const char* WINAPI GetTerminalVersion() {
   static char* version;

   if (!version) {
      // get the version numbers
      uint major, minor, hotfix, build;
      BOOL result = GetTerminalVersionNumbers(&major, &minor, &hotfix, &build);
      if (!result) return(NULL);

      // compose version string
      char* format = "%d.%d.%d.%d";
      uint size = _scprintf(format, major, minor, hotfix, build) + 1;   // +1 for the terminating '\0'
      version = new char[size];                                         // on the heap
      sprintf_s(version, size, format, major, minor, hotfix, build);
   }

   return(version);
   #pragma EXPORT
}


/**
 * Return the terminal's build number.
 *
 * @return uint - build number or 0 if an error occurred
 */
uint WINAPI GetTerminalBuild() {
   uint dummy, build;
   if (!GetTerminalVersionNumbers(&dummy, &dummy, &dummy, &build))
      return(NULL);
   return(build);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Konstante eines MQL-Fehlercodes zurück.
 *
 * @param  int error - MQL-Fehlercode
 *
 * @return char*
 */
const char* WINAPI ErrorToStr(int error) {
   switch (error) {
      case NO_ERROR                       : return("NO_ERROR"                       ); //      0

      // trade server errors
      case ERR_NO_RESULT                  : return("ERR_NO_RESULT"                  ); //      1
      case ERR_COMMON_ERROR               : return("ERR_COMMON_ERROR"               ); //      2
      case ERR_INVALID_TRADE_PARAMETERS   : return("ERR_INVALID_TRADE_PARAMETERS"   ); //      3
      case ERR_SERVER_BUSY                : return("ERR_SERVER_BUSY"                ); //      4
      case ERR_OLD_VERSION                : return("ERR_OLD_VERSION"                ); //      5
      case ERR_NO_CONNECTION              : return("ERR_NO_CONNECTION"              ); //      6
      case ERR_NOT_ENOUGH_RIGHTS          : return("ERR_NOT_ENOUGH_RIGHTS"          ); //      7
      case ERR_TOO_FREQUENT_REQUESTS      : return("ERR_TOO_FREQUENT_REQUESTS"      ); //      8
      case ERR_MALFUNCTIONAL_TRADE        : return("ERR_MALFUNCTIONAL_TRADE"        ); //      9
      case ERR_ACCOUNT_DISABLED           : return("ERR_ACCOUNT_DISABLED"           ); //     64
      case ERR_INVALID_ACCOUNT            : return("ERR_INVALID_ACCOUNT"            ); //     65
      case ERR_TRADE_TIMEOUT              : return("ERR_TRADE_TIMEOUT"              ); //    128
      case ERR_INVALID_PRICE              : return("ERR_INVALID_PRICE"              ); //    129
      case ERR_INVALID_STOP               : return("ERR_INVALID_STOP"               ); //    130
      case ERR_INVALID_TRADE_VOLUME       : return("ERR_INVALID_TRADE_VOLUME"       ); //    131
      case ERR_MARKET_CLOSED              : return("ERR_MARKET_CLOSED"              ); //    132
      case ERR_TRADE_DISABLED             : return("ERR_TRADE_DISABLED"             ); //    133
      case ERR_NOT_ENOUGH_MONEY           : return("ERR_NOT_ENOUGH_MONEY"           ); //    134
      case ERR_PRICE_CHANGED              : return("ERR_PRICE_CHANGED"              ); //    135
      case ERR_OFF_QUOTES                 : return("ERR_OFF_QUOTES"                 ); //    136
      case ERR_BROKER_BUSY                : return("ERR_BROKER_BUSY"                ); //    137
      case ERR_REQUOTE                    : return("ERR_REQUOTE"                    ); //    138
      case ERR_ORDER_LOCKED               : return("ERR_ORDER_LOCKED"               ); //    139
      case ERR_LONG_POSITIONS_ONLY_ALLOWED: return("ERR_LONG_POSITIONS_ONLY_ALLOWED"); //    140
      case ERR_TOO_MANY_REQUESTS          : return("ERR_TOO_MANY_REQUESTS"          ); //    141
      case ERR_TRADE_MODIFY_DENIED        : return("ERR_TRADE_MODIFY_DENIED"        ); //    145
      case ERR_TRADE_CONTEXT_BUSY         : return("ERR_TRADE_CONTEXT_BUSY"         ); //    146
      case ERR_TRADE_EXPIRATION_DENIED    : return("ERR_TRADE_EXPIRATION_DENIED"    ); //    147
      case ERR_TRADE_TOO_MANY_ORDERS      : return("ERR_TRADE_TOO_MANY_ORDERS"      ); //    148
      case ERR_TRADE_HEDGE_PROHIBITED     : return("ERR_TRADE_HEDGE_PROHIBITED"     ); //    149
      case ERR_TRADE_PROHIBITED_BY_FIFO   : return("ERR_TRADE_PROHIBITED_BY_FIFO"   ); //    150

      // runtime errors
      case ERR_NO_MQLERROR                : return("ERR_NO_MQLERROR"                ); //   4000
      case ERR_WRONG_FUNCTION_POINTER     : return("ERR_WRONG_FUNCTION_POINTER"     ); //   4001
      case ERR_ARRAY_INDEX_OUT_OF_RANGE   : return("ERR_ARRAY_INDEX_OUT_OF_RANGE"   ); //   4002
      case ERR_NO_MEMORY_FOR_CALL_STACK   : return("ERR_NO_MEMORY_FOR_CALL_STACK"   ); //   4003
      case ERR_RECURSIVE_STACK_OVERFLOW   : return("ERR_RECURSIVE_STACK_OVERFLOW"   ); //   4004
      case ERR_NOT_ENOUGH_STACK_FOR_PARAM : return("ERR_NOT_ENOUGH_STACK_FOR_PARAM" ); //   4005
      case ERR_NO_MEMORY_FOR_PARAM_STRING : return("ERR_NO_MEMORY_FOR_PARAM_STRING" ); //   4006
      case ERR_NO_MEMORY_FOR_TEMP_STRING  : return("ERR_NO_MEMORY_FOR_TEMP_STRING"  ); //   4007
      case ERR_NOT_INITIALIZED_STRING     : return("ERR_NOT_INITIALIZED_STRING"     ); //   4008
      case ERR_NOT_INITIALIZED_ARRAYSTRING: return("ERR_NOT_INITIALIZED_ARRAYSTRING"); //   4009
      case ERR_NO_MEMORY_FOR_ARRAYSTRING  : return("ERR_NO_MEMORY_FOR_ARRAYSTRING"  ); //   4010
      case ERR_TOO_LONG_STRING            : return("ERR_TOO_LONG_STRING"            ); //   4011
      case ERR_REMAINDER_FROM_ZERO_DIVIDE : return("ERR_REMAINDER_FROM_ZERO_DIVIDE" ); //   4012
      case ERR_ZERO_DIVIDE                : return("ERR_ZERO_DIVIDE"                ); //   4013
      case ERR_UNKNOWN_COMMAND            : return("ERR_UNKNOWN_COMMAND"            ); //   4014
      case ERR_WRONG_JUMP                 : return("ERR_WRONG_JUMP"                 ); //   4015
      case ERR_NOT_INITIALIZED_ARRAY      : return("ERR_NOT_INITIALIZED_ARRAY"      ); //   4016
      case ERR_DLL_CALLS_NOT_ALLOWED      : return("ERR_DLL_CALLS_NOT_ALLOWED"      ); //   4017
      case ERR_CANNOT_LOAD_LIBRARY        : return("ERR_CANNOT_LOAD_LIBRARY"        ); //   4018
      case ERR_CANNOT_CALL_FUNCTION       : return("ERR_CANNOT_CALL_FUNCTION"       ); //   4019
      case ERR_EX4_CALLS_NOT_ALLOWED      : return("ERR_EX4_CALLS_NOT_ALLOWED"      ); //   4020
      case ERR_NO_MEMORY_FOR_RETURNED_STR : return("ERR_NO_MEMORY_FOR_RETURNED_STR" ); //   4021
      case ERR_SYSTEM_BUSY                : return("ERR_SYSTEM_BUSY"                ); //   4022
      case ERR_DLL_EXCEPTION              : return("ERR_DLL_EXCEPTION"              ); //   4023
      case ERR_INTERNAL_ERROR             : return("ERR_INTERNAL_ERROR"             ); //   4024
      case ERR_OUT_OF_MEMORY              : return("ERR_OUT_OF_MEMORY"              ); //   4025
      case ERR_INVALID_POINTER            : return("ERR_INVALID_POINTER"            ); //   4026
      case ERR_FORMAT_TOO_MANY_FORMATTERS : return("ERR_FORMAT_TOO_MANY_FORMATTERS" ); //   4027
      case ERR_FORMAT_TOO_MANY_PARAMETERS : return("ERR_FORMAT_TOO_MANY_PARAMETERS" ); //   4028
      case ERR_ARRAY_INVALID              : return("ERR_ARRAY_INVALID"              ); //   4029
      case ERR_CHART_NOREPLY              : return("ERR_CHART_NOREPLY"              ); //   4030
      case ERR_INVALID_FUNCTION_PARAMSCNT : return("ERR_INVALID_FUNCTION_PARAMSCNT" ); //   4050
      case ERR_INVALID_PARAMETER          : return("ERR_INVALID_PARAMETER"          ); //   4051
      case ERR_STRING_FUNCTION_INTERNAL   : return("ERR_STRING_FUNCTION_INTERNAL"   ); //   4052
      case ERR_ARRAY_ERROR                : return("ERR_ARRAY_ERROR"                ); //   4053
      case ERR_SERIES_NOT_AVAILABLE       : return("ERR_SERIES_NOT_AVAILABLE"       ); //   4054
      case ERR_CUSTOM_INDICATOR_ERROR     : return("ERR_CUSTOM_INDICATOR_ERROR"     ); //   4055
      case ERR_INCOMPATIBLE_ARRAYS        : return("ERR_INCOMPATIBLE_ARRAYS"        ); //   4056
      case ERR_GLOBAL_VARIABLES_PROCESSING: return("ERR_GLOBAL_VARIABLES_PROCESSING"); //   4057
      case ERR_GLOBAL_VARIABLE_NOT_FOUND  : return("ERR_GLOBAL_VARIABLE_NOT_FOUND"  ); //   4058
      case ERR_FUNC_NOT_ALLOWED_IN_TESTER : return("ERR_FUNC_NOT_ALLOWED_IN_TESTER" ); //   4059
      case ERR_FUNCTION_NOT_CONFIRMED     : return("ERR_FUNCTION_NOT_CONFIRMED"     ); //   4060
      case ERR_SEND_MAIL_ERROR            : return("ERR_SEND_MAIL_ERROR"            ); //   4061
      case ERR_STRING_PARAMETER_EXPECTED  : return("ERR_STRING_PARAMETER_EXPECTED"  ); //   4062
      case ERR_INTEGER_PARAMETER_EXPECTED : return("ERR_INTEGER_PARAMETER_EXPECTED" ); //   4063
      case ERR_DOUBLE_PARAMETER_EXPECTED  : return("ERR_DOUBLE_PARAMETER_EXPECTED"  ); //   4064
      case ERR_ARRAY_AS_PARAMETER_EXPECTED: return("ERR_ARRAY_AS_PARAMETER_EXPECTED"); //   4065
      case ERS_HISTORY_UPDATE             : return("ERS_HISTORY_UPDATE"             ); //   4066   Status
      case ERR_TRADE_ERROR                : return("ERR_TRADE_ERROR"                ); //   4067
      case ERR_RESOURCE_NOT_FOUND         : return("ERR_RESOURCE_NOT_FOUND"         ); //   4068
      case ERR_RESOURCE_NOT_SUPPORTED     : return("ERR_RESOURCE_NOT_SUPPORTED"     ); //   4069
      case ERR_RESOURCE_DUPLICATED        : return("ERR_RESOURCE_DUPLICATED"        ); //   4070
      case ERR_INDICATOR_CANNOT_INIT      : return("ERR_INDICATOR_CANNOT_INIT"      ); //   4071
      case ERR_INDICATOR_CANNOT_LOAD      : return("ERR_INDICATOR_CANNOT_LOAD"      ); //   4072
      case ERR_NO_HISTORY_DATA            : return("ERR_NO_HISTORY_DATA"            ); //   4073
      case ERR_NO_MEMORY_FOR_HISTORY      : return("ERR_NO_MEMORY_FOR_HISTORY"      ); //   4074
      case ERR_NO_MEMORY_FOR_INDICATOR    : return("ERR_NO_MEMORY_FOR_INDICATOR"    ); //   4075
      case ERR_END_OF_FILE                : return("ERR_END_OF_FILE"                ); //   4099
      case ERR_FILE_ERROR                 : return("ERR_FILE_ERROR"                 ); //   4100
      case ERR_WRONG_FILE_NAME            : return("ERR_WRONG_FILE_NAME"            ); //   4101
      case ERR_TOO_MANY_OPENED_FILES      : return("ERR_TOO_MANY_OPENED_FILES"      ); //   4102
      case ERR_CANNOT_OPEN_FILE           : return("ERR_CANNOT_OPEN_FILE"           ); //   4103
      case ERR_INCOMPATIBLE_FILEACCESS    : return("ERR_INCOMPATIBLE_FILEACCESS"    ); //   4104
      case ERR_NO_TICKET_SELECTED         : return("ERR_NO_TICKET_SELECTED"         ); //   4105
      case ERR_SYMBOL_NOT_AVAILABLE       : return("ERR_SYMBOL_NOT_AVAILABLE"       ); //   4106
      case ERR_INVALID_PRICE_PARAM        : return("ERR_INVALID_PRICE_PARAM"        ); //   4107
      case ERR_INVALID_TICKET             : return("ERR_INVALID_TICKET"             ); //   4108
      case ERR_TRADE_NOT_ALLOWED          : return("ERR_TRADE_NOT_ALLOWED"          ); //   4109
      case ERR_LONGS_NOT_ALLOWED          : return("ERR_LONGS_NOT_ALLOWED"          ); //   4110
      case ERR_SHORTS_NOT_ALLOWED         : return("ERR_SHORTS_NOT_ALLOWED"         ); //   4111
      case ERR_AUTOMATED_TRADING_DISABLED : return("ERR_AUTOMATED_TRADING_DISABLED" ); //   4112
      case ERR_OBJECT_ALREADY_EXISTS      : return("ERR_OBJECT_ALREADY_EXISTS"      ); //   4200
      case ERR_UNKNOWN_OBJECT_PROPERTY    : return("ERR_UNKNOWN_OBJECT_PROPERTY"    ); //   4201
      case ERR_OBJECT_DOES_NOT_EXIST      : return("ERR_OBJECT_DOES_NOT_EXIST"      ); //   4202
      case ERR_UNKNOWN_OBJECT_TYPE        : return("ERR_UNKNOWN_OBJECT_TYPE"        ); //   4203
      case ERR_NO_OBJECT_NAME             : return("ERR_NO_OBJECT_NAME"             ); //   4204
      case ERR_OBJECT_COORDINATES_ERROR   : return("ERR_OBJECT_COORDINATES_ERROR"   ); //   4205
      case ERR_NO_SPECIFIED_SUBWINDOW     : return("ERR_NO_SPECIFIED_SUBWINDOW"     ); //   4206
      case ERR_OBJECT_ERROR               : return("ERR_OBJECT_ERROR"               ); //   4207
      case ERR_CHART_PROP_INVALID         : return("ERR_CHART_PROP_INVALID"         ); //   4210
      case ERR_CHART_NOT_FOUND            : return("ERR_CHART_NOT_FOUND"            ); //   4211
      case ERR_CHARTWINDOW_NOT_FOUND      : return("ERR_CHARTWINDOW_NOT_FOUND"      ); //   4212
      case ERR_CHARTINDICATOR_NOT_FOUND   : return("ERR_CHARTINDICATOR_NOT_FOUND"   ); //   4213
      case ERR_SYMBOL_SELECT              : return("ERR_SYMBOL_SELECT"              ); //   4220
      case ERR_NOTIFICATION_SEND_ERROR    : return("ERR_NOTIFICATION_SEND_ERROR"    ); //   4250
      case ERR_NOTIFICATION_PARAMETER     : return("ERR_NOTIFICATION_PARAMETER"     ); //   4251
      case ERR_NOTIFICATION_SETTINGS      : return("ERR_NOTIFICATION_SETTINGS"      ); //   4252
      case ERR_NOTIFICATION_TOO_FREQUENT  : return("ERR_NOTIFICATION_TOO_FREQUENT"  ); //   4253
      case ERR_FTP_NOSERVER               : return("ERR_FTP_NOSERVER"               ); //   4260
      case ERR_FTP_NOLOGIN                : return("ERR_FTP_NOLOGIN"                ); //   4261
      case ERR_FTP_CONNECT_FAILED         : return("ERR_FTP_CONNECT_FAILED"         ); //   4262
      case ERR_FTP_CLOSED                 : return("ERR_FTP_CLOSED"                 ); //   4263
      case ERR_FTP_CHANGEDIR              : return("ERR_FTP_CHANGEDIR"              ); //   4264
      case ERR_FTP_FILE_ERROR             : return("ERR_FTP_FILE_ERROR"             ); //   4265
      case ERR_FTP_ERROR                  : return("ERR_FTP_ERROR"                  ); //   4266
      case ERR_FILE_TOO_MANY_OPENED       : return("ERR_FILE_TOO_MANY_OPENED"       ); //   5001
      case ERR_FILE_WRONG_FILENAME        : return("ERR_FILE_WRONG_FILENAME"        ); //   5002
      case ERR_FILE_TOO_LONG_FILENAME     : return("ERR_FILE_TOO_LONG_FILENAME"     ); //   5003
      case ERR_FILE_CANNOT_OPEN           : return("ERR_FILE_CANNOT_OPEN"           ); //   5004
      case ERR_FILE_BUFFER_ALLOC_ERROR    : return("ERR_FILE_BUFFER_ALLOC_ERROR"    ); //   5005
      case ERR_FILE_CANNOT_DELETE         : return("ERR_FILE_CANNOT_DELETE"         ); //   5006
      case ERR_FILE_INVALID_HANDLE        : return("ERR_FILE_INVALID_HANDLE"        ); //   5007
      case ERR_FILE_UNKNOWN_HANDLE        : return("ERR_FILE_UNKNOWN_HANDLE"        ); //   5008
      case ERR_FILE_NOT_TOWRITE           : return("ERR_FILE_NOT_TOWRITE"           ); //   5009
      case ERR_FILE_NOT_TOREAD            : return("ERR_FILE_NOT_TOREAD"            ); //   5010
      case ERR_FILE_NOT_BIN               : return("ERR_FILE_NOT_BIN"               ); //   5011
      case ERR_FILE_NOT_TXT               : return("ERR_FILE_NOT_TXT"               ); //   5012
      case ERR_FILE_NOT_TXTORCSV          : return("ERR_FILE_NOT_TXTORCSV"          ); //   5013
      case ERR_FILE_NOT_CSV               : return("ERR_FILE_NOT_CSV"               ); //   5014
      case ERR_FILE_READ_ERROR            : return("ERR_FILE_READ_ERROR"            ); //   5015
      case ERR_FILE_WRITE_ERROR           : return("ERR_FILE_WRITE_ERROR"           ); //   5016
      case ERR_FILE_BIN_STRINGSIZE        : return("ERR_FILE_BIN_STRINGSIZE"        ); //   5017
      case ERR_FILE_INCOMPATIBLE          : return("ERR_FILE_INCOMPATIBLE"          ); //   5018
      case ERR_FILE_IS_DIRECTORY          : return("ERR_FILE_IS_DIRECTORY"          ); //   5019
      case ERR_FILE_NOT_FOUND             : return("ERR_FILE_NOT_FOUND"             ); //   5020
      case ERR_FILE_CANNOT_REWRITE        : return("ERR_FILE_CANNOT_REWRITE"        ); //   5021
      case ERR_FILE_WRONG_DIRECTORYNAME   : return("ERR_FILE_WRONG_DIRECTORYNAME"   ); //   5022
      case ERR_FILE_DIRECTORY_NOT_EXIST   : return("ERR_FILE_DIRECTORY_NOT_EXIST"   ); //   5023
      case ERR_FILE_NOT_DIRECTORY         : return("ERR_FILE_NOT_DIRECTORY"         ); //   5024
      case ERR_FILE_CANT_DELETE_DIRECTORY : return("ERR_FILE_CANT_DELETE_DIRECTORY" ); //   5025
      case ERR_FILE_CANT_CLEAN_DIRECTORY  : return("ERR_FILE_CANT_CLEAN_DIRECTORY"  ); //   5026
      case ERR_FILE_ARRAYRESIZE_ERROR     : return("ERR_FILE_ARRAYRESIZE_ERROR"     ); //   5027
      case ERR_FILE_STRINGRESIZE_ERROR    : return("ERR_FILE_STRINGRESIZE_ERROR"    ); //   5028
      case ERR_FILE_STRUCT_WITH_OBJECTS   : return("ERR_FILE_STRUCT_WITH_OBJECTS"   ); //   5029
      case ERR_WEBREQUEST_INVALID_ADDRESS : return("ERR_WEBREQUEST_INVALID_ADDRESS" ); //   5200
      case ERR_WEBREQUEST_CONNECT_FAILED  : return("ERR_WEBREQUEST_CONNECT_FAILED"  ); //   5201
      case ERR_WEBREQUEST_TIMEOUT         : return("ERR_WEBREQUEST_TIMEOUT"         ); //   5202
      case ERR_WEBREQUEST_REQUEST_FAILED  : return("ERR_WEBREQUEST_REQUEST_FAILED"  ); //   5203

      // user defined errors: 65536-99999 (0x10000-0x1869F)
      case ERR_RUNTIME_ERROR              : return("ERR_RUNTIME_ERROR"              ); //  65536
      case ERR_NOT_IMPLEMENTED            : return("ERR_NOT_IMPLEMENTED"            ); //  65537
      case ERR_INVALID_INPUT_PARAMETER    : return("ERR_INVALID_INPUT_PARAMETER"    ); //  65538
      case ERR_INVALID_CONFIG_PARAMVALUE  : return("ERR_INVALID_CONFIG_PARAMVALUE"  ); //  65539
      case ERS_TERMINAL_NOT_YET_READY     : return("ERS_TERMINAL_NOT_YET_READY"     ); //  65540   Status
      case ERR_INVALID_TIMEZONE_CONFIG    : return("ERR_INVALID_TIMEZONE_CONFIG"    ); //  65541
      case ERR_INVALID_MARKET_DATA        : return("ERR_INVALID_MARKET_DATA"        ); //  65542
      case ERR_CANCELLED_BY_USER          : return("ERR_CANCELLED_BY_USER"          ); //  65543
      case ERR_FUNC_NOT_ALLOWED           : return("ERR_FUNC_NOT_ALLOWED"           ); //  65544
      case ERR_INVALID_COMMAND            : return("ERR_INVALID_COMMAND"            ); //  65545
      case ERR_ILLEGAL_STATE              : return("ERR_ILLEGAL_STATE"              ); //  65546
      case ERS_EXECUTION_STOPPING         : return("ERS_EXECUTION_STOPPING"         ); //  65547   Status
      case ERR_ORDER_CHANGED              : return("ERR_ORDER_CHANGED"              ); //  65548
      case ERR_HISTORY_INSUFFICIENT       : return("ERR_HISTORY_INSUFFICIENT"       ); //  65549
      case ERR_CONCURRENT_MODIFICATION    : return("ERR_CONCURRENT_MODIFICATION"    ); //  65550
   }

   char* szFormat = "%d";

   if (error >= ERR_WIN32_ERROR) {                                   // >=100000
      error   -= ERR_WIN32_ERROR;
      szFormat = "ERR_WIN32_ERROR+%d";
   }

   uint size = _scprintf(szFormat, error) + 1;                       // +1 for the terminating '\0'
   char* szchar = new char[size];                                    // TODO: close memory leak
   sprintf_s(szchar, size, szFormat, error);

   return(szchar);
   #pragma EXPORT
}


/**
 * Wrap a string in double quote characters.
 *
 * @param  char* value
 *
 * @return char* - resulting string or the string "NULL" if a NULL pointer was specified
 */
const char* WINAPI DoubleQuoteStr(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter value = 0x%p (not a valid pointer)", value));
   if (!value) return("NULL");

   uint size = strlen(value) + 3;                                    // +2 for the quotes, +1 for the terminating '\0'
   char* szchar = new char[size];                                    // TODO: close memory leak
   sprintf_s(szchar, size, "\"%s\"", value);

   return(szchar);
   #pragma EXPORT
}


/**
 * Return a readable version of a combination of INIT_* configuration flags.
 *
 * @param  uint flags - INIT_* flags
 *
 * @return char*
 */
const char* WINAPI InitFlagsToStr(uint flags) {
   std::stringstream ss;

   if (!flags) {
      ss << "|0";
   }
   else {
      if (flags & INIT_TIMEZONE           ) ss << "|INIT_TIMEZONE";
      if (flags & INIT_PIPVALUE           ) ss << "|INIT_PIPVALUE";
      if (flags & INIT_BARS_ON_HIST_UPDATE) ss << "|INIT_BARS_ON_HIST_UPDATE";
      if (flags & INIT_CUSTOMLOG          ) ss << "|INIT_CUSTOMLOG";
      if (flags & INIT_DOESNT_REQUIRE_BARS) ss << "|INIT_DOESNT_REQUIRE_BARS";
   }
   std::string str = ss.str();
   uint size = str.size();

   if (!size)
      return("");                                                    // the terminating '\0' is copied too
   return(strcpy(new char[size], str.c_str()+1));                    // TODO: close memory leak
   #pragma EXPORT
}


/**
 * Return a readable version of a combination of DEINIT_* configuration flags.
 *
 * @param  uint flags - DEINIT_* flags
 *
 * @return char*
 */
const char* WINAPI DeinitFlagsToStr(uint flags) {
   std::stringstream ss;
   ss << "|" << flags;                                               // atm no DEINIT flags exist

   std::string str = ss.str();
   uint size = str.size();

   if (!size)
      return("");                                                    // the terminating '\0' is copied too
   return(strcpy(new char[size], str.c_str()+1));                    // TODO: close memory leak
   #pragma EXPORT
}


/**
 * Return a readable version of a combination of test flags.
 *
 * @param  uint flags - TF_* flags
 *
 * @return char*
 */
const char* WINAPI TestFlagsToStr(uint flags) {
   std::stringstream ss;

   if (!flags) {
      ss << "|0";
   }
   else {
      if ( flags & TF_TEST                                  ) ss << "|TF_TEST";
      if ((flags & TF_VISUAL_TEST)     == TF_VISUAL_TEST    ) ss << "|TF_VISUAL_TEST";
      if ((flags & TF_OPTIMIZING_TEST) == TF_OPTIMIZING_TEST) ss << "|TF_OPTIMIZING_TEST";
   }
   std::string str = ss.str();
   uint size = str.size();

   if (!size)
      return("");                                                    // the terminating '\0' is copied too
   return(strcpy(new char[size], str.c_str()+1));                    // TODO: close memory leak
   #pragma EXPORT
}


/**
 * Convert a BOOL value to the string "TRUE" or "FALSE".
 *
 * @param  BOOL value
 *
 * @return char*
 */
const char* WINAPI BoolToStr(BOOL value) {
   if (value)
      return("TRUE");
   return("FALSE");
   #pragma EXPORT
}
