#ifndef CSE167_Cube2_h
#define CSE167_Cube2_h

#include <iostream>
#include "Geode.h"

class Cube2 : public Geode
{

public:
	Material material;

	float size;
	float halfSize;

	Cube2(float);
	virtual ~Cube2(void);

	virtual void update();
	virtual void render();


};

#endif