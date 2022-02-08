//COSC302-003 
//Lab2 - Quicksort/Quickselect
//Author: Kellen Leland

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <iomanip>
#include <utility>


using namespace std;

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
	
private:
	string firstname;
	string lastname;
	string phonenum;
};

const int MAX_FILENAME = 50; //max length for input file
const int VALID_FILE = 0;
const int INVALID_FILE = -1;
const string STL = "-stl";
const string RPIVOT = "-rpivot";

int checkFile(char[]);  // checks if file valid (exists and not empty)

//overloaded insertion and extraction operators for data class
istream & operator>>(istream &in, data &r) {
	in >> r.firstname >> r.lastname >> r.phonenum;
	return in;
}

ostream & operator<<(ostream &out, const data &r) {
	string temp;
	temp = r.lastname + ", " + r.firstname;
	out << setw(24) << left << temp << setw(12) << left << r.phonenum;
	return out;
}

//templated quicksort function
//Parameters: vector<Tdata> A - array to be sorted
//			  int left - index of left most element
//			  int right - index of right most element
//Pre-conditions: overloaded < operator for Tdata class
//Post-condition: data in A is sorted
template <typename Tdata>
void quicksort(vector<Tdata> &A, int left, int right) {
	//chooses random pivot
	if(left >= right) return;
	int random = left + rand() % (right-left+1);
	int pindex = random;
	Tdata pivot = A[pindex];
	swap(A[pindex], A[right]);

	//partitions data
	int i = left - 1;
	int j = right;
	while(1){
		while(i <= j && A[++i] < pivot);
		while(j > i && pivot < A[--j]);
		if(i>=j) break;
		swap(A[i], A[j]);
	}
	pindex = i;
	swap(A[i], A[right]);
	//recursively calls quicksort on sub-arrays
	quicksort(A, left, pindex-1);
	quicksort(A, pindex+1, right);
	
}

//templated quickselect function
//Parameters: vector<Tdata> A - array to be searched and partitioned
//			  int left - left most element for search
//			  int right - right most element for search
//			  int k - key index 
//Post-condition: finds key if exists in array and partitions data 
template <typename Tdata>
void quickselect(vector<Tdata> &A, int left, int k, int right) {
	while(1){
		//chooses random pivot
		int random = left + rand() % (right-left+1);
		int pindex = random;
		Tdata pivot = A[pindex];
		swap(A[pindex], A[right]);
		
		//partitions data
		int i = left - 1;
		int j = right;
		while(1){
			while(i <= j && A[++i] < pivot);
			while(j > i && pivot < A[--j]);
			if(i>=j) break;
			swap(A[i], A[j]);
		}
		pindex = i;
		swap(A[i], A[right]);
	
		//if key found, returns. else runs through correct sub-array
		if(pindex == k){
			return;
		}else if(k < pindex){
			right = pindex-1;
		}else {
			left = pindex+1;
		}
	}
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

int main(int argc, char *argv[]) {
	vector<data> A;
	string mode;
	ifstream fin;
	int fileStatus;
	char fileName[MAX_FILENAME];
	data din;
	data temp;
	
	mode = argv[1];

	//command line arg check
	if((argc != 3 && argc != 5) || (mode != STL && mode != RPIVOT)){
		cerr << "usage: ./Qsort -stl | -rpivot [k0 k1] file.txt" << endl;
		return 1;
	} else {
		if(argc == 3){
			strcpy(fileName, argv[2]);
		}else if(argc == 5){
			strcpy(fileName, argv[4]);
		}
		fileStatus = checkFile(fileName);
		if(fileStatus == VALID_FILE) { // if input file valid, load data into vector
			fin.open(fileName);
			while(fin >> din) {
				A.push_back(din);
			}
			fin.close();
			if(mode == STL){ //if -stl option chosen, applies stl sort and prints
				sort(A.begin(), A.end());
				printlist(A.begin(), A.end());
			}else if(mode == RPIVOT && argc == 3) { //if -rpivot and 3 args, runs quicksort and prints
				int left = 0;
				int right = (int)A.size()-1;
				quicksort(A, left, right);
				printlist(A.begin(), A.end());
			}else if(mode == RPIVOT && argc == 5){ //if -rpivot and 5 args, runs quickselect and sorts data from k0-k1
				int left = 0; 
				int right = (int)A.size()-1;
				int k0 = atoi(argv[2]);
				int k1 = atoi(argv[3]);
				if(k0 < 0) k0 = 0; //k0 validity check
				int N = (int)A.size();
				if(k1 >= N) k1 = (N-1); //k1 validity check
				quickselect(A, left, k0, right);
				quickselect(A, k0, k1, right);
				quicksort(A, k0, k1);
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
