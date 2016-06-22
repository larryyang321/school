include ../../../latex.Makefile


.SUFFIXES: .tar
%.tar: %
	tar czf $@ -C $^ .


all: a2.pdf sploit1.tar sploit2.tar sploit3.tar sploit4.tar sploit5.tar;

clean:
	rm -f a2.pdf
	rm -f sploit1.tar sploit2.tar sploit3.tar sploit4.tar sploit5.tar
