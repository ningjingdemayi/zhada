#ifndef __LOG__
#define __LOG__

#include <string>

class Log
{
public:
	Log()
	{
		this->user_id = "0";
		this->enter_time = 0;
		this->out_time = 0;
		this->in_60_count = 0;
	}

public:
	std::string user_id;
	int enter_station_id;
	int out_station_id;
	unsigned int enter_time;
	unsigned int out_time;

	int in_60_count;
};


#endif