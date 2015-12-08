#include "Group.h"
#include <algorithm>
#include <GL/glut.h>

Group::Group() : Node()
{
	angle = 0.0;
	swingup = true;

	dist = 0.0;
	dir = 0;
	rad = 7;
	move = 5.0;
}

Group::~Group()
{
	//
}

void Group::addChild(Node * n) {
	children.push_back(n);
}

void Group::draw(Matrix4 C)
{
	/*
	if (bounding) {
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		Matrix4 C2 = C;
		float max = std::max(std::max(C2.get(0, 0), C2.get(1, 1)), C2.get(2, 2));
		max = max * rad;
		if (this->rad == 1)
			std::cout << "rad: " << rad << std::endl;
		float down = C2.get(3, 1) + move;
		//C.print("before");
		C2.set(0, 0, max);
		C2.set(1, 1, max);
		C2.set(2, 2, max);
		C2.set(3, 1, down);
		//C.print("after");
		glMultMatrixf(C2.ptr());
		drawBoundingSphere(1);
		glPopMatrix();
	}
	*/
	for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
		// *it is a Node *
		(*it)->bounding = this->bounding;
		(*it)->draw(C);
	}


}

void Group::draw(Matrix4 C, Vector3 v, double r, int wire)
{
	if (bounding && wire == 1) {
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		Matrix4 C2 = C;
		float max = std::max(std::max(C2.get(0, 0), C2.get(1, 1)), C2.get(2, 2));
		max = max * 7;
		float down = C2.get(3, 1) - 0.5;
		//C.print("before");
		C2.set(0, 0, max);
		C2.set(1, 1, max);
		C2.set(2, 2, max);
		C2.set(3, 1, down);
		//C.print("after");
		glMultMatrixf(C2.ptr());
		drawBoundingSphere();
		glPopMatrix();
		wire = 0;
	}
	
	int c = 0;
	if (wire >= 2)
		wire--;
	for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
		// *it is a Node *
		r = radius*r;
		v = center.add(v);
		(*it)->bounding = this->bounding;
		(*it)->draw(C,v,r, wire);
	}

	
}

void Group::update( )
{
	if (swingup) {
		angle += 1.0;
		if (angle > 30.0) {
			swingup = false;
			angle = 30.0;
		}		
	}
	else {
		angle -= 1.0;
		if (angle < -30.0) {
			swingup = true;
			angle = -30.0;
		}

	}

	dist += 0.1;
	if (dist > 9.0) {
		dir++;
		if (dir > 12)
			dir = 1;
		dist = 0.0;
	}

}