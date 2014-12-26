#ifndef __ROAD_H__
#define __ROAD_H__

#include <vector>

class Croad;
class Road;
class Car;
typedef std::vector<Car>::iterator CarRef;
typedef std::vector<Road>::iterator RoadRef;

#include "croad.h"

class Road
{
public:
	Road(CroadRef begin, CroadRef end);
	~Road();
	
	void render();
	
	// if crossroad changed it's position
	void countPoints();
	
	// average speed
	double avgSpeed();
	
	double lenth(void);
	
	// crossroad, from where cars goes from this road
	CroadRef begin;
	
	// crossroad, where cars goes from this road
	CroadRef end;
	
	// lenth of road
	double len;
	
	// way back
	RoadRef backWay;
	
	double angle;
	
	std::vector<CarRef> car;
	
private:
	
	// just for render
	struct roadPoints
	{
		Pos splB; // spliter Begin
		Pos splE; // spliter End
		Pos edgeB; // edge Begin
		Pos edgeE; // edge End
	} points;
};

#endif