#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef enum direction {
	DIR_LEFT, //0
	DIR_UP, //1
	DIR_RIGHT, //2
	DIR_DOWN, //3
} DIR_T;

struct cell {
	cell() { x = 0; y = 0; }
	cell(int x, int y) { this->x = x; this->y = y; }

	int x;
	int y;
};

DIR_T get_dir(int x1, int y1, int x2, int y2) {
	if(x1 == x2) {
		if(y1 < y2) return DIR_DOWN;
		if(y1 > y2) return DIR_UP;
	}
	else if(y1 == y2) {
		if(x1 < x2) return DIR_RIGHT;
		if(x1 > x2) return DIR_LEFT;
	}
	return (DIR_T)EXIT_SUCCESS;
}

bool solve(bool ***M, int **V, cell *stackarr, int& stack_size, cell T, cell S);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: ./Mazesolve maze.txt path.txt\n");
	return 1;
	} 

	//open input file for reading
	FILE* in;
	in = fopen(argv[1], "r");

	//open output file for writing
	FILE* out;
	out = fopen(argv[2], "w");

	int Nrows;
	int Ncols;

	//determine Nrow,Ncol from input file
	fscanf(in, "MAZE %i %i\n", &Nrows, &Ncols);

	//allocates bool M[Ncols][Nrows][4]
	bool ***M = new bool**[Ncols];
	for(int i = 0; i < Ncols; i++) {
		M[i] = new bool*[Nrows];
		for(int j = 0; j < Nrows; j++) {
			M[i][j] = new bool[4];
		}
	}

	//intitializes bool M[][][]
	for(int i = 0; i < Ncols; i++) {
		for(int j = 0; j < Nrows; j++) {
			for(int k = 0; k < 4; k++) {
				M[i][j][k] = false;
			}
		}
	}

	//set boundary walls - i is columns, j is rows
	for(int i = 0; i < Ncols; i++) {
		for(int j = 0; j < Nrows; j++) {
			if(j == 0 && i == 0) { //if top left cell
				M[i][j][0] = true;
				M[i][j][1] = true;
			}
			if(j == 0 && i == Ncols-1) { //if top right cell
				M[i][j][1] = true;
				M[i][j][2] = true;
			}
			if(j == Nrows-1 && i == 0) { //if bottom left cell
				M[i][j][0] = true;
				M[i][j][3] = true;
			}
			if(j == Nrows-1 && i == Ncols-1) { //if bottom right cell
				M[i][j][2] = true;
				M[i][j][3] = true;
			}
			if(i == 0) { //if left boundary cell
				M[i][j][0] = true;
			}
			if(i == Ncols-1) { //if right boundary cell
				M[i][j][2] = true;
			}
			if(j == 0) { //if top boundary cell
				M[i][j][1] = true;
			}
			if(j == Nrows-1) { //if bottom boundary cell
				M[i][j][3] = true;
			}
		}
	}

	//read input file, set interior walls
	int x1,y1,x2,y2;
	while(fscanf(in, "%i %i %i %i\n",&y1,&x1,&y2,&x2) == 4) {
		M[x1][y1][ get_dir(x1, y1, x2, y2) ] = true;
		M[x2][y2][ get_dir(x2, y2, x1, y1) ] = true;
	}

#if 0 //prints maze matrix	
	for(int i = 0; i < Ncols; i++) {
		cout << "col:" << i << " ";
		for(int j = 0; j < Nrows; j++) {
			cout << "row:" << j << " ";
			for(int k = 0; k < 4; k++) {
				cout << "k:" << k << " " << (int)M[i][j][k] << " ";
			}
		}
		cout << endl;
	}
#endif

	//source cell
	cell S = cell();
	//sink cell
	cell T = cell(Ncols-1, Nrows-1);
	//allocates stack array
	cell *stackarr = new cell[Ncols*Nrows];
	//initializes stack array
	for(int i = 0; i < Ncols*Nrows; i++) {
		stackarr[i].x = 0;
		stackarr[i].y = 0;
	}
	int stack_size = 0;
	//allocates visited array
	int **V = new int*[Ncols];
	for(int i = 0; i < Ncols; i++) {
		V[i] = new int[Nrows];
	}
	//initializes visited array
	for(int i = 0; i < Ncols; i++) {
		for(int j = 0; j < Nrows; j++) {
			V[i][j] = 0;
		}
	}
	//calls solve function, finds path through maze which is stored in stackarr[]
	solve(M, V, stackarr, stack_size, T, S);
	//write PATH, Nrow, Ncol header
	fprintf(out, "PATH %d %d\n", Nrows, Ncols);	
	//write cells on path to file
	for(int i = 0; i < stack_size; i++) {
		fprintf(out, "%d %d\n", stackarr[i].y, stackarr[i].x);
	}
	for(int i = 0; i < Ncols; i++) {
		for(int j = 0; j < Nrows; j++) {
			delete[] M[i][j];
		}
		delete[] M[i];
	}
	delete[] M;
	for(int i = 0; i < Ncols; i++) {
		delete[] V[i];
	}
	delete[] V;	
	delete[] stackarr;
	//close open files
	fclose(out);
}

//maze solve function, recursively searches all possible paths until sink is reached, returns false is no solution
bool solve(bool ***M, int **V, cell *stackarr, int& stack_size, cell T, cell S) {
	cell point, to; //stores current position and cell to go to in the next step
	point = S; //current point  = S

	V[point.x][point.y] = 1; //current cell, mark as visited
	stackarr[stack_size] = point; //push current cell onto stack array
	stack_size++;

	if(point.x == T.x && point.y == T.y) return true; //sink has been found

	for(int d = 0; d < 4; d++) { //for all possible directions
		if(M[point.x][point.y][d]) continue; //if there is a wall in this direction, continue

		//compute to.x and to.y based off direction
		if(d == 0) {
			to.x = point.x - 1;
			to.y = point.y;
		}
		else if(d == 1) {
			to.x = point.x;
			to.y = point.y - 1;
		}
		else if(d == 2) {
			to.x = point.x + 1;
			to.y = point.y;
		}
		else if(d == 3) {
			to.x = point.x;
			to.y = point.y + 1;
		}
		if(V[to.x][to.y] == 1) continue; //to cell has already been visited

		if(solve(M, V, stackarr, stack_size, T, to) == true) return true; //sink has been found
	}

	V[point.x][point.y] = 0; //set visited to false since popping
	stackarr[stack_size] = cell(); //pop stack
	stack_size--; 
	return false;
}
