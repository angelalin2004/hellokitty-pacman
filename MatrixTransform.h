#ifndef CSE167_MatrixTransform_h
#define CSE167_MatrixTransform_h

#include <iostream>
#include "Group.h"



class MatrixTransform : public Group
{

public:

	Matrix4 M;

	MatrixTransform(Matrix4 m);
	~MatrixTransform(void);

	virtual void draw(Matrix4 C);
	virtual void draw(Matrix4 C, Vector3 v, double r, int wire);
	virtual void update();

};

#endif