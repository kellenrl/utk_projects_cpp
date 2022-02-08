#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>

#include "PPM.h"

using namespace std;

void PPM::read(char fileName[]) { 
	ifstream fin;
	fin.open(fileName);
	if(fin.is_open(), ios::binary){
		fin >> magicid >> Ncols >> Nrows >> maxvalue;
		if(magicid != "P6" || maxvalue != 255) {
			fin.close();
			return;
		}
		fin.ignore(256, '\n');
		int size = (Ncols * Nrows);
		pixels = new RGB[size];
		int n = (size*3);
		int numRead, totalRead = 0;
		while(1) {
			fin.read((char*)pixels, n);
			numRead = fin.gcount();
			totalRead += numRead;
			if(numRead == 0 && fin.eof()) break;
		}
		img = &pixels;
		fin.close();
		if(totalRead != size*3) return;
	}	
	return;
}

void PPM::write(char fileName[]) {
	if(Ncols == 0 || Nrows == 0) {
		cerr << "No image to save." << endl;
	}
	ofstream fout;
	fout.open(fileName, ios::binary);
	fout << "P6\n" << Ncols << " " << Nrows << "\n" << maxvalue << "\n"; 
	int size = (Ncols * Nrows);
	int n = (size*3);
	fout.write((char *)pixels, n);
	fout.close();
}
