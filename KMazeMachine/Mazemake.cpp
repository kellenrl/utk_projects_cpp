#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <unistd.h>

#include "dset.h"

using namespace std;

struct cell {
	cell() { x = 0; y = 0; }
	cell(int x, int y) { this->x = x; this->y = y; }

	int x;
	int y;
};

void swap(cell &c1, cell &c2) { 
	cell temp;
	temp = c1;
	c1 = c2;
	c2 = temp;
};

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "usage: ./Mazemake nrow ncol maze.txt\n");
		return 1;
	}

	int Nrows = atoi(argv[1]);
	int Ncols = atoi(argv[2]);

	int N = ((Ncols - 1)*Nrows) + ((Nrows - 1)*Ncols);
	cell wall[N][2];

	int lastCol = Ncols - 1;
	int lastRow = Nrows - 1;

	//populate wall[][] with interior grid walls
	int k = 0;
	while(k < N) {
		for(int i = 0; i < lastCol; i++) { //lastCol
			for(int j = 0; j < Nrows; j++) { //Ncols 
				wall[k][0] = cell(i,j); //current
				wall[k][1] = cell(i+1,j); //right
				k++;
			}
		}
		for(int i = 0; i < Ncols; i++) { //Nrows
			for(int j = 0; j < lastRow; j++) { //lastRow
				wall[k][0] = cell(i,j); //current
				wall[k][1] = cell(i,j+1); //below
				k++;
			}
		}
	}
	//randomnly permute list of walls	
	srand(time(NULL));
	for(int i = k-1; i > 0; i--) {
		int randnum = rand() % k;
		swap(wall[i][0], wall[randnum][0]);
		swap(wall[i][1], wall[randnum][1]);
	}
	
	FILE* out;
	out = fopen(argv[3], "w");
	fprintf(out, "MAZE %s %s\n", argv[1], argv[2]);

	dset ds(Ncols*Nrows);
	int i = 0;
	while(i < N) {
		//convert 2D to 1D
		int ii = wall[i][0].x + (wall[i][0].y * Ncols);
		int jj = wall[i][1].x + (wall[i][1].y * Ncols);

		//if not already merged
		if(ds.find(ii) != ds.find(jj)) {
			ds.merge(ii,jj);
		}
		//cant merge, wall exists - print wall
		else {
			fprintf(out, "%3d %3d %3d %3d\n", wall[i][0].y, wall[i][0].x, wall[i][1].y, wall[i][1].x);
		}
		if(ds.size() == 1) break;
		i++;
	}
	i++;

	//print remaining walls
	for(;i < N; i++) {
			fprintf(out, "%3d %3d %3d %3d\n", wall[i][0].y, wall[i][0].x, wall[i][1].y, wall[i][1].x);
	}
	fclose(out);
	return 0;
}
