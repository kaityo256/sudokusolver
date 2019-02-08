all: a.out

CC=g++
CPPFLAGS=-O3 -mavx2
DEPFLAGS=-MM -MG

a.out: main.cpp sudoku_lib/grid.cpp sudoku_lib/grid.hpp
	$(CC) $(CPPFLAGS) main.cpp sudoku_lib/grid.cpp -o $@

dep:
	$(CC) $(DEPFLAGS) $(SRC) $(HED) >makefile.depend

.PHONY: clean

clean:
	rm -f a.out $(OBJ)

-include makefile.depend
