OBJECTS := sploit1 sploit2 sploit3

all: $(OBJECTS);

sploit1: sploit1.c shellcode.h
sploit2: sploit2.c shellcode.h
sploit3: sploit3.c shellcode.h

%:
	gcc -Wall -ggdb $< -o $@

clean:
	rm -rf $(OBJECTS)


send:
	rsync -rtlv `pwd`/ uwaterloo-ugster:/home/kcarruth/uml/share
