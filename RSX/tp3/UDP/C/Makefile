CC	= gcc
CFLAGS	= -Wall -Werror -D_BSD_SOURCE -ansi -pedantic
CFLAGS += -g

all: clean realclean sendUDP

sendUDP.o: sendUDP.c
	$(CC) $(CFLAGS) -c sendUDP.c -o sendUDP.o

sendUDP: sendUDP.o
	$(CC) $(CFLAGS) sendUDP.o -o sendUDP

.PHONY: clean realclean

clean:
	$(RM) sendUDP.o

realclean:
	$(RM) *~ sendUDP