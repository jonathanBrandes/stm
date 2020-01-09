#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "argparse.h"

/**
 * printBasic
 *
 * Used to print basic information of the currently executing program
 */
void printBasic() {
	printf("\nUsage: stm [-o OUTFILE] [-h] INFILE\n");
	printf("Try 'stm -h' for more information\n\n");
}

/**
 * printHelp
 *
 * Used to print the help message of the currently executing program
 */

static void printHelp() {
	printf("\nUsage: stm [-o OUTFILE] [-h] INFILE\n");
	printf("\nA program used to convert stero LPCM wav files to mono.\n");
	printf("\n\t-o,		Provide an OUTFILE, the name for the mono file to be stored\n");
	printf("\t-h,		Print usage information message\n\n");
}

/**
 * handleArgs
 * input params: argc - the number of command line arguments passed into program
 * 				 argv - the command line arguments passed into program
 *				 inFile - the file name of the stero input file
 * 				 outFile - the file name for the mono output file
 *
 * Processes the command line arguments of the function
 */
int handleArgs(int argc, char **argv, char *inFile, char *outFile) {

	int c, index;
	opterr = 0;	/* Suppress standard error printing */
	const char *fileName = NULL;

	while ((c = getopt(argc, argv, "o:h")) != -1) {
		switch (c) {
		case 'o':
			strncpy(outFile, optarg, strlen(optarg));
			fileName = strrchr(outFile, '.');
			if ( !fileName || strcmp(fileName, ".wav") != 0) {
				printf("Invalid file extension for OUTFILE\n");
				printBasic();
				return 1;
			}
			break;
		case 'h':
			printHelp();
			return 1;
		case '?':
			if (optopt == 'o')
				printf("No OUTFILE specified\n");
			else if (isprint(optopt))
				printf("-%c is not a valid argument for this tool\n", optopt);
			else
				printf("Unknown option %x\n", optopt);
			printBasic();
			return 1;
		default:
			return 1;
		}
	}

	index = optind;

	if (index == argc) {
		printf("No INFILE specified\n");
		printBasic();
		return 1;
	} else if (index > argc) {
		printf("Too many arguments for this tool");
		printBasic();
		return 1;
	} else {
			strncpy(inFile, argv[index], strlen(argv[index]));
			fileName = strrchr(inFile, '.');
			if ( !fileName || strcmp(fileName, ".wav") != 0) {
				printf("Invalid file extension for INFILE\n");
				printBasic();
				return 1;
			}
	}
	return 0;
}
