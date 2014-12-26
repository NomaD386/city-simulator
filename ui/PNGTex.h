#ifndef __PNGTEX_H__
#define __PNGTEX_H__

#include <GL/glut.h>
#include <vector>
#include "lodepng.h"

class PNGTex
{
public:
	PNGTex();
	PNGTex(const char* filename);
	~PNGTex();

	void load(const char* filename);
	void use();
	
	unsigned _w;
	unsigned _h;

private:
	GLuint* texture;
	bool loaded;
};

#endif