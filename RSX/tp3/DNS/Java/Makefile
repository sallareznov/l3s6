JC = javac

all: clean realclean DNS

DNS:
	mkdir classes && cd src && $(JC) DNS.java -d ../classes

.PHONY: clean realclean

clean:
	$(RM) -r classes

realclean:
	$(RM) -r *~
