BINPATH= /usr/bin
CC= gcc

CFLAGS= -g -ansi -pedantic -Wall -Werror -O3 -fomit-frame-pointer

PROG_EXEC= smake
SRC= linesBin.c parser.c main.c process.c smakeUtils.c
OBJ= $(SRC:.c=.o)

all: ${PROG_EXEC} clean

dev: ${PROG_EXEC}

${PROG_EXEC}: ${OBJ}
	@echo compilation of ${PROG_EXEC}	
	$(CC) $(CFLAGS) -o ${PROG_EXEC} ${OBJ}

%.o : %.c
	@echo -n 'compilation of $< ... '
	@${CC} $(CFLAGS) -o $@ -c $<
	@echo done

clean-all: clean
	rm $(PROG_EXEC)

clean:
	rm -fr *.o

install: all
	cp ${PROG_EXEC}	$BINPATH
