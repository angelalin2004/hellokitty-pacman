#ifndef CSE167_RandHouse_h
#define CSE167_RandHouse_h

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Geode.h"
#include "OBJObject.h"
#include "Matrix4.h"
#include "Globals.h"

class RandHouse : public Geode
{

public:
	Material material;
	int brand, rrand, drand;

	std::vector<Drawable*>* drawablesf, *drawables_body, *drawables_roof, *drawables_door;
	Matrix4 changef;
	OBJObject * body1, *body2, *body3, *body4, *body5;
	OBJObject *roof1, *roof2, *roof3, *roof4, *roof5;
	OBJObject *door1, *door2, *door3, *door4, *door5;

	RandHouse(int, int, int);
	virtual ~RandHouse(void);

	virtual void update();
	virtual void render();
	virtual void randomizeRender();


};

#endif