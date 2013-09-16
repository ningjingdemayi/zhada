#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Station.h"

using namespace std;

vector<Station> stations;

bool stations_exist(int station_id);
void print_stations(const vector<Station> & _stations);
void print_station(const  Station &station);
void load_stations(const char * file_name);

int main()
{
	ifstream fin("subway.txt");

	char line[64] = {0};
	string id;
	string name;
	Subway subway_now;
	int i = 0;
	while(fin.getline(line , sizeof(line)))
	{
		stringstream ss(line);ss >> id;ss >> name;
			
		if(id == "")	continue;
		if(name == "")	{	subway_now.subway_id = ++i ;	subway_now.subway_name = id;continue;}

		Station station;
		station.station_id = atoi(id.c_str());
		station.subway_id = subway_now.subway_id;
		station.station_name = name;

		if( ! stations_exist(station.station_id))
			stations.push_back(station);
		//cout << id << "--" << name << endl;
		id.clear();
		name.clear();
	}

	
	fin.close();

	//print_stations(stations);

	vector<Station>::const_iterator it = stations.begin();
	int how_many_change_in_1 = 0;
	while(it != stations.end())
	{
		if(it->subway_id == 1 && it->subways_amount > 1)
		{
			print_station(*it);
			how_many_change_in_1 ++;
		}
		++ it;
	}

	cout << how_many_change_in_1 << endl;

	ofstream fout("output1.txt");
	fout << how_many_change_in_1 ;
	fout.close();

	getchar();
	return 0;
}



bool stations_exist(int station_id)
{
	vector<Station>::iterator it = stations.begin();
	while(it != stations.end())
	{
		if(it->station_id == station_id)
		{
			it->subways_amount ++;
			return true;
		}
		++ it;
	}
	return false;
}

void print_stations(const vector<Station> & _stations)
{
	vector<Station>::const_iterator it = _stations.begin();
	while(it != _stations.end())
	{
		cout << "id:" << it->station_id
			<< " name:"<< it->station_name 
			<< " subway_id:"<< it->subway_id 
			<< " subway_amount:"<< it->subways_amount << endl;
		++ it;
	}
}

void print_station(const  Station &station)
{
	cout << "id:" << station.station_id
		<< " name:"<< station.station_name
		<< " subway_id:"<< station.subway_id 
		<< " subway_amount:"<< station.subways_amount << endl;
}
