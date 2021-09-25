#include "expander.h"
#include "lib/array.h"


/**
 * Initialize the array elements in the specified range with a custom value.
 *
 * @param  int values[]         - array
 * @param  int size             - number of elements in the array
 * @param  int initValue        - initialization value
 * @param  int from             - start index of initialization
 * @param  int count [optional] - number of elements to initialize (default: all elements from start index to the end of the array)
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeIntArray(int values[], int size, int initValue, int from, int count/*=INT_MAX*/) {
   if ((uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   if (size < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (from < 0 || from >= size)         return(error(ERR_INVALID_PARAMETER, "invalid parameter from: %d", from));
   if (count < 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must be non-negative)", count));

   if (count == INT_MAX) {
      count = size - from;
   }
   else {
      // @see  https://stackoverflow.com/questions/3944505/detecting-signed-overflow-in-c-c#3947943
      if (count > INT_MAX-from)          return(error(ERR_INVALID_PARAMETER, "integer overflow detected (from: %d, count: %d)", from, count));
      if (from+count > size)             return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (exceeds array boundaries)", count));
   }

   std::fill_n(&values[from], count, initValue);
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize the array elements in the specified range with a custom value.
 *
 * @param  double values[]         - array
 * @param  int    size             - number of elements in the array
 * @param  double initValue        - initialization value
 * @param  int    from             - start index of initialization
 * @param  int    count [optional] - number of elements to initialize (default: all elements from start index to the end of the array)
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeDoubleArray(double values[], int size, double initValue, int from, int count/*=INT_MAX*/) {
   if ((uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   if (size < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (from < 0 || from >= size)         return(error(ERR_INVALID_PARAMETER, "invalid parameter from: %d", from));
   if (count < 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must be non-negative)", count));

   if (count == INT_MAX) {
      count = size - from;
   }
   else {
      // @see  https://stackoverflow.com/questions/3944505/detecting-signed-overflow-in-c-c#3947943
      if (count > INT_MAX-from)          return(error(ERR_INVALID_PARAMETER, "integer overflow detected (from: %d, count: %d)", from, count));
      if (from+count > size)             return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (exceeds array boundaries)", count));
   }

   std::fill_n(&values[from], count, initValue);
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of a timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards elements
 * at the beginning (the oldest values of a timeseries array).
 *
 * @param  int buffer[]   - timeseries array
 * @param  int size       - number of elements in the array
 * @param  int count      - number of elements to shift
 * @param  int emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftIntIndicatorBuffer(int buffer[], int size, int count, int emptyValue) {
   if ((uint)buffer < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (size < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (count < 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d", count));
   if (!size || !count) return(TRUE);

   MoveMemory((void*)&buffer[0], &buffer[count], (size-count)*sizeof(buffer[0]));

   std::fill_n(&buffer[size-count], count, emptyValue);
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of a timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards elements
 * at the beginning (the oldest values of a timeseries array).
 *
 * @param  double buffer[]   - timeseries array
 * @param  int    size       - number of elements in the array
 * @param  int    count      - number of elements to shift
 * @param  double emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftDoubleIndicatorBuffer(double buffer[], int size, int count, double emptyValue) {
   if ((uint)buffer < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (size < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (count < 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d", count));
   if (!size || !count) return(TRUE);

   MoveMemory((void*)&buffer[0], &buffer[count], (size-count)*sizeof(buffer[0]));

   std::fill_n(&buffer[size-count], count, emptyValue);
   return(TRUE);
   #pragma EXPANDER_EXPORT
}
