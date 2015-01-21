#include "utils.h"

int firstPowerOfTwo(int n)
{
	int k = 2;

	while (k <= n)
	{
		k *= 2;
	}

	return k;
}

short * loadWaveFile(char *fname, int *ds)
{
	FILE *fp;

	if (fopen_s(&fp, fname, "rb") == 0)
	{
		BYTE id[5]; // four bytes to hold 'RIFF'
		short *sound_buffer;
		DWORD size; // 32 bit value to hold file size 
		short format_tag, channels, block_align, bits_per_sample; // 16 bit values 
		DWORD format_length, sample_rate, avg_bytes_sec, data_size; // 32 bit values 

		fread_s(id, sizeof(id), sizeof(BYTE), 4, fp); //read in first four bytes 
		id[4] = NULL;
		//Log("%s\n", id);
		if (!strcmp((const char *)id, "RIFF"))
		{
			fread_s(&size, sizeof(DWORD), sizeof(DWORD), 1, fp); // read in 32bit size value
			//Log("%ld\n", size);
			fread_s(id, sizeof(id), sizeof(BYTE), 4, fp); // read in 4 byte string now 
			//Log("%s\n", id);
			if (!strcmp((const char *)id, "WAVE"))
			{
				fread_s(id, sizeof(id), sizeof(BYTE), 4, fp); // read in 4 bytes "fmt "; 
				//Log("%s\n", id);
				fread_s(&format_length, sizeof(format_length), sizeof(DWORD), 1, fp);
				//Log("format_length: %ld\n", format_length);
				fread_s(&format_tag, sizeof(format_tag), sizeof(short), 1, fp);
				//Log("format_tag:%d\n", format_tag);
				fread_s(&channels, sizeof(channels), sizeof(short), 1, fp); // 1 mono, 2 stereo 
				//Log("channels: %d\n", channels);
				fread_s(&sample_rate, sizeof(sample_rate), sizeof(DWORD), 1, fp);
				//Log("sample_rate: %ld\n", sample_rate);
				fread_s(&avg_bytes_sec, sizeof(avg_bytes_sec), sizeof(DWORD), 1, fp);
				//Log("avg_bytes_ses: %d\n", avg_bytes_sec);
				fread_s(&block_align, sizeof(block_align), sizeof(short), 1, fp);
				//Log("block_align: %d\n", block_align);
				fread_s(&bits_per_sample, sizeof(bits_per_sample), sizeof(short), 1, fp);
				//Log("bits_per_sample: %d\n", bits_per_sample);
				fread_s(id, sizeof(id), sizeof(BYTE), 4, fp); // read in 'data' 
				//Log("%s\n", id);
				if (!strcmp((const char *)id, "data"))
				{
					fread_s(&data_size, sizeof(data_size), sizeof(DWORD), 1, fp); // how many bytes of sound data we have 
					Log("data_size: %ld\n", data_size);
					*(ds) = data_size;
					sound_buffer = (short *)malloc(sizeof(short) * data_size);
					if (sound_buffer != NULL)
					{
						fread_s(sound_buffer, sizeof(short) * data_size, sizeof(short), data_size, fp); // read in whole sound data chunk 

						return sound_buffer;
					}
					else
					{
						Log("Error: sound_buffer allocation failed\n");
					}
				}
				else
				{
					Log("Error: no data header found\n");
				}
			}
			else
			{
				Log("Error: RIFF file but not a wave file\n");
			}
		}
		else
		{
			Log("Error: not a RIFF file\n");
		}
	}

	return NULL;
}

double hammingWindow(int n, int N)
{
	double a = 0.54;

	return a - (1.0 - a) * cos((2 * M_PI * n) / (N - 1));
}

int serializeCoefficients(double **mfccs, int mfccsCount, char *fname)
{
	FILE *fp;

	if (fopen_s(&fp, fname, "w") == 0)
	{
		fprintf(fp, "%d\n", mfccsCount);
		for (int i = 0; i < mfccsCount; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				fprintf_s(fp, "%lf ", mfccs[i][j]);
			}
			fprintf_s(fp, "\n");
		}

		fclose(fp);

		return 1;
	}

	Log("Error: creating file failed\n");
	return -1;
}

