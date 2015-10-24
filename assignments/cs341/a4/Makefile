include ../../../latex.Makefile


all: a4.pdf

test: bipartiteDisks


bipartiteDisks: bipartiteDisks.cpp
	${CXX} -std=c++11 -g -Wall $^ -o $@


clean:
	rm -f a4.pdf
	rm -f bipartiteDisks
