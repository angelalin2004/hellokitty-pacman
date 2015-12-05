#include "Node.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Node::Node()
{
	radius = 1.1;
	center = Vector3(0.0, 0.0, 0.0);
	bounding = false;
}

Node::~Node()
{

}

void Node::drawBoundingSphere() {
	float default[4] = { 0.0, 1.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)default);
	glutWireSphere(radius, 50, 50);
	default[0] = 0.2;
	default[1] = 0.2;
	default[2] = 0.2;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)default);
}
