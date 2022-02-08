#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include <bitset>
#include "PPM.h"
#include "Rnumgen.h"

using namespace std;

const char ETX = 0x3;

struct pixel { //struct to hold pixel index info for pixel_list
	pixel(int n_i = 0, int n_j = 0) {
		i = n_i;
		j = n_j;
	}
	int i;
	int j;
};

void set_pixel_list(PPM &img, vector<pixel> &pixel_list) { //creates pixel targets list of every pixel at even row/col index
	for(int i = 0; i < img.get_Nrows(); i += 2) {
		for(int j = 0; j < img.get_Ncols(); j += 2) {
			pixel_list.push_back(pixel(i,j));
		}
	}
}

void encode(PPM &img, vector<pixel> &pixel_list) { //encodes img eith msg from user using target pixels from pixel_list
	char c = 0;
	unsigned char temp = 0;
	unsigned char x = 0;
	int count = 0;
	while(cin.get(c) && count < img.get_Nrows()*img.get_Ncols() - 7){
		for(int j = 0; j < 7; j++) {
			temp = c; // temp is bit from msg to be encoded
			if(count % 3 == 0) {
				x = img[pixel_list[count].i][pixel_list[count].j].R; //x is char from img
			} else if ( count % 3 == 1) {
				x = img[pixel_list[count].i][pixel_list[count].j].G; //x is char from img
			} else {
				x = img[pixel_list[count].i][pixel_list[count].j].B; //x is char from img
				}
			temp = temp >> j; //shift jth bit to LSB
			temp = temp & 0x1; //zero out all other bits
			x &= 0xFE; //zeros out LSB of pixel bit
			x |= (temp & 0x1); //x LSB now equals temp LSB
			if(count % 3 == 0) {
				img[pixel_list[count].i][pixel_list[count].j].R = x; //img pixel LSB now encoded
			} else if ( count % 3 == 1) {
				img[pixel_list[count].i][pixel_list[count].j].G = x; //img pixel LSB now encoded
			} else {
				img[pixel_list[count].i][pixel_list[count].j].B = x; //img pixel LSB now encoded
			}
			count++; //ups count for pixel list, gets next pixel to be encoded
		}
	}
	for(int j = 0; j < 7; j++) { //encodes ETX char at end of msg for use by decoder
		temp = ETX;
		if(count % 3 == 0) {
			x = img[pixel_list[count].i][pixel_list[count].j].R; //x is char from img
		} else if ( count % 3 == 1) {
			x = img[pixel_list[count].i][pixel_list[count].j].G; //x is char from img
		} else {
			x = img[pixel_list[count].i][pixel_list[count].j].B; //x is char from img
		}
		temp = temp >> j;
		temp = temp & 0x1;
		x &= 0xFE;
		x |= (temp & 0x1);
		if(count % 3 == 0) {
			img[pixel_list[count].i][pixel_list[count].j].R = x; //img pixel LSB now encoded
		} else if ( count % 3 == 1) {
			img[pixel_list[count].i][pixel_list[count].j].G = x; //img pixel LSB now encoded
		} else {
			img[pixel_list[count].i][pixel_list[count].j].B = x; //img pixel LSB now encoded
		}
		count++; //ups count for pixel list, gets next pixel to be encoded
	}
}

void decode(PPM  &img, vector<pixel> &pixel_list) { //decodes msg from img based on target pixels in pixel_list
	unsigned char temp = 0;
	unsigned char c = 0;
	int count = 0;
	while(c != ETX) {
		c = 0;
		temp = 0;
		for(int j = 0; j < 7; j++) {
			if(count % 3 == 0) {
				temp = img[pixel_list[count].i][pixel_list[count].j].R;
			} else if (count % 3 == 1) {
				temp = img[pixel_list[count].i][pixel_list[count].j].G;
			} else {
				temp = img[pixel_list[count].i][pixel_list[count].j].B;
			}
			temp = temp & 0x1; //zeros out all except LSB
			temp = temp << j; //shifts LSB to jth position
			c |= temp; //c now equals c + temp
			count++;
		}
		if(c == ETX) break;
		cout.put(c);
	}
}

int checkFile(char[]);  // checks if file valid (exists and not empty)

const int VALID_FILE = 0;
const int INVALID_FILE = -1;
const int MAX_FILENAME = 50;
const string ENCODE = "-encode";
const string DECODE = "-decode";

int main(int argc, char *argv[]) {
	string mode;
	char fileName[MAX_FILENAME];
	char newName[MAX_FILENAME];
	int fileStatus;
	if(argc != 3 && argc != 4) {
		cerr << "usage: ./Crypto -encode|decode [-seed=N] image.ppm" << endl;
	} else if(argv[1] != ENCODE && argv[1] != DECODE) {
		cerr << "usage: ./Crypto -encode|decode [-seed=N] image.ppm" << endl;
	} else {
		if(argc == 3) { 
			mode = argv[1];
			strcpy(fileName, argv[2]);
			fileStatus = checkFile(fileName);
			if(fileStatus == INVALID_FILE) return 1;
		} else {
			mode = argv[1];
			strcpy(fileName, argv[3]);
			fileStatus = checkFile(fileName);
			if(fileStatus == INVALID_FILE) return 1;
		}
		if(mode == ENCODE) {
			PPM img;
			img.read(fileName);
			vector<pixel> pixel_list;
			set_pixel_list(img, pixel_list);
			encode(img, pixel_list);
			strcpy(newName, strtok(fileName, "."));
			strcat(newName, "_wmsg.ppm");
			img.write(newName);
		} else {
			PPM img;
			img.read(fileName);
			vector<pixel> pixel_list;
			set_pixel_list(img, pixel_list);
			decode(img, pixel_list);
		}
	}
		return 0;
}

int checkFile(char fileName[]){
	ifstream fin;
	char c;
	fin.open(fileName);
	if(fin.fail()) {
		cout << "Input file " << fileName << " does not exist." << endl;
		return (INVALID_FILE);
	}
	fin.get(c);
	if(fin.eof()) {
		fin.close();
		cout << "Input file " << fileName << " is empty." << endl;
		return (INVALID_FILE);
	} else {
		fin.putback(c);
		return (VALID_FILE);
	}
}
