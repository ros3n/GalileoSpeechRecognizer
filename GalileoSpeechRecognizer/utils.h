#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
#include "arduino.h"
#include "config.h"

//typedef char BYTE, *PBYTE, *LPBYTE;
//typedef long DWORD, *PDWORD, *LPDWORD;

int firstPowerOfTwo(int n);
short * loadWaveFile(char *fname, int *nos);
double hammingWindow(int n, int N);
int serializeCoefficients(double **mfccs, int mfccsCount, char *fname);
double ** deserializeCoefficients(char *fname, int *mfccsCount);

#endif
