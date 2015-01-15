# $Id: Makefile,v 1.2 2007/04/04 15:18:50 marquet Exp $
#------------------------------------------------------------

CC      = gcc
CFLAGS  = -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -DDEBUG
CFLAGS += -g
LDFLAGS = -lpthread

BINARIES = 	mtcs
OBJECTS = 	mtcs.o tools.o cnct.o stats.o

all : $(BINARIES) $(OBJECTS)

###------------------------------
### Entries
###------------------------------------------------------------
mtcs : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

###------------------------------
### Dependencies
###------------------------------------------------------------
# thanks to 'make depend'
cnct.o: cnct.c config.h tools.h cnct.h
mtcs.o: mtcs.c tools.h config.h cnct.h
tools.o: tools.c tools.h
stats.o: stats.c stats.h

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean::
	$(RM) $(BINARIES)
realclean:: clean 
	$(RM) $(OBJECTS) $(TRASHFILES)
depend: 
	$(CC) $(CFLAGS) -MM *.c 

