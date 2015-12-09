#ifndef CSE167_RandHouse_h
#define CSE167_RandHouse_h

#include "Shader.h"
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
	Shader * shader;
	Material material;
	int brand, rrand, drand;
	bool makered;
	

	std::vector<Drawable*>* drawablesf, *drawables_body, *drawables_roof, *drawables_door;
	Matrix4 changef;
	OBJObject * body1, *body2, *body3, *body4, *body5;
	OBJObject *roof1, *roof2, *roof3, *roof4, *roof5;
	OBJObject *door1, *door2, *door3, *door4, *door5;
	OBJObject * body, *roof, *door;

	RandHouse();
	RandHouse(OBJObject *, OBJObject *, OBJObject *);
	virtual ~RandHouse(void);

	virtual void update();
	virtual void render();
	virtual void randomizeRender();
	virtual void makeRed();
	virtual void makeNotRed();

};

#endif