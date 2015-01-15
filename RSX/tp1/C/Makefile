CC		= gcc
CFLAGS	= -Wall -Werror -D_BSD_SOURCE -ansi -pedantic
CFLAGS += -g

all: sendUDP

sendUDP:
	$(CC) $(CFLAGS)	sendUDP.c -o sendUDP

.PHONY: clean

clean:
	$(RM) sendUDP