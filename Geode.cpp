#include "Geode.h"
#include <algorithm>

Geode::Geode() : Node()
{
	rad = 7;
	move = 5.0;
}

Geode::~Geode()
{
	//
}

void Geode::draw(Matrix4 C)
{
	// set OpenGL's ModelView matrix to the current C matrix


	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(C.ptr());
	render();
	glPopMatrix();

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	float max = std::max(std::max(C.get(0, 0), C.get(1, 1)), C.get(2, 2)) *rad;
	float down = C.get(3, 1) + move;
	C.set(0, 0, max);
	C.set(1, 1, max);
	C.set(2, 2, max);
	C.set(3, 1, down);
	glMultMatrixf(C.ptr());
	if (bounding) {
		if (colliding) {
			drawBoundingSphere(0);
		}
		else {
			drawBoundingSphere(2);
		}
	}
	glPopMatrix();
}

void Geode::draw(Matrix4 C, Vector3 v, double r, int wire)
{
	// set OpenGL's ModelView matrix to the current C matrix
	
	
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(C.ptr());
	render();
	glPopMatrix();

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	float max = std::max(std::max(C.get(0, 0), C.get(1, 1)), C.get(2, 2));
	C.set(0, 0, max);
	C.set(1, 1, max);
	C.set(2, 2, max);
	glMultMatrixf(C.ptr());
	if (bounding) {
		drawBoundingSphere();
	}
	glPopMatrix();
}

void Geode::update()
{
	std::cout << "Warning: update() is not implemented" << std::endl;
}
