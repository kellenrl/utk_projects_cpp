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
	bool operator<(const data& d2)const {
		if(lastname != d2.lastname){
			return lastname < d2.lastname;
		}else if(firstname != d2.firstname){
			return firstname < d2.firstname;
		}else if(phonenum != d2.phonenum){
			return phonenum < d2.phonenum;
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

const int MAX_FILENAME = 50;
const int VALID_FILE = 0;
const int INVALID_FILE = -1;
const string STL = "-stl";
const string RPIVOT = "-rpivot";

int checkFile(char[]);  // checks if file valid (exists and not empty)

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

template <typename Tdata>
void quicksort(vector<Tdata> &A, int left, int right) {
	if(left >= right) return;
	int random = left + rand() % (right-left+1);
	int pindex = random;
	Tdata pivot = A[pindex];
	swap(A[pindex], A[right]);
	
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
	quicksort(A, left, pindex-1);
	quicksort(A, pindex+1, right);
	
}

template <typename Tdata>
void quickselect(vector<Tdata> &A, int left, int k, int right) {
	if(left >= right) return;
	int random = left + rand() % (right-left+1);
	int pindex = random;
	Tdata pivot = A[pindex];
	swap(A[pindex], A[right]);
	
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
	
	if(pindex == k) return;
	if(k < pindex){
		right = pindex-1;
		quickselect(A, left, k, right);
	}else {
		left = pindex+1;
		quickselect(A, left, k, right);
	}
}

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
			if(mode == STL){
				sort(A.begin(), A.end());
				printlist(A.begin(), A.end());
			}else if(mode == RPIVOT && argc == 3) {
				int left = 0;
				int right = (int)A.size()-1;
				quicksort(A, left, right);
				printlist(A.begin(), A.end());
			}else if(mode == RPIVOT && argc == 5){
				int left = 0;
				int right = (int)A.size()-1;
				int k0 = atoi(argv[2]);
				int k1 = atoi(argv[3]);
				if(k0 < 0) k0 = 0;
				int N = (int)A.size();
				if(k1 >= N) k1 = (N-1);
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
// Parameters: char fileName[20] - name of input file
// Returns: if file valid returns VALID_FILE, if empty or non-existent
//          returns INVALID_FILE and prints error message.
int checkFile(char fileName[]){
	ifstream fin;
	char c;
	fin.open(fileName);
	if(fin.fail()) {
		cout << "Input file " << fileName << " does not exist." << endl;
		cout << endl;
		return (INVALID_FILE);
	}
	fin.get(c);
	if(fin.eof()) {
		fin.close();
		cout << "Input file " << fileName << " is empty." << endl;
		cout << endl;
		
		return (INVALID_FILE);
	} else {
		fin.putback(c);
		return (VALID_FILE);
	}
}

