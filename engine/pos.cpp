#include <math.h>
#include "pos.h"

const double PI = acos(-1);

Pos::Pos()
{
	x = 0;
	y = 0;
}

Pos::Pos(double _x, double _y)
{
	x = _x;
	y = _y;
}

Pos::~Pos()
{
	
}

double Pos::distance()
{
	return sqrt(pow(x, 2) + pow(y, 2));
}

double Pos::distance(Pos other)
{
	return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}

double Pos::angle()
{
	if (y >= 0) return acos(x / distance());
	else return 2 * PI - acos(x / distance());
}

double Pos::angle(Pos other)
{
	if (other.y - y >= 0) return acos((other.x - x) / distance(other));
	else return 2 * PI - acos((other.x - x) / distance(other));
}