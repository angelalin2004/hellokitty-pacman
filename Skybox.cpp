#include "Skybox.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Window.h"
#include "math.h"

Skybox::Skybox() : Drawable()
{
	halfSize = 200.0;
	back = new Texture ("../darkcity_bk.ppm");
	base = new Texture("../darkcity_dn.ppm");
	front = new Texture("../darkcity_ft.ppm");
	left = new Texture("../darkcity_lf.ppm");
	right = new Texture("../darkcity_rt.ppm");
	top = new Texture("../darkcity_up.ppm");
}

Skybox::~Skybox()
{
	//Delete any dynamically allocated memory/objects here
}


void Skybox::draw(DrawData& data)
{

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glDisable(GL_LIGHTING);
	//Apply the material properties
	//From here forward anything drawn will be drawn with this material
	//material.apply();

	//Set the OpenGL Matrix mode to ModelView (used when drawing geometry)
	glMatrixMode(GL_MODELVIEW);

	//Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(toWorld.ptr());

	//Make cube!
	//Note: The glBegin, and glEnd should always be as close to the glNormal/glVertex calls as possible
	//These are special calls that 'freeze' many internal states of OpenGL.
	//Once the glBegin state is active many of the calls made to OpenGL (like glMultMatrixf) will be IGNORED!
	//As a good habit, only call glBegin just before you need to draw, and call end just after you finish

	
	// Draw front face:
	front->bind();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0), glVertex3f(-halfSize, -halfSize, halfSize);
	glTexCoord2d(0.0, 0.0), glVertex3f(-halfSize, halfSize, halfSize);
	glTexCoord2d(1.0, 0.0), glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2d(1.0, 1.0), glVertex3f(halfSize, -halfSize, halfSize);
	glEnd();
	front->unbind();


	// Draw left side:
	left->bind();
	glBegin(GL_QUADS);
	glTexCoord2d(1.0, 1.0), glVertex3f(-halfSize, -halfSize, halfSize);
	glTexCoord2d(0.0, 1.0), glVertex3f(-halfSize, -halfSize, -halfSize);
	glTexCoord2d(0.0, 0.0), glVertex3f(-halfSize, halfSize, -halfSize);
	glTexCoord2d(1.0, 0.0), glVertex3f(-halfSize, halfSize, halfSize);
	glEnd();
	left->unbind();

	// Draw right side:
	right->bind();
	glBegin(GL_QUADS);

	glTexCoord2d(1.0, 1.0), glVertex3f(halfSize, -halfSize, -halfSize);
	glTexCoord2d(0.0, 1.0), glVertex3f(halfSize, -halfSize, halfSize);
	glTexCoord2d(0.0, 0.0), glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2d(1.0, 0.0), glVertex3f(halfSize, halfSize, -halfSize);
	glEnd();
	right->unbind();

	// Draw back face:
	back->bind();
	glBegin(GL_QUADS);
	
	glTexCoord2d(0.0, 1.0), glVertex3f(halfSize, -halfSize, -halfSize);
	glTexCoord2d(0.0, 0.0), glVertex3f(halfSize, halfSize, -halfSize);
	glTexCoord2d(1.0, 0.0), glVertex3f(-halfSize, halfSize, -halfSize);
	glTexCoord2d(1.0, 1.0), glVertex3f(-halfSize, -halfSize, -halfSize);
	glEnd();
	back->unbind();

	// Draw top side:
	top->bind();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0), glVertex3f(-halfSize, halfSize, halfSize);
	glTexCoord2d(0.0, 0.0), glVertex3f(-halfSize, halfSize, -halfSize);
	glTexCoord2d(1.0, 0.0), glVertex3f(halfSize, halfSize, -halfSize);
	glTexCoord2d(1.0, 1.0), glVertex3f(halfSize, halfSize, halfSize);
	
	glEnd();
	top->unbind();

	// Draw bottom side:
	/*
	base->bind();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0), glVertex3f(-halfSize, -halfSize, halfSize);
	glTexCoord2d(1.0, 0.0), glVertex3f(halfSize, -halfSize, halfSize);
	glTexCoord2d(1.0, 1.0), glVertex3f(halfSize, -halfSize, -halfSize);
	glTexCoord2d(0.0, 1.0), glVertex3f(-halfSize, -halfSize, -halfSize);
	glEnd();
	base->unbind();
	*/
	glEnd();

	//The above glBegin, glEnd, glNormal and glVertex calls can be replaced with a glut convenience function
	//glutSolidCube(size);

	//Pop the save state off the matrix stack
	//This will undo the multiply we did earlier
	glPopMatrix();
	glEnable(GL_LIGHTING);
}


void Skybox::update(UpdateData& data)
{
	//
}