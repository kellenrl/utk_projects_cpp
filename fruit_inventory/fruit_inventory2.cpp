#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

const int VALID_FILE = 0;
const int INVALID_FILE = -1;
const string SORTALL = "-sortall";
const string SORTONE = "-sortone";
const string INORDER = "-inorder";

struct fruit {
	bool operator<(const fruit& fruit2) const {return name < fruit2.name; };
	bool operator<=(const fruit& fruit2) const {return name <= fruit2.name; };
	bool operator==(const fruit& fruit2) const {return name == fruit2.name; };
	bool operator+=(const fruit& fruit2) {return quantity += fruit2.quantity; };
	bool operator>(const fruit& fruit2) const {return name > fruit2.name; };
	bool operator>=(const fruit& fruit2) const {return name >= fruit2.name; };
	bool operator!=(const fruit& fruit2) const {return name != fruit2.name; };
	
	string name;
	float quantity;
	float cost;
	fruit() { name = ""; quantity = 0; cost = 0;};
};

struct node {
	node* nextPtr;
	node* prevPtr;
	fruit item;
	node() { nextPtr = prevPtr = NULL; item = fruit();};
};

int checkFile(char[]); // checks if file valid (exists and not empty)

int main(int argc, char* argv[])
{
	ifstream fin;
	int fileStatus;
	char fileName[50];
	fruit tempFruit;
	node *linked_list = new node();
	string mode;

	if(argc != 3) {
		cerr << "Usage: ./Fruit2 -inorder|sortall|sortone filename" << endl;
		exit(1);
	}
	mode = argv[1];
	if(argc != 3 || (mode != INORDER && mode != SORTALL && mode != SORTONE)) {
		cerr << "Usage: ./Fruit2 -inorder|sortall|sortone filename" << endl;
		exit(1);
	} else {
		if(mode == INORDER){
			strcpy(fileName, argv[2]);
			fileStatus = checkFile(fileName);
			if(fileStatus == VALID_FILE) {
				fin.open(fileName);
				fin >> tempFruit.name;
				fin >> tempFruit.quantity;
				fin >> tempFruit.cost;
				node *newNode = new node();
				newNode->prevPtr = linked_list;
				linked_list->nextPtr = newNode;
				newNode->item = tempFruit;
				node *curPtr = newNode;
				while(fin >> tempFruit.name) {
					fin >> tempFruit.quantity;
					fin >> tempFruit.cost;
					newNode = new node();
					newNode->prevPtr = curPtr;
					curPtr->nextPtr = newNode;
					newNode->item = tempFruit;
					curPtr = newNode;
				}
				curPtr = linked_list->nextPtr;
				float tempCost = 0;
				float totalCost = 0;
				while(curPtr != NULL) {
					cout << setfill('.') << setw(20) << left << curPtr->item.name << " ";
					cout << setfill(' ') << fixed << setprecision(2) << setw(5) << right << curPtr->item.quantity << " x ";
					cout << setw(4) << curPtr->item.cost << " = ";
					tempCost = (curPtr->item.quantity * curPtr->item.cost);
					totalCost += tempCost;
					cout << setw(7) << tempCost << "  : ";
					cout << setw(8) << totalCost << endl;
					curPtr = curPtr->nextPtr;
				}
			}
		} else if(mode == SORTALL){
			strcpy(fileName, argv[2]);
			fileStatus = checkFile(fileName);
			if(fileStatus == VALID_FILE) {
				fin.open(fileName);
				node *curPtr;
				curPtr = linked_list;
				while(fin >> tempFruit.name){ //read in data from file
					fin >> tempFruit.quantity;
					fin >> tempFruit.cost;
					node *newNode = new node();
					newNode->item = tempFruit;
					if(linked_list->nextPtr == NULL){ // if list is empty, insert node
						newNode->prevPtr = linked_list;
						linked_list->nextPtr = newNode;
						curPtr = newNode; // curPtr now pts to first node in the list
					}
					else {
						while (curPtr != newNode){
							if(newNode->item < curPtr->item){ // if new item < current item, insert in front
								curPtr->prevPtr->nextPtr = newNode;
								newNode->prevPtr = curPtr->prevPtr;
								curPtr->prevPtr = newNode;
								newNode->nextPtr = curPtr;
								newNode->item = tempFruit;
								curPtr = newNode;
							} else if(curPtr->nextPtr == NULL){ // if at end of list, insert in back
								newNode->prevPtr = curPtr;
								curPtr->nextPtr = newNode;
								newNode->item = tempFruit;
								curPtr = newNode;
							} else if(newNode->item == curPtr->item && newNode->item != curPtr->nextPtr->item){
								curPtr->nextPtr->prevPtr = newNode;
								newNode->nextPtr = curPtr->nextPtr;
								newNode->prevPtr = curPtr;
								curPtr->nextPtr = newNode;
								curPtr = newNode;
							} else {
								curPtr = curPtr->nextPtr;
							}
						}
					}
					curPtr = linked_list->nextPtr;
				}
				curPtr = linked_list->nextPtr;
				float tempCost = 0;
				float totalCost = 0;
				while(curPtr != NULL) {
					cout << setfill('.') << setw(20) << left << curPtr->item.name << " ";
					cout << setfill(' ') << fixed << setprecision(2) << setw(5) << right << curPtr->item.quantity << " x ";
					cout << setw(4) << curPtr->item.cost << " = ";
					tempCost = (curPtr->item.quantity * curPtr->item.cost);
					totalCost += tempCost;
					cout << setw(7) << tempCost << "  : ";
					cout << setw(8) << totalCost << endl;
					curPtr = curPtr->nextPtr;
				}
			}
		} else if(mode == SORTONE){
			strcpy(fileName, argv[2]);
			fileStatus = checkFile(fileName);
			if(fileStatus == VALID_FILE) {
				fin.open(fileName);
				node *curPtr;
				curPtr = linked_list;
				while(fin >> tempFruit.name){ //read in data from file
					fin >> tempFruit.quantity;
					fin >> tempFruit.cost;
					node *newNode = new node();
					newNode->item = tempFruit;
					if(linked_list->nextPtr == NULL){ // if list is empty, insert node
						newNode->prevPtr = linked_list;
						linked_list->nextPtr = newNode;
						curPtr = newNode; // curPtr now pts to first node in the list
					}
					else {
						while (curPtr != newNode){
							if(newNode->item < curPtr->item){ // if new item < current item, insert in front
								curPtr->prevPtr->nextPtr = newNode;
								newNode->prevPtr = curPtr->prevPtr;
								curPtr->prevPtr = newNode;
								newNode->nextPtr = curPtr;
								newNode->item = tempFruit;
								curPtr = newNode;
							} else if(curPtr->nextPtr == NULL && curPtr->item != newNode->item){ // if at end of list, insert in back
								newNode->prevPtr = curPtr;
								curPtr->nextPtr = newNode;
								newNode->item = tempFruit;
								curPtr = newNode;
							} else if(newNode->item == curPtr->item){ // if equal, add quantities and delete newNode
								curPtr->item += newNode->item;
								delete newNode;
								break;
							} else {
								curPtr = curPtr->nextPtr;
							}
						}
					}
					curPtr = linked_list->nextPtr;
				}
				curPtr = linked_list->nextPtr;
				float tempCost = 0;
				float totalCost = 0;
				while(curPtr != NULL) {
					cout << setfill('.') << setw(20) << left << curPtr->item.name << " ";
					cout << setfill(' ') << fixed << setprecision(2) << setw(5) << right << curPtr->item.quantity << " x ";
					cout << setw(4) << curPtr->item.cost << " = ";
					tempCost = (curPtr->item.quantity * curPtr->item.cost);
					totalCost += tempCost;
					cout << setw(7) << tempCost << "  : ";
					cout << setw(8) << totalCost << endl;
					curPtr = curPtr->nextPtr;
				}
				fin.close();
				node* temp = new node();
				while(linked_list != NULL){ // deallocates all nodes in the list
					temp = linked_list;
					linked_list = linked_list->nextPtr;
					delete temp;
				}
			}
			
		}
		return 0;
	}
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

