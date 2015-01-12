#include "featureExtraction.h"

double * extractFeatures(short *samples)
{
	int N = firstPowerOfTwo(S);
	double *P;
	complex *x;
	complex *X;
	double *fftBins;
	double *filterBankEnergies;
	double *DCT;

	x = (complex *)malloc(sizeof(complex_t) * N);

	if (x == NULL)
	{
		Log("Error: memory allocation failed\n");
		
		return NULL;
	}

	for (int i = 0; i < S; i++)
	{
		x[i].re = (double)samples[i] * hammingWindow(i, N);
		x[i].im = 0;
	}

	for (int i = S; i < N; i++)
	{
		x[i].re = 0;
		x[i].im = 0;
	}
	
	X = fft(x, N);

	P = (double *)malloc(sizeof(double) * (N / 2 + 1));

	if (P == NULL)
	{
		Log("Error: memory allocation failed\n");

		return NULL;
	}

	for (int i = 0; i <= N / 2; i++)
	{
		P[i] = (1.0 / (double)N) * pow(magnitude(X[i]), 2);
	}

	double melLowerBound = 1125.0 * log(1.0 + ((double)MEL_LOWER_BOUND_HZ / 700.0));
	double melUpperBound = 1125.0 * log(1.0 + ((double)MEL_UPPER_BOUND_HZ / 700.0));

	fftBins = (double *)malloc(sizeof(double) * (FILTERBANKS + 2));
	
	if (fftBins == NULL)
	{
		Log("Error: memory allocation failed\n");

		return NULL;
	}
	
	double diff = (melUpperBound - melLowerBound) / ((double)FILTERBANKS + 1);
	double filter;

	for (int i = 0; i < FILTERBANKS + 2; i++)
	{
		filter = 700.0 * (exp((melLowerBound + (double)i * diff) / 1125.0) - 1.0);
		fftBins[i] = floor(filter * (N + 1) / SAMPLING_FREQ);
	}

	filterBankEnergies = (double *)malloc(sizeof(double) * FILTERBANKS);

	if (filterBankEnergies == NULL)
	{
		Log("Error: memory allocation failed\n");

		return NULL;
	}

	for (int m = 1; m <= FILTERBANKS; m++)
	{
		double result = 0;
		for (int k = 0; k <= N / 2; k++)
		{
			if (k >= fftBins[m - 1] && k <= fftBins[m])
			{
				result += P[k] * ((k - fftBins[m - 1]) / (fftBins[m] - fftBins[m - 1]));
			}
			else if (k >= fftBins[m] && k <= fftBins[m + 1])
			{
				result += P[k] * ((fftBins[m + 1] - k) / (fftBins[m + 1] - fftBins[m]));
			}
		}
		filterBankEnergies[m - 1] = log(result);
	}

	DCT = (double *)malloc(sizeof(double) * FILTERBANKS);

	if (DCT == NULL)
	{
		Log("Error: memory allocation failed\n");

		return NULL;
	}

	DCT[0] = 0.0;
	double N1 = (1 / sqrt((double)FILTERBANKS));
	for (int m = 0; m < FILTERBANKS; m++)
	{
		DCT[0] += N1 * filterBankEnergies[m];
	}

	N1 = sqrt(2.0 / (double)FILTERBANKS);

	for (int k = 1; k < FILTERBANKS; k++)
	{
		DCT[k] = 0.0;
		for (int m = 0; m < FILTERBANKS; m++)
		{
			DCT[k] += N1 * filterBankEnergies[m] * cos((M_PI * (double)k * (2.0 * (double)m + 1)) / (2.0 * (double)FILTERBANKS));
		}
	}

	for (int k = 0; k < FILTERBANKS; k++)
	{
		Log("%lf\n", DCT[k]);
	}

	free(P);
	free(x);
	free(X);
	free(fftBins);
	free(filterBankEnergies);
	return DCT;
}