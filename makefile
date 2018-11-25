all: a.out
SRC=$(shell ls *.cpp)
HED=$(shell ls *.hpp)

CC=g++
CPPFLAGS=-O3 -mavx2

a.out: $(SRC) $(HED)
	$(CC) $(CPPFLAGS) $(SRC) -o $@


.PHONY: clean

clean:
	rm -f a.out
