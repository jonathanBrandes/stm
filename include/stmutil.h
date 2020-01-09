#ifndef STMUTIL_H_
#define STMUTIL_H_

#include <stdio.h>
#include <stdint.h>
/* Define the wav file header struct for simple processing */
typedef struct{
    char waveCkId[4];           /* RIFF */
    uint32_t waveCkSize;        /* Chunk size 4 + n */
    char waveId[4];             /* WAVE */
    char fmtCkId[4];            /* fmt */
    uint32_t fmtCkSize;         /* fmt chunk size: only 16 for PCM */
    uint16_t wFormatTag;        /* Format Code: only 0x0001 for this tool */
    uint16_t nChannels;         /* Channel num: only 2 for this tool */
    uint32_t nSamplesPerSec;    /* Sampling rate */
    uint32_t nAvgBytesPerSec;   /* Data Rate */
    uint16_t nBlockAlingn;      /* Data block size */
    uint16_t nBitsPerSample;    /* Bits per sample */
} WAV_FILE_HEADER;

/* Define the wav file header struct for simple processing */
typedef struct {
    char dataCkId[4];           /* data */
    uint32_t dLen;
} DATA_CHUNK_HEADER;

/* Struct used to traverse 8-bit stero audio sample */
typedef struct {
	uint8_t channel[2];
}EIGHT_BIT_STERO;

/* Struct used to traverse 16-bit stero audio sample */
typedef struct {
	int16_t channel[2];
}SIXTEEN_BIT_STERO;

int readWavHeaders(FILE *file, WAV_FILE_HEADER *wavHeader, DATA_CHUNK_HEADER *dataHeader);
void steroToMono(uint8_t *data, WAV_FILE_HEADER *wavFileHeader, DATA_CHUNK_HEADER *dataChunkHeader);
void printWavHeader( WAV_FILE_HEADER* wavHeader );
void printDataHeader( DATA_CHUNK_HEADER * dataHeader);
#endif /* STMUTIL_H_ */
