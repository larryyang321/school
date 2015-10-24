include ../../../dot.Makefile
include ../../../latex.Makefile


all: a2_sub.pdf
	make -C q2

a2_sub.pdf: a2_sub.tex blockCFG.png defuseCFG.png


clean:
	rm -f a2_sub.pdf
	make -C q2 clean
