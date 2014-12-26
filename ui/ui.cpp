#include <GL/glut.h>
#include <stdlib.h>
#include <ctime>
#include "ui.h"
#include "../engine/trsys.h"
#include "map.h"

Trsys *trsys;
Map map;

bool k[256]; // key pressed
bool ks[256]; // special

enum MouseState { MapMove, Other };

MouseState mouseState;
Pos mousePos;

void display();
void timer(int);
void adv(int);
void mouseMove(int, int);
void mousePressed(int, int, int, int);
void mousePrMove(int, int);
void keyboard(unsigned char, int, int);
void keyboardUp(unsigned char, int, int);
void specialKeyboard(int, int, int);
void specialKeyboardUp(int, int, int);

void init(int argc, const char **argv)
{
	srand(time(NULL));
	glutInit(&argc, (char **)argv);
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	// w = 640;
	// h = 480;
	glutInitWindowSize(w, h);
	glutCreateWindow("Transport system");
	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glOrtho(- w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	trsys = new Trsys;
	
	glutDisplayFunc(display);
	glutTimerFunc(20, timer, 0);
	glutTimerFunc(10, adv, 0);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mousePrMove);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKeyboard);
	glutSpecialUpFunc(specialKeyboardUp);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	
	glutFullScreen();
	
	// help.init();
	
	glutMainLoop();
}

/////////////////////////////////////////////////////////
//                      CALLBACKS                      //
/////////////////////////////////////////////////////////

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (k[(int)'w']) map.move(Up);
	if (k[(int)'s']) map.move(Down);
	if (k[(int)'a']) map.move(Left);
	if (k[(int)'d']) map.move(Right);
	if (k[(int)'+'] || k[(int)'=']) map.zoomIn();
	if (k[(int)'-']) map.zoomOut();
	if (ks[GLUT_KEY_PAGE_DOWN]) trsys->slower();
	if (ks[GLUT_KEY_PAGE_UP]) trsys->faster();
	
	if (k[(int)'1']) trsys->manipulate(1);
	if (k[(int)'2']) trsys->manipulate(2);
	if (k[(int)'3']) trsys->manipulate(3);
	if (k[(int)'4']) trsys->manipulate(4);
	
	map.advance();
	trsys->render();
	glutSwapBuffers();
}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(20, timer, 0);
}

void adv(int)
{
	trsys->advance();
	glutTimerFunc(2, adv, 0);
}

void mouseMove(int, int)
{

}

void mousePressed(int button, int state, int x, int y)
{
	if (button == 3) map.zoomIn();
	if (button == 4) map.zoomOut();
	mouseState = MapMove;
	mousePos = Pos(x, y);
	mousePrMove(-1, -1);
}


void mousePrMove(int x, int y)
{
	static Pos prev;
	if (x < 0)
	{
		prev = mousePos;
	}
	else
	{
		if (mouseState == MapMove)
		{
			map._d.x = map._d.x - (prev.x - x) / map.scale;
			map._d.y = map._d.y + (prev.y - y) / map.scale;
			prev = Pos(x, y);
		}
	}
}

void keyboard(unsigned char key, int, int)
{
	if (key == 27)
	{
		delete trsys;
		exit(0);
	}
	switch (key)
	{
	case '1':
		trsys->manipulate(1);
		break;
	case '2':
		trsys->manipulate(2);
		break;
	case '3':
		trsys->manipulate(3);
		break;
	case '4':
		trsys->manipulate(4);
		break;
	case 'r':
		map.reset();
		break;
	case ' ':
		trsys->pause();
		break;
	case 't':
		trsys->followCar();
		break;
	case 'e':
		trsys->renderDest();
		break;
	case 'c':
		trsys->clearTemp();
		break;
	}
	k[key] = true;
}

void keyboardUp(unsigned char key, int, int)
{
	k[key] = false;
}

void specialKeyboard(int key, int, int)
{
	ks[key] = true;
}
void specialKeyboardUp(int key, int, int)
{
	ks[key] = false;
}