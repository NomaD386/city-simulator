#include "PNGTex.h"

PNGTex::PNGTex()
{
	loaded = false;
}

PNGTex::PNGTex(const char* filename)
{
	
}

PNGTex::~PNGTex()
{
	glDeleteTextures(1, texture);
	delete[] texture;
}

void PNGTex::load(const char *fileName)
{
	if (!loaded)
	{
		texture = new GLuint[1];
		std::vector<unsigned char> image;
		unsigned error = lodepng::decode(image, _w, _h, fileName);
		if (error != 0) exit(7);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
		image.clear();
		loaded = true;
	}
}

void PNGTex::use(void)
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
}