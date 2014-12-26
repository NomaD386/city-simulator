#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "trsys.h"
#include "../ui/map.h"

extern Map map;

bool filereadProblem = false;

void countWay(std::vector<Car>::iterator);
void clearTemp();
void invCroad(CroadRef croad);
CroadRef shotDist(bool = true);

std::vector<Croad> Trsys::croad;
std::vector<Car> Trsys::car;

std::vector<CroadRef> Trsys::border;
std::vector<CroadRef> Trsys::inside;

bool Trsys::traceCar;
double Trsys::speed;
bool Trsys::simPaused;

Trsys::Trsys()
{
	speed = 0.1;
	traceCar = false;
	simPaused = false;
	
	Car::getExpected = getExpected;
	Car::renderDest = false;
	
	if (!readCity("input/map.in", "input/rel.in"))
	{
		generateCity(5);
	}
	initBackways();
	generateCars(50);

	// car.front().manual = true;
}

bool Trsys::readCity(const char* map, const char* rel)
{
	std::ifstream mapFile;
	mapFile.open(map, std::ios::in);
	// READ CROSSROADS FROM 'map.in'
	int a = 0;
	if (!mapFile)
	{
		std::cerr << "no map file\n";
		return false;
	}
	else
	{
		mapFile >> a;
		for (int i = 0; i < a; ++i)
		{
			double x;
			double y;
			mapFile >> x >> y;
			croad.push_back(Croad(Pos(x + rand() % 11 - 5, y + rand() % 11 - 5)));
		}
	}
	mapFile.close();
	std::ifstream relFile;
	mapFile.open(rel, std::ios::in);
	// READ CROSSROADS FROM 'rel.in'
	if (!mapFile)
	{
		std::cerr << "no map file\n";
		croad.clear();
		return false;
	}
	else
	{
		for (int i = 0; i < a; ++i)
		{
			int relnum;
			mapFile >> relnum;
			int temp;
			for (int j = 0; j < relnum; ++j)
			{
				mapFile >> temp;
				croad[i].road.push_back(Road(croad.begin() + i, croad.begin() + temp));
			}
		}
	}
	return true;
}

void Trsys::generateCity(int size)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			croad.push_back(Pos((i - (size - 1) / 2.0) * 25 + rand() % 11 - 5, (j - (size - 1) / 2.0) * 25 + rand() % 11 - 5));
		}
	}
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			if (i > 0)
			croad[i * size + j].road.push_back(Road(croad.begin() + i * size + j, croad.begin() + (i - 1) * size + j));
			if (i < size - 1)
			croad[i * size + j].road.push_back(Road(croad.begin() + i * size + j, croad.begin() + (i + 1) * size + j));
			if (j > 0)
			croad[i * size + j].road.push_back(Road(croad.begin() + i * size + j, croad.begin() + i * size + j - 1));
			if (j < size - 1)
			croad[i * size + j].road.push_back(Road(croad.begin() + i * size + j, croad.begin() + i * size + j + 1));
		}
	}
}

void Trsys::initBackways()
{
	for (std::vector<Croad>::iterator i = croad.begin(); i != croad.end(); ++i)
	{
		for (std::vector<Road>::iterator j = i->road.begin(); j != i->road.end(); ++j)
		{
			for (std::vector<Road>::iterator k = j->end->road.begin(); k != j->end->road.end(); ++k)
			{
				if (k->begin == j->end && k->end == j->begin)
				{
					j->backWay = k;
				}
			}
		}
	}
}

void Trsys::generateCars(int num)
{
	for (int i = 0; i < 50; ++i)
	{
		CroadRef a = croad.begin() + rand() % croad.size();
		RoadRef r = a->road.begin() + rand() % a->road.size();
		a = croad.begin() + rand() % croad.size();
		RoadRef f = a->road.begin() + rand() % a->road.size();
		car.push_back(Car(r, rand() % (int)(r->len - 1), f, rand() % (int)(f->len - 1)));
	}
	for (std::vector<Car>::iterator i = car.begin(); i != car.end(); ++i)
	{
		i->thisCar = i;
		i->pos->car.push_back(i);
	}
	
	for (CarRef i = car.begin(); i != car.end(); ++i)
	{
		i->interact();
	}
}

Trsys::~Trsys()
{
	croad.clear();
	car.clear();
	border.clear();
	inside.clear();
}

void Trsys::advance()
{
	if (!simPaused && !car.empty())
	{
		for (std::vector<Car>::iterator i = car.begin(); i < car.end(); ++i)
		{
			if (i->advance())
			{
				CroadRef cr = croad.begin() + rand() % croad.size();
				RoadRef r = cr->road.begin() + rand() % cr->road.size();
				i->setFinDest(r, rand() % (int)r->len);
				// car.erase(i);
			}
		}
	}
}

