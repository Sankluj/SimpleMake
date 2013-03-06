#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#include "linesBin.h"

int buildObjFile(char *cFile, int *nbFlags, char **flagsBin) {
	int errCode = 1;
	int tmp = 0;
	int printCtr = 0;
	
	tmp = strlen(cFile) + 1;
	flagsBin[*nbFlags] = (char *) malloc(tmp);
	memcpy(flagsBin[*nbFlags], cFile, tmp);
	
	flagsBin[*nbFlags+1] = NULL;
	
	while(printCtr <= *nbFlags) {
		printf("%s ", flagsBin[printCtr]);
		printCtr++;
	}
	printf("\n");
	 
	if (fork() == 0) {
		if (execvp(flagsBin[0], flagsBin) >= 0) {
			exit(0);
		}
		else {
			printf("*** ERROR: exec failed\n");
			exit(808);
		}
	} else {
		pid_t fils;
		int status;

		fils = wait(&status);
		if (WIFEXITED(status)) {
			printf("fils %d terminé avec code %d \n", fils, WEXITSTATUS(status));
			errCode = WEXITSTATUS(status);
		}
	}
	free(flagsBin[*nbFlags]);
	return errCode;
}

int buildExecFile(int nbFiles, char **linesBin, int nbLib, char **libBin) {
	int errCode = 1;
	int tmp = 0;
	int printCtr = 0;
	int ctr = 0;
	
	while (ctr < nbLib) {
		tmp = strlen(libBin[ctr]) + 1;
		linesBin[nbFiles+ctr] = (char *) malloc(tmp);
		memcpy(linesBin[nbFiles+ctr], libBin[ctr], tmp);
		ctr++;
	}
		
	linesBin[nbFiles+ctr] = NULL;
	
	while(printCtr < (nbFiles+ctr)) {
		printf("%s ", linesBin[printCtr]);
		printCtr++;
	}
	printf("\n");
	
	if (fork() == 0) {
		if (execvp(linesBin[0], linesBin) >= 0) {
			exit(0);
		}
		else {
			printf("*** ERROR: exec failed\n");
			exit(909);
		}
	} else {
		pid_t fils;
		int status;

		fils = wait(&status);
		if (WIFEXITED(status)) {
			printf("fils %d terminé avec code %d \n", fils, WEXITSTATUS(status));
			errCode = WEXITSTATUS(status);
		}
	}
	return errCode;
}

int checkHFiles(int nbFiles, char **linesBin) {
	int ctr = 0;
	int maxModifTime = 0;
	
	while (ctr < nbFiles) {	
		int modifTime = 0;
		struct stat sb;
		
		if (stat(linesBin[ctr], &sb) < 0) {
			printf("%s: ", linesBin[ctr]);
			return(606);
		}
		
		modifTime = sb.st_mtime;
		
		if (maxModifTime == 0 || maxModifTime < modifTime)
			maxModifTime = modifTime;
			
		ctr++;
	}
	printf("modifMax des H: %d \n", maxModifTime);
	return maxModifTime;
}

int checkObjFiles(int hAge, int nbFlags, char **flagsBin, char* srcLine, int cModifTime) {
	int errCode = 0;
	char *pc;
	char *srcLineCopy;
	struct stat sb;
	int tmp;
		
	tmp = strlen(srcLine) + 1;
	srcLineCopy = (char *) malloc(tmp);
	memcpy(srcLineCopy, srcLine, tmp);
		
	/* On considère le .o associé au fichier .c */
	pc = strtok (srcLineCopy, ".");
	strcat(pc, ".o");
	
	/* Test de l'existence du fichier .o */
	if (stat(pc, &sb) < 0) {
		printf("############ construire %s \n", pc);
		errCode = buildObjFile(srcLine, &nbFlags, flagsBin);
		if (errCode != 0) {
			free(srcLineCopy);
			return errCode;
		}
		else
			return time(NULL);
	}
	/* si le fichier existe qu'il est plus vieux qu'un 
	 *  quelconque .h ou plus vieux que son .c associé 
	 */
	else if (sb.st_mtime < cModifTime || sb.st_mtime < hAge) {
		printf("############ reconstruire %s \n", pc);
		errCode = buildObjFile(srcLine, &nbFlags, flagsBin);
		if (errCode != 0) {
			free(srcLineCopy);
			return errCode;
		}
		else
			return time(NULL);
	} else {
		free(srcLineCopy);
		return sb.st_mtime;
	}
}

int checkCFiles(int hAge, int nbFiles, char **linesBin, int nbFlags, char **flagsBin) {
	int ctr = 3;
	int maxModifTime = 0;
	
	while (ctr < nbFiles) {	
		int cModifTime = 0;
		int oModifTime = 0;
		struct stat sb;
		char *pc;
		
		/* Verification de la présence du fichier .c */
		if (stat(linesBin[ctr], &sb) < 0) {
			printf("%s: ", linesBin[ctr]);
			return 707;
		}
		
		cModifTime = sb.st_mtime;
		oModifTime = checkObjFiles(hAge, nbFlags, flagsBin, linesBin[ctr], cModifTime);
		
		/* on remplace le .c de la structure lb->srcLine par le .o */ 
		pc = strtok (linesBin[ctr], ".");
		strcat(pc, ".o");
		
		if (maxModifTime == 0 || maxModifTime < oModifTime)
			maxModifTime = oModifTime;
				
		ctr++;
	}
	printf("modifMax des O: %d \n", maxModifTime);
	return maxModifTime;
}

int checkExecFile(int oAge, int nbFiles, char **linesBin, int nbFlags, char **flagsBin) {
	int errCode = 0;
	struct stat sb;
	if (stat(linesBin[2], &sb) < 0) {
		perror("############ Construction du binaire");
		errCode = buildExecFile(nbFiles, linesBin, nbFlags, flagsBin);
	} else if (sb.st_mtime < oAge) {
		printf("############ Reconstruction du binaire\n");
		errCode = buildExecFile(nbFiles, linesBin, nbFlags, flagsBin);
	}
	return errCode;
}

int processing(struct linesBin *lb) {
	int hAge;
	int oAge;
	
	hAge = checkHFiles(lb->head_ctr, lb->headLine);
	if (hAge == 606)
		return hAge;
	oAge = checkCFiles(hAge, lb->src_ctr, lb->srcLine, lb->flags_ctr, lb->flagsLine);
	if (oAge == 707 || oAge == 808)
		return oAge;
	return checkExecFile(oAge, lb->src_ctr, lb->srcLine, lb->lib_ctr, lb->libLine);
}
