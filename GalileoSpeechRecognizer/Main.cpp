// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "arduino.h"
#include <process.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <iostream>
#include "complex.h"
#include "config.h"
#include "utils.h"
#include "featureExtraction.h"

using namespace std;

int input[S];

//long long milliseconds_now() {
//	static LARGE_INTEGER s_frequency;
//static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
//if (s_use_qpc) {
//LARGE_INTEGER now;
//QueryPerformanceCounter(&now);
//return (1000LL * now.QuadPart) / s_frequency.QuadPart;
//}
//else {
//	return GetTickCount();
//}
//}

/*void handleInput()//void *params)
{
	char *inputAmpStr = (char *)malloc(sizeof(char) * 15);
	complex *x = (complex *)malloc(sizeof(struct complex_t) * N);
	//long long start, stop;

	for (int i = S + 1; i < N; i++)
	{
		x[i].re = 0;
		x[i].im = 0;
	}

	while (true)
	{
		//start = milliseconds_now();
		for (int i = 0; i < S; i++)
		{
			x[i].re = analogRead(MICROPHONE_PIN);
			x[i].im = 0;
		}

		//stop = milliseconds_now();
		//long long res = stop - start;
		//std::string number;
		//std::stringstream strstream;
		//strstream << res;
		//strstream >> inputAmpStr;
		//cout << number << endl;
		complex *X = fft(x, N);

		for (int i = 0; i < N; i++)
		{
			Log(L"re = %lf, im = %ld\n", X[i].re, X[i].im);
		}

		delay(5000);
	}

	//_endthread();
}

void analyzeInput(void *params)
{

	while (true)
	{
		//WaitForSingleObject(hAmpMutex, INFINITE);

		//ReleaseMutex(hAmpMutex);
	}
}*/

int _tmain(int argc, _TCHAR* argv[])
{
	//pinMode(MICROPHONE_PIN, INPUT);

	short *samples = loadWaveFile("60Hz_7000Hz_1s.wav");

	if (samples == NULL)
	{
		Log("Error when loading samples\n");
		return -1;
	}

	double *features = extractFeatures(samples);

	if (features == NULL)
	{
		Log("Error: feature extraction failed\n");

		return -1;
	}

	samples = loadWaveFile("audiocheck.net_sin_1000Hz_-3dBFS_3s.wav");

	if (samples == NULL)
	{
		Log("Error when loading samples\n");
		return -1;
	}

	features = extractFeatures(samples);

	if (features == NULL)
	{
		Log("Error: feature extraction failed\n");

		return -1;
	}

	//Log(L"Launching handleInput thread..\n");
	//_beginthread(handleInput, 0, NULL);
	//handleInput();
	//Log(L"Done\n");

	//	Log(L"Launching analyzeInput thread..\n");
	//	_beginthread(analyzeInput, 0, NULL);
	Log(L"Done\n");
}