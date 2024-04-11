#include "expander.h"
#include "lib/math.h"

#include <cmath>


/**
 * Return the exponent part of a double value.
 *
 * @param  double value
 *
 * @return int - exponent
 */
int WINAPI DoubleExp(double value) {
   double absValue = abs(value);
   if (!absValue) return(0);
   return((int) floor(log10(absValue)));
   #pragma EXPANDER_EXPORT

   // @see  https://stackoverflow.com/questions/29582919/find-exponent-and-mantissa-of-a-double-to-the-base-10
   // @see  https://social.msdn.microsoft.com/Forums/vstudio/en-US/a5cca71e-75d0-4c3f-88af-3bf1899d6556/double-get-mantissa-and-exponent-in-base-10-representation
}


/**
 * Whether a double holds an infinite value.
 *
 * Exported version of function isInfinite(), for API completeness.
 *
 * @param  double value
 *
 * @return BOOL
 */
BOOL WINAPI IsInfinite(double value) {
   return(value && value+value == value);
   #pragma EXPANDER_EXPORT
}


/**
 * Whether a double holds the value "not-a-number".
 *
 * Exported version of function isNaN(), as in terminals (build < 416) the comparison NaN==NaN returns TRUE/is broken.
 *
 * @param  double value
 *
 * @return BOOL
 */
BOOL WINAPI IsNaN(double value) {
   return(value != value);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a double value representing "positive infinity".
 *
 * Exported version of const INF, as in terminals (build > 509 && build < 603) MathLog(0) fails to produce it.
 *
 * @return double
 */
double WINAPI Math_INF() {
   return(INF);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a double value representing "not-a-number".
 *
 * Exported version of const NaN, for API completeness.
 *
 * @return double
 */
double WINAPI Math_NaN() {
   return(NaN);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the base-10 logarithm of a double value.
 *
 * @param  double value
 *
 * @return double - base-10 logarithm
 */
double WINAPI MathLog10(double value) {
   return(log10(value));
   #pragma EXPANDER_EXPORT
}


/**
 * IEE754 can represent integers exactly as floats up to about 2^24 (16'777'216).
 * IEE754 can represent all integers exactly as doubles.
 *
 *
 *
 * (1) round() for float in C++
 *     links to (2) und (3)
 *     @see  http://stackoverflow.com/questions/485525/round-for-float-in-c
 *
 *
 * (2) Harder than it looks: rounding float to nearest integer, part 1-3
 *     @see  http://blog.frama-c.com/index.php?post/2013/05/02/nearbyintf1
 *
 *
 * (3) A Concise way to implement round() in C
 *     links to Newlib
 *     @see  http://stackoverflow.com/questions/4572556/concise-way-to-implement-round-in-c/4572877#4572877
 */


/**
 * Round a float to the given precision.
 *
 * @param  float value
 * @param  int   digits - position to round at (default: 0)
 *
 * @return float - rounded value
 *
 * @example
 * <pre>
 *  round(1234.56789,  3) => 1234.568
 *  round(1234.56789,  0) => 1235
 *  round(1234.56789, -2) => 1200
 * </pre>
 */
float WINAPI round(float value, int digits/*=0*/) {
   double dValue=value, factor=1;

   if (digits) {
      factor = pow(10., digits);
      dValue *= factor;
   }
   dValue = round(dValue);
   if (digits) dValue /= factor;
   if (!value) value = 0;                                // convert -0 to +0

   #pragma warning(push)
   #pragma warning(disable:4244)                         // C4244: conversion from 'double' to 'float', possible loss of data
   return(dValue);
   #pragma warning(pop)
}


/**
 * Round a double to the given precision.
 *
 * @param  double value
 * @param  int    digits - position to round at (default: 0)
 *
 * @return double - rounded value
 *
 * @example
 * <pre>
 *  round(1234.56789,  3) => 1234.568
 *  round(1234.56789,  0) => 1235
 *  round(1234.56789, -2) => 1200
 * </pre>
 */
double WINAPI round(double value, int digits/*=0*/) {
   double factor = 1;

   if (digits) {
      factor = pow(10., digits);
      value *= factor;
   }
   if (value < 0) value = ceil (value-0.5);              // Adding/subtracting 0.5 fails to round to the nearest
   else           value = floor(value+0.5);              // integer for several inputs including 0.49999999999999994.

   if (digits) value /= factor;
   if (!value) value = 0;                                // convert -0 to +0

   return(value);
}
