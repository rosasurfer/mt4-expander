#include "expander.h"
#include "lib/array.h"


/**
 * Initialize a range of array elements with a custom value.
 *
 * @param  T   values[]         - array
 * @param  int size             - number of elements in the array
 * @param  T   initValue        - custom initialization value
 * @param  int from             - start index of initialization
 * @param  int count [optional] - number of elements to initialize (default: all elements from the start index to the end)
 *
 * @return BOOL - success status
 */
template <typename T>
BOOL WINAPI InitializeArray(T values[], int size, T initValue, int from, int count/*=INT_MAX*/) {
   if ((uint)values < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter values: 0x%p (not a valid pointer)", values));
   if (size < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (from < 0 || from >= size)         return(error(ERR_INVALID_PARAMETER, "invalid parameter from: %d", from));
   if (count < 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must be non-negative)", count));

   if (count == INT_MAX) {
      count = size - from;
   }
   else {
      // @see  https://stackoverflow.com/questions/3944505/detecting-signed-overflow-in-c-c#3947943
      if (count > INT_MAX-from) return(error(ERR_INVALID_PARAMETER, "integer overflow detected (from: %d, count: %d)", from, count));
      if (from+count > size)    return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (exceeds array boundaries)", count));
   }

   std::fill_n(&values[from], count, initValue);
   return(TRUE);
}


// explicit template instantiation to make definitions accessible to the linker
template BOOL InitializeArray<char>  (char[],   int, char,   int, int);
template BOOL InitializeArray<int>   (int[],    int, int,    int, int);
template BOOL InitializeArray<float> (float[],  int, float,  int, int);
template BOOL InitializeArray<double>(double[], int, double, int, int);


/**
 * Initialize a range of <char> array elements with a custom value.
 *
 * @param  char values[]  - array
 * @param  int  size      - number of elements in the array
 * @param  char initValue - custom initialization value
 * @param  int  from      - start index of initialization
 * @param  int  count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeCharArray(char values[], int size, char initValue, int from, int count) {
   return(InitializeArray(values, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <int> array elements with a custom value.
 *
 * @param  int values[]  - array
 * @param  int size      - number of elements in the array
 * @param  int initValue - custom initialization value
 * @param  int from      - start index of initialization
 * @param  int count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeIntArray(int values[], int size, int initValue, int from, int count) {
   return(InitializeArray(values, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <float> array elements with a custom value.
 *
 * @param  float  values[]  - array
 * @param  int    size      - number of elements in the array
 * @param  float  initValue - custom initialization value
 * @param  int    from      - start index of initialization
 * @param  int    count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeFloatArray(float values[], int size, float initValue, int from, int count) {
   return(InitializeArray(values, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <double> array elements with a custom value.
 *
 * @param  double values[]  - array
 * @param  int    size      - number of elements in the array
 * @param  double initValue - custom initialization value
 * @param  int    from      - start index of initialization
 * @param  int    count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeDoubleArray(double values[], int size, double initValue, int from, int count) {
   return(InitializeArray(values, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of an MQL timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  T   buffer[]   - timeseries array
 * @param  int size       - number of elements in the array
 * @param  int count      - number of elements to shift
 * @param  T   emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
template <typename T>
BOOL WINAPI ShiftIndicatorBuffer(T buffer[], int size, int count, T emptyValue) {
   if ((uint)buffer < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (size < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d", size));
   if (count < 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d", count));
   if (!size || !count) return(TRUE);

   MoveMemory((void*)&buffer[0], &buffer[count], (size-count)*sizeof(buffer[0]));

   std::fill_n(&buffer[size-count], count, emptyValue);
   return(TRUE);
}


// explicit template instantiation to make definitions accessible to the linker
template BOOL ShiftIndicatorBuffer<char>  (char[],   int, int, char);
template BOOL ShiftIndicatorBuffer<int>   (int[],    int, int, int);
template BOOL ShiftIndicatorBuffer<float> (float[],  int, int, float);
template BOOL ShiftIndicatorBuffer<double>(double[], int, int, double);


/**
 * Shift the content of a <char> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  char buffer[]   - timeseries array
 * @param  int  size       - number of elements in the array
 * @param  int  count      - number of elements to shift
 * @param  char emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftCharIndicatorBuffer(char buffer[], int size, int count, char emptyValue) {
   return(ShiftIndicatorBuffer(buffer, size, count, emptyValue));
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of an <int> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  int buffer[]   - timeseries array
 * @param  int size       - number of elements in the array
 * @param  int count      - number of elements to shift
 * @param  int emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftIntIndicatorBuffer(int buffer[], int size, int count, int emptyValue) {
   return(ShiftIndicatorBuffer(buffer, size, count, emptyValue));
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of a <float> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  float  buffer[]   - timeseries array
 * @param  int    size       - number of elements in the array
 * @param  int    count      - number of elements to shift
 * @param  float  emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftFloatIndicatorBuffer(float buffer[], int size, int count, float emptyValue) {
   return(ShiftIndicatorBuffer(buffer, size, count, emptyValue));
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of a <double> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  double buffer[]   - timeseries array
 * @param  int    size       - number of elements in the array
 * @param  int    count      - number of elements to shift
 * @param  double emptyValue - initialization value for elements becoming "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftDoubleIndicatorBuffer(double buffer[], int size, int count, double emptyValue) {
   return(ShiftIndicatorBuffer(buffer, size, count, emptyValue));
   #pragma EXPANDER_EXPORT
}
