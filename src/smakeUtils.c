#include <stdio.h>

int debugPrinter = 0;

void setDebugPrinter(int boolean) {
	if (boolean == 1) {
		debugPrinter = 1;
	}
}

void print(char *string) {
	if (debugPrinter == 1) {
		printf("%s", string);
	}
}

void prints(char *string, char *arg) {
	if (debugPrinter == 1) {
		printf("%s %s \n", string, arg);
	}
}

void printi(char *string, int arg) {
	if (debugPrinter == 1) {
		printf("%s %d \n", string, arg);
	}
}
