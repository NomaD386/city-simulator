#ifndef __POS_H__
#define __POS_H__

class Pos
{
public:
	Pos();
	Pos(double x, double y);
	~Pos();

	double x;
	double y;
	
	double distance();
	double distance(Pos other);
	double angle();
	double angle(Pos other);
};

#endif