#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>
#include <limits>
#include <utility>

using namespace std;
	
const int VERT = 1; //delete
const int HORZ = 2; //insert
const int DIAG = 4; //match
const int DELcost = 1;
const int INScost = 1;
const int MATcost = 0;
const string empty = "-";

template <typename T>
class matrix {
public:
	matrix() { data = NULL; }

	~matrix() {
		if(data) {
			if(data[0]) {
				delete [] data[0];
			}
		delete [] data;

		data = NULL;
		}
	}

	void assign(int _Nrows, int _Ncols, const T &value=T()) {
		Nrows = _Nrows;
		Ncols = _Ncols;

		data = new T *[Nrows];
		data[0] = new T [Nrows*Ncols];
		for(int i = 1; i < Nrows; i++) {
			data[i] = data[i-1] + Ncols;
		}

		for(int i = 0; i < Nrows; i++) {
			for(int j = 0; j < Ncols; j++) {
				data[i][j] = value;
			}
		}
	}

	int get_Nrows() const { return Nrows; }
	int get_Ncols() const { return Ncols; }

	T * operator[] (int i) { return data[i]; }

private:
	int Nrows, Ncols;
	T **data;
};

struct seq {
	int x; //line number in text2
	int y; //line number in text1
	int op; //operation 1 = VERT, 2 = HORZ, 4 = DIAG
};

class LCS {
  public:
    void text1_push_back(string s) { this->text1.push_back(s); }
    void text2_push_back(string s) { this->text2.push_back(s); }
	void compute_alignment();
	void print_alignment();
	void report_difference();
	int get_m() { return this->m; }
	int get_n() { return this->n; }
	int get_link() { return link[get_m()-1][get_n()-1]; }

  private:
	void report_difference(int&, int&, int&);
	void condense_print();

	int m, n;
    // text1 buffer
	vector<string> text1;
    // text2 buffer
	vector<string> text2;

	stack<seq> trace;

	matrix<int> cost; //edit costs
	matrix<int> link; //alignment info

	int Nalign; //num equal costs alignments

};

void LCS::compute_alignment() {
	m = text1.size();
	n = text2.size();

	cost.assign(m, n, numeric_limits<int>::max());
	link.assign(m, n, numeric_limits<int>::max());

	cost[0][0] = 0;
	link[0][0] = 0;

	for(int i = 1; i < m; i++) {
		cost[i][0] = cost[i-1][0] + DELcost;
		link[i][0] = VERT;
	}

	for(int j = 1; j < n; j++) {
		cost[0][j] = cost[0][j-1] + INScost;
		link[0][j] = HORZ;
	}

	for(int i = 1; i < m; i++) {
		for(int j = 1; j < n; j++) {
			if(text1[i] == text2[j]) {
				cost[i][j] = cost[i-1][j-1] + MATcost;
				link[i][j] = DIAG;
			}
		

			int delcost = cost[i-1][j] + DELcost;
			if(delcost < cost[i][j]) {
				cost[i][j] = delcost;
				link[i][j] = VERT;
			}

			int inscost = cost[i][j-1] + INScost;
			if(inscost < cost[i][j]) {
				cost[i][j] = inscost;
				link[i][j] = HORZ;
			}
		}
	}
}
void LCS::report_difference() {
	int i = get_m()-1;
	int j = get_n()-1;
	int link_ij = get_link();
	report_difference(i, j, link_ij);	
	condense_print();
}

void LCS::report_difference(int &i, int &j, int &link_ij) {
	if(link_ij == 0) {
		return;
	}
	seq temp;
	temp.x = i;
	temp.y = j;
	if(link_ij == DIAG) {
		temp.op = DIAG; //match
		trace.push(temp);
		i = i-1;
		j = j-1;
	} else if(link_ij == VERT) {
		temp.op = VERT; //delete
		trace.push(temp);
		i = i-1;
	} else {
		temp.op = HORZ; //insert
		trace.push(temp);
		j = j-1;
	}
	link_ij = link[i][j];
	report_difference(i, j, link_ij);
}

