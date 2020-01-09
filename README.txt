The stero-to-mono (stm) utility is used to convert two channel (left and right) stero audio files to mono (single channel). This is done by reducing the volume of each channel and adding them together to create a mono mix.

Included in the directory is a  python test script (stmTester.py) that runs through the different error conditions that the tool handles. The tool also converts an 8-bit sample file and writes it to the default output.wav file, an 8-bit sample file and writes it to a user defined file name (8-bit-mono.wav) and a 16-bit sample file to a user defined file name (16-bit-mono.wav). Below is an example output of the stmTester.py tool.

jbrandes@ubuntu:~/stm$ python stmTester.py 

Invalid paramater passed to tester tool. Try 'all' or <testnumber>

1.  Test help parameter
2.  Test invalid parameter
3.  Test no input file passed
4.  Test no output file passed
5.  Test invalid input file extension
6.  Test invalid output file extension
7.  Test non-existent input file passed
8.  Test invalid path for output file passed
9.  Test file with not enough data for header
10. Test truncated data file
11. Test invalid wav file Chunk ID
12. Test invalid wav Format ID
13. Test invalid Format Chunk ID
14. Test invalid audio format
15. Test invalid sample rate
16. Test file is 2 channel stero
17. Test 8 bit stero to mono default output file name
18. Test 8 bit stero to mono custom file name
19. Test 16 bit stero to mono custom file name

jbrandes@ubuntu:~/stm$ python stmTester.py 1
####################################
Running Test: Test help parameter
####################################

Ran command: 
/home/jbrandes/stm/stm -h 
--------
Expected:
------

Usage: stm [-o OUTFILE] [-h] INFILE

A program used to convert stero LPCM wav files to mono.

    -o,     Provide an OUTFILE, the name for the mono file to be stored
    -h,     Print usage information message


Result:
------

Usage: stm [-o OUTFILE] [-h] INFILE

A program used to convert stero LPCM wav files to mono.

    -o,     Provide an OUTFILE, the name for the mono file to be stored
    -h,     Print usage information message


------
####################################
        PASSED
#################################### 

All test WAVE input files can be found in the inputs directory. These files have been downloaded from http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/Samples.html and various versions were created with altered header information to test the error handling of the tool. Below is a description of the test input files.

    1. M1F1-uint8-AFsp.wav - valid 8-bit unsigned wav file
    2. M1F1-int16-AFsp.wav - valid 16-bit signed wav file
    3. M1F1-int32-AFsp.wav - invalid 32-bit signed wav file
    4. small-header.wav - invalid truncated wav header file
    5. bad-data-length.wav - invalid truncated data header file
    6. bad-RIFF.wav - invalid wav file chunk ID
    7. bad-WAV.wav - invalid wav format ID
    8. bad-fmt.wav - invalid format chunkn ID
    9. M1F1-Alaw-AFsp.wav - invalid audio format
    10. mono-input.wav - invalid mono mix file

NOTE: The tool assumes that none of the files will include a "fact" header before "data" headers in the format, as PCM files with a sample rate of 16-bits or less do not contain this header according to http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html. If the file includes a "fact" header directly after the "wav" header then the tool will fail stating there is no "data" header. I considered adding a loop to check for the "fact" header, which would SEEK through the cksize of the header and check for the "data" chunkId in the next header. This check is not part of the delivered code.