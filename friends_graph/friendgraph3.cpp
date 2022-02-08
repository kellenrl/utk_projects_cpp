#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>
#include <set>
#include <fstream>

using namespace std;

void set_oldfriends(vector <string> &name, vector< set<int> > &friends, int M0, int M1) {
	int N = (int)name.size();
	friends.resize(N);
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
			friends[i].insert(*it);
			friends[*it].insert(i);
			it++;
		}
	}
}

void set_newfriends(vector< set<int> > &friends, vector< set<int> > &new_friends) {
	int N = (int)friends.size();
	new_friends.resize(N);
	for (int i=0; i<N; i++) {
		for(set<int>::iterator iti = friends[i].begin(); iti != friends[i].end(); iti++) {
			for(set<int>::iterator itj = friends[*iti].begin(); itj != friends[*iti].end(); itj++) {
				if(*itj != i) { 
					int match = 0;
					for(set<int>::iterator itk = friends[i].begin(); itk != friends[i].end(); itk++) {
						if(*itk == *itj) {
							match++;	
						}
					}
					if(match == 0) {
						new_friends[i].insert(*itj);
					}
				}
			}
		}
	}	
	#if 0 
	for (int i=0; i<N; i++) {
		for(int j = 0; j < friends[i].size(); j++) {
			for(int k = 0; k < friends[friends[i][j]].size(); k++) {
				if(friends[friends[i][j]][k] != i) { 
					int match = 0;
					for(int l = 0; l < friends[i].size(); l++) {
						if(friends[i][l] == friends[friends[i][j]][k]) {
							match++;
						}
					}
					if(match == 0) {
						new_friends[i].insert(friends[friends[i][j]][k]); 
					}
				}
			}
		}
	}	
	#endif
}

void writetofile(const char *fname, vector<string> &name, vector< set<int> > &friends) {
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
		set<int>::iterator tempit;
		int count = 0;
		//pretty-print name[i] and name[j] (see assignment) 
		fout << setw(maxlength+1) << left << name[i] << ":";
		set<int>::iterator it;
		for(it = friends[i].begin(); it != friends[i].end(); it++) {
			if(count % 8 == 0 && count != 0) {
				fout << endl;
				fout << setw(maxlength+1) << left << name[i] << ":";	
			}
			fout << " " << setw(maxlength) << left << name[*it];
			count++;
		}
		fout << endl;
	}
	//close file
	fout.close();
}	

const string SEED = "-seed";	

int main(int argc, char *argv[]) {
	//parse argc, argv arguments
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
	vector< set<int> > friends;
	//declare 2D-vector-array called new_friends
	vector< set<int> > new_friends;

	set_oldfriends(name, friends, M0, M1);
	writetofile("doknow3.txt", name, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow3.txt", name, new_friends);
}
