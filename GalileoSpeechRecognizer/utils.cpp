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

BYTE * loadWaveFile(char *fname)
{
	FILE *fp;

	if (fopen_s(&fp, fname, "rb") == 0)
	{
		BYTE id[5], *sound_buffer; // four bytes to hold 'RIFF' 
		DWORD size; // 32 bit value to hold file size 
		short format_tag, channels, block_align, bits_per_sample; // 16 bit values 
		DWORD format_length, sample_rate, avg_bytes_sec, data_size, i; // 32 bit values 

		fread_s(id, sizeof(id), sizeof(BYTE), 4, fp); //read in first four bytes 
		id[4] = NULL;
		Log("%s\n", id);
		if (!strcmp((const char *)id, "RIFF"))
		{
			fread_s(&size, sizeof(DWORD), sizeof(DWORD), 1, fp); // read in 32bit size value
			Log("%ld\n", size);
			fread_s(id, sizeof(id), sizeof(BYTE), 4, fp); // read in 4 byte string now 
			Log("%s\n", id);
			if (!strcmp((const char *)id, "WAVE"))
			{
				fread_s(id, sizeof(id), sizeof(BYTE), 4, fp); // read in 4 bytes "fmt "; 
				Log("%s\n", id);
				fread_s(&format_length, sizeof(format_length), sizeof(DWORD), 1, fp);
				Log("format_length: %ld\n", format_length);
				fread_s(&format_tag, sizeof(format_tag), sizeof(short), 1, fp);
				Log("format_tag:%d\n", format_tag);
				fread_s(&channels, sizeof(channels), sizeof(short), 1, fp); // 1 mono, 2 stereo 
				Log("channels: %d\n", channels);
				fread_s(&sample_rate, sizeof(sample_rate), sizeof(DWORD), 1, fp);
				Log("sample_rate: %ld\n", sample_rate);
				fread_s(&avg_bytes_sec, sizeof(avg_bytes_sec), sizeof(DWORD), 1, fp);
				Log("avg_bytes_ses: %d\n", avg_bytes_sec);
				fread_s(&block_align, sizeof(block_align), sizeof(short), 1, fp);
				Log("block_align: %d\n", block_align);
				fread_s(&bits_per_sample, sizeof(bits_per_sample), sizeof(short), 1, fp);
				Log("bits_per_sample: %d\n", bits_per_sample);
				fread_s(id, sizeof(id), sizeof(BYTE), 4, fp); // read in 'data' 
				Log("%s\n", id);
				if (!strcmp((const char *)id, "data"))
				{
					fread_s(&data_size, sizeof(data_size), sizeof(DWORD), 1, fp); // how many bytes of sound data we have 
					Log("data_size: %ld\n", data_size);
					sound_buffer = (BYTE *)malloc(sizeof(BYTE) * data_size);
					if (sound_buffer != NULL)
					{
						fread_s(sound_buffer, sizeof(BYTE) * data_size, sizeof(BYTE), data_size, fp); // read in whole sound data chunk 

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