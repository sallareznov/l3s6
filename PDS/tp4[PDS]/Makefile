CC		= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g

all: ff race

ff: ff.o
	$(CC) $(CFLAGS) -o ff ff.o

race: race.o
	$(CC) $(CFLAGS) -o race race.o

.PHONY: clean realclean

clean:
	$(RM) ff.o race.o

realclean:
	$(RM) *~ ff race
