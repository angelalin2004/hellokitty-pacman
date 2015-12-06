#ifndef House_h
#define House_h

#include "Drawable.h"

class House : public Drawable {

public:
	
	int nVerts;
	static float vertices[126];
	static float colors[126];
	static int indices[60];

	virtual void draw(DrawData&);
};

#endif
