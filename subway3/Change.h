#ifndef __CHANGE__
#define __CHANGE__

class Change
{
public:
	int enter_station_id;
	int out_station_id;
};

class Vote
{
public:
	Vote()
	{
		this->station1 = this->station2 = this->vote = 0;
	}

public:
	int station2;
	int station1;
	int vote;
};

#endif