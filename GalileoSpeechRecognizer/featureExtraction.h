#ifndef _FEATUREEXTRACTION_H_
#define _FEATUREEXTRACTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
#include "arduino.h"
#include "config.h"
#include "complex.h"
#include "utils.h"
#include "fft.h"

double * extractFeatures(short *samples);

#endif
