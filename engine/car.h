#ifndef __CAR_H__
#define __CAR_H__

#include <vector>
#include <iostream>

class RoadPos;
class Car;
typedef std::vector<Car>::iterator CarRef;

#include "croad.h"
#include "pos.h"

enum State { OnRoad, Arrived, OnCrossroad, Impossible };
enum TurnDir { TLeft, TRight };

class Car
{
public:
	Car(RoadRef pos, double dist, RoadRef _end, double findist);
	~Car();
	
	void setFinDest(RoadRef dest, double dist);

	void render();
	
	bool advance();
	
	State state;
	TurnDir turn;
	
	RoadRef prev;
	RoadRef pos;
	RoadRef expected;
	double expdist;
	double dist;
	RoadRef dest;
	double findist;
	
	std::vector<CroadRef> way;
	
	CarRef thisCar;
	
	Pos rpos; // real position - for render

	Pos destpos; // position of the destination

	static double sysSpd;
	static bool renderDest;

	static RoadRef (*getExpected)(Car*);

	void findFwdCar();

	double breakDist();

	void interact();
	
	void interactCar();
	
	void interactCroad();
	
	void checkCroad();
	
	void manipulate(int);

	bool busy(CroadRef c);

	bool manual;

	// it should be some statistic information here
	double speed;

private:
	
	void lockCroad(CroadRef c);
	
	void unlockCroad(CroadRef c);
	
	void addToRoad();
	
	void rmFromRoad();
	
	// for manipulation
	void accelerate();
	void halfSpeed();
	void fastBreak();
	void slowBreak();
	
	
	// for render
	Pos getRPos();
	bool isFwdCar;
	CarRef fwdCar;
	double angle;
	double stdist;
	double expang;
	
};

#endif