#ifndef CSE167_RandHouse_h
#define CSE167_RandHouse_h

#include <iostream>
#include "Geode.h"

class RandHouse : public Geode
{

public:
	Material material;

	float size;
	float halfSize;

	RandHouse();
	virtual ~RandHouse(void);

	virtual void update();
	virtual void render();


};

#endif