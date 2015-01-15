CC	= ocamlc
CFLAGS	= -c

OBJ =  lsysteme.cmo lsysteme.cmi tortue.cmi tortue.cmo
MOD = graphics.cma lsysteme.cmo

all : mlsysteme

lsysteme.cmo: lsysteme.ml
	$(CC) $(CFLAGS) lsysteme.ml

mlsysteme: lsysteme.cmo tortue.ml
	$(CC) $(MOD) -o mlsysteme tortue.ml

.PHONY: clean realclean

clean:
	$(RM) $(OBJ) *~

realclean: clean
	$(RM) mlsysteme
