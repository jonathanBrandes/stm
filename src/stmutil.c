#include "stmutil.h"
#include <stdio.h>
#include <string.h>

//#define DEBUG
#define WAVE_FORMAT_PCM 0x0001

/**
 * steroToMono
 * input params: 	data - buffer that contains the stero L&R channel samples
 * 					wavFileHeader - pointer to the WAVE header structure
 * 					dataChunkHeader - pointer to the data chunk header structure
 *
 * Converts 2 channel stero samples to a single mono sample by reducing volume of each
 * channel by 50% and adding them together. The function also updates the wav and chunk
 * headers with the new channel count, wave chunk size and data chunk size.
 */
void steroToMono(uint8_t *data, WAV_FILE_HEADER *wavFileHeader, DATA_CHUNK_HEADER *dataChunkHeader) {

	int dataIndex = 0;

	/* sample->channel[0] = Left Speaker sample->channel[1] = Right Speaker */
	if (wavFileHeader->nBitsPerSample == 8) {

		EIGHT_BIT_STERO *sampleEnd = (EIGHT_BIT_STERO*)&data[dataChunkHeader->dLen];

		for (EIGHT_BIT_STERO *sample = (EIGHT_BIT_STERO*)data; sample < sampleEnd; ++sample) {


			data[dataIndex++] = sample->channel[0] * .5 + sample->channel[1] * .5;
		}
	} else {

		int16_t mono;

		SIXTEEN_BIT_STERO *sampleEnd = (SIXTEEN_BIT_STERO*)&data[dataChunkHeader->dLen];

		for (SIXTEEN_BIT_STERO *sample= (SIXTEEN_BIT_STERO*)data; sample < sampleEnd; ++sample) {

			mono = sample->channel[0] * .5 + sample->channel[1] * .5 ;

			/* Store in little endian */
			data[dataIndex*2] = mono;
			data[dataIndex++*2+1] = mono >> 8;
		}
	}

	/* Update header information */
	wavFileHeader->nChannels = 1;	/*Single channel for mono */
	wavFileHeader->waveCkSize = sizeof(WAV_FILE_HEADER) + sizeof(DATA_CHUNK_HEADER) + dataChunkHeader->dLen/2;

	/* Update data header to decrease size for mono */
	dataChunkHeader->dLen = dataChunkHeader->dLen/2;

}

/**
 * readWavHeaders
 * input params: 	file - wav file to read the wav and data headers into structures
 * 					wavFileHeader - pointer to the WAVE header structure
 * 					dataChunkHeader - pointer to the data chunk header structure
 *
 * Reads the WAVE file into the passed structure pointers and checks the header information to make sure
 * the tool supports the wav file type passed and there are not errors in the headers.
 */
int readWavHeaders(FILE *file, WAV_FILE_HEADER *wavFileHeader, DATA_CHUNK_HEADER *dataChunkHeader) {

	if (fread((void*)wavFileHeader, sizeof(WAV_FILE_HEADER), (size_t)1, file) != 1) {
			printf("Error reading INFILE wav header\n");
			return 1;
	}

#ifdef DEBUG
	printWavHeader(wavFileHeader);
#endif

		/* Check wav file Chunk ID header value */
		if(strncmp(wavFileHeader->waveCkId, "RIFF", sizeof(wavFileHeader->waveCkId)) != 0) {
			printf("Invalid wav file Chunk ID\n");
			return 1;
		}

		/* Check WAVE ID header value */
		if(strncmp(wavFileHeader->waveId, "WAVE", sizeof(wavFileHeader->waveId)) != 0) {
			printf("Invalid wav Format ID\n");
			return 1;
		}

		/* Check fmt Chunk ID header value */
		if(strncmp(wavFileHeader->fmtCkId, "fmt ", sizeof(wavFileHeader->fmtCkId)) != 0) {
			printf("Invalid fmt Chunk ID\n");
			return 1;
		}

		/* Check WAVE format to make sure it is PCM */
		if(wavFileHeader->wFormatTag != WAVE_FORMAT_PCM) {
			printf("wav format is not PCM\n");
			return 1;
		}

		/* Check number of bits per samples */
		if(wavFileHeader->nBitsPerSample != 8 && wavFileHeader->nBitsPerSample != 16) {
			printf("Invalid number of samples per second in wav file\n");
			return 1;
		}

		/* Check that file is stero */
		if(wavFileHeader->nChannels != 2) {
			printf("wav file is not stero\n");
			return 1;
		}

		/* Read data Chunk Header */
		if (fread((void*)dataChunkHeader, sizeof(DATA_CHUNK_HEADER), (size_t)1, file) != 1) {
			printf("Error reading INFILE data header\n");
			return 1;
		}

#ifdef DEBUG
	printDataHeader(dataChunkHeader);
#endif


		/* Check for correct chunk ID, only need to account for data chunks as that is all we handle */
		if(strncmp(dataChunkHeader->dataCkId, "data", sizeof(dataChunkHeader->dataCkId)) != 0) {
			printf("Invalid data chunk header\n");
			return 1;
		}

	return 0;
}

/**
 * printWavHeaders
 * input params: 	wavFileHeader - pointer to the WAVE header structure
 *
 * Prints all fields of the wav file header.
 */
void printWavHeader( WAV_FILE_HEADER* wavFileHeader ){

	fprintf(stderr, "Wav Chunk ID : %s\n", wavFileHeader->waveCkId);
	fprintf(stderr, "\nWav Chunk Size : %d", wavFileHeader->waveCkSize);
	fprintf(stderr, "\nWav ID: %s", wavFileHeader->waveId);
	fprintf(stderr, "\nFormat Chunk ID %s: ", wavFileHeader->fmtCkId);
	fprintf(stderr, "\nFormat Chunk Size: %d", wavFileHeader->fmtCkSize);
	fprintf(stderr, "\nFile Format: %d", wavFileHeader->wFormatTag);
	fprintf(stderr, "\nNumber of Channels: %d", wavFileHeader->nChannels);
	fprintf(stderr, "\nSample Rate: %d", wavFileHeader->nSamplesPerSec);
	fprintf(stderr, "\nByte Rate: %d", wavFileHeader->nAvgBytesPerSec);
	fprintf(stderr, "\nBlock Align: %d", wavFileHeader->nBlockAlingn);
	fprintf(stderr, "\nBits Per Sample: %d\n\n", wavFileHeader->nBitsPerSample);
}

/**
 * printWavHeaders
 * input params: 	wavFileHeader - pointer to the WAVE header structure
 *
 * Prints all fields of the data chunk header.
 */
void printDataHeader( DATA_CHUNK_HEADER * dataChunkHeader) {

	fprintf(stderr, "Data Chunk ID: %s", dataChunkHeader->dataCkId);
	fprintf(stderr, "\nData Len: %d\n\n", dataChunkHeader->dLen);
}
