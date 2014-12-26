#include "temp.h"

Temp::Temp()
{
	state = TUnknown;
	dist = 0;
}

Temp::~Temp()
{
	
}

void Temp::clear()
{
	state = TUnknown;
	dist = 0;
}