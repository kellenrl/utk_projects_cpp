#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>
#include <set>
#include <fstream>

using namespace std;

void set_oldfriends(vector <string> &name, vector< vector<int> > &friends, int M0, int M1) {
	int N = (int)name.size();
	//initialize 2D-vector-array friends (hint: use vector::assign())
	friends.resize(N);
	for(int i = 0; i < N; i++) {
		friends[i].assign(N,0);
	}
	for (int i=0; i<N; i++) {
		set<int> doknow;

		//initialize M (random number of friends: function of M0, M1)
		int M = M0 + rand() % M1;

		while ((int)doknow.size() < M) {
			//compute j (random friend index: hint j != i)
			int j = rand() % N;
			while(j == i) {
				j = rand() % N;
			}
			doknow.insert(j);
		}
		//update the corresponding pairs of friends entries
		set<int>::iterator it = doknow.begin();
		while(it != doknow.end()) {
			friends[i][*it] = 1;
			friends[*it][i] = 1;
			it++;
		}
	 }
}

void set_newfriends(vector< vector<int> > &friends, vector< vector<int> > &new_friends) {
	int N = (int)friends.size();
	new_friends.resize(N);
	for(int i = 0; i < N; i++) {
		new_friends[i].assign(N,0);
	}
	for (int i=0; i<N; i++) {
		for (int j = 0; j < N; j++) {
			if(friends[i][j] == 1) {		
				for (int k = 0; k < N; k++ ) {
					if(friends[j][k] == 1) {
						if (k != i && friends[i][k] != 1) {
						//update the corresponding pairs of new_friends entries
						new_friends[i][k] = 1;
						new_friends[k][i] = 1;
						}
					}
				}
			}		
		}	
	}	
}

void writetofile(const char *fname, vector<string> &name, vector< vector<int> > &friends) {
	//open file
	ofstream fout;
	fout.open(fname);

	int N = (int)name.size();
	//determine max name length
	int maxlength = 0;
	for(int i = 0; i < N; i++) {
		int curlength = name[i].length();
		if(curlength > maxlength) maxlength = curlength;
	}
	for (int i=0; i<N; i++) {
		int count = 0;
		//pretty-print name[i] and name[j] (see assignment) 
		fout << setw(maxlength+1) << left << name[i] << ":";
		for(int j = 0; j < N; j++) {
			if(friends[i][j] == 1) {
				if(count % 8 == 0 && count != 0) {
					fout << endl;
					fout << setw(maxlength+1) << left << name[i] << ":";	
				}
			fout << " " << setw(maxlength) << left << name[j];
			count++;
			}
		}
		fout << endl;
	}
	//close file
	fout.close();
}	

const string SEED = "-seed";	

int main(int argc, char *argv[]) {
	//print usage message and exit if invalid
	int seed = 0;
	int mode = 0;
	if(argc != 1 && argc != 3) {
		cerr << "usage: ./Friendnet1 [-seed N]" << endl;
		return 1;
	}
	if(argc == 1) {
		mode = 1;
	} else {
		if(argv[1] != SEED) {
			cerr << "usage: ./Friendnet1 [-seed N]" << endl;
			return 1; 
		} else {
			seed = atoi(argv[2]);
		}
	}
	//seed random number generator
	if(mode == 1) {
		srand(time(NULL));
	} else {
		srand(seed);
	}
	vector<string> name;
	//read strings from stdin into name vector
	string n;
	while(cin >> n) {
		name.push_back(n);
	}
	int M0 = 1; //min number of friends
	int M1 = 3; //max number of friends

	//declare 2D-vector-array called friends
	vector< vector<int> > friends;
	//declare 2D-vector-array called new_friends
	vector< vector<int> > new_friends;

	set_oldfriends(name, friends, M0, M1);
	writetofile("doknow1.txt", name, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow1.txt", name, new_friends);
}
