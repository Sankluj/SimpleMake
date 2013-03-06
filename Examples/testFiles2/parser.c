#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"
#include "linesBin.h"
#include "parser.h"

#define LINEMAXLENGTH 200

static void jumpLineToSpace(char *line)
{
    char *p = strchr(line, '\n');

    if (p)
    {
        *p = ' ';
    }
}

static char* extractPath(char *filePath)
{
	int tmp;
	char *buff;
	char *buffNext;
	char *path;
	
	tmp = strlen(filePath) + 1;
	path = (char *) malloc(tmp);
	buffNext =  "";
	
	/* Pour recuperer un chemin absolu */
	if(filePath[0] == '/')
		strcat(path, "/");
		
	buff = strtok (filePath,"/");
	while (buff != NULL && buffNext != NULL)
	{				
		if ((buffNext = strtok (NULL, "/")) != NULL) {
			strcat(path, buff);
			strcat(path, "/");
		}
		buff = buffNext;
	}
	printf("Tokenizing complete : %s\n", path);
	free(buffNext);
	free(buff);
	return path;
}

int parseFile(char* name) {
	int errCode;
	FILE* stream;
	char *path;
	char line[LINEMAXLENGTH];
	struct linesBin *lb = linesBin_creator();
	
	if((stream = fopen(name, "r"))) {
			
		path = extractPath(name);
		if (strlen(path) > 2) {
			if (chdir(path) == -1) {
				return 404;
			}
		}
		
		printf("============= parsing ============\n");
		while(fgets(line, sizeof line, stream) != NULL){
			if(strlen(line) > 1){
				jumpLineToSpace(line);
				errCode = linesBin_parseLine(line[0], line+2, lb);
				if (errCode != 0) {
					free(path);
					linesBin_destructor(lb);
					fclose(stream);
					return errCode;
				}
			}
		}
		printf("============ checking ============\n");
		errCode = linesBin_check(lb);
		if (errCode != 0) {
			free(path);
			linesBin_destructor(lb);
			fclose(stream);
			return errCode;
		}
		printf("============ printing ============\n");
		linesBin_printer(lb);
		printf("=========== processing ===========\n");
		errCode = processing(lb);
		if (errCode != 0) {
			free(path);
			linesBin_destructor(lb);
			fclose(stream);
			return errCode;
		}
		printf("============ cleaning ============\n");	
		linesBin_destructor(lb);
		free(path);
		printf("=============== end ==============\n");
		
		fclose(stream);
		return(errCode);
	}else {
		linesBin_destructor(lb);
		return 101;
	}
}
