#include "Kitty.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "math.h"

Kitty::Kitty(OBJObject * k) : Geode()
{
	kitty = k;
	
	material.emissionColor = Color(0.2, 0.2, 0.2);

}

Kitty::~Kitty()
{
	//Delete any dynamically allocated memory/objects here
}

void Kitty::update()
{
	//
}

void Kitty::render()
{
	
	kitty->material = material;
	kitty->draw(Globals::drawData);
}