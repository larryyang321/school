CXX_FLAGS := -g -Wall -Wextra


.SUFFIXES: .cc .cpp .o

.cc.o:
	${CXX} $(CXX_FLAGS) -c $<

.cpp.o:
	${CXX} $(CXX_FLAGS) -c $<
