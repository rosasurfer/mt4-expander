#pragma once
#pragma warning(push)
#pragma warning(disable: 4060)         // switch statement contains no 'case' or 'default' labels
#pragma warning(disable: 4065)         // switch statement contains 'default' but no 'case' labels
#pragma warning(disable: 4101)         // unreferenced local variable
#pragma warning(disable: 4996)         // std::basic_string<>::copy: Function call with parameters that may be unsafe


#include <sstream>


typedef unsigned   int   uint;
typedef          __int64 int64;
typedef unsigned __int64 uint64;


/**
 * Ermittelt die Größe eines Elements eines Typs ohne vorhandene Instanz.
 */
#define sizeofMember(type, member) sizeof(((type*)NULL)->member)


/**
 * Process a debug mesage.
 */
#define debug(...)   _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

int   _debug(char* fileName, char* funcName, int line, const char*        format, ...);
int   _debug(char* fileName, char* funcName, int line, const std::string &format, ...);
void __debug(char* fileName, char* funcName, int line, const char*        format, const va_list &args);


/**
 * to_string()-Ersatz für fehlenden C++11-Support in Visual Studio 2008.
 *
 * VS 2005 - not supported
 * VS 2008 - not supported
 * VS 2010 - supported
 *
 * @param  <typename T> value
 *
 * @return std::string
 *
 * Note: Template functions have to be declared and defined in the same file.
 */
template <typename T> std::string to_string(T value) {
   std::ostringstream os;
   os << value;
   return(os.str());
}


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende feste Werte zurückzugeben.
 * Alle Parameter werden ignoriert.
 */
bool _true (...);
bool _false(...);
int  _NULL (...);


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende variable Werte zurückzugeben.
 * Außer dem ersten werden alle übergebenen Parameter ignoriert.
 */
bool   _bool  (bool   value, ...);
char   _char  (char   value, ...);
int    _int   (int    value, ...);
float  _float (float  value, ...);
double _double(double value, ...);
