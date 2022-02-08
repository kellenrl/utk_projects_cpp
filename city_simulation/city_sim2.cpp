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
#include <map>
#include <limits>
#include <stack>

using namespace std;

int checkFile(const char[]);  // checks if file valid (exists and not empty)

const int VALID_FILE = 0;
const int INVALID_FILE = -1;
const string GRAPHINFO = "-graphinfo";
const string DISTANCE = "-distance";
const string TIME = "-time";
const char* inputFile = "city_list.txt"; //input file
typedef enum { WHITE, BLACK } vcolor_t;

class city {
public:
	string getName() { return this->name; } //get functions for private member data
	string getType() { return this->type; }
	int getZone() { return this->zone; }
	int getPopulation() { return this->population; }
	float getLatitude() { return this->latitude; }
	float getLongitude() { return this->longitude; }
	int getIndex() { return this->index; }

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
	int index;

};

class costtable { //holds tables of distances and travel times between all cities
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

float costtable::operator()(int mode, int i, int j) { //overloaded function operator for index access into tables
	if(i < j) swap(i, j);
	if(mode == 0) return table[0][i*(i+1)/2+j];
	else return table[1][i*(i+1)/2+j];
}

istream& operator>>(istream& in, city& c) { //overloaded insertion operator for city class
	float tempLat, tempLong;
	static int index = 0;
	in >> c.zone >> c.name >> c.type >> tempLat >> tempLong >> c.population;
	c.latitude = tempLat*(M_PI/180);
	c.longitude = tempLong*(M_PI/180);
	c.index = index;
	index++;
	return in;
}

ostream& operator<<(ostream& out, const city& c) { //overloaded extraction operator for city class
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
	fin.close();
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

vector< vector<int> > create_citygraph(vector<city> cities, costtable ct) { //creates graph of connected cities, returns sparse vector graph
	vector<int> gateway;
	vector<int> regional;
	vector< vector<int> > graph;
	vector<int> temp;
	int nearest;

	graph.resize((int)cities.size());

	//inserts regional cities into graph
	for(int i = 0; i < (int)cities.size(); i++) {
		if(cities[i].getType() == "REGIONAL") {
			regional.push_back(i);
		} else {
			gateway.push_back(i);
		}
	}
	for(vector<int>::iterator iti = regional.begin(); iti != regional.end(); iti++) {
		for(vector<int>::iterator itj = regional.begin(); itj != regional.end(); itj++) {
			if(*iti == *itj) continue;
			if(cities[*iti].getZone() == cities[*itj].getZone()) {
				graph[*iti].push_back(*itj);
				graph[*itj].push_back(*iti);
			}
		}
		for(vector<int>::iterator itk = gateway.begin(); itk != gateway.end(); itk++) {
			if(*iti == *itk) continue;
			if(cities[*iti].getZone() == cities[*itk].getZone()) {
				temp.push_back(*itk);
				nearest = *itk;
			}
		}
		for(int i = 0; i < (int)temp.size(); i++) {
			if(ct(0,*iti,temp[i]) < ct(0,*iti,nearest)) {
				nearest = temp[i];
			}
		}
		temp.clear();
		graph[*iti].push_back(nearest);
		graph[nearest].push_back(*iti);
	}
	//inserts gateway cities into graph
	for(vector<int>::iterator iti = gateway.begin(); iti != gateway.end(); iti++) {
		int idex = *iti;
		int zone = -1;
		float dist = 0;
		int cdex;
		for(vector<int>::iterator itj = gateway.begin(); itj != gateway.end(); itj++) {
			int jIndex = *itj;
			if(*iti == *itj) continue;
			if(cities[*iti].getZone() == cities[*itj].getZone()) {
				graph[*iti].push_back(*itj);
				graph[*itj].push_back(*iti);
			} else {
				if(ct(0,idex,jIndex) <= 5200) {
					if(zone == -1) {
						dist = ct(0,idex,jIndex);
						zone = cities[jIndex].getZone();
						cdex = jIndex;
					}
					else if(zone != cities[jIndex].getZone()) {
						graph[idex].push_back(cdex);
						graph[cdex].push_back(idex);
						dist = ct(0,idex,jIndex);
						zone = cities[jIndex].getZone();
						cdex = jIndex;
					}
					else if(ct(0,idex,jIndex) < dist) {
						dist = ct(0,idex,jIndex);
						zone = cities[jIndex].getZone();
						cdex = jIndex;
					}
				}
			}
		}
		if(zone != -1) {
			graph[idex].push_back(cdex);
			graph[cdex].push_back(idex);
		}
	}
	//gets rid of duplicates and resizes vectors
	for(int i = 0; i < (int)cities.size(); i++) {
		vector<int>::iterator tempit;
		sort(graph[i].begin(), graph[i].end());
		tempit = unique(graph[i].begin(), graph[i].end());
		graph[i].resize(tempit - graph[i].begin());
	}
	return graph;
}
void write_citygraph(vector<city> cities, vector< vector<int> > graph, costtable ct) { //pretty prints graph of cities
	ofstream fout;
	fout.open("city_graph.txt");
	fout << "CITY GRAPH:" << endl;
	fout << endl;

	for(int i = 0; i < (int)cities.size(); i++) {
		fout << setw(3) << right << i << " " << cities[i].getName() << endl;
		vector<int>::iterator it;
		for(it = graph[i].begin(); it != graph[i].end(); it++) {
			fout << "    " << setw(2) << right << cities[*it].getIndex() << " " << setw(20) << left << cities[*it].getName()
				<< setw(6) << fixed << right << setprecision(1) << ct(0,i,*it) << " " << "miles" << setw(6) << right << ct(1,i,*it)
				<< " " << "hours" << endl;
		}
		if(i == (int)cities.size()-1) break;
		fout << endl;
	}
	fout.close();
}

//computes shortest route from source to sink and pretty prints results, mode = 0 is by distance, mode = 1 by time
void dijkstra_route(vector< vector<int> > graph, vector<city> cities, costtable ct,int mode, int source, int sink) {
	vector<vcolor_t> vcolor;
	vector<float> vdist;
	vector<int> vlink;

	vcolor.assign(cities.size(), WHITE);
	vdist.assign(cities.size(), numeric_limits<float>::max());
	vdist[source] = 0;
	vlink.assign(cities.size(), -1);
	vlink[source] = source;
	while (1) {
		int next_i = -1;
		float mindist = numeric_limits<float>::max();
		for (int i=0; i<(int)vcolor.size(); i++) {
			if (vcolor[i] == WHITE && mindist > vdist[i]) {
			next_i = i;
			mindist = vdist[i];
			}
		}	
		int i = next_i;
		if (i == -1)
			return;
		vcolor[i] = BLACK;
		if (i == sink)
			break;
		for (int k=0; k<(int)graph[i].size(); k++) {
			int j = graph[i][k];
			float wij = ct(mode,i,j); 
			if (vcolor[j] == WHITE) {
				if (vdist[j] > vdist[i] + wij) {
					vdist[j] = vdist[i] + wij;
					vlink[j] = i;
				}		
			}
		}
	}
	stack<int> S;
	for(int i = sink; i != source; i=vlink[i]) {
		S.push(i);
	}
	S.push(source);
	float totalHours = 0;
	float totalMiles = 0;
			cout << setw(7) << fixed << setprecision(1) << right << vdist[source] << " miles" << setw(6) << totalHours << " hours : "
			<< setw(2) << right << cities[source].getIndex() << " " << setw(18) << left << cities[source].getName() << endl;
	while(!S.empty()) {
		int temp; //to city
		int i = S.top(); //from city
		if(i==sink) break;
		S.pop();
		if(!S.empty()) temp = S.top();
		if(mode == 0) {
			totalHours += ct(1,i,temp);
			cout << setw(7) << fixed << setprecision(1) << right << vdist[temp] << " miles" << setw(6) << totalHours << " hours : "
				<< setw(2) << right << temp << " " << setw(18) << left << cities[temp].getName() << setw(7) << right << ct(0,i,temp) << " miles"
				<< setw(6) << right << ct(1,i,temp) << " hours" << endl;
		}
		if(mode == 1) {
			totalMiles += ct(0,i,temp);
			cout << setw(7) << fixed << setprecision(1) << right << totalMiles << " miles" << setw(6) << vdist[temp] << " hours : "
				<< setw(2) << right << temp << " " << setw(18) << left << cities[temp].getName() << setw(7) << right << ct(0,i,temp) << " miles"
				<< setw(6) << right << ct(1,i,temp) << " hours" << endl;
		}
	}
	cout << endl;
}

int main(int argc, char *argv[]) {
	string mode;
	int costmode;
	string sourceKey;
	string sinkKey;
	int source;
	int sink;

	//commandline option decoding
	if(argc != 2) {
		cerr << "usage: ./Citysim2 -distance|time|graphinfo" << endl;
		return 1;
	} else if(argv[1] != GRAPHINFO && argv[1] != TIME && argv[1] != DISTANCE) {
		cerr << "usage: ./Citysim2 -distance|time|graphinfo" << endl;
		return 1;
	} else {
		mode = argv[1];
		//city graph declarations
		vector<city> cities;
		read_cityinfo(cities);

		//set up costtables
		costtable ct;
		ct.make_tables(cities);
	
		vector< vector<int> > graph = create_citygraph(cities, ct);
		map<string, int> cityMap;
		for(int i = 0; i < (int)cities.size(); i++) {
			string tempStr = cities[i].getName();
			int tempInt = cities[i].getIndex();
			cityMap.insert(pair<string, int>(tempStr, tempInt));
		}
		if (mode == GRAPHINFO) {
			write_citygraph(cities, graph, ct);
			write_cityinfo(cities);
			write_distancetable(cities, ct);
			write_timetable(cities, ct);
		} else if(mode == DISTANCE) {
			costmode = 0;
				cout << "Enter> ";
			while(cin >> sourceKey >> sinkKey) {
				if(cityMap.find(sourceKey) == cityMap.end()) {
					cerr << sourceKey << ": prefix not found.\n" << endl;
					cout << "Enter> ";
					continue;
				}
				else source = cityMap[sourceKey];
				if(cityMap.find(sinkKey) == cityMap.end()) {
					cerr << sinkKey << ": prefix not found.\n" << endl;
					cout << "Enter> ";
					continue;
				}
				else {
					sink = cityMap[sinkKey];
					dijkstra_route(graph,cities,ct,costmode,source,sink);
					cout << "Enter> ";
				}
			}
		cout << endl;
		} else if(mode == TIME) {
			costmode = 1;
				cout << "Enter> ";
			while(cin >> sourceKey >> sinkKey) {
				if(cityMap.find(sourceKey) == cityMap.end()) {
					cerr << sourceKey << ": prefix not found.\n" << endl;
					cout << "Enter> ";
					continue;
				}
				else source = cityMap[sourceKey];
				if(cityMap.find(sinkKey) == cityMap.end()) {
					cerr << sinkKey << ": prefix not found.\n" << endl;
					cout << "Enter> ";
					continue;
				}
				else {
					sink = cityMap[sinkKey];
					dijkstra_route(graph,cities,ct,costmode,source,sink);
					cout << "Enter> ";
				}
			}
		cout << endl;
		}
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
