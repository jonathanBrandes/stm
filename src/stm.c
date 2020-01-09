#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"
#include "stmutil.h"

#define MAX_FILE_LEN 4096

int main(int argc, char** argv) {

	uint8_t * _data = NULL;
	int ret = 1;
	FILE *_steroFile = NULL, *_monoFile = NULL;
	char _inFileName[MAX_FILE_LEN] = {}, _outFileName[MAX_FILE_LEN] = {};
	WAV_FILE_HEADER _pwavFileHeader;
	DATA_CHUNK_HEADER _pdataChunkHeader;

	if (argc < 2) {
			printBasic();
			return 1;
	}

	/** Read arugments passed by command line */
	if (handleArgs(argc, argv, _inFileName, _outFileName)) {
		return 1;
	}

	/** Check if outfile parameter was passed */
	if (strlen(_outFileName) == 0) {
			strcpy(_outFileName,"output.wav"); /* Assign default output name */
	}

	_steroFile = fopen(_inFileName, "rb");
	if (!_steroFile) {
		printf("Cannot open file: %s\n", _inFileName);
		return 1;
	}

	if (readWavHeaders(_steroFile, &_pwavFileHeader, &_pdataChunkHeader) != 0) {
		goto error;
	}

	/** Allocate buffer to read all of the samples */
	_data = (uint8_t*)malloc(_pdataChunkHeader.dLen*sizeof(uint8_t));

	if (_data == NULL) {
		printf("Error allocating memory\n");
		goto error;
	}

	/* Read data Chunk Header */
	if(fread((void*)_data, sizeof(uint8_t), _pdataChunkHeader.dLen, _steroFile) != _pdataChunkHeader.dLen) {
		printf("Data chunk read error\n");
		goto error;
	}

	/* Convert stero sample to mono mix */
	steroToMono(_data, &_pwavFileHeader, &_pdataChunkHeader);

	/* Open mono mix file to write the converted data */
	_monoFile = fopen(_outFileName, "wb");
	if (!_monoFile) {
		printf("Cannot create file: %s\n", _outFileName);
		goto error;
	}

	/* Write updated headers and mono mix to output file */
	if(fwrite((void*)&_pwavFileHeader, sizeof(WAV_FILE_HEADER), (size_t)1, _monoFile) != 1 ||
		fwrite((void*)&_pdataChunkHeader, sizeof(DATA_CHUNK_HEADER), (size_t)1, _monoFile) != 1 ||
		fwrite((void*)_data, sizeof(uint8_t), _pdataChunkHeader.dLen, _monoFile) != _pdataChunkHeader.dLen) {
		printf("Error writing mono file");
		goto error;
	} else {
		printf("Converted stero file %s to mono file %s\n", _inFileName, _outFileName);
		ret = 0;
	}

	error:

	if (_monoFile != NULL) fclose(_monoFile);
	if (_steroFile!= NULL) fclose(_steroFile);
	if (_data != NULL) free(_data);

	return ret;
}
