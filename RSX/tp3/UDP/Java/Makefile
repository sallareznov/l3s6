JC = javac

all: clean realclean ReceiveUDP

ReceiveUDP:
	mkdir classes && cd src && $(JC) ReceiveUDP.java -d ../classes

.PHONY: clean realclean

clean:
	$(RM) -r classes

realclean:
	$(RM) -r *~
