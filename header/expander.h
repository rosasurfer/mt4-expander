#pragma once

#pragma warning(push)
#pragma warning(disable:4060)                                        // switch statement contains no 'case' or 'default' labels
#pragma warning(disable:4065)                                        // switch statement contains 'default' but no 'case' labels
#pragma warning(disable:4101)                                        // unreferenced local variable
#pragma warning(disable:4996)                                        // function call with parameters that may be unsafe

#include "stdafx.h"
#include "shared/defines.h"                                          // definitions shared between C++ and MQL
#include "shared/errors.h"                                           // error codes shared between C++ and MQL

#include <string>
#include <sstream>


// export unmangled names without a DEF file
#define EXPANDER_EXPORT comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)


// type aliases
typedef unsigned   int   uint;
typedef          __int64 int64;
typedef unsigned __int64 uint64;
typedef time_t           datetime;                                   // a 32-bit signed long
typedef std::string      string;


#define INVALID_HWND                   ((HWND)-1)
#define MAX_CHART_DESCRIPTION_LENGTH   ((MAX_SYMBOL_LENGTH) + 1 + 7) // "SYMBOL,Monthly"


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
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,                               // launched via template
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,                                // launched via call of iCustom()
   LT_MANUAL    = LAUNCHTYPE_MANUAL                                  // launched manually (by hand)
};


// MQL program root functions
enum RootFunction {
   RF_INIT      = ROOTFUNCTION_INIT,
   RF_START     = ROOTFUNCTION_START,
   RF_DEINIT    = ROOTFUNCTION_DEINIT
};


// MQL program initialize reasons
enum InitializeReason {
   IR_USER              = INITREASON_USER,
   IR_TEMPLATE          = INITREASON_TEMPLATE,
   IR_PROGRAM           = INITREASON_PROGRAM,
   IR_PROGRAM_AFTERTEST = INITREASON_PROGRAM_AFTERTEST,
   IR_PARAMETERS        = INITREASON_PARAMETERS,
   IR_TIMEFRAMECHANGE   = INITREASON_TIMEFRAMECHANGE,
   IR_SYMBOLCHANGE      = INITREASON_SYMBOLCHANGE,
   IR_RECOMPILE         = INITREASON_RECOMPILE
};


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


// Debugging & error handling.
#define debug(...)   _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define warn(...)    _warn (__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define error(...)   _error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

int   _debug(const char* fileName, const char* funcName, int line, const char*   format, ...                );
int   _debug(const char* fileName, const char* funcName, int line, const string& format, ...                );
void __debug(const char* fileName, const char* funcName, int line, const char*   format, const va_list& args);

int   _warn (const char* fileName, const char* funcName, int line, int code, const char*   format, ...                );
int   _warn (const char* fileName, const char* funcName, int line, int code, const string& format, ...                );
void __warn (const char* fileName, const char* funcName, int line, int code, const char*   format, const va_list& args);

int   _error(const char* fileName, const char* funcName, int line, int code, const char*   format, ...                );
int   _error(const char* fileName, const char* funcName, int line, int code, const string& format, ...                );
void __error(const char* fileName, const char* funcName, int line, int code, const char*   format, const va_list& args);


// Helper functions returning fixed values. All parameters are ignored.
int         WINAPI _CLR_NONE    (...);
int         WINAPI _EMPTY       (...);
const char* WINAPI _EMPTY_STR   (...);
HWND        WINAPI _INVALID_HWND(...);
int         WINAPI _NULL        (...);
bool        WINAPI _true        (...);
bool        WINAPI _false       (...);
BOOL        WINAPI _TRUE        (...);
BOOL        WINAPI _FALSE       (...);


// Helper functions returning variable values. All parameters except the first one are ignored.
bool        WINAPI _bool        (bool   value, ...);
char        WINAPI _char        (char   value, ...);
int         WINAPI _int         (int    value, ...);
float       WINAPI _float       (float  value, ...);
double      WINAPI _double      (double value, ...);
BOOL        WINAPI _BOOL        (BOOL   value, ...);


/**
 * Return the size of a type member without an actual instance.
 */
#define sizeofMember(type, member) sizeof(((type*)NULL)->member)


/**
 * to_string() replacement for missing C++11 support in VS 2008.
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
