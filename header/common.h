#pragma once

#pragma warning(push)
#pragma warning(disable: 4060)         // switch statement contains no 'case' or 'default' labels
#pragma warning(disable: 4065)         // switch statement contains 'default' but no 'case' labels
#pragma warning(disable: 4101)         // unreferenced local variable
#pragma warning(disable: 4996)         // std::basic_string<>::copy: Function call with parameters that may be unsafe


#define NOMINMAX                       // alternativ: std::min<int>(3, 4) oder  (std::min)(3, 4);
#include <algorithm>
#include <sstream>


typedef unsigned   int   uint;
typedef          __int64 int64;
typedef unsigned __int64 uint64;
typedef unsigned   int   datetime;


#define INVALID_HWND  ((HWND)-1)


/**
 * Ermittelt die Größe eines Elements eines Typs ohne vorhandene Instanz.
 */
#define sizeofMember(type, member) sizeof(((type*)NULL)->member)


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
 * std::getline() replacement which can handle lines with any line endings.
 */
std::istream& getLine(std::istream &is, std::string &str);
