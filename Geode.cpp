#include "Geode.h"
#include <algorithm>

Geode::Geode() : Node()
{

}

Geode::~Geode()
{
	//
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
