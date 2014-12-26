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
#include "fft.h"
#include "utils.h"

using namespace std;

const int MICROPHONE_PIN = 0;
const int SAMPLING_FREQ = 16000; // Hz
const double FRAME_LENGTH = 0.025;
const int S = 400; //number of samples ( SAMPLING_FREQ * FRAME_LENGTH ) VS has complained about not constant expression
int input[S];
int N = firstPowerOfTwo(S);

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

void handleInput()//void *params)
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
}

int _tmain(int argc, _TCHAR* argv[])
{
	complex a = fromPolar(1, 1.31);
	complex b;
	b.re = 3;
	b.im = 6;
	add(a, b);

	Log(L"re = %lf, im = %lf\n", a.re, a.im);

	//pinMode(MICROPHONE_PIN, INPUT);

	BYTE *samples = loadWaveFile("wavTones.com.unregistred.sin_1000Hz_-6dBFS_3s.wav");

	if (samples == NULL)
	{
		Log("Error when loading samples\n");
		return -1;
	}

	complex *x = (complex *)malloc(sizeof(complex_t) * N);

	for (int i = 0; i < S; i++)
	{
		x[i].re = 100 * sin(i * 2 * M_PI / 160) + 100;// (double)samples[i];
		x[i].im = 0;
	}

	for (int i = S; i < N; i++)
	{
		x[i].re = 0;
		x[i].im = 0;
	}
	/*
	for (int i = 0; i < N; i++)
	{
		//Log(L"i = %d, re = %lf, im = %lf\n", i, x[i].re, x[i].im);
		Log("%lf\n", x[i].re);
	}

	Log("\n***************************\n\n");
	*/
	complex *X = fft(x, N);

	double m, res = 0;
	int j;

	for (int i = 1; i < N/2; i++)
	{
		m = magnitude(X[i]);
		Log(L"f = %d, mag = %lf\n", i * SAMPLING_FREQ / N, m);
		
		if (m > res)
		{
			Log(L"f = %d, mag = %lf\n", i * SAMPLING_FREQ / N, m);
			res = m;
			j = i;
		}
	}

	Log(L"f = %d, mag = %lf\n", j * SAMPLING_FREQ / N, res);

	//Log(L"Launching handleInput thread..\n");
	//_beginthread(handleInput, 0, NULL);
	//handleInput();
	//Log(L"Done\n");

	//	Log(L"Launching analyzeInput thread..\n");
	//	_beginthread(analyzeInput, 0, NULL);
	//	Log(L"Done\n");	
}