double ** deserializeCoefficients(char *fname, int *mfccsCount)
{
	FILE *fp;

	if (fopen_s(&fp, fname, "rb") == 0)
	{
		double **mfccs;
		double t;
		char c;

		fscanf_s(fp, "%d\n", mfccsCount);

		mfccs = (double **)malloc(sizeof(double *) * *(mfccsCount));
		for (int i = 0; i < *(mfccsCount); i++)
		{
			mfccs[i] = (double *)malloc(sizeof(double) * 13);
			for (int j = 0; j < 13; j++)
			{
				fscanf_s(fp, "%lf ", &t);
				mfccs[i][j] = t;
			}
			fscanf_s(fp, "\n", &c);
		}

		fclose(fp);

		return mfccs;	
	}

	return NULL;
}

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

int handleInput(short *input)
{
	LARGE_INTEGER now1;
	LARGE_INTEGER now2;
	QueryPerformanceCounter(&now1);
	bool recording = false;
	int sample;
	int i = 0;
	int j = 0;
	while (true)
	{
		QueryPerformanceCounter(&now2);
		if (now2.QuadPart - now1.QuadPart >= 895)
		{
			now1 = now2;
			sample = analogRead(MICROPHONE_PIN);

			if (sample > 560)
			{
				if (!recording)
				{
					recording = true;
					Log("start\n");
				}
			}

			if (recording)
			{
				input[i++] = (short)sample;
				if (i > 25000 && sample < 520)
				{
					return i;
				}
			}
			/*j++;
			if (j > 16000) {
			Log("bump\n");
			j = 0;
			}//*/
		}
	}
}

/*	short *samples = loadWaveFile("disable.wav");

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

/*	double **input = (double **)malloc(sizeof(double *) * 98);

for (int i = 0; i < 98; i++)
{
double *features = extractFeatures(samples + i * 160);
input[i] = (double *)malloc(sizeof(double)* 13);
for (int j = 0; j < 13; j++)
input[i][j] = features[j];
free(features);
}

serializeCoefficients(input, 98, "disable.mfcc");

samples = loadWaveFile("enable.wav");

if (samples == NULL)
{
Log("Error when loading samples\n");
return -1;
}

int i2 = 88;

double **input2 = (double **)malloc(sizeof(double *) * i2);

for (int i = 0; i < i2; i++)
{
double *features = extractFeatures(samples + i * 160);
input2[i] = (double *)malloc(sizeof(double) * 13);
for (int j = 0; j < 13; j++)
input2[i][j] = features[j];
free(features);
}


serializeCoefficients(input2, i2, "enable.mfcc");

Log("dtw\n");
double dist = dtw(input, 98, input2, i2);

Log("%lf\n", dist);

/*features = extractFeatures(samples);

if (features == NULL)
{
Log("Error: feature extraction failed\n");

return -1;
}*/


//enable = deserializeCoefficients("enable.mfcc", &c1);
//disable = deserializeCoefficients("disable.mfcc", &c2);

/*while (true)
{
int samplesCount = handleInput(input);
int frames = (samplesCount - FRAME_LENGTH) / 160;

short *denoised = (short*)malloc(sizeof(short)* samplesCount);

for (int i = 1; i < samplesCount - 3; i++)
{
denoised[i] = (input[i - 1] + input[i] + input[i + 1] + input[i + 2]) / 4;
}

//for (int i = 1; i < samplesCount - 3; i++)
//Log("%d\n", denoised[i]);

double **samples = (double **)malloc(sizeof(double *)* frames);
Log("samples = %d\n", samplesCount);

for (int i = 0; i < frames; i++)
{
double *features = extractFeatures(denoised + i * 160);
samples[i] = (double *)malloc(sizeof(double)* 13);
for (int j = 0; j < 13; j++)
{
samples[i][j] = features[j];
}
free(features);
}
free(denoised);

Log("dtw enable\n");
double dist = dtw(enable, c1, input, nos);
Log("%lf\n", dist);

Log("dtw disble\n");
dist = dtw(disable, c2, input, nos);
Log("%lf\n", dist);
free(samples);
//}*/

/*
int c1 = 0, c2 = 0;
double **enable, **disable;
*/