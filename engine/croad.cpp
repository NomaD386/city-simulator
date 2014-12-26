#include <GL/glut.h>
#include <math.h>
#include "croad.h"
#include "../ui/map.h"

extern Map map;

Croad::Croad(Pos _pos)
{
	busy = false;
	pos = _pos;
}

Croad::~Croad()
{
	busy = false;
	road.clear();
}

void Croad::render()
{
	static const double PI = acos(-1);
	static Pos p;
	p.x = (pos.x + map.d.x) * map.scale;
	p.y = (pos.y + map.d.y) * map.scale;
	
	glColor3d(0.5, 0.5, 0.5);
	
	// if (temp.state == TUnknown) glColor3d(0.5, 0.5, 0.5);
	// if (temp.state == TInside) glColor3d(1, 1, 1);
	// if (temp.state == TBorder) glColor3d(0, 1, 0);
	// if (temp.state == TStart) glColor3d(1, 0, 0);
	// if (busy) glColor3d(1.0, 0.0, 0.0);
	// else glColor3d(0.0, 1.0, 0.0);
	
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * PI; i += 0.1)
	{
		glVertex2d(p.x + cos(i) * map.scale * 2, p.y + sin(i) * map.scale * 2);
	}
	glEnd();
	
	glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (double i = 0; i < 2 * PI; i += 0.1)
	{
		glVertex2d(p.x + cos(i) * map.scale * 2, p.y + sin(i) * map.scale * 2);
	}
	glEnd();
}

void Croad::renderRoad()
{
	for (std::vector<Road>::iterator i = road.begin(); i < road.end(); i++)
	{
		i->render();
	}
}