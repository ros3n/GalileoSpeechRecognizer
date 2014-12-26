#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#define _USE_MATH_DEFINES
#include "math.h"

typedef struct complex_t
{
	double re;
	double im;
} complex;

complex fromPolar(double radius, double argument);
double magnitude(complex a);
complex add(complex a, complex b);
complex substract(complex a, complex b);
complex multiply(complex a, complex b);
#endif
