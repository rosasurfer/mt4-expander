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
#include <sstream>


// type aliases
typedef unsigned   char    uchar;
typedef            wchar_t wchar;
typedef unsigned   int     uint;
typedef          __int64   int64;
typedef unsigned __int64   uint64;
typedef DWORD              color;                           // MQL type color
typedef time_t             datetime;                        // MQL type datetime: a 32-bit signed long


namespace rsf {}
using namespace rsf;

using std::string;
using std::wstring;


#define CLR_NONE             0xFFFFFFFFL                    // different types/same value in C++ and MQL
#define NO_ERROR                      0L                    // different types/same value in C++ and MQL

#define DUMPMODE_HEX                   1
#define DUMPMODE_CHAR                  2
#define INVALID_HWND                   ((HWND)-1)
#define MAX_CHARTDESCRIPTION_LENGTH    ((MAX_SYMBOL_LENGTH) + 1 + 7) // e.g. "SYMBOL,Monthly"

#if !defined(MAX_PATH)                                      // e.g. the max. path on drive D is "D:\some-256-chars-path-string<NUL>"
#define MAX_PATH                        _MAX_PATH           // max. length of full pathname
#endif
#define MAX_DRIVE                       _MAX_DRIVE          // max. length of drive component
#define MAX_DIR                         _MAX_DIR            // max. length of path component
#define MAX_FNAME                       _MAX_FNAME          // max. length of file name component
#define MAX_EXT                         _MAX_EXT            // max. length of extension component


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
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,                      // launched via template
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,                       // launched via call of iCustom()
   LT_MANUAL    = LAUNCHTYPE_MANUAL                         // launched manually (by hand)
};


// MQL program core functions
enum CoreFunction {
   CF_INIT      = COREFUNCTION_INIT,
   CF_START     = COREFUNCTION_START,
   CF_DEINIT    = COREFUNCTION_DEINIT
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
   UR_UNDEFINED         = UNINITREASON_UNDEFINED,
   UR_REMOVE            = UNINITREASON_REMOVE,
   UR_RECOMPILE         = UNINITREASON_RECOMPILE,
   UR_CHARTCHANGE       = UNINITREASON_CHARTCHANGE,
   UR_CHARTCLOSE        = UNINITREASON_CHARTCLOSE,
   UR_PARAMETERS        = UNINITREASON_PARAMETERS,
   UR_ACCOUNT           = UNINITREASON_ACCOUNT,
   // since terminal build > 509
   UR_TEMPLATE          = UNINITREASON_TEMPLATE,
   UR_INITFAILED        = UNINITREASON_INITFAILED,
   UR_CLOSE             = UNINITREASON_CLOSE
};


// Debugging and error handling.
#define dump(...)    _dump (__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define debug(...)   _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define warn(...)    _warn (__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define error(...)   _error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

int   _dump (const char* fileName, const char* funcName, int line, const void* data, uint size, uint mode = DUMPMODE_HEX);

int   _debug(const char* fileName, const char* funcName, int line, const char*   format, ...                );
int   _debug(const char* fileName, const char* funcName, int line, const string& format, ...                );
void __debug(const char* fileName, const char* funcName, int line, const char*   format, const va_list& args);

int   _warn (const char* fileName, const char* funcName, int line, int code, const char*   format, ...                );
int   _warn (const char* fileName, const char* funcName, int line, int code, const string& format, ...                );
void __warn (const char* fileName, const char* funcName, int line, int code, const char*   format, const va_list& args);

int   _error(const char* fileName, const char* funcName, int line, int code, const char*   format, ...                );
int   _error(const char* fileName, const char* funcName, int line, int code, const string& format, ...                );
void __error(const char* fileName, const char* funcName, int line, int code, const char*   format, const va_list& args);


// Helper functions returning constant values. All parameters are ignored.
inline int         WINAPI _EMPTY        (...) { return(EMPTY       ); }
inline const char* WINAPI _EMPTY_STR    (...) { return(""          ); }
inline       char* WINAPI _EMPTY_NEW_STR(...) { char* s = new char[1]; s[0] = 0; return(s); }
inline HWND        WINAPI _INVALID_HWND (...) { return(INVALID_HWND); }
inline int         WINAPI _NULL         (...) { return(NULL        ); }
inline bool        WINAPI _true         (...) { return(true        ); }
inline BOOL        WINAPI _TRUE         (...) { return(TRUE        ); }
inline bool        WINAPI _false        (...) { return(false       ); }
inline BOOL        WINAPI _FALSE        (...) { return(FALSE       ); }
inline color       WINAPI _CLR_NONE     (...) { return(CLR_NONE    ); }
inline color       WINAPI _NaC          (...) { return(NaC         ); }
inline datetime    WINAPI _NaT          (...) { return(NaT         ); }


// Helper functions returning variable values. All parameters except the first one are ignored.
inline bool        WINAPI _bool  (bool   value, ...) { return(value); }
inline BOOL        WINAPI _BOOL  (BOOL   value, ...) { return(value); }
inline char        WINAPI _char  (char   value, ...) { return(value); }
inline int         WINAPI _int   (int    value, ...) { return(value); }
inline float       WINAPI _float (float  value, ...) { return(value); }
inline double      WINAPI _double(double value, ...) { return(value); }


/**
 * Return the size of a type member without an actual instance.
 */
#define sizeofMember(type, member) sizeof(((type*)NULL)->member)


/**
 * C++11 to_string() replacement for VS 2008.
 *
 * @param  <typename T> value
 *
 * @return string
 */
template <typename T> string to_string(T value) {
   std::ostringstream os;
   os << value;
   return(os.str());
}
