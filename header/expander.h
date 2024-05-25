#pragma once

#pragma warning(push)
#pragma warning(disable:4060)                               // switch statement contains no 'case' or 'default' labels
#pragma warning(disable:4065)                               // switch statement contains 'default' but no 'case' labels
#pragma warning(disable:4101)                               // unreferenced local variable
#pragma warning(disable:4996)                               // deprecation warnings and function calls with parameters that may be unsafe

#define  EXPANDER_EXPORT  comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)

#include "stdafx.h"
#include "shared/defines.h"                                 // shared between C++ and MQL
#include "shared/errors.h"                                  // shared between C++ and MQL

#include <iomanip>
#include <string>
#include <mbstring.h>


// type aliases
typedef unsigned   char      uchar;
typedef            wchar_t   wchar;
typedef unsigned   int       uint;
typedef          __int64     int64;
typedef unsigned __int64     uint64;
typedef            DWORD     color;                         // MQL4 color
typedef          __time32_t  time32;                        // MQL4 32-bit timestamp
typedef          __time64_t  time64;                        // MQL5 64-bit timestamp
typedef            tm        TM;                            // C time structure

namespace rsf {}                                            // define namespace 'rsf'
using namespace rsf;                                        // always prefer namespace 'rsf'

using std::size_t;
using std::string;
using std::wstring;

#define CLR_NONE             0xFFFFFFFFL                    // different types/same value in C++ and MQL
#define NO_ERROR                      0L                    // different types/same value in C++ and MQL

#define DUMPMODE_HEX                   1
#define DUMPMODE_CHAR                  2
#define INVALID_HWND                   ((HWND)-1)
#define MAX_CHARTDESCRIPTION_LENGTH    ((MAX_SYMBOL_LENGTH) + 1 + 7) // e.g. "SYMBOL,Monthly"

#if !defined(MAX_PATH)                                      // e.g. the max. path on drive D is "D:\some-256-chars-path-string<NUL>"
 #define MAX_PATH                      _MAX_PATH            // max. length of full pathname
#endif
#define MAX_DRIVE                      _MAX_DRIVE           // max. length of drive component
#define MAX_DIR                        _MAX_DIR             // max. length of path component
#define MAX_FNAME                      _MAX_FNAME           // max. length of file name component
#define MAX_EXT                        _MAX_EXT             // max. length of extension component


// MQL program types (flags)
enum ProgramType {
   PT_INDICATOR = PROGRAMTYPE_INDICATOR,
   PT_EXPERT    = PROGRAMTYPE_EXPERT,
   PT_SCRIPT    = PROGRAMTYPE_SCRIPT
};


// MQL module types (flags)
enum ModuleType {
   MT_INDICATOR = MODULETYPE_INDICATOR,
   MT_EXPERT    = MODULETYPE_EXPERT,
   MT_SCRIPT    = MODULETYPE_SCRIPT,
   MT_LIBRARY   = MODULETYPE_LIBRARY
};


// MQL program launch types
enum LaunchType {
   LT_TEMPLATE = LAUNCHTYPE_TEMPLATE,                       // launched via template
   LT_PROGRAM  = LAUNCHTYPE_PROGRAM,                        // launched via call of iCustom()
   LT_USER     = LAUNCHTYPE_USER                            // launched by user
};


// MQL program core functions
enum CoreFunction {
   CF_INIT   = COREFUNCTION_INIT,
   CF_START  = COREFUNCTION_START,
   CF_DEINIT = COREFUNCTION_DEINIT
};


