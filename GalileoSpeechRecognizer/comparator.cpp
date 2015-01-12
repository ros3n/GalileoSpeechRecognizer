#include "comparator.h"

double dtw(double **sample, int sample_len, double **pattern, int pattern_len)
{
	double **DTW = (double **)malloc(sizeof(double *) * (pattern_len + 1));
	for (int i = 0; i <= pattern_len; i++)
	{
		DTW[i] = (double *)malloc(sizeof(double) * (sample_len + 1));
	}

	int w = 0;
	w = (int)fmax((double)w, (double)abs(pattern_len - sample_len)); // adapt window size (*)

	for (int i = 0; i <= pattern_len; i++)
	{
		for (int j = 0; j <= sample_len; j++)
		{
			DTW[i][j] = 99999.0;
		}
	}

	DTW[0][0] = 0.0;

	for (int i = 1; i <= pattern_len; i++)
	{
		for (int j = (int)fmax(1.0, (double)(i - w)); j <= (int)fmin((double)sample_len, (double)(i + w)); j++)
		{
			double cost = distance(pattern[i - 1], sample[j - 1]);
			//Log("%d %d\n\n", i, j);
			DTW[i][j] = cost + fmin(fmin((double)DTW[i - 1][j], (double)DTW[i][j - 1]), (double)DTW[i - 1][j - 1]);
			//Log("%d %d %lf\n\n", i, j, DTW[i][j]);
		}
	}

	double result = DTW[pattern_len][sample_len];
	free(DTW);
	return result;
}

double distance(double *a, double *b)
{
	double sum = 0.0;
	for (int i = 0; i < 13; i++)
	{
		sum += pow(a[i] - b[i], 2);
	}

	return sqrt(sum);
}