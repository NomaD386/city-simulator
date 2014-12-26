#include "map.h"

const double maxScale = 500;
const double minScale = 1;
const double defScale = 6;


double Map::scale = defScale;
Pos Map::d = Pos(0, 0);
double Map::_scale = defScale;
Pos Map::_d = Pos(0, 0);


Map::Map()
{}

Map::~Map()
{}

void Map::advance()
{
	if (d.distance(_d) > 100)
	{
		d = _d;
	}
	d.x -= (d.x - _d.x) / 10;
	d.y -= (d.y - _d.y) / 10;
	scale -= (scale - _scale) / 5;
}

void Map::setScale(double s)
{
	scale = s;
}

void Map::setDispl(Pos _d)
{
	d = _d;
}

void Map::moveScale(double s)
{
	scale += s;
}

void Map::moveDispl(Pos _d)
{
	d.x += _d.x;
	d.y += _d.y;
}

void Map::reset()
{
	scale = defScale;
	d = Pos(0, 0);
	_scale = defScale;
	_d = Pos(0, 0);
}

void Map::move(Direction dir)
{
	switch(dir)
	{
	case Up:
		_d.y += 10 / scale;
		break;
	case Down:
		_d.y -= 10 / scale;
		break;
	case Left:
		_d.x -= 10 / scale;
		break;
	case Right:
		_d.x += 10 / scale;
		break;
	}
}

void Map::zoomIn()
{
	_scale *= 1.05;
	if (_scale > maxScale)
	{
		_scale = maxScale;
	}
}

void Map::zoomOut()
{
	_scale /= 1.05;
	if (_scale < minScale)
	{
		_scale = minScale;
	}
}