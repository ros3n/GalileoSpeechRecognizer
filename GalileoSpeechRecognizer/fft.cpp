#include "fft.h"

complex * fft(complex *x, int N)
{
	complex *X = (complex *)malloc(sizeof(struct complex_t) * N);
	complex *d, *e, *D, *E;
	int k;

	if (N == 1)
	{
		X[0] = x[0];

		return X;
	}

	d = (complex *)malloc(sizeof(struct complex_t) * N / 2);
	e = (complex *)malloc(sizeof(struct complex_t) * N / 2);

	for (k = 0; k < N / 2; k++)
	{
		d[k] = x[2 * k];
		e[k] = x[2 * k + 1];
	}

	D = fft(d, N / 2);
	E = fft(e, N / 2);

	for (k = 0; k < N / 2; k++)
	{
		X[k] = add(E[k], multiply(D[k], fromPolar(1, -2.0 * M_PI * k / N)));
		X[k + N / 2] = substract(E[k], multiply(D[k], fromPolar(1, -2.0 * M_PI * k / N)));
	}

	free(D);
	free(E);

	return X;
}
