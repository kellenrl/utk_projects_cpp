#ifndef __PPM_H__
#define __PPM_H__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct RGB { 
  RGB() { R = 0; G = 0; B = 0; }

  unsigned char R, G, B;
};

class PPM { 
  public:
    PPM() { img = NULL; Ncols = 0; Nrows = 0; magicid = ""; maxvalue = 0; }
    ~PPM() { delete[] pixels; }

	void read(char fileName[]);
	void write(char fileName[]);

	RGB* operator[](int i) { return &pixels[i*Ncols]; }

	int get_Nrows() { return this->Nrows; }
	int get_Ncols() { return this->Ncols; }

  private:
	RGB **img;
	RGB *pixels;
	int Ncols;
	int Nrows;
	string magicid;
	int maxvalue;
};

#endif
