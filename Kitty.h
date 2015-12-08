#ifndef CSE167_Kitty_h
#define CSE167_Kitty_h

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Geode.h"
#include "OBJObject.h"
#include "Matrix4.h"
#include "Globals.h"

class Kitty : public Geode
{

public:
	Material material;
	OBJObject * kitty;

	Kitty(OBJObject * k);
	virtual ~Kitty(void);

	virtual void update();
	virtual void render();

};

#endif