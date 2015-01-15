CC		= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g

all: stail utail

stail: stail.o
	$(CC) $(CFLAGS) -o stail stail.o

utail: utail.o
	$(CC) $(CFLAGS) -o utail utail.o

.PHONY: clean realclean

clean:
	$(RM) stail.o utail.o

realclean:
	$(RM) *~ stail utail