void LCS::condense_print() {
	int matchCount = 0;
	int delCount = 0;
	int insCount = 0;
	int delStart = 0;
	int insStart = 0;
	while(!trace.empty()){
		seq cur = trace.top();
		trace.pop();
		if(cur.op == DIAG) {
			matchCount++;
		} else if(cur.op == VERT) {
			delCount++;
			if(delCount == 1) { delStart = cur.x; }
		} else if(cur.op == HORZ) {
			insCount++;
			if(insCount == 1) { insStart = cur.y; }
		}
		if(matchCount > 0) {
			if(delCount > 0 && insCount > 0) {
				cout << delStart << "," << delStart+delCount-1 << "c" << insStart << "," << insStart+insCount-1 << endl; 
				for(int i = delStart; i < delStart+delCount; i++) {
					cout << "< " <<  text1[i] << endl;		
				}
				cout << "---" << endl;
				for(int i = insStart; i < insStart+insCount; i++) {
					cout << "> " << text2[i] << endl;
				}
			}
			else if(delCount > 0) {
				if(delCount > 1) {
					cout << delStart << "," << delStart+delCount-1 << "d" << insStart<< endl;
				} else {
					cout << delStart << "d" << insStart+delCount-1 << endl; 
				}
				for(int i = delStart; i < delStart+delCount; i++) {
					cout << "< " <<  text1[i] << endl;
				}
			}
			else if(insCount > 0) {
				if(insCount > 1) {
					cout << delStart << "a" << insStart << "," << insStart+insCount-1 << endl;
				} else {
					cout << delStart << "a" << insStart+insCount-1 << endl; 
				}
				for(int i = insStart; i < insStart+insCount; i++) {
					cout << "> " <<  text2[i] << endl;
				}
			}
			matchCount = 0;
			delCount = 0;
			insCount = 0;
		}
	}
	if(delCount > 0) {
		if(delCount > 1) {
			cout << delStart << "," << delStart+delCount-1 << "d" << delStart << endl;
		} else {
			cout << delStart << "d" << insStart+delCount-1 << endl; 
		}
		for(int i = delStart; i < delStart+delCount; i++) {
			cout << "< " <<  text1[i] << endl;
		}
	}
	else if(insCount > 0) {
		if(insCount > 1) {
			cout << insStart << "a" << insStart << "," << insStart+insCount-1 << endl;
		} else {
			cout << insStart << "a" << insStart+insCount-1 << endl; 
		}
		for(int i = insStart; i < insStart+insCount; i++) {
			cout << "> " <<  text2[i] << endl;
		}
	}
	else if(delCount > 0 && insCount > 0) {
		cout << delStart << "," << delStart+delCount-1 << "c" << insStart << "," << insStart+insCount-1 << endl; 

		for(int i = delStart; i < delStart+delCount; i++) {
			cout << "< " <<  text1[i] << endl;
		}
		cout << "---" << endl;
		for(int i = insStart; i < insStart+insCount; i++) {
			cout << "> " << text2[i] << endl;
		}
	}	
}
void LCS::print_alignment() {
	stack<string> alignment_x;
	stack<string> alignment_y;

	int i = m-1, j = n-1, link_ij = 0;
	while((link_ij=link[i][j]) != 0) {
		if(link_ij == DIAG) {
			alignment_x.push(text1[i]);
			alignment_y.push(text2[j]);
			i = i-1, j = j-1;
		} else if(link_ij == VERT) {
			alignment_x.push(text1[i]);
			alignment_y.push(text2[0]);
			i = i - 1;
		} else { // link_ij == HORZ
			alignment_x.push(text1[0]);
			alignment_y.push(text2[j]);
			j = j - 1;
		}
	}

	cout << "** " << endl;
	while(!alignment_x.empty()) {
		cout << alignment_x.top() << endl;
		alignment_x.pop();
	}
	cout << "\n";

	cout << "** " << endl;
	while(!alignment_y.empty()) {
		cout << alignment_y.top() << endl;
		alignment_y.pop();
	}
	cout << "\n";
}

int main(int argc, char *argv[])
{
  // check two input files are specified on command line
  if(argc != 3) {
	cerr << "usage: ./Diff8 file1 file2" << endl;
	return 1;
  }

  LCS lcs;  // instantiate your "LCS based diff" object

  // read the text from file1 into the lcs.text1 buffer
  ifstream fin;
  fin.open(argv[1]);
  string s;
  lcs.text1_push_back(empty);
  while(getline(fin, s)) {
	lcs.text1_push_back(s);
  }
  fin.close();

  // read the text from file2 into the lcs.text2 buffer
  fin.open(argv[2]);
  lcs.text2_push_back(empty);
  while(getline(fin, s)) {
	lcs.text2_push_back(s);
  }
  fin.close();
  
  lcs.compute_alignment();
  lcs.report_difference();
}
