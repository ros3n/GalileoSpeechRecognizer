#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
#include "arduino.h"

typedef unsigned char BYTE, *PBYTE, *LPBYTE;
typedef unsigned long DWORD, *PDWORD, *LPDWORD;

int firstPowerOfTwo(int n);
BYTE * loadWaveFile(char *fname);
double hammingWindow(int n, int N);

#endif
