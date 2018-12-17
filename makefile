all: a.out

CC=g++
CPPFLAGS=-O3 -mavx2

a.out: main.cpp sudoku_lib/grid.cpp
	$(CC) $(CPPFLAGS) $^ -o $@


.PHONY: clean

clean:
	rm -f a.out
