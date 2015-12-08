#ifndef CSE167_Geode_h
#define CSE167_Geode_h

#include <iostream>
#include <GL/glut.h>
#include "Node.h"


class Geode : public Node
{

public:

	Geode(void);
	~Geode(void);

	virtual void draw(Matrix4 C);
	virtual void draw(Matrix4 C, Vector3 v, double r, int wire);
	virtual void update();
	virtual void render() = 0;

	int rad;
	double move;
};

#endif