CC_FLAGS :=


.SUFFIXES: .c .o

.c.o:
	${CC} $(CC_FLAGS) -c $<
