#ifndef __TRSYS_H__
#define __TRSYS_H__

#include "croad.h"
#include "car.h"

class Trsys
{
public:
	Trsys();
	bool readCity(const char* map, const char* rel);
	void generateCity(int size);
	void initBackways();
	void generateCars(int num);
	
	~Trsys();

	void advance();
	void render();
	
	void faster();
	void slower();
	
	void pause(bool);
	void followCar(bool);
	
	void pause();
	void followCar();
	void renderDest();
	
	void manipulate(int);
	
	static void clearTemp();
private:

	static RoadRef getExpected(Car *);
	
	static void invCroad(CroadRef croad);
	static CroadRef shotDist(bool = true);
	
	static std::vector<Croad> croad;
	static std::vector<Car> car;
	
	static std::vector<CroadRef> border;
	static std::vector<CroadRef> inside;
	
	static bool traceCar;
	static double speed;
	static bool simPaused;
};

#endif