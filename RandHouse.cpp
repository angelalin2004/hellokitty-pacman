#include "RandHouse.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "math.h"

RandHouse::RandHouse() : Geode()
{
	this->size = size;
	this->halfSize = size / 2.0;
}

RandHouse::~RandHouse()
{
	//Delete any dynamically allocated memory/objects here
}

void RandHouse::update()
{
	//
}

void RandHouse::render()
{
	glutSolidCube(1.0);
}

