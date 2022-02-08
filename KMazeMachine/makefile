CC = g++
CFLAGS = -g -Wall

all: Mazemake Mazesolve

Mazemake: Mazemake.cpp dset.h dset.o
	$(CC) $(CFLAGS) -o $@ Mazemake.cpp dset.o

Mazesolve: Mazesolve.cpp dset.h dset.o
	$(CC) $(CFLAGS) -o $@ Mazesolve.cpp dset.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<


clean:
	        rm -f Mazemake dset.o 
			rm -f Mazesolve dset.o
			rm -f maze.txt
			rm -f path.txt
			rm -f maze.ppm
			rm -f path.ppm
