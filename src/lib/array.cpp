#include "expander.h"


/**
 * Initialize the array elements in the specified range with a custom value.
 *
 * @param  double values[]         - array
 * @param  int    size             - number of elements in the array
 * @param  double initValue        - custom initialization value
 * @param  int    from             - start index of initialization
 * @param  int    count [optional] - number of elements to initialize (default: all elements from start index to the end of the array)
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeDoubleArray(double values[], int size, double initValue, int from, int count/*=EMPTY*/) {
   if ((uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   if (size < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (from < 0 || from >= size)         return(error(ERR_INVALID_PARAMETER, "invalid parameter from: %d", from));
   if (count < (int)EMPTY)               return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must be non-negative)", count));
   if (from+count > size)                return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (exceeds array boundaries)", count));

   if (count == EMPTY)
      count = size - from;
   std::fill_n(&values[from], count, initValue);

   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of a timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards elements
 * at the beginning (the oldest values of a timeseries array).
 *
 * @param  double buffer[]   - timeseries array
 * @param  int    bufferSize - number of elements in the array
 * @param  int    elements   - number of elements to shift
 * @param  double emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftIndicatorBuffer(double buffer[], int bufferSize, int elements, double emptyValue) {
   if ((uint)buffer < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (bufferSize < 0)                   return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d", bufferSize));
   if (elements < 0)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter elements: %d", elements));
   if (!bufferSize || !elements) return(TRUE);

   MoveMemory((void*)&buffer[0], &buffer[elements], (bufferSize-elements)*sizeof(buffer[0]));

   for (int i=bufferSize-elements; i < bufferSize; i++) {
      buffer[i] = emptyValue;
   }
   return(TRUE);
   #pragma EXPANDER_EXPORT
}
