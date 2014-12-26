#include <GL/glut.h>
#include <math.h>
#include "road.h"
#include "../ui/map.h"
#include <iostream>

extern Map map;

const double PI = acos(-1);

Road::Road(CroadRef _begin, CroadRef _end)
{
	begin = _begin;
	end = _end;
	
	// count len
	len = begin->pos.distance(end->pos);
	angle = begin->pos.angle(end->pos);
	countPoints();
}

Road::~Road()
{
	car.clear();
}

void Road::render()
{
	glColor3d(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2d((points.edgeB.x + map.d.x) * map.scale, (points.edgeB.y + map.d.y) * map.scale);
		glVertex2d((points.edgeE.x + map.d.x) * map.scale, (points.edgeE.y + map.d.y) * map.scale);
	glEnd();
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex2d((points.splB.x + map.d.x) * map.scale, (points.splB.y + map.d.y) * map.scale);
		glVertex2d((points.splE.x + map.d.x) * map.scale, (points.splE.y + map.d.y) * map.scale);
	glEnd();
}

void Road::countPoints()
{
	points.splB = begin->pos;
	points.splE = end->pos;
	points.edgeB = begin->pos;
	points.edgeE = end->pos;
	points.edgeB.x += cos(angle + PI / 2) * 2;
	points.edgeB.y += sin(angle + PI / 2) * 2;
	points.edgeE.x += cos(angle + PI / 2) * 2;
	points.edgeE.y += sin(angle + PI / 2) * 2;
}

double Road::avgSpeed()
{
	return 1;
}

double Road::lenth(void)
{
	return len / avgSpeed();
}