void Trsys::render()
{
	if (traceCar)
	{
		if (!car.empty())
		{
			map._d = car.front().rpos;
		}
	}
	
	for (CroadRef i = croad.begin(); i < croad.end(); i++)
	{
		i->renderRoad();
	}
	
	for (CroadRef i = croad.begin(); i < croad.end(); i++)
	{
		i->render();
	}
	
	for (std::vector<Car>::iterator i = car.begin(); i < car.end(); i++)
	{
		i->render();
	}
	
}

void Trsys::faster()
{
	speed *= 1.1;
	if (speed > 5) speed = 5;
	Car::sysSpd = speed;
}

void Trsys::slower()
{
	speed /= 1.1;
	if (speed < 0.01) speed = 0.01;
	Car::sysSpd = speed;
}

void Trsys::pause(bool state)
{
	simPaused = state;
}

void Trsys::followCar(bool state)
{
	traceCar = state;
}

void Trsys::renderDest()
{
	Car::renderDest = !Car::renderDest;
}

void Trsys::pause()
{
	simPaused = !simPaused;
}

void Trsys::followCar()
{
	traceCar = !traceCar;
}

RoadRef Trsys::getExpected(Car *c)
{
	clearTemp();
	bool destSet = false;
	
	c->pos->end->temp.state = TStart;
	inside.push_back(c->pos->end);
	
	invCroad(c->pos->end);
	
	if (c->pos->end == c->dest->end)
	{
		return c->dest->backWay;
	}
	
	if (c->pos->end == c->dest->begin)
	{
		return c->dest;
	}
	
	while (true)
	{
		if (c->dest->begin->temp.state != TUnknown
				&& c->dest->end->temp.state != TUnknown)
		{
			if (border.empty())
			{
				break;
			}
			if (c->dest->begin->temp.dist + c->findist / c->dest->avgSpeed() > shotDist()->temp.dist)
			{
				break;
			}
		}
		if (border.empty())
		{
			if (c->dest->begin->temp.state != TUnknown && c->dest->end->temp.state != TUnknown)
			{
				c->state = Impossible;
			}
			return c->pos->backWay;
		}
		else
		{
			invCroad(shotDist());
		}
	}
	
	if (c->dest->begin->temp.dist + (2 * c->findist - c->dest->len) / c->dest->avgSpeed() < c->dest->end->temp.dist)
	{
		c->way.push_back(c->dest->begin);
	}
	else
	{
		c->way.push_back(c->dest->end);
	}
	
	while (c->way.back()->temp.state != TStart)
	{
		c->way.push_back(c->way.back()->temp.prev);
	}
	
	if (!c->way.empty())
	{
		c->way.pop_back();
	}
	
	for (RoadRef i = c->pos->end->road.begin(); i != c->pos->end->road.end(); ++i)
	{
		if (&i->end->pos == &c->way.back()->pos)
		{
			destSet = true;
			return i;
		}
	}
	if (!destSet)
	{
		std::cerr << "EXCEPTION: destSet == false";
		exit(1);
	}
	return c->pos->end->road.begin() + rand() % c->pos->end->road.size();
}

void Trsys::clearTemp()
{
	for (std::vector<CroadRef>::iterator i = border.begin(); i != border.end(); ++i)
	{
		(*i)->temp.clear();
	}
	for (std::vector<CroadRef>::iterator i = inside.begin(); i != inside.end(); ++i)
	{
		(*i)->temp.clear();
	}
	border.clear();
	inside.clear();
}

void Trsys::invCroad(CroadRef c)
{
	for (RoadRef i = c->road.begin(); i < c->road.end(); ++i)
	{
		if (i->end->temp.state == TBorder)
		{
			if (i->end->temp.dist > c->temp.dist + i->lenth())
			{
				i->end->temp.dist = c->temp.dist + i->lenth();
				i->end->temp.prev = c;
			}
		}
		if (i->end->temp.state == TUnknown)
		{
			border.push_back(i->end);
			i->end->temp.state = TBorder;
			i->end->temp.prev = c;
			i->end->temp.dist = i->begin->temp.dist + i->lenth();
		}
	}
	if (c->temp.state == TBorder)
	{
		inside.push_back(c);
		c->temp.state = TInside;
	}
	for (std::vector<CroadRef>::iterator i = border.begin(); i != border.end(); ++i)
	{
		if ((*i)->temp.state != TBorder)
		{
			border.erase(i);
			i--;
		}
	}
}

CroadRef Trsys::shotDist(bool recount)
{
	static CroadRef shot;
	if (border.empty())
	{
		exit(10);
	}
	else
	{
		if (recount)
		{
			shot = border.front();
			for (std::vector<CroadRef>::iterator i = border.begin(); i != border.end(); ++i)
			{
				if ((*i)->temp.dist < shot->temp.dist && (*i)->temp.dist > 0)
				{
					shot = *i;
				}
			}
		}
	}
	return shot;
}

void Trsys::manipulate(int a)
{
	car.front().manipulate(a);
}