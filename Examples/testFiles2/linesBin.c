#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linesBin.h"

#define LINEMAX 200

struct linesBin * linesBin_creator() {
								
	struct linesBin *lb = (struct linesBin *) malloc(sizeof(struct linesBin));
	
	lb->src_ctr = 3;
	lb->head_ctr = 0;
	lb->exec_ctr = 2;
	lb->flags_ctr = 2;
	lb->lib_ctr = 0;
	
	lb->srcLine = (char **) malloc(LINEMAX * sizeof(char *));
	lb->headLine = (char **) malloc(LINEMAX * sizeof(char *));
	lb->flagsLine = (char **) malloc(LINEMAX * sizeof(char *));
	lb->libLine = (char **) malloc(LINEMAX * sizeof(char *));
	
	lb->srcLine[0] = "gcc";
	lb->srcLine[1] = "-o";
	lb->flagsLine[0] = "gcc";
	lb->flagsLine[1] = "-c";
	
	return lb;
}

void linesBin_addLine(int *pos, char *lineToAdd, char **lineBin) {
	int tmp;
	char *pc;
	
	pc = strtok (lineToAdd," ");
	while (pc != NULL)
	{
		tmp = strlen(pc) + 1;
					
		lineBin[*pos] = (char *) malloc(tmp);
		
		memcpy(lineBin[*pos], pc, tmp);	   
				
		pc = strtok (NULL, " ");
		*pos = *pos + 1;
	}
}

int linesBin_parseLine(char type, char *line, struct linesBin *lb) {
	int errCode;
	
	switch(type) {
			
		case 'C' :	linesBin_addLine(&(lb->src_ctr), line, lb->srcLine);
					break;	
		case 'H' :	linesBin_addLine(&(lb->head_ctr), line, lb->headLine);
					break;
		case 'E' :	if(lb->exec_ctr >= 3) {
						errCode = ERROR_TOO_MANY_EXEC;
						return errCode;
					}
					linesBin_addLine(&(lb->exec_ctr), line, lb->srcLine);
					break;
		case 'F':	linesBin_addLine(&(lb->flags_ctr), line, lb->flagsLine);
					break;
		case 'D' : 	linesBin_addLine(&(lb->flags_ctr), line, lb->flagsLine);
					break;
		case 'B' :	linesBin_addLine(&(lb->lib_ctr), line, lb->libLine);
					break;
		case '#' :	printf("Commment : %s \n", line);
					break;
		default  :	printf("Non reconnu \n");
					break;
	} 
	errCode = 0;
	return errCode;
}

int linesBin_check(struct linesBin *lb) {
	int errCode;
	if(lb->exec_ctr == 2){
		errCode = 303;
		return errCode;
	}
	if (lb->src_ctr == 3){
		errCode = 505;
		return errCode;
	}
	errCode = 0;
	return errCode;
}

void linesBin_destructor(struct linesBin *lb) {
	int cc = 3;
	int hc = 0;
	int fc = 2;
	int bc = 0;
	
	while (cc < lb->src_ctr) {
		free(lb->srcLine[cc++]);
	}
	while (hc < lb->head_ctr) {
		free(lb->headLine[hc++]);
	}
	while (fc < lb->flags_ctr) {
		free(lb->flagsLine[fc++]);
	}
	while (bc < lb->lib_ctr) {
		free(lb->libLine[bc++]);
	}
	
	free(lb->srcLine);
	free(lb->headLine);
	free(lb->flagsLine);
	free(lb->libLine);
	
	free(lb);
}

void linesBin_printer(struct linesBin *lb) {
	int cc = 0;
	int hc = 0;
	int fc = 0;
	int bc = 0;

	while (cc < lb->src_ctr) {
		printf("Cline: %s \n", lb->srcLine[cc++]);
	}
	while (hc < lb->head_ctr) {
		printf("Hline: %s \n", lb->headLine[hc++]);
	}
	while (fc < lb->flags_ctr) {
		printf("Fline: %s \n", lb->flagsLine[fc++]);
	}
	while (bc < lb->lib_ctr) {
		printf("Bline: %s \n", lb->libLine[bc++]);
	}
}
