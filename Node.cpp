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
	colliding = false;
}

Node::~Node()
{

}

void Node::drawBoundingSphere() {
	float default[4] = { 0.0, 1.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)default);
	
	default[0] = 0.8;
	default[1] = 0.8;
	default[2] = 0.8;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)default);
	
	glutWireSphere(radius, 50, 50);
	default[0] = 0.2;
	default[1] = 0.2;
	default[2] = 0.2;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)default);
}

void Node::drawBoundingSphere(int i) {
	float default[4] = { 0.0, 0.0, 0.0, 1.0 };;
	if (i == 0)
		default[0] = 1.0;
	else if (i == 1) {
		default[1] = 1.0;
	}
	else if (i == 2) {
		default[2] = 1.0;
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)default);
	glutWireCube(radius*1.5);
	//glutWireSphere(radius, 50, 50);
	default[0] = 0.2;
	default[1] = 0.2;
	default[2] = 0.2;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)default);
}
