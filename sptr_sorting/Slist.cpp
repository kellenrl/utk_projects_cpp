#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <cstring>
#include <vector>
#include "Slist.h"

class data {
public:
	bool operator<(const data& rhs)const { // checks lastname, firstname, then phonenumber
		if(lastname != rhs.lastname){
			return lastname < rhs.lastname;
		}else if(firstname != rhs.firstname){
			return firstname < rhs.firstname;
		}else if(phonenum != rhs.phonenum){
			return phonenum < rhs.phonenum;
		}else {
			return false;
		}
	}
	friend istream & operator>>(istream &, data &);
	friend ostream & operator<<(ostream &, const data &);
	void setID(int count) {this->id = count; } //set function for private data id

private:
	string firstname;
	string lastname;
	string phonenum;
	int id;
};

//overloaded insertion and extraction operators for data class
istream & operator>>(istream &in, data &r) {
	static int count = 0;
	count++;
	r.id = count;
	in >> r.firstname >> r.lastname >> r.phonenum;
	return in;
}

ostream & operator<<(ostream &out, const data &r) {
	string temp;
	temp = r.lastname + " " + r.firstname;
	out << " " << setw(24) << left << temp << setw(12) << left << r.phonenum << setw(9) 
		<< right << r.id;
	return out;
}

//templated print function
//Parameters: begin iterator, end iterator
//Pre-condition: overloaded << operator for class T
template <typename T>
void printlist(T begin,T end){
	for(T it = begin; it != end; ++it){
		cout << *it << endl;
	}
}

int checkFile(char[]);  // checks if file valid (exists and not empty)

const int MAX_FILENAME = 50; //max length for input file
const int VALID_FILE = 0;
const int INVALID_FILE = -1;
const string MERGESORT = "-mergesort";
const string QUICKSORT = "-quicksort";
const string QSORT_R3 = "-qsort_r3";

int main(int argc, char *argv[]) {
	ifstream fin;
	data din;
	slist<data> A;
	int fileStatus;
	char fileName[MAX_FILENAME];
	string mode;

	if(argc != 3) {
		cerr << "usage: ./Slist -mergesort|quicksort|qsort_r3 file.txt" << endl;
		return 1;
	} else if(argv[1] != MERGESORT && argv[1] != QUICKSORT && argv[1] != QSORT_R3) {
		cerr << "usage: ./Slist -mergesort|quicksort|qsort_r3 file.txt" << endl;
		return 1;
	} else {
		mode = argv[1];
		strcpy(fileName, argv[2]);
		fileStatus = checkFile(fileName);
		if(fileStatus == VALID_FILE) {
			fin.open(fileName);
			while (fin >> din) { //loads slist with data from file
				A.push_back(din);
			}
			fin.close();
			if(mode == QUICKSORT) { //applies std::sort indirectly through sptr vector
				A.sort(mode);
				printlist(A.begin(), A.end());
			} else if(mode == MERGESORT) { //applies std::stable_sort indirectly through sptr vector
				A.sort(mode);
				printlist(A.begin(), A.end());
			} else { //applies qsort_r3 with 3 way partitioning indirectly through sptr vector
				A.sort(mode);
				printlist(A.begin(), A.end());
			}
		}
	}
	return 0;
}

// checkFile: checks validity of input file (if exists and not empty)
// Parameters: char fileName[MAX_FILENAME] - name of input file
// Returns: if file valid returns VALID_FILE, if empty or non-existent
//          returns INVALID_FILE and prints error message.
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

