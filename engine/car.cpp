#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "car.h"
#include "../ui/map.h"
#include "../ui/PNGTex.h"

const double PI = acos(-1);

extern Map map;
double Car::sysSpd = 0.1;
bool Car::renderDest = false;

RoadRef (*Car::getExpected)(Car*);

PNGTex carTex;

Car::Car(RoadRef _pos, double _dist, RoadRef _dest, double _findist)
{
	static bool texinit = false;
	if (!texinit)
	{
		carTex.load("res/car.png");
		texinit = true;
		carTex.use();
	}
	speed = 0.1;
	pos = _pos;
	prev = _pos;
	expected = pos;
	dist = _dist;
	dest = _dest;
	findist = _findist;
	expang = pos->begin->pos.angle(pos->end->pos);
	if (expang < 0) expang += 2 * PI;
	if (expang > 2 * PI) expang = 2 * PI;

	angle = expang;
}

Car::~Car()
{

}

void Car::setFinDest(RoadRef _dest, double _dist)
{
	dest = _dest;
	this->findist = _dist;
	expected = getExpected(this);
	state = OnRoad;
}

double roadAngle(RoadRef r1, RoadRef r2)
{
	const double PI = acos(-1);
	double a1;
	double a2;
	double result = 0;
	a1 = r1->angle + PI;
	a2 = r2->angle;
	if (a1 > PI * 2) a1 -= PI * 2;
	if (a1 < 0) a1 += PI * 2;
	if (a2 > PI * 2) a1 -= PI * 2;
	if (a2 < 0) a1 += PI * 2;

	if (a1 - a2 > PI) a1 -= PI * 2;
	if (a1 - a2 < - PI) a2 -= PI * 2;

	result = (a1 + a2) / 2;

	return result;
}

Pos Car::getRPos()
{
	if (state != OnCrossroad)
	{
		rpos.x = pos->begin->pos.x + (pos->end->pos.x - pos->begin->pos.x) / pos->len * dist;
		rpos.y = pos->begin->pos.y + (pos->end->pos.y - pos->begin->pos.y) / pos->len * dist;
	}
	else
	{
		rpos.x = pos->begin->pos.x;
		rpos.y = pos->begin->pos.y;
	}

	if (angle - expang >= PI) angle -= 2 * PI;
	if (angle - expang <= -PI) angle += 2 * PI;
	if (state == OnRoad)
	{
		rpos.x += cos(angle - PI / 2);
		rpos.y += sin(angle - PI / 2);
	}

	if (state == OnCrossroad)
	{

		if (turn == TLeft)
		{
			rpos.x += cos(angle - PI / 2);
			rpos.y += sin(angle - PI / 2);
		}
		else
		{
			rpos.x -= cos(angle - PI / 2);
			rpos.y -= sin(angle - PI / 2);

			double angRoad = (pos->angle - roadAngle(prev, pos));
			if (angRoad < 0) angRoad *= -1;

			rpos.x += cos(roadAngle(prev, pos)) * 2 / sin(angRoad);
			rpos.y += sin(roadAngle(prev, pos)) * 2 / sin(angRoad);

			stdist = 2 / tan(angRoad);
		}
	}

	return rpos;
}

