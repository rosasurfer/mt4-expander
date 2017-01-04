#pragma once

#pragma warning(push)
#pragma warning(disable:4060)          // switch statement contains no 'case' or 'default' labels
#pragma warning(disable:4065)          // switch statement contains 'default' but no 'case' labels
#pragma warning(disable:4101)          // unreferenced local variable
#pragma warning(disable:4996)          // std::string<>::copy: Function call with parameters that may be unsafe


#define  NOMINMAX                      // alternativ: std::min<int>(3, 4) oder  (std::min)(3, 4);
#define _USE_32BIT_TIME_T
#include <algorithm>
#include <sstream>


typedef unsigned   int   uint;
typedef          __int64 int64;
typedef unsigned __int64 uint64;
typedef time_t           datetime;     // signed long
typedef std::string      string;


#define INVALID_HWND  ((HWND)-1)


/**
 * Ermittelt die Größe eines Elements eines Typs ohne vorhandene Instanz.
 */
#define sizeofMember(type, member) sizeof(((type*)NULL)->member)


/**
 * std::getline() replacement for reading lines with any line endings.
 */
std::istream& getline(std::istream &is, string &line);


/**
 * to_string() replacement for missing C++11 support in VS 2008 (supported since VS 2010).
 *
 * @param  <typename T> value
 *
 * @return string
 *
 * Note: Template functions have must be declared and defined in the same place.
 */
template <typename T> string to_string(T value) {
   std::ostringstream os;
   os << value;
   return(os.str());
}
