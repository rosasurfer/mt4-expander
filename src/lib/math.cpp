#include "expander.h"
#include "lib/math.h"

#include <math.h>
#include <cmath>


/**
 * IEE754 can represent integers exactly as floats, up to about 2^24 (16,777,216).
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
   #pragma warning(disable:4244)
   return(dValue);                                       // C4244: conversion from 'double' to 'float', possible loss of data
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
