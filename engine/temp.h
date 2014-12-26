#ifndef _TEMP_H__
#define _TEMP_H__

#include <vector>

class Croad;

typedef std::vector<Croad>::iterator CroadRef;
typedef std::vector<CroadRef>::iterator CroadRefRef;


enum TempState { TUnknown, TInside, TBorder, TStart };


class Temp
{
public:
	Temp();
	~Temp();
	
	void clear();
	
	TempState state;
	CroadRef prev;
	double dist;
};

#endif