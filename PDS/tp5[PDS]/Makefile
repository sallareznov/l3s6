CC		= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g

all: do

makeargv.o: makeargv.c
	$(CC) $(CFLAGS) -c makeargv.c -o makeargv.o

do.o: do.c
	$(CC) $(CFLAGS) -c do.c -o do.o

do: do.o makeargv.o
	$(CC) $(CFLAGS) do.o makeargv.o -o do

.PHONY: clean realclean

clean:
	$(RM) do.o makeargv.o

realclean:
	$(RM) *~ do
