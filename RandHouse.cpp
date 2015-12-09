#include "RandHouse.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "math.h"

RandHouse::RandHouse() : Geode()
{
	randomizeRender();
	makered = false;
	shader = new Shader("../toon.vert", "../toon.frag", true);
	/*
	//drawablesf = new std::vector<Drawable*>();
	drawables_body = new std::vector<Drawable*>();
	drawables_roof = new std::vector<Drawable*>();
	drawables_door = new std::vector<Drawable*>();

	
	body1 = new OBJObject("../Body1T.obj");
	changef.makeScale(0.35);
	body1->toWorld = body1->toWorld * changef;
	body2 = new OBJObject("../Body2T.obj");
	body3 = new OBJObject("../Body3T.obj");
	body4 = new OBJObject("../Body4T.obj");
	changef.makeScale(0.65, 0.65, 0.75);
	body4->toWorld = body4->toWorld * changef;
	body5 = new OBJObject("../Body5T.obj");
	changef.makeScale(1.0, 0.45, 1.0);
	body5->toWorld = body5->toWorld * changef;
	drawables_body->push_back(body1);
	drawables_body->push_back(body2);
	drawables_body->push_back(body3);
	drawables_body->push_back(body4);
	drawables_body->push_back(body5);


	roof1 = new OBJObject("../Roof1T.obj");
	changef.makeTranslate(0.0, -4.0, 0.0);
	roof1->toWorld = roof1->toWorld * changef;
	roof2 = new OBJObject("../Roof2T.obj");
	roof3 = new OBJObject("../Roof3T.obj");
	roof4 = new OBJObject("../Roof4T.obj");
	roof5 = new OBJObject("../Roof5T.obj");
	drawables_roof->push_back(roof1);
	drawables_roof->push_back(roof2);
	drawables_roof->push_back(roof3);
	drawables_roof->push_back(roof4);
	drawables_roof->push_back(roof5);



	door1 = new OBJObject("../Door1T.obj");
	door2 = new OBJObject("../Door2T.obj");
	door3 = new OBJObject("../Door3T.obj");
	door4 = new OBJObject("../Door4T.obj");
	door5 = new OBJObject("../Door5T.obj");
	changef.makeScale(0.85);
	door5->toWorld = door5->toWorld * changef;
	changef.makeTranslate(0.0, 1.0, 0.0);
	door5->toWorld = door5->toWorld * changef;
	drawables_door->push_back(door1);
	drawables_door->push_back(door2);
	drawables_door->push_back(door3);
	drawables_door->push_back(door4);
	drawables_door->push_back(door5);

	// scale down objects
	for (int i = 0; i < drawables_body->size(); i++) {
		changef.makeScale(0.05);
		drawables_body->at(i)->toWorld = drawables_body->at(i)->toWorld * changef;
		changef.makeTranslate(0.0, -5.0, 0.0);
		drawables_body->at(i)->toWorld = changef*drawables_body->at(i)->toWorld;
	}
	for (int i = 0; i < drawables_roof->size(); i++) {
		changef.makeTranslate(0.0, 7.0, 0.0);
		drawables_roof->at(i)->toWorld = changef*drawables_roof->at(i)->toWorld;
		changef.makeScale(0.05);
		drawables_roof->at(i)->toWorld = drawables_roof->at(i)->toWorld * changef;
		changef.makeTranslate(0.0, -5.0, 0.0);
		drawables_roof->at(i)->toWorld = changef*drawables_roof->at(i)->toWorld;
	}
	for (int i = 0; i < drawables_door->size(); i++) {
		changef.makeTranslate(0.0, 2.0, 4.25);
		drawables_door->at(i)->toWorld = changef*drawables_door->at(i)->toWorld;
		changef.makeScale(0.05);
		drawables_door->at(i)->toWorld = drawables_door->at(i)->toWorld * changef;
		// scale doors thicker
		changef.makeScale(1.0, 1.0, 2.0);
		drawables_door->at(i)->toWorld = drawables_door->at(i)->toWorld * changef;
		// rotate along X axis 90 degrees
		changef.makeRotateX((-1.0) * 90 * 3.14159265 / 180.0);
		drawables_door->at(i)->toWorld = drawables_door->at(i)->toWorld * changef;
		// rotate along Z axis 90 degrees
		changef.makeRotateY(90 * 3.14159265 / 180.0);
		drawables_door->at(i)->toWorld = drawables_door->at(i)->toWorld * changef;
		changef.makeTranslate(0.0, -5.0, 0.0);
		drawables_door->at(i)->toWorld = changef*drawables_door->at(i)->toWorld;
	}
	*/
}

RandHouse::RandHouse(OBJObject * b, OBJObject * r, OBJObject * d ) : Geode()
{
	body = b;
	if (r == NULL) {
		std::cerr << "got here" << std::endl;
	}
	roof = r;
	door = d;
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
	//randomizeRender();
	material.ambientColor = Color(1.0, 0.0, 0.5);
	material.diffuseColor = Color::diffuseMaterialDefault();
	//material.emissionColor = Color(0.15, 0.15, 0.15);
	if (makered == true) {
		material.ambientColor = Color(1.0, 0.0, 0.0);
		material.diffuseColor = Color(1.0, 0.0, 0.0);
	}
	//shader->bind();
	body->material = material;
	body->draw(Globals::drawData);
	body->material.reset();	
	if (roof != NULL) {
		roof->material = material;
		roof->draw(Globals::drawData);
		roof->material.reset();
	}
	if (door != NULL) {
		door->material = material;
		door->draw(Globals::drawData);
		door->material.reset();
	}
}

void RandHouse::randomizeRender()
{
	srand(time(NULL));
	brand = rand() % 5;
	rrand = rand() % 5;
	drand = rand() % 5;
}

void RandHouse::makeRed()
{
	makered = true;
}

void RandHouse::makeNotRed()
{
	makered = false;
}