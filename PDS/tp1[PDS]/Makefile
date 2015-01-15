CC		= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g

all: prlimit maccess

prlimit:
	$(CC) $(CFLAGS) prlimit.c -o prlimit

maccess:
	$(CC) $(CFLAGS) maccess.c -o maccess

.PHONY: clean

clean:
	$(RM) prlimit maccess
