CC		= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g
CFLAGS += -pg

all: clean realclean mdu

mdu:
	$(CC) $(CFLAGS) mdu.c -o mdu

.PHONY: clean realclean

clean:
	$(RM) mdu

realclean:
	$(RM) *~
