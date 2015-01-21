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

int _tmain(int argc, _TCHAR* argv[])
{
	pinMode(MICROPHONE_PIN, INPUT);

	int nos = 0;

	short *samples = loadWaveFile("data/agh2.wav", &nos);

	nos = (int)((nos - FRAME_LENGTH) / 160);

	if (samples == NULL)
	{
		Log("Error when loading samples\n");
		return -1;
	}

	double **input = (double **)malloc(sizeof(double *) * nos);

	for (int i = 0; i < nos; i++)
	{
		double *features = extractFeatures(samples + i * 160);
		if (features == NULL)
		{
			Log("Error: feature extraction failed\n");

			return -1;
		}
		input[i] = (double *)malloc(sizeof(double)* 13);
		for (int j = 0; j < 13; j++)
		input[i][j] = features[j];
		free(features);
	}

	serializeCoefficients(input, nos, "data/out.mfcc");

	char * testSuite[] = { "data/proba.mfcc", "data/proba_seba.mfcc", "data/test.mfcc", "data/test_seba.mfcc", "data/agh.mfcc", "data/agh_seba.mfcc", "data/kubek.mfcc", "data/kubek_seba.mfcc" };
	int p = 0;

	for (int i = 0; i < 8; i++)
	{
		double **pattern = deserializeCoefficients(testSuite[i], &p);

		Log("dtw %s\n", testSuite[i]);
		double dist = dtw(input, nos, pattern, p);
		Log("distance: %lf\n\n", dist);
	}
}

/*
int i2 = 0;

samples = loadWaveFile("data/agh.wav", &i2);

i2 = (int)((i2 - FRAME_LENGTH) / 160);

if (samples == NULL)
{
	Log("Error when loading samples\n");
	return -1;
}

double **input2 = (double **)malloc(sizeof(double *)* i2);

for (int i = 0; i < i2; i++)
{
	double *features = extractFeatures(samples + i * 160);
	if (features == NULL)
	{
		Log("Error: feature extraction failed\n");

		return -1;
	}
	input2[i] = (double *)malloc(sizeof(double)* 13);
	for (int j = 0; j < 13; j++)
		input2[i][j] = features[j];
	free(features);
}


serializeCoefficients(input2, i2, "data/agh.mfcc");
*/