#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "parser.h"

#define SMAKEFILE "smakefile"

void printUsage() {
    printf("Usage: smake [smakefile]\n");
    printf("	default name of the smakefile is \"%s\"\n", SMAKEFILE);
    printf("	Valid options:\n");
    printf("		-v             : verbose, print debug output \n");
    printf("		-k             : ignore error breakpoint\n");
 	printf("		--usage        : print smake usage\n");
 	printf("		--smake-usage  : print smakefile syntax\n");
}

void printSmakeUsage() {
    printf("SmakeUsage: smakefile\n");
    printf("	default name of the smakefile is \"%s\"\n", SMAKEFILE);
    printf("	smakefile syntax:\n");
    printf("		#: commentary \n");
    printf("		E: one and only one binary file\n");
    printf("		C: source files\n");
    printf("		H: header files\n");
    printf("		F: gcc flags\n");
    printf("		B: gcc library flags\n");
}

int main(int argc, char *argv[]){
	int errCode = EXIT_FAILURE;
	char* fileName = NULL; 
	
	if(argc < 2){
		fileName = SMAKEFILE;	
	} else if (argc == 2) {
		if(strcmp(argv[1], "--usage") == 0) {
			printUsage();
			return 0;
		} else if(strcmp(argv[1], "--smake-usage") == 0) {
			printSmakeUsage();
			return 0;
		} else {
			fileName = argv[1];			
		}
	} else if (argc == 3) {
		fileName = argv[2];
		if(strcmp(argv[1], "-v") == 0) {
			
		} else if (strcmp(argv[1], "-k") == 0) {
			
		} else {
			printUsage();
			return errCode;
		}
	} else {
		printUsage();
		return errCode;
	}
	
	errCode = parseFile(fileName);
	if (errCode != 0) {
		if (errCode == ERROR_SMAKEFILE_NOT_FOUND)
			printf("ERROR CODE: %d: smakefile not found.\n", errCode);
		if (errCode == ERROR_TOO_MANY_EXEC) {
			printf("ERROR CODE: %d: more than one executable in the smakefile.\n", errCode);	
			printSmakeUsage();
		}
	 	if (errCode == ERROR_NO_EXEC) {
			printf("ERROR CODE: %d: no executable in the smakefile.\n", errCode);
			printSmakeUsage();
		}
		if (errCode == ERROR_DIR_NOT_FOUND) 
			printf("ERROR CODE: %d: smakefile directory not found.\n", errCode);
		if (errCode == ERROR_NO_SRC)
			printf("ERROR CODE: %d: no source files in the smakefile.\n", errCode);
		if (errCode == ERROR_HEADER_NOT_FOUND)
			printf("ERROR CODE: %d: header file is missing.\n", errCode);
		if (errCode == ERROR_SRC_NOT_FOUND)
			printf("ERROR CODE: %d: source file is missing.\n", errCode);
		if (errCode == ERROR_EXEC_FAILED)
			printf("ERROR CODE: %d: exec failed.\n", errCode);
	} else {
		printf("Compilation Done.");
	}
	return errCode;
}
