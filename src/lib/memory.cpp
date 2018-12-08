#include "expander.h"
#include "struct/mt4/MqlStr.h"


/**
 * Return the memory location of a BOOL array. Helper function to resolve addresses in MQL.
 *
 * @param  BOOL values[] - MQL bool array (the internal MetaTrader implementation is an integer array)
 *
 * @return uint - memory location or NULL in case of errors
 */
uint WINAPI GetBoolsAddress(const BOOL values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the memory location of a double array. Helper function to resolve addresses in MQL.
 *
 * @param  double values[] - MQL double array
 *
 * @return uint - memory location or NULL in case of errors
 */
uint WINAPI GetDoublesAddress(const double values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the memory location of an integer array. Helper function to resolve addresses in MQL.
 *
 * @param  int values[] - MQL integer array
 *
 * @return uint - memory location or NULL in case of errors
 */
uint WINAPI GetIntsAddress(const int values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the memory location of a C-like Ansi or Unicode string. Helper function to resolve addresses in MQL.
 *
 * @param  TCHAR* value - MQL Ansi or Unicode string
 *
 * @return uint - memory location or NULL in case of errors
 *
 *
 * Note: In MQL this function must be used only with string array elements. If the parameter is a single string the terminal
 *       will create and pass a copy to the DLL. After the function returnes the copy will be immediately destroyed and the
 *       resolved address becomes invalid.
 */
uint WINAPI GetStringAddress(const TCHAR* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));
   return((uint) value);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the memory location of an MQL string array. Helper function to resolve addresses in MQL.
 *
 * @param  MqlStr values[] - MQL string array
 *
 * @return uint - memory location or NULL in case of errors
 */
uint WINAPI GetStringsAddress(const MqlStr values[]) {
   if (values && (uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   return((uint) values);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the memory location of any type instance. Helper function to resolve addresses in MQL.
 *
 * @param  void* value - pointer to an instance of any type
 *
 * @return uint - memory location or NULL in case of errors
 */
uint WINAPI GetPointedAddress(const void* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", value));
   return((uint) value);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether or not the content of two memory ranges is considered equal. Convenient helper to hide the non-intuitive memcmp()
 * syntax.
 *
 * @param  void*   bufferA - first memory location
 * @param  void*   bufferB - second memory location
 * @param  size_t* size    - size of the memory ranges to compare in bytes
 *
 * @return BOOL
 */
BOOL WINAPI MemCompare(const void* bufferA, const void* bufferB, size_t size) {
   if (bufferA == bufferB)   return(TRUE);                           // if pointers are equal values are too
   if (!bufferA || !bufferB) return(FALSE);                          // if one is a NULL pointer the other can't
   return(memcmp(bufferA, bufferB, size) == 0);                      // both are not NULL pointers
   #pragma EXPANDER_EXPORT
}
