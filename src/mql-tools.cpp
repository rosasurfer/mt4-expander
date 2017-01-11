#include "expander.h"
#include <sstream>


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
const char* WINAPI IntToHexStr(const void* value) {
   return(IntToHexStr((int)value));
}


/**
 * Return a readable version of an InitializeReason constant.
 *
 * @param  InitializeReason reason - init reason
 *
 * @return char* - readable version or NULL if the reason is invalid
 */
const char* WINAPI InitReasonToStr(InitializeReason reason) {
   switch (reason) {
      case NULL                : return("NULL"                );
      case IR_USER             : return("IR_USER"             );
      case IR_TEMPLATE         : return("IR_TEMPLATE"         );
      case IR_PROGRAM          : return("IR_PROGRAM"          );
      case IR_PROGRAM_AFTERTEST: return("IR_PROGRAM_AFTERTEST");
      case IR_PARAMETERS       : return("IR_PARAMETERS"       );
      case IR_TIMEFRAMECHANGE  : return("IR_TIMEFRAMECHANGE"  );
      case IR_SYMBOLCHANGE     : return("IR_SYMBOLCHANGE"     );
      case IR_RECOMPILE        : return("IR_RECOMPILE"        );
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
 * @param  uint period - Timeframe-ID
 *
 * @return char*
 */
const char* WINAPI PeriodToStr(uint period) {
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
   error(ERR_INVALID_PARAMETER, "invalid parameter period = %d", (int)period);
   return(NULL);
   #pragma EXPORT
}


/**
 * Gibt die Beschreibung einer Timeframe-ID zurück.
 *
 * @param  uint period - Timeframe-ID bzw. Anzahl der Minuten je Chart-Bar
 *
 * @return char*
 */
const char* WINAPI PeriodDescription(uint period) {
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
   size_t size = _scprintf("%d", period) + 1;
   char* szchar = new char[size];                                    // TODO: Speicherleck schließen
   sprintf_s(szchar, size, "%d", period);

   return(szchar);
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI TimeframeToStr(uint timeframe) {
   return(PeriodToStr(timeframe));
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI TimeframeDescription(uint timeframe) {
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
      case UR_UNDEFINED  : return("UR_UNDEFINED"  );
      case UR_REMOVE     : return("UR_REMOVE"     );
      case UR_RECOMPILE  : return("UR_RECOMPILE"  );
      case UR_CHARTCHANGE: return("UR_CHARTCHANGE");
      case UR_CHARTCLOSE : return("UR_CHARTCLOSE" );
      case UR_PARAMETERS : return("UR_PARAMETERS" );
      case UR_ACCOUNT    : return("UR_ACCOUNT"    );
      case UR_TEMPLATE   : return("UR_TEMPLATE"   );            // ab Build > 509
      case UR_INITFAILED : return("UR_INITFAILED" );            // ...
      case UR_CLOSE      : return("UR_CLOSE"      );            // ...
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
uint WINAPI SetupTickTimer(HWND hWnd, int millis, DWORD flags/*=NULL*/) {
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
void WINAPI RemoveTickTimers() {
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
HANDLE WINAPI GetWindowProperty(HWND hWnd, const char* lpName) {
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
HANDLE WINAPI RemoveWindowProperty(HWND hWnd, const char* lpName) {
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
BOOL WINAPI SetWindowProperty(HWND hWnd, const char* lpName, HANDLE value) {
   return(SetProp(hWnd, lpName, value));
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
      return("NULL");
   return(value);
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
BOOL WINAPI GetTerminalVersions(uint* major, uint* minor, uint* hotfix, uint* build) {
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
      BOOL result = GetTerminalVersions(&major, &minor, &hotfix, &build);
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
   if (!GetTerminalVersions(&dummy, &dummy, &dummy, &build))
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
 * Wrap a C string in double quote characters and return a C string.
 *
 * @param  char* value
 *
 * @return char* - resulting C string or the string "NULL" if a NULL pointer was specified
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
 * Wrap a C string in double quote characters and return a std::string.
 *
 * @param  char* value
 *
 * @return string - resulting C string or the string "NULL" if a NULL pointer was specified
 */
string WINAPI doubleQuoteStr(const char* value) {
   if (!value) return(string("NULL"));
   return(string(value).insert(0, "\"").append("\""));               // Visual Assist bug
}


/**
 * Wrap a std::string in double quote characters and return a std::string.
 *
 * @param  string &value
 *
 * @return string - new string
 */
string WINAPI doubleQuoteStr(const string &value) {
   if (value.empty()) return(string(""));
   return(string(value).insert(0, "\"").append("\""));               // Visual Assist bug
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
      if (flags & INIT_NO_BARS_REQUIRED   ) ss << "|INIT_NO_BARS_REQUIRED";
   }
   string str = ss.str();
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

   string str = ss.str();
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


/**
 * Copy the symbol-timeframe description as in the title bar of a chart window to the specified buffer. If the buffer is too
 * small the string in the buffer is truncated. The string is always terminated with a NULL character.
 *
 * @param  char* symbol
 * @param  uint  timeframe
 * @param  char* buffer
 * @param  uint  bufferSize
 *
 * @return uint - Amount of copied characters not counting the terminating NULL character or {bufferSize} if the buffer is
 *                too small and the string in the buffer was truncated.
 *                NULL in case of an error.
 */
uint WINAPI GetChartDescription(const char* symbol, uint timeframe, char* buffer, uint bufferSize) {
   uint symbolLength = strlen(symbol);
   if (!symbolLength || symbolLength > MAX_SYMBOL_LENGTH) return(error(ERR_INVALID_PARAMETER, "invalid parameter symbol = %s", DoubleQuoteStr(symbol)));
   if (!buffer)                                           return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer = %p", buffer));
   if ((int)bufferSize <= 0)                              return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize = %d", bufferSize));

   char* szTimeframe;

   switch (timeframe) {
      case PERIOD_M1 : szTimeframe = "M1";      break;               // 1 minute
      case PERIOD_M5 : szTimeframe = "M5";      break;               // 5 minutes
      case PERIOD_M15: szTimeframe = "M15";     break;               // 15 minutes
      case PERIOD_M30: szTimeframe = "M30";     break;               // 30 minutes
      case PERIOD_H1 : szTimeframe = "H1";      break;               // 1 hour
      case PERIOD_H4 : szTimeframe = "H4";      break;               // 4 hour
      case PERIOD_D1 : szTimeframe = "Daily";   break;               // 1 day
      case PERIOD_W1 : szTimeframe = "Weekly";  break;               // 1 week
      case PERIOD_MN1: szTimeframe = "Monthly"; break;               // 1 month
      default:
         return(error(ERR_INVALID_PARAMETER, "invalid parameter timeframe = %d", timeframe));
   }

   // create the result in a temporary buffer
   char* format = "%s,%s";
   uint  size   = symbolLength + strlen(szTimeframe) + 2;            // symbol + 1 + timeframe + \0
   char* result = (char*)alloca(size);                               // on the stack
   int copied = sprintf_s(result, size, format, symbol, szTimeframe);
   if (copied <= 0) return(error(ERR_WIN32_ERROR+GetLastError(), "sprintf_s() failed, %d chars copied", copied));

   // copy the result to the destination buffer
   uint len = strlen(result);                                        // len should be equal to size-1

   if (len < bufferSize) {                                           // destination buffer is large enough
      strncpy(buffer, result, len);
      buffer[len] = 0;
      return(len);
   }

   strncpy(buffer, result, bufferSize-1);                            // destination buffer is too small
   buffer[bufferSize-1] = 0;
   return(bufferSize);
}


/**
 * Return the name of the terminal's installation directory.
 *
 * @return string* - directory name (without trailing path separator)
 */
const string& WINAPI getTerminalPath() {
   static string result;
   if (result.empty()) {
      char buffer[MAX_PATH];                                         // on the stack
      GetModuleFileNameA(NULL, buffer, MAX_PATH);                    // TODO: handle errors

      string fileName(buffer);
      string::size_type pos = fileName.find_last_of("\\/");
      result = fileName.substr(0, pos);
   }
   return(result);
}


/**
 * Return a human-readable version of an operation type as a C string.
 *
 * @param  int type - operation type
 *
 * @return char*
 */
const char* WINAPI OperationTypeToStr(int type) {
   switch (type) {
      case OP_BUY      : return("OP_BUY"      );
      case OP_SELL     : return("OP_SELL"     );
      case OP_BUYLIMIT : return("OP_BUYLIMIT" );
      case OP_SELLLIMIT: return("OP_SELLLIMIT");
      case OP_BUYSTOP  : return("OP_BUYSTOP"  );
      case OP_SELLSTOP : return("OP_SELLSTOP" );
      case OP_BALANCE  : return("OP_BALANCE"  );
      case OP_CREDIT   : return("OP_CREDIT"   );
      case OP_UNDEFINED: return("OP_UNDEFINED");
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type=%d (not an operation type)", type));
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI OrderTypeToStr(int type) {
   return(OperationTypeToStr(type));
   #pragma EXPORT
}


/**
 * Return a description of an operation type as a C string.
 *
 * @param  int type - operation type
 *
 * @return char*
 */
const char* WINAPI OperationTypeDescription(int type) {
   switch (type) {
      case OP_BUY      : return("Buy"       );
      case OP_SELL     : return("Sell"      );
      case OP_BUYLIMIT : return("Buy Limit" );
      case OP_SELLLIMIT: return("Sell Limit");
      case OP_BUYSTOP  : return("Stop Buy"  );
      case OP_SELLSTOP : return("Stop Sell" );
      case OP_BALANCE  : return("Balance"   );
      case OP_CREDIT   : return("Credit"    );
      case OP_UNDEFINED: return("undefined" );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type=%d (not an operation type)", type));
   #pragma EXPORT
}


/**
 * Alias
 */
const char* WINAPI OrderTypeDescription(int type) {
   return(OperationTypeDescription(type));
   #pragma EXPORT
}


//
// printf() format codes:
//
// @see  http://www.cplusplus.com/reference/cstdio/printf/
// @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm


/**
 * Format a numeric value as a C string.
 *
 * @param  doube value
 * @param  char* format - printf() format control string
 *
 * @return char* - formatted string or NULL pointer if an error occurred
 */
const char* WINAPI NumberFormat(double value, const char* format) {
   if ((uint)format < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string str = numberFormat(value, format);

   size_t size = str.length() + 1;                                   // +1 for the terminating '\0'
   char* buffer = new char[size];                                    // TODO: close memory leak
   if (!strcpy(buffer, str.c_str()))
      return(NULL);
   return(buffer);
   #pragma EXPORT
}


/**
 * Format a numeric value as a std::string.
 *
 * @param  doube value
 * @param  char* format - printf() format control string
 *
 * @return string - formatted string or empty string if an error occurred
 */
string WINAPI numberFormat(double value, const char* format) {
   if ((uint)format < MIN_VALID_POINTER) return(_EMPTY_STR(error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format)));

   size_t size = _scprintf(format, value) + 1;                       // +1 for the terminating '\0'
   char* buffer = (char*) alloca(size);                              // on the stack
   sprintf_s(buffer, size, format, value);

   return(string(buffer));
}


/**
 * Return a terminal configuration value as a boolean. Queries the global and the local configuration with the local configu-
 * ration superseding the global one. Boolean values can be expressed by "0" or "1", "On" or "Off", "Yes" or "No" and "true" or
 * "false" (case insensitive). An empty value of an existing key is considered FALSE and a numeric value is considered TRUE if
 * its nominal value is non-zero. Trailing configuration comments (text following the ";" character) are ignored.
 *
 * @param  char* section      - configuration section name
 * @param  char* key          - configuration key
 * @param  BOOL  defaultValue - alternative value to return if the specified value was not found
 *
 * @return BOOL - configuration value
 */
BOOL WINAPI GetConfigBool(const char* section, const char* key, BOOL defaultValue/*=FALSE*/) {
   // Es ist schneller, immer globale und lokale Konfiguration auszuwerten (intern jeweils nur ein Aufruf von GetPrivateProfileString()).
   //BOOL result = GetGlobalConfigBool(section, key, defaultValue);
   //return(GetLocalConfigBool (section, key, result));
}