// MQL program initialize reasons
enum InitializeReason {                                     // +-- init reason --------------------------------+-- ui -----------+-- applies --+
   IR_USER              = INITREASON_USER,                  // | loaded by the user (also in tester)           |    input dialog |   I, E, S   |   I = indicators
   IR_TEMPLATE          = INITREASON_TEMPLATE,              // | loaded by a template (also at terminal start) | no input dialog |   I, E      |   E = experts
   IR_PROGRAM           = INITREASON_PROGRAM,               // | loaded by iCustom()                           | no input dialog |   I         |   S = scripts
   IR_PROGRAM_AFTERTEST = INITREASON_PROGRAM_AFTERTEST,     // | loaded by iCustom() after end of test         | no input dialog |   I         |
   IR_PARAMETERS        = INITREASON_PARAMETERS,            // | input parameters changed                      |    input dialog |   I, E      |
   IR_TIMEFRAMECHANGE   = INITREASON_TIMEFRAMECHANGE,       // | chart period changed                          | no input dialog |   I, E      |
   IR_SYMBOLCHANGE      = INITREASON_SYMBOLCHANGE,          // | chart symbol changed                          | no input dialog |   I, E      |
   IR_RECOMPILE         = INITREASON_RECOMPILE,             // | reloaded after recompilation                  | no input dialog |   I, E      |
   IR_TERMINAL_FAILURE  = INITREASON_TERMINAL_FAILURE       // | terminal failure                              |    input dialog |      E      |   @see https://github.com/rosasurfer/mt4-mql/issues/1
};                                                          // +-----------------------------------------------+-----------------+-------------+


// MQL program uninitialize reasons
enum UninitializeReason {
   UR_UNDEFINED   = UNINITREASON_UNDEFINED,
   UR_REMOVE      = UNINITREASON_REMOVE,
   UR_RECOMPILE   = UNINITREASON_RECOMPILE,
   UR_CHARTCHANGE = UNINITREASON_CHARTCHANGE,
   UR_CHARTCLOSE  = UNINITREASON_CHARTCLOSE,
   UR_PARAMETERS  = UNINITREASON_PARAMETERS,
   UR_ACCOUNT     = UNINITREASON_ACCOUNT,
   // since terminal build > 509
   UR_TEMPLATE    = UNINITREASON_TEMPLATE,
   UR_INITFAILED  = UNINITREASON_INITFAILED,
   UR_CLOSE       = UNINITREASON_CLOSE
};


// Debugging and error handling
#define dump(...)   _dump (__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define debug(...)  _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define warn(...)   _warn (__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define error(...)  _error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

int __cdecl _dump (const char* fileName, const char* funcName, int line, const void* data, uint size, uint mode=DUMPMODE_HEX);
int __cdecl _debug(const char* fileName, const char* funcName, int line, const char* message, ...);
int __cdecl _warn (const char* fileName, const char* funcName, int line, int code, const char* message, ...);
int __cdecl _error(const char* fileName, const char* funcName, int line, int code, const char* message, ...);

BOOL WINAPI IsDebugAccountServer();
BOOL WINAPI IsDebugAccountNumber();
BOOL WINAPI IsDebugEC();


// Helper functions returning constant values. All parameters are ignored.
inline int         __cdecl _EMPTY        (...) { return(EMPTY       ); }                // only __cdecl supports variadics
inline int         __cdecl _EMPTY_VALUE  (...) { return(EMPTY_VALUE ); }
inline const char* __cdecl _EMPTY_STR    (...) { return(""          ); }
inline       char* __cdecl _EMPTY_NEW_STR(...) { char* s = new char[1]; s[0] = 0; return(s); }
inline HWND        __cdecl _INVALID_HWND (...) { return(INVALID_HWND); }
inline int         __cdecl _NULL         (...) { return(NULL        ); }
inline bool        __cdecl _true         (...) { return(true        ); }
inline BOOL        __cdecl _TRUE         (...) { return(TRUE        ); }
inline bool        __cdecl _false        (...) { return(false       ); }
inline BOOL        __cdecl _FALSE        (...) { return(FALSE       ); }
inline color       __cdecl _CLR_NONE     (...) { return(CLR_NONE    ); }
inline color       __cdecl _NaC          (...) { return(NaC         ); }
inline time32      __cdecl _NaT32        (...) { return(NaT         ); }
inline time64      __cdecl _NaT64        (...) { return(NaT         ); }


// Helper functions returning variable values. All parameters except the first one are ignored.
inline bool        __cdecl _bool  (bool   value, ...) { return(value); }
inline BOOL        __cdecl _BOOL  (BOOL   value, ...) { return(value); }
inline char        __cdecl _char  (char   value, ...) { return(value); }
inline int         __cdecl _int   (int    value, ...) { return(value); }
inline float       __cdecl _float (float  value, ...) { return(value); }
inline double      __cdecl _double(double value, ...) { return(value); }


/**
 * Return the size of a type member without an actual instance.
 */
#define sizeofMember(type, member) sizeof(((type*)NULL)->member)
