#include "expander.h"
#include "lib/array.h"


/**
 * Initialize a range of array elements with a custom value.
 *
 * @param  T   array[]          - array
 * @param  int size             - number of elements in the array
 * @param  T   initValue        - custom initialization value
 * @param  int from             - start index of initialization
 * @param  int count [optional] - number of elements to initialize (default: all elements from start index to the end)
 *
 * @return BOOL - success status
 */
template <typename T>
BOOL WINAPI InitializeArray(T array[], int size, T initValue, int from, int count/*=INT_MAX*/) {
   if ((uint)array < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter array: 0x%p (not a valid pointer)", array));
   if (size < 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d (must not be negative)", size));
   if (from < 0 || from >= size)        return(error(ERR_INVALID_PARAMETER, "invalid parameter from: %d (out of range)", from));
   if (count < 0)                       return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must not be negative)", count));

   if (count == INT_MAX) {
      count = size - from;
   }
   else {
      // @see  https://stackoverflow.com/questions/3944505/detecting-signed-overflow-in-c-c#3947943
      if (count > INT_MAX-from) return(error(ERR_INVALID_PARAMETER, "integer overflow detected (from: %d, count: %d)", from, count));
      if (from+count > size)    return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (exceeds range)", count));
   }

   std::fill_n(&array[from], count, initValue);
   return(TRUE);
}


// explicit template instantiation to make definitions accessible to the linker
template BOOL InitializeArray<bool>  (bool  [], int, bool,   int, int);
template BOOL InitializeArray<char>  (char  [], int, char,   int, int);
template BOOL InitializeArray<short> (short [], int, short,  int, int);
template BOOL InitializeArray<int>   (int   [], int, int,    int, int);
template BOOL InitializeArray<int64> (int64 [], int, int64,  int, int);
template BOOL InitializeArray<float> (float [], int, float,  int, int);
template BOOL InitializeArray<double>(double[], int, double, int, int);


/**
 * Initialize a range of <MQL4:bool> array elements with a custom value.
 *
 * @param  BOOL array[]   - array with sizeof(element) = 4
 * @param  int  size      - number of elements in the array
 * @param  BOOL initValue - custom initialization value
 * @param  int  from      - start index of initialization
 * @param  int  count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeBOOLArray(BOOL array[], int size, BOOL initValue, int from, int count) {
   return(InitializeArray(array, size, (int)(initValue!=0), from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <MQL5:bool> array elements with a custom value.
 *
 * @param  bool array[]   - array with sizeof(element) = 1
 * @param  int  size      - number of elements in the array
 * @param  bool initValue - custom initialization value
 * @param  int  from      - start index of initialization
 * @param  int  count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeBoolArray(bool array[], int size, bool initValue, int from, int count) {
   return(InitializeArray(array, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <char> array elements with a custom value.
 *
 * @param  char array[]   - array
 * @param  int  size      - number of elements in the array
 * @param  char initValue - custom initialization value
 * @param  int  from      - start index of initialization
 * @param  int  count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeCharArray(char array[], int size, char initValue, int from, int count) {
   return(InitializeArray(array, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <short> array elements with a custom value.
 *
 * @param  short array[]   - array
 * @param  int   size      - number of elements in the array
 * @param  short initValue - custom initialization value
 * @param  int   from      - start index of initialization
 * @param  int   count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeShortArray(short array[], int size, short initValue, int from, int count) {
   return(InitializeArray(array, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <int> array elements with a custom value.
 *
 * @param  int array[]   - array
 * @param  int size      - number of elements in the array
 * @param  int initValue - custom initialization value
 * @param  int from      - start index of initialization
 * @param  int count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeIntArray(int array[], int size, int initValue, int from, int count) {
   return(InitializeArray(array, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <MQL5:long> array elements with a custom value.
 *
 * @param  int64 array[]   - array with sizeof(element) = 8
 * @param  int   size      - number of elements in the array
 * @param  int64 initValue - custom initialization value
 * @param  int   from      - start index of initialization
 * @param  int   count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeLongArray(int64 array[], int size, int64 initValue, int from, int count) {
   return(InitializeArray(array, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <float> array elements with a custom value.
 *
 * @param  float  array[]   - array
 * @param  int    size      - number of elements in the array
 * @param  float  initValue - custom initialization value
 * @param  int    from      - start index of initialization
 * @param  int    count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeFloatArray(float array[], int size, float initValue, int from, int count) {
   return(InitializeArray(array, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Initialize a range of <double> array elements with a custom value.
 *
 * @param  double array[]   - array
 * @param  int    size      - number of elements in the array
 * @param  double initValue - custom initialization value
 * @param  int    from      - start index of initialization
 * @param  int    count     - number of elements to initialize
 *
 * @return BOOL - success status
 */
BOOL WINAPI InitializeDoubleArray(double array[], int size, double initValue, int from, int count) {
   return(InitializeArray(array, size, initValue, from, count));
   #pragma EXPANDER_EXPORT
}


/**
 * Search an <int> array for a value and return its index.
 *
 * @param  int  array[]                   - array to search
 * @param  int  size                      - number of elements in the array
 * @param  int  value                     - value to search
 * @param  BOOL reverseIndexed [optional] - whether the array should be reverse indexed like an MQL timeseries array (default: no)
 *
 * @return int - index of the first match or EMPTY (-1) if the value was not found;
 *               -2 in case of errors
 */
int WINAPI SearchIntArray(const int array[], int size, int value, BOOL reverseIndexed = FALSE) {

   return(EMPTY);
   #pragma EXPANDER_EXPORT
}
















/**
 * Shift the content of an MQL timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  T   buffer[]   - timeseries array
 * @param  int size       - number of elements in the array
 * @param  int count      - number of elements to shift
 * @param  T   emptyValue - initialization value for elements interpreted as "empty"
 *
 * @return BOOL - success status
 */
template <typename T>
BOOL WINAPI ShiftIndicatorBuffer(T buffer[], int size, int count, T emptyValue) {
   if ((uint)buffer < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter buffer: 0x%p (not a valid pointer)", buffer));
   if (size < 0)                         return(error(ERR_INVALID_PARAMETER, "invalid parameter size: %d (must not be negative)", size));
   if (count < 0)                        return(error(ERR_INVALID_PARAMETER, "invalid parameter count: %d (must not be negative)", count));
   if (count > size)                     return(error(ERR_INVALID_PARAMETER, "invalid parameter count=%d for size=%d (out of range)", count, size));
   if (!size || !count) return(TRUE);

   if (count < size) {
      MoveMemory((void*)&buffer[0], &buffer[count], (size-count)*sizeof(buffer[0]));
   }
   std::fill_n(&buffer[size-count], count, emptyValue);
   return(TRUE);
}


// explicit template instantiation to make definitions accessible to the linker
template BOOL ShiftIndicatorBuffer<bool>  (bool  [], int, int, bool);
template BOOL ShiftIndicatorBuffer<char>  (char  [], int, int, char);
template BOOL ShiftIndicatorBuffer<short> (short [], int, int, short);
template BOOL ShiftIndicatorBuffer<int>   (int   [], int, int, int);
template BOOL ShiftIndicatorBuffer<int64> (int64 [], int, int, int64);
template BOOL ShiftIndicatorBuffer<float> (float [], int, int, float);
template BOOL ShiftIndicatorBuffer<double>(double[], int, int, double);


/**
 * Shift the content of an <MQL4:bool> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  BOOL buffer[]   - timeseries array with sizeof(element) = 4
 * @param  int  size       - number of elements in the array
 * @param  int  count      - number of elements to shift
 * @param  BOOL emptyValue - initialization value for elements interpreted as "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftBOOLIndicatorBuffer(BOOL buffer[], int size, int count, BOOL emptyValue) {
   return(ShiftIndicatorBuffer(buffer, size, count, (int)(emptyValue!=0)));
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of an <MQL5:bool> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  bool buffer[]   - timeseries array with sizeof(element) = 1
 * @param  int  size       - number of elements in the array
 * @param  int  count      - number of elements to shift
 * @param  bool emptyValue - initialization value for elements interpreted as "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftBoolIndicatorBuffer(bool buffer[], int size, int count, bool emptyValue) {
   return(ShiftIndicatorBuffer(buffer, size, count, emptyValue));
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of a <char> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  char buffer[]   - timeseries array
 * @param  int  size       - number of elements in the array
 * @param  int  count      - number of elements to shift
 * @param  char emptyValue - initialization value for elements interpreted as "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftCharIndicatorBuffer(char buffer[], int size, int count, char emptyValue) {
   return(ShiftIndicatorBuffer(buffer, size, count, emptyValue));
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of a <short> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  short buffer[]   - timeseries array
 * @param  int   size       - number of elements in the array
 * @param  int   count      - number of elements to shift
 * @param  short emptyValue - initialization value for elements interpreted as "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftShortIndicatorBuffer(short buffer[], int size, int count, short emptyValue) {
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
 * @param  int emptyValue - initialization value for elements interpreted as "empty"
 *
 * @return BOOL - success status
 */
BOOL WINAPI ShiftIntIndicatorBuffer(int buffer[], int size, int count, int emptyValue) {
   return(ShiftIndicatorBuffer(buffer, size, count, emptyValue));
   #pragma EXPANDER_EXPORT
}


/**
 * Shift the content of an <MQL5:long> timeseries array (e.g. an indicator buffer) by the specified number of elements. Discards
 * elements at the beginning (the oldest values of a timeseries).
 *
 * @param  int64 buffer[]   - timeseries array with sizeof(element) = 8
 * @param  int   size       - number of elements in the array
 * @param  int   count      - number of elements to shift
 * @param  int64 emptyValue - initialization value for elements interpreted as "empty"
 *
 * @return BOOL - success status
 *
 * Note: The function name refers to the MQL5 type <long> (a 64 bit wide integer).
 */
BOOL WINAPI ShiftLongIndicatorBuffer(int64 buffer[], int size, int count, int64 emptyValue) {
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