void Car::render()
{
	rpos = getRPos();

	rpos.x += map.d.x;
	rpos.y += map.d.y;

	rpos.x *= map.scale;
	rpos.y *= map.scale;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor3d(1, 1, 1);
	glBegin(GL_POLYGON);
	glTexCoord2d(1, 0);
	glVertex2d(rpos.x + cos(angle + PI / 6) * map.scale * 1.4,
			   rpos.y + sin(angle + PI / 6) * map.scale * 1.4);
	glTexCoord2d(0, 0);
	glVertex2d(rpos.x + cos(angle + 5 * PI / 6) * map.scale * 1.4,
			   rpos.y + sin(angle + 5 * PI / 6) * map.scale * 1.4);
	glTexCoord2d(0, 1);
	glVertex2d(rpos.x + cos(angle + 7 * PI / 6) * map.scale * 1.4,
			   rpos.y + sin(angle + 7 * PI / 6) * map.scale * 1.4);
	glTexCoord2d(1, 1);
	glVertex2d(rpos.x + cos(angle + 11 * PI / 6) * map.scale * 1.4,
			   rpos.y + sin(angle + 11 * PI / 6) * map.scale * 1.4);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	rpos.x /= -map.scale;
	rpos.y /= -map.scale;

	rpos.x += map.d.x;
	rpos.y += map.d.y;

	destpos.x = dest->begin->pos.x + (dest->end->pos.x - dest->begin->pos.x) / dest->len * findist;
	destpos.y = dest->begin->pos.y + (dest->end->pos.y - dest->begin->pos.y) / dest->len * findist;

	destpos.x += map.d.x;
	destpos.y += map.d.y;

	destpos.x *= map.scale;
	destpos.y *= map.scale;

	if (renderDest)
	{
		glColor3d(0, 0, 0);
		glBegin(GL_POLYGON);
		for (double i = 0; i < 2 * PI; i += 0.1)
		{
			glVertex2d(destpos.x + cos(i) * map.scale / 1.5, destpos.y + sin(i) * map.scale / 1.5);
		}
		glEnd();
	}
	// glBegin(GL_LINES);
	// glVertex2d(-(rpos.x - map.d.x) * map.scale, -(rpos.y - map.d.y) * map.scale);
	// glVertex2d(destpos.x, destpos.y);	
	// glEnd();
}

double halfAngle(RoadRef r1, RoadRef r2)
{
	const double PI = acos(-1);
	double a1;
	double a2;
	double result = 0;
	a1 = r1->angle;
	a2 = r2->angle + PI;
	if (a1 > PI * 2) a1 -= PI * 2;
	if (a1 < 0) a1 += PI * 2;

	if (a1 - a2 > PI) a1 -= PI * 2;
	if (a1 - a2 < - PI) a2 -= PI * 2;

	result = (a1 - a2);

	return result;
}

bool Car::advance()
{
	// <for interaction>

	if (dist < 1.5)
	{
		lockCroad(pos->begin);
	}
	else
	{
		unlockCroad(pos->begin);
	}

	if (dist > pos->len - 4)
	{
		lockCroad(pos->end);
	}
	else
	{
		unlockCroad(pos->end);
	}



	// </for interaction>


	if (pos == dest && dist > findist)
	{
		state = Arrived;
		return true;
	}
	if (pos == dest->backWay && dist > pos->len - findist)
	{
		state = Arrived;
		return true;
	}
	interact();
	if (angle == expang)
	{
		state = OnRoad;
	}
	if (dist < pos->len - 6 && state == OnRoad)
	{
		dist += speed * Car::sysSpd;
		angle = expang;
		expected = pos;
	}
	else
	{
		if (expected == pos)
		{
			expected = getExpected(this);
			double angRoad = pos->angle - roadAngle(pos, expected);

			if (angle - expected->angle > PI) angle -= 2 * PI;
			if (angle - expected->angle < -PI) angle += 2 * PI;

			expdist = pos->len + 2 / tan(angRoad);

			stdist = 0;

			if (expdist > pos->len)
			{
				expdist = pos->len;
				stdist = 0;
			}
			if (expected == pos->backWay)
			{
				expdist = pos->len;
				stdist = 0;
			}
			if (angle == expang)
			{
				state = OnRoad;
			}
		}

		if (state == OnRoad)
		{
			dist += speed * Car::sysSpd;
			if (dist > expdist)
			{
				prev = pos;
				rmFromRoad();
				pos = expected;
				dist = 0;
				expang = expected->angle;
				state = OnCrossroad;
				angle += 0.001;
				if (pos->begin == expected->end)
				{
					turn = TLeft;
					angle = expang - PI;
					angle += speed * Car::sysSpd;
				}
				addToRoad();
			}
		}

		if (state == OnCrossroad)
		{
			if (angle - expang <= -PI) angle += 2 * PI;
			if (angle - expang >= PI) angle -= 2 * PI;

			if (angle < expang)
			{
				angle += speed * Car::sysSpd;
				turn = TLeft;
				if (angle > expang)
				{
					angle = expang;
					state = OnRoad;
					dist = stdist;
				}
			}
			else
			{
				angle -= speed * Car::sysSpd;
				turn = TRight;
				if (angle < expang)
				{
					angle = expang;
					state = OnRoad;
					dist = stdist;
				}
			}
		}
	}
	return false;
}

