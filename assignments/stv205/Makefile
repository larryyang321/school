include ../../latex.Makefile


.tex.pdf:
	latexmk -xelatex $< && latexmk -bibtex -c


all: a1.pdf a2.pdf

clean:
	rm -f a1.pdf
	rm -f a2.pdf
