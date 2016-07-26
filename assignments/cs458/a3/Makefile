include ../../../dot.Makefile
include ../../../latex.Makefile


all: a3.pdf src.tar

a3.pdf: a1network.png
src.tar:
	tar cvf src.tar *crypt

clean:
	rm -f a3.pdf
	rm -f src.tar
