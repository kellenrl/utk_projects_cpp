CC = g++
CFLAGS = -g -Wall

all: KMazeMachine KMazeSolver

KMazeMachine: KMazeMachine.cpp dset.h dset.o
	$(CC) $(CFLAGS) -o $@ KMazeMachine.cpp dset.o

KMazeSolver: KMazeSolver.cpp dset.h dset.o
	$(CC) $(CFLAGS) -o $@ KMazeSolver.cpp dset.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<


clean:
	        rm -f KMazeMachine dset.o 
			rm -f KMazeSolver dset.o
			rm -f maze.txt
			rm -f path.txt
			rm -f maze.ppm
			rm -f path.ppm
