#ifndef LINESBIN_H
    #define LINESBIN_H
    #include <stdio.h>

    struct linesBin {
		int src_ctr;
		int head_ctr;
		int exec_ctr;
		int flags_ctr;
		int lib_ctr;

        char **srcLine;
		char **headLine;
		char **flagsLine;
		char **libLine;
    };
	
	struct linesBin * linesBin_creator();
	
	void linesBin_destructor(struct linesBin *lb);
	
	void linesBin_printer(struct linesBin *lb);
	
	int linesBin_parseLine(char type, char *line, struct linesBin *lb);
	
	int linesBin_check(struct linesBin *lb);
#endif
