#ifndef __MAP_H__
#define __MAP_H__

#include "../engine/pos.h"

enum Direction { Up, Down, Left, Right };

class Map
{
public:
	Map();
	~Map();

	void advance();
	void setScale(double s);
	void setDispl(Pos d);
	void moveScale(double s);
	void moveDispl(Pos d);
	void reset();
	void move(Direction dir);
	void zoomIn();
	void zoomOut();

	static double scale;
	static Pos d;
	static double _scale;
	static Pos _d;
};

#endif