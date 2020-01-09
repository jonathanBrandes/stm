#Test script for stm command line utility.

import subprocess
import os
import sys

#Setup env
cmd = os.getcwd() + "/stm"

def runTest(test):
    print "####################################\nRunning Test: " + test[1] + "\n####################################\n"
    output = subprocess.Popen(test[2], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = output.communicate()

    print "Ran command: "
    for cmd in test[2]:
        print cmd,
    print "\n--------"

    print "Expected:\n------\n" + test[3] + "\nResult:\n------\n" + stdout +"\n------"

    if (stdout == test[3]):
        print "####################################\n\t\tPASSED\n####################################\n"
        return 0
    else:
        print "####################################\n\t\tFAILED\n####################################\n"
        return 1

def printTests():
    for test in tests:
        print str(test[0]) + ".\t" + test[1]

printBasic = "\nUsage: stm [-o OUTFILE] [-h] INFILE\nTry 'stm -h' for more information\n\n"
printHelp = "\nUsage: stm [-o OUTFILE] [-h] INFILE\n\nA program used to convert stero LPCM wav files to mono.\n\n\t-o,\t\tProvide an OUTFILE, \
the name for the mono file to be stored\n\t-h,\t\tPrint usage information message\n\n"
#
#CLI tests
#
tests = [(1,"Test help parameter", (cmd, "-h"), printHelp), 
        (2,"Test invalid parameter", (cmd, "-g"), "-g is not a valid argument for this tool\n" + printBasic),
        (3,"Test no input file passed", (cmd, "-o", "inputs/M1F1-uint8-AFsp.wav"), "No INFILE specified\n" + printBasic), 
        (4,"Test no output file passed", (cmd, "inputs/M1F1-uint8-AFsp.wav", "-o"), "No OUTFILE specified\n" + printBasic), 
        (5,"Test invalid input file extension", (cmd, "inputs/M1F1-uint8-AFsp.txt"), "Invalid file extension for INFILE\n" + printBasic),
        (6,"Test invalid output file extension", (cmd, "goodWav.wav", "-o", "inputs/M1F1-uint8-AFsp.txt"), "Invalid file extension for OUTFILE\n" + printBasic),
#
#File tests
#
        (7,"Test non-existent input file passed", (cmd, "nofile.wav"), "Cannot open file: nofile.wav\n"),
        (8,"Test invalid path for output file passed", (cmd, "inputs/M1F1-uint8-AFsp.wav", "-o", "nopath/nopath.wav"), "Cannot create file: nopath/nopath.wav\n"),
        (9,"Test file with not enough data for header", (cmd, "inputs/small-header.wav"), "Error reading INFILE wav header\n"),
        (10,"Test truncated data file", (cmd, "inputs/bad-data-length.wav"), "Data chunk read error\n"),
#
#Header tests
#
        (11,"Test invalid wav file Chunk ID", (cmd, "inputs/bad-RIFF.wav"), "Invalid wav file Chunk ID\n"),
        (12,"Test invalid wav Format ID", (cmd, "inputs/bad-WAVE.wav"), "Invalid wav Format ID\n"),
        (13,"Test invalid Format Chunk ID", (cmd, "inputs/bad-fmt.wav"), "Invalid fmt Chunk ID\n"),
        (14,"Test invalid audio format", (cmd, "inputs/M1F1-Alaw-AFsp.wav"), "wav format is not PCM\n"),
        (15,"Test invalid sample rate", (cmd, "inputs/M1F1-int32-AFsp.wav"), "Invalid number of samples per second in wav file\n"),
        (16,"Test file is 2 channel stero", (cmd, "inputs/mono-input.wav"), "wav file is not stero\n"),
#
#Success tests
#
        (17,"Test 8 bit stero to mono default output file name", (cmd, "inputs/M1F1-uint8-AFsp.wav"), "Converted stero file inputs/M1F1-uint8-AFsp.wav to mono file output.wav\n"),
        (18,"Test 8 bit stero to mono custom file name", (cmd, "inputs/M1F1-uint8-AFsp.wav", "-o", "mono-8bit.wav"), "Converted stero file inputs/M1F1-uint8-AFsp.wav to mono file mono-8bit.wav\n"),
        (19,"Test 16 bit stero to mono custom file name", (cmd, "inputs/M1F1-int16-AFsp.wav", "-o", "mono-16bit.wav"), "Converted stero file inputs/M1F1-int16-AFsp.wav to mono file mono-16bit.wav\n"),
        ]


if not (os.path.isfile(cmd) and os.access(cmd, os.X_OK)):
    print "Cannot find stm utility in current directory."
    quit()

if len(sys.argv) == 1:
    print "\nInvalid paramater passed to tester tool. Try 'all' or <testnumber>\n"
    printTests()
    quit()

if (sys.argv[1] == "all"):
    success = fail = 0
    for test in tests:
        if runTest(test) == 0:
            success += 1
        else:
            fail += 1
    print "Total PASSED: " + str(success) + "\nTotal FAILED: " + str(fail)
elif (int(sys.argv[1]) > 0 and int(sys.argv[1]) < 20):
    runTest(tests[int(sys.argv[1])-1])
else:
    print "\nInvalid paramater passed to tester tool. Try 'all' or <testnumber>\n"
    printTests()
