#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <cctype>
#include <math.h>
#include <cmath>

using namespace std;

int checkFile(const char[]);  // checks if file valid (exists and not empty)

const int VALID_FILE = 0;
const int INVALID_FILE = -1;
const string GRAPHINFO = "-graphinfo";
const char* inputFile = "city_list.txt"; //input file

class city {
public:
	string getName() { return this->name; } //get functions for private member data
	string getType() { return this->type; }
	int getZone() { return this->zone; }
	int getPopulation() { return this->population; }
	float getLatitude() { return this->latitude; }
	float getLongitude() { return this->longitude; }

	friend istream& operator>>(istream& , city&);
	friend ostream& operator<<(ostream& , const city&);

	friend class costtable;
private:
	string name;
	string type;
	int zone;
	int population;
	float latitude;
	float longitude;

};

class costtable {
public:
	void make_tables(vector<city> cities);
	float operator()(int mode, int i, int j);
	
private:
	vector<float> table[2];
};

void costtable::make_tables(vector<city> cities) { //loads costtable table array with distance and time data
	for(int i = 0; i < (int)cities.size(); i++) {
		for(int j = 0; j <= i; j++) {
			float dist;
			float time;
			dist = 2*3982*asin(sqrt(pow(sin((cities[j].getLatitude()-cities[i].getLatitude())/2),2)+cos(cities[i].getLatitude())
					*cos(cities[j].getLatitude())*pow(sin((cities[j].getLongitude()-cities[i].getLongitude())/2),2)));	
			dist = 10*round(dist/10.0);
			table[0].push_back(dist);
			if(cities[i].type == "GATEWAY" || cities[j].type == "GATEWAY") { 
				time = dist/570;
			} else {
				time = dist/60;
			}
			table[1].push_back(time);
		}
	}
}

float costtable::operator()(int mode, int i, int j) {
	if(i < j) swap(i, j);
	if(mode == 0) return table[0][i*(i+1)/2+j];
	else return table[1][i*(i+1)/2+j];
}

istream& operator>>(istream& in, city& c) {
	float tempLat, tempLong;
	in >> c.zone >> c.name >> c.type >> tempLat >> tempLong >> c.population;
	c.latitude = tempLat*(M_PI/180);
	c.longitude = tempLong*(M_PI/180);
	return in;
}

ostream& operator<<(ostream& out, const city& c) {
	out << setw(18) << left << c.name << setw(12) << left << c.type << setw(2) << left << c.zone
		<< setw(10) << right << c.population << setw(8) << right << fixed << setprecision(2) << c.latitude*(180/M_PI)
		<< setw(8) << right << fixed << setprecision(2) << c.longitude*(180/M_PI);
	return out;
}

void read_cityinfo(vector<city> &cities) { //reads city info from inputFile and loads into cities vector
	ifstream fin;
	int fileStatus;
	fileStatus = checkFile(inputFile);
	if(fileStatus == INVALID_FILE) exit(EXIT_FAILURE);
	else {
		fin.open(inputFile);
		city temp;
		char c;
		string dump;
		fin.get(c);
		while (!fin.eof()) {
			if(c == '#' || isspace(c)) {
				getline(fin, dump);
			} else {
				fin.putback(c);
				fin >> temp;
				cities.push_back(temp);
				fin.ignore();
			}
			fin.get(c);
		}
	}
}

void write_cityinfo(const vector<city> cities) { //writes city data to city_info.txt output file
	ofstream fout;
	fout.open("city_info.txt");
	int n = (int)cities.size();
	fout << "CITY INFO (N=" << n << "):\n" << endl;
	for(int i = 0; i < n; i++) {
		fout << setw(3) << right << i << " " << cities[i] << endl;
	}
	fout.close();
}

void write_distancetable(vector<city> cities, costtable ct) { //writes distance table data to city_distancetable.txt
	ofstream fout;
	string routeStr;
	fout.open("city_distancetable.txt");
	fout << "DISTANCE TABLE:" << endl;
	for(int i = 0; i < (int)cities.size(); i++) {
		for(int j = 0; j < i; j++) {
			routeStr = cities[i].getName() + " to " + cities[j].getName() + " ";
			fout << setw(3) << right << i << " "  << setw(38) << left << setfill('.') << routeStr << setw(7) << setfill(' ') << setprecision(1)
				<< right << fixed << ct(0,i,j) << " miles" << endl;	
		}
		fout << endl;
	}
	fout.close();
}

void write_timetable(vector<city> cities, costtable ct) { //writes time table data to city_timetable.txt
	ofstream fout;
	string routeStr;
	fout.open("city_timetable.txt");
	fout << "TIME TABLE:" << endl;
	for(int i = 0; i < (int)cities.size(); i++) {
		for(int j = 0; j < i; j++) {
			routeStr = cities[i].getName() + " to " + cities[j].getName() + " ";
			fout << setw(3) << right << i << " "  << setw(38) << left << setfill('.') << routeStr << setw(5) << setfill(' ') << setprecision(1)
				<< right << fixed << ct(1,i,j) << " hours" << endl;	
		}
		fout << endl;
	}
	fout.close();
}

//create_citygraph() { }
//write_citygraph() { }

//dijkstra_route() { }

int main(int argc, char *argv[]) {
	//commandline option decoding
	if(argc != 2) {
		cerr << "usage: ./Citysim1 -graphinfo" << endl;
		return 1;
	} else if(argv[1] != GRAPHINFO) {
		cerr << "usage: ./Citysim1 -graphinfo" << endl;
		return 1;
	} else {
		//city graph declarations
		vector<city> cities;
		read_cityinfo(cities);
		write_cityinfo(cities);

		//set up costtables
		costtable ct;
		ct.make_tables(cities);
		//create_citygraph()

		//if (mode == graphinfo) {
		//write_cityinfo()

	    write_distancetable(cities, ct);
	    //write_timetable()
		write_timetable(cities, ct);
		//write_citygraph()

		//while (not done) {
			//enter> from, to cities
			//dijkstra_route(costmode,from,to) 
		//}
	//}
	}
	return 0;
}

int checkFile(const char fileName[]){ //checks input file for validity (exists and not empty)
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
