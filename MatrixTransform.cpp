#include "MatrixTransform.h"

MatrixTransform::MatrixTransform(Matrix4 m) : Group()
{
	M = m;
}

MatrixTransform::~MatrixTransform()
{
	
}

void MatrixTransform::draw(Matrix4 C, Vector3 v, double r, int wire)
{
	C = C * M;
	Group::draw(C,v,r, wire);
}

void MatrixTransform::update()
{
	std::cout << "Warning: update() is not implemented" << std::endl;
}