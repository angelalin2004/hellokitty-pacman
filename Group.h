#ifndef CSE167_Group_h
#define CSE167_Group_h

#include <iostream>
#include <list>

#include "Node.h"

class Group : public Node
{

public:

	Group(void);
	~Group(void);

	std::list<Node*> children;

	void addChild(Node *);
	void removeChild(Node *);
	virtual void draw(Matrix4 C);
	virtual void draw(Matrix4 C, Vector3 v, double d, int wire);
	virtual void update();

	float angle;
	bool swingup;

	float dist;
	int dir;

};

#endif