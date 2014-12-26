#ifndef _CROAD_H__
#define _CROAD_H__

#include <vector>

class Temp;
class Croad;
class Road;
class RoadPos;
typedef std::vector<Croad>::iterator CroadRef;
typedef std::vector<CroadRef>::iterator CroadRefRef;

#include "pos.h"
#include "road.h"
#include "temp.h"
class Croad
{
public:
	Croad(Pos pos);
	~Croad();
	
	void render();
	void renderRoad();
	
	Pos pos;
	std::vector<CroadRef> incomRoad;
	std::vector<Road> road;
	Temp temp;
	bool busy;
	CarRef car;
};

#endif