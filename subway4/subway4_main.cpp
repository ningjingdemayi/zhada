#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>

#include <stdio.h>

#include "Station.h"
#include "Log.h"

using namespace std;

vector<Station> stations;

vector<Log> logs;


bool stations_exist(int station_id);
void print_stations(const vector<Station> & _stations);
void print_station(const  Station &station);
void load_stations(const char * filename);

long int StringtoTime(const char *str);
bool insert_logs(Log & log);
void print_logs(const vector<Log> & _logs);
void load_log(const char * fileName);

bool is_enter_line1(const int enter_station_id);

void BubbleSort(vector<Log> & logs , vector<Log>::size_type count);

int check_in60_counts(int index)
{
	int counts = 0;
	for(int i = index  ; i < logs.size() ; i++)
	{
		++ counts ;
		if(logs[i].enter_time > logs[index].enter_time + 60)
			return counts;
	}
	return counts;
}
std::string timeStampToHReadble(long  timestamp)
{
	const time_t rawtime = (const time_t)timestamp;

	struct tm * dt;
	char timestr[30];
	char buffer [30];

	dt = localtime(&rawtime);
	// use any strftime format spec here
	strftime(timestr, sizeof(timestr), "%H:%M:%S", dt);
	sprintf(buffer,"%s", timestr);
	std::string stdBuffer(buffer);
	return stdBuffer;
}
int main()
{
	load_stations("subway.txt");

	// load ling1 and sorted
	load_log("sample.log");
	
	unsigned int start = logs[0].enter_time;
	unsigned int step = 60;
	int index = 0;
	vector<Log>::iterator it = logs.begin();
	while(it != logs.end())
	{
		it->in_60_count = check_in60_counts(index);
		++ index;
		++ it;
	}


	int bigest_now = 0;
	int index_now = 0;
	int index2 = 0;
	vector<Log>::iterator it2 = logs.begin();
	while(it2 != logs.end())
	{
		if(it2->in_60_count > bigest_now)
		{
			bigest_now = it2->in_60_count;
			index_now = index2;
		}
		++ index2;
		++ it2;
	}

	cout << bigest_now << endl;
	cout << logs[index_now].in_60_count << endl;
	unsigned int start_time = logs[index_now].enter_time;
	unsigned int end_time = start_time + 60;
	cout <<  timeStampToHReadble(start_time)  << " "<<  timeStampToHReadble(end_time ) ;
	

	ofstream fout("output4.txt");
	fout << bigest_now << endl;
	fout <<  timeStampToHReadble(start_time)  << " "<<  timeStampToHReadble(end_time ) ;
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
		cout << " enter_time:" << it->enter_time;
		cout << " user_id:" << it->user_id;
		cout << " enter_station_id:" << it->enter_station_id;
		//cout << " out_station_id:" << it->out_station_id;
		//cout << " out_time:" << it->out_time;
		cout << endl;

		

		++ it;
	}
}

bool is_enter_line1(const int enter_station_id)
{
	vector<Station>::const_iterator it = stations.begin();
	while(it != stations.end())
	{
		if(it->subway_id == 1 && enter_station_id == it->station_id)
			return true;
		++it;
	}
	
	return false;
}

// this time we only load enter line1
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
	
		if(log_now_str[4] != "entered")
			continue;
		log.enter_time = time_int;
		log.enter_station_id = atoi(log_now_str[6].c_str());
		if( !is_enter_line1(log.enter_station_id ))
			continue;

		insert_logs(log);

	}
	fin.close();
	BubbleSort(logs , logs.size());
	print_logs(logs);
	cout << "load log success!" << endl;
}

void BubbleSort(vector<Log> & logs , vector<Log>::size_type count)
{
	Log temp;
	for (int i = 1; i < count; i++)
	{
		for (int j = count - 1; j >= i; j--)
		{
			if (logs[j].enter_time < logs[ j - 1 ].enter_time)
			{
				temp = logs[j - 1];
				logs[j - 1] = logs[j];
				logs[j] = temp;
			}
		}
	}
}
