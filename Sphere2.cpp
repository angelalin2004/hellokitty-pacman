#include "Sphere2.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


Sphere2::Sphere2(double radius, int slices, int stacks) : Geode()
{
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;
}

void Sphere2::update()
{
	//
}

void Sphere2::render()
{
	glutSolidSphere(1.0, 50, 50);
}