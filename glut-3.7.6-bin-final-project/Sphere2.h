#ifndef CSE167_Sphere2_h
#define CSE167_Sphere2_h

#include <iostream>
#include "Geode.h"

class Sphere2 : public Geode
{

public:

	Material material;

	double radius;
	int slices, stacks;

	Sphere2(double, int, int);

	virtual void update();
	virtual void render();
};

#endif