static const double maxSpeed = 0.1;


void Car::lockCroad(CroadRef c)
{
	if (!c->busy)
	{
		c->car = thisCar;
		c->busy = true;
	}
}

void Car::unlockCroad(CroadRef c)
{
	if (c->car == this->thisCar)
	{
		c->busy = false;
	}
}

void Car::addToRoad()
{
	pos->car.push_back(thisCar);
}

void Car::rmFromRoad()
{
	for (std::vector<CarRef>::iterator i = prev->car.begin(); i != prev->car.end(); ++i)
	{
		if (*i == thisCar)
		{
			prev->car.erase(i);
			return;
		}
	}
}

void Car::accelerate()
{
	speed += 0.02 * Car::sysSpd;
	if (speed > maxSpeed)
	{
		speed = maxSpeed;
	}
}

void Car::halfSpeed()
{
	if (speed < maxSpeed / 2)
	{
		speed += 0.02 * Car::sysSpd;
		if (speed > maxSpeed / 2)
		{
			speed = maxSpeed / 2;
		}
	}
	else
	{
		if (speed > maxSpeed / 2)
		{
			speed -= 0.02 * Car::sysSpd;
			if (speed < maxSpeed / 2) speed = maxSpeed / 2;
		}
	}
}

void Car::fastBreak()
{
	speed -= 0.06 * Car::sysSpd;
	if (speed < 0) speed = 0;
}

void Car::slowBreak()
{
	if (speed > maxSpeed / 2)
	{
		speed -= 0.02 * Car::sysSpd;
		if (speed < maxSpeed / 2) speed = maxSpeed / 2;
	}
}

void Car::manipulate(int a)
{
	switch (a)
	{
	case 1:
		accelerate();
		break;
	case 2:
		halfSpeed();
		break;
	case 3:
		fastBreak();
		break;
	case 4:
		slowBreak();
		break;
	}
}

void Car::findFwdCar()
{
	if (pos->car.empty())
	{
		isFwdCar = false;
	}
	else
	{
		fwdCar = pos->car.front();
		if (fwdCar->dist > dist)
		{
			isFwdCar = true;
		}
		else
		{
			isFwdCar = false;
		}
		for (std::vector<CarRef>::iterator i = pos->car.begin(); i < pos->car.end(); ++i)
		{
			if ((*i)->dist > dist && (*i)->dist < fwdCar->dist)
			{
				isFwdCar = true;
				fwdCar = *i;
			}
		}
		if (fwdCar == thisCar)
		{
			isFwdCar = false;
		}
	}
}

double Car::breakDist()
{
	return dist + (speed + 0.003 * (speed / 0.006 - 1)) * speed / 0.006 + 0.1;
}

bool Car::busy(CroadRef c)
{
	if (c->busy && c->car != thisCar) return true;
	return false;
}

void Car::interactCar()
{
	if (dist > fwdCar->dist - 3 - speed * 20)
	{
		fastBreak();
	}
	else
	{
		accelerate();
	}
}

void Car::interactCroad()
{
	if (breakDist() > pos->len - 2.5 && busy(pos->end))
	{
		fastBreak();
	}
	else
	{
		accelerate();
	}
}

void Car::interact()
{
	if (!manual)
	{
		findFwdCar();
		if (isFwdCar)
		{
			interactCar();
		}
		else
		{
			interactCroad();
		}

		// SpdAction action;
		// SpdAction intCar = interactCar();
		// SpdAction intCroad = interactCroad();
		// // action = intCroad;
		// action = intCroad;
		// if (intCar > intCroad) action = intCar;

		// switch (action)
		// {
		// case FastBreak: fastBreak(); break;
		// case SlowBreak: slowBreak(); break;
		// case HalfSpeed: halfSpeed(); break;
		// case Idle: accelerate(); break;
		// }
	}
}