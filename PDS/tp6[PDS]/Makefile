# $Id: Makefile,v 1.3 2005/03/29 09:42:40 marquet Exp $
#------------------------------------------------------------

CC      = gcc
CFLAGS  = -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500          
SRCS 	= common.c sighandlers.c jobs.c cmd.c pipe.c mshell.c  
OBJS 	= common.o sighandlers.o jobs.o  cmd.o pipe.o mshell.o
EXEC 	= mshell

# main entries
all: $(EXEC)

mshell: $(OBJS)
	$(CC)  $(OBJS) -o $@

# gcc -MM generated dependencies
cmd.o: cmd.c jobs.h common.h
common.o: common.c
jobs.o: jobs.c jobs.h common.h
mshell.o: mshell.c cmd.h sighandlers.h jobs.h common.h
sighandlers.o: sighandlers.c jobs.h common.h sighandlers.h

# misc
.PHONY: clean mrpopoer realclean
clean::
	rm -f $(OBJS) *~

realclean mrproper:: clean
	rm -f $(EXEC)