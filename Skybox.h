#ifndef CSE167_Skybox_h
#define CSE167_Skybox_h

#include "Drawable.h"
#include "Texture.h"

class Skybox : public Drawable
{

public:

	float halfSize;
	Texture * back;
	Texture * base;
	Texture * front;
	Texture * left;
	Texture * right;
	Texture * top;

	Skybox();
	virtual ~Skybox(void);

	virtual void draw(DrawData&);
	virtual void update(UpdateData&);


};

#endif