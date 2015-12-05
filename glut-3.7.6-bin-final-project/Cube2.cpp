#include "Cube2.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "math.h"

Cube2::Cube2(float size) : Geode()
{
	this->size = size;
	this->halfSize = size / 2.0;
}

Cube2::~Cube2()
{
	//Delete any dynamically allocated memory/objects here
}

void Cube2::update()
{
	//
}

void Cube2::render()
{
	glutSolidCube(1.0);
}

