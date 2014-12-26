#include "complex.h"

complex fromPolar(double radius, double argument)
{
	complex c;
	c.re = radius * cos(argument);
	c.im = radius * sin(argument);

	return c;
}

double magnitude(complex c)
{
	return sqrt(c.re * c.re + c.im * c.im);
}

complex add(complex a, complex b)
{
	complex c;
	c.re = a.re + b.re;
	c.im = a.im + b.im;

	return c;
}

complex substract(complex a, complex b)
{
	complex c;
	c.re = a.re - b.re;
	c.im = a.im - b.im;

	return c;
}

complex multiply(complex a, complex b)
{
	complex c;
	c.re = a.re * b.re - a.im * b.im;
	c.im = a.im * b.re + a.re * b.im;

	return c;
}