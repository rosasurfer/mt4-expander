#include "expander.h"


/**
 * Shift the content of a timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards elements
 * at the beginning (the oldest values of a timeseries array).
 *
 * @param  double buffer[]   - timeseries array
 * @param  int    bufferSize - array size, i.e. the number of elements in the array
 * @param  int    elements   - number of elements to shift
 * @param  double emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftIndicatorBuffer(double buffer[], int bufferSize, int elements, double emptyValue) {
   if (buffer && (uint)buffer < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (bufferSize < 0)                             return(error(ERR_INVALID_PARAMETER, "invalid parameter bufferSize: %d", bufferSize));
   if (elements < 0)                               return(error(ERR_INVALID_PARAMETER, "invalid parameter elements: %d", elements));
   if (!bufferSize || !elements) return(TRUE);

   MoveMemory((void*)&buffer[0], &buffer[elements], (bufferSize-elements)*sizeof(buffer[0]));

   for (int i=bufferSize-elements; i < bufferSize; i++) {
      buffer[i] = emptyValue;
   }
   return(TRUE);
   #pragma EXPANDER_EXPORT
}
