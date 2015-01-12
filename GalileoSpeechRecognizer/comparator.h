#ifndef _COMPARATOR_H_
#define _COMPARATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stdafx.h"
#include "arduino.h"

double dtw(double **sample, int sample_len, double **pattern, int pattern_len);
double distance(double *a, double *b);

#endif
