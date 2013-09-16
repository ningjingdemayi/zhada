#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>

#include "Station.h"
#include "Log.h"
#include "Change.h"

using namespace std;

vector<Station> stations;

vector<Log> logs;

vector<Change> change_table;

vector<Vote> votes;


bool stations_exist(int station_id);
void print_stations(const vector<Station> & _stations);
void print_station(const  Station &station);
void load_stations(const char * filename);

long int StringtoTime(const char *str);
bool insert_logs(Log & log);
void print_logs(const vector<Log> & _logs);
void load_log(const char * fileName);


int find_station_line(int station_id);
bool station_in_same_subway(int enter_station_id , int out_station_id);

void generate_change_table();
void vote_change_table();

int main()
{
	load_stations("subway.txt");
	load_log("sample.log");

	generate_change_table();
	vote_change_table();

	//ofstream fout("test.txt");
	vector<Vote>::iterator it = votes.begin();
	Vote vote_now;
	while(it != votes.end())
	{
		if(vote_now.vote < it->vote)
			vote_now = *it;
		//fout << "station1:" << it->station1 << " station2:" << it->station2 << " vote:" << it->vote << endl;
		++ it;
	}
		
	cout << vote_now.vote << endl;
	cout << change_table.size() - vote_now.vote << endl;
	//fout.close();

	ofstream fout("output3.txt");
	fout << vote_now.station1 << " " << vote_now.station2 << endl;
	fout << change_table.size() - vote_now.vote ;

	fout.flush();
	fout.close();


	//cout << StringtoTime("2013-08-24 21:10:47") << endl;
	//cout << StringtoTime("2013-08-24 21:11:48") << endl;

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

void load_stations(const char * filename)
{
	ifstream fin(filename);

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
}



long int StringtoTime(const char *str)
{
	struct tm t;
	int year,month,day, hour,minite,second;
	sscanf(str,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&minite,&second);
	t.tm_year = year-1900;
	t.tm_mon = month;
	t.tm_mday = day;
	t.tm_hour = hour;
	t.tm_min = minite;
	t.tm_sec = second;
	t.tm_isdst = 0;

	time_t t_of_day;
	t_of_day=mktime(&t);
	long int time;
	time = t_of_day;
	return time;

}

bool insert_logs(Log & log)
{
	vector<Log> ::iterator it = logs.begin();
	while(it != logs.end())
	{
		if(it->user_id == log.user_id)
		{
			if(it->enter_time == 0)
			{
				it->enter_time = log.enter_time;
				it->enter_station_id = log.enter_station_id;
			}
			if(it->out_time == 0)
			{
				it->out_time = log.out_time;
				it->out_station_id = log.out_station_id;
			}

			return true;
		}
		++ it;
	}

	logs.push_back(log);// here maybe  wrong

	return false;
}

void print_logs(const vector<Log> & _logs)
{
	vector<Log>::const_iterator it = _logs.begin();
	int i = 0;
	while(it != _logs.end())
	{
		++ i;
		cout << i;
		cout << " user_id:" << it->user_id;
		cout << " enter_station_id:" << it->enter_station_id;
		cout << " out_station_id:" << it->out_station_id;
		cout << " enter_time:" << it->enter_time;
		cout << " out_time:" << it->out_time;
		cout << endl;

		

		++ it;
	}
}

void load_log(const char *fileName)
{
	ifstream fin(fileName);

	string log_now_str[14];
	char line[256] = {0};

	cout << "loading log..." << endl;
	while(fin.getline(line , sizeof(line)))
	{
		stringstream ss(line);
		for(int i = 0 ; i < 14 ; ++ i)
		{
			ss >> log_now_str[i];
		}

		if(log_now_str[0] == "#")
			continue;


		Log log;
		log.user_id = log_now_str[10];
		stringstream sst;
		sst << log_now_str[0] << " " << log_now_str[1];
		string time_str = sst.str();
		unsigned int time_int = StringtoTime(time_str.c_str());
	
		if(log_now_str[4] == "entered")
		{
			log.enter_time = time_int;
			log.enter_station_id = atoi(log_now_str[6].c_str());
		}
		if(log_now_str[4] == "left")
		{
			log.out_time = time_int;
			log.out_station_id = atoi(log_now_str[6].c_str());
		}


		insert_logs(log);

		//cout << log_now_str[4] <<  log_now_str[6] << "---" << log_now_str[10] << endl; 
	}
	fin.close();
	//print_logs(logs);
	cout << "load log success!" << endl;
}


int find_station_line(int station_id)
{
	vector<Station> ::const_iterator it = stations.begin();
	while(it != stations.end())
	{
		if(it->station_id == station_id)
			return it->subway_id;
		++ it;
	}

	return 0;
}

bool station_in_same_subway(int enter_station_id , int out_station_id)
{
	int subway1 = find_station_line(enter_station_id);
	int subway2 = find_station_line(out_station_id);

	if(subway1 == subway2)
		return true;
	else
		return false;
}

void generate_change_table()
{
	vector<Log>::const_iterator it = logs.begin();
	while(it != logs.end())
	{
		if(! station_in_same_subway(it->enter_station_id , it->out_station_id))
		{
			Change change;
			change.enter_station_id = it->enter_station_id;
			change.out_station_id = it->out_station_id;
			change_table.push_back(change);
		}
		++ it;
	}
}

bool insert_votes(const Change & change)
{
	vector<Vote>::iterator it = votes.begin();
	while(it != votes.end())
	{
		if(
			(it->station1 == change.enter_station_id && it->station2 == change.out_station_id)
		||	(it->station1 == change.out_station_id && it->station2 == change.enter_station_id)
		)
		
		{
			++ it->vote;
			return true;
		}
		++ it;
	}

	Vote vote ;
	vote.vote = 1;
	vote.station1 = change.enter_station_id;
	vote.station2 = change.out_station_id;

	votes.push_back(vote);

	return false;

}

void vote_change_table()
{
	vector<Change> ::const_iterator it = change_table.begin();
	while(it != change_table.end())
	{
		insert_votes(*it);
		++ it;
	}
}