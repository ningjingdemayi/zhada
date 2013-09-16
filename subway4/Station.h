#ifndef __STATION__
#define __STATION__

#include <string>

class Subway
{
public:
	int subway_id;
	std::string subway_name;
};

class Station
{

public:
	Station(){this->subways_amount = 1;}
	Station(int & swi , int & si , std::string & sn )
		: subway_id(swi) , station_id(si) , station_name(sn)
	{}

public:
	int subway_id;
	int station_id;
	int subways_amount;
	std::string station_name;
};

#endif