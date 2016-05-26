include ../../../latex.Makefile


OBJECTS := sploit1 sploit2 sploit3 sploit4

all: $(OBJECTS) a1.pdf;

sploit1: sploit1.c shellcode.h
sploit2: sploit2.c shellcode.h
sploit3: sploit3.c shellcode.h
sploit4: sploit4.c shellcode.h

%:
	gcc -Wall -ggdb $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f a1.pdf
