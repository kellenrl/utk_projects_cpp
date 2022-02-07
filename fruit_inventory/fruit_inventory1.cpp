#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int VALID_FILE = 0;
const int INVALID_FILE = -1;
const string SORTALL = "-sortall";
const string SORTONE = "-sortone";
const string INORDER = "-inorder";

struct fruit {
	bool operator<(const fruit& fruit2) const
	{
		return name < fruit2.name;
	};
	string name;
	float quantity;
	float cost;
};

int checkFile(char[]); // checks if file valid (exists and not empty)
void prettyPrint(vector<fruit>); // prints in correct format

int main(int argc, char* argv[])
{
	ifstream fin;
	int fileStatus;
	char fileName[50];
	fruit tempFruit;
	vector<fruit> array_list;
	string mode;
	
	if(argc != 3) {
		cerr << "Usage: ./Fruit2 -inorder|sortall|sortone filename" << endl;
		exit(1);
	}

	mode = argv[1];
	if(argc != 3 || (mode != INORDER && mode != SORTALL && mode != SORTONE)) {
		cerr << "Usage: ./Fruit1 -inorder|sortall|sortone filename" << endl;
		exit(1);
	} else {
		strcpy(fileName, argv[2]);
		fileStatus = checkFile(fileName);
		if(fileStatus == VALID_FILE) { // if input file valid, load data into vector
			fin.open(fileName);
			while(fin >> tempFruit.name) {
				fin >> tempFruit.quantity;
				fin >> tempFruit.cost;
				array_list.push_back(tempFruit);
			}
			fin.close();
		}
		if(mode == SORTALL || mode == SORTONE) { // if sort mode selected by user, sorts data
			stable_sort(array_list.begin(), array_list.end());
			if(mode == SORTONE) { // if sortone selected, adds duplicate item quantities and deletes duplicates
				for(size_t i = 0; i < array_list.size(); i++) {
					for(size_t j = i + 1; j < array_list.size(); j++) {
						if(array_list[i].name == array_list[j].name) {
							array_list[i].quantity += array_list[j].quantity;
							array_list.erase(array_list.begin() + j);
							j--;
						}
					}
				}
				prettyPrint(array_list);
			} else {
				prettyPrint(array_list);
			}
		} else if(mode == INORDER) { // if inorder selected, prints in order input from file
			prettyPrint(array_list);
		}
	}
	return 0;
}
// checkFile: checks validity of input file (if exists and not empty)
// Parameters: char fileName[20] - name of input file
// Returns: if file valid returns VALID_FILE, if empty or non-existent
//          returns INVALID_FILE and prints error message.
int checkFile(char fileName[])
{
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
void prettyPrint(vector<fruit> array_list)
{
	float tempCost = 0;
	float totalCost = 0;
	for(size_t i = 0; i < array_list.size(); i++) {
		cout << setfill('.') << setw(20) << left << array_list[i].name << " ";
		cout << setfill(' ') << fixed << setprecision(2) << setw(5) << right << array_list[i].quantity << " x ";
		cout << setw(4) << array_list[i].cost << " = ";
		tempCost = (array_list[i].quantity * array_list[i].cost);
		totalCost += tempCost;
		cout << setw(7) << tempCost << "  : ";
		cout << setw(8) << totalCost << endl;
	}
}

