#ifndef CSE167_Node_h
#define CSE167_Node_h

#include <iostream>
#include "Matrix4.h"
#include "Material.h"
#include "Vector3.h"

class Node
{

public:

	Node(void);
	~Node(void);

	virtual void draw(Matrix4 C) = 0;
	virtual void draw(Matrix4 C, Vector3 v, double d, int wire) = 0;
	virtual void update() = 0;
	virtual void drawBoundingSphere(void);

	Vector3 center;
	double radius;

	bool bounding;
};

#endif