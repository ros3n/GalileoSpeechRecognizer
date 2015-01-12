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
#include "comparator.h"

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

	short *samples = loadWaveFile("disable.wav");

	if (samples == NULL)
	{
		Log("Error when loading samples\n");
		return -1;
	}

	/*double *features = extractFeatures(samples);

	if (features == NULL)
	{
		Log("Error: feature extraction failed\n");

		return -1;
	}*/

	double **input = (double **)malloc(sizeof(double *) * 185);

	for (int i = 0; i < 185; i++)
	{
		double *features = extractFeatures(samples + i * 160);
		input[i] = (double *)malloc(sizeof(double)* 13);
		for (int j = 0; j < 13; j++)
			input[i][j] = features[j];
		free(features);
	}

	samples = loadWaveFile("60Hz_7000Hz_3s.wav");

	if (samples == NULL)
	{
		Log("Error when loading samples\n");
		return -1;
	}

	double **input2 = (double **)malloc(sizeof(double *) * 595);

	for (int i = 0; i < 595; i++)
	{
		double *features = extractFeatures(samples + i * 160);
		input2[i] = (double *)malloc(sizeof(double) * 13);
		for (int j = 0; j < 13; j++)
			input2[i][j] = features[j];
		free(features);
	}
	Log("dtw\n");
	double dist = dtw(input, 185, input2, 595);

	Log("%lf\n", dist);

	/*features = extractFeatures(samples);

	if (features == NULL)
	{
		Log("Error: feature extraction failed\n");

		return -1;
	}*/

	//Log(L"Launching handleInput thread..\n");
	//_beginthread(handleInput, 0, NULL);
	//handleInput();
	//Log(L"Done\n");

	//	Log(L"Launching analyzeInput thread..\n");
	//	_beginthread(analyzeInput, 0, NULL);
	Log(L"Done\n");
}