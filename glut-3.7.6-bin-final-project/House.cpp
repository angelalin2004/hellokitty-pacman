#include "House.h"
#include "math.h"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

// This data structure defines a simple house

int nVerts = 42;    // your vertex array needs to have this many entries

					// These are the x,y,z coordinates of the vertices of the triangles
float House::vertices[] = {
	-4,-4,4, 4,-4,4, 4,4,4, -4,4,4,     // front face
	-4,-4,-4, -4,-4,4, -4,4,4, -4,4,-4, // left face
	4,-4,-4,-4,-4,-4, -4,4,-4, 4,4,-4,  // back face
	4,-4,4, 4,-4,-4, 4,4,-4, 4,4,4,     // right face
	4,4,4, 4,4,-4, -4,4,-4, -4,4,4,     // top face
	-4,-4,4, -4,-4,-4, 4,-4,-4, 4,-4,4, // bottom face

	-20,-4,20, 20,-4,20, 20,-4,-20, -20,-4,-20, // grass
	-4,4,4, 4,4,4, 0,8,4,                       // front attic wall
	4,4,4, 4,4,-4, 0,8,-4, 0,8,4,               // left slope
	-4,4,4, 0,8,4, 0,8,-4, -4,4,-4,             // right slope
	4,4,-4, -4,4,-4, 0,8,-4 };                   // rear attic wall

												 // These are the RGB colors corresponding to the vertices, in the same order
float House::colors[] = {
	1,0,0, 1,0,0, 1,0,0, 1,0,0,  // front is red
	0,1,0, 0,1,0, 0,1,0, 0,1,0,  // left is green
	1,0,0, 1,0,0, 1,0,0, 1,0,0,  // back is red
	0,1,0, 0,1,0, 0,1,0, 0,1,0,  // right is green
	0,0,1, 0,0,1, 0,0,1, 0,0,1,  // top is blue
	0,0,1, 0,0,1, 0,0,1, 0,0,1,  // bottom is blue

	0,0.5,0, 0,0.5,0, 0,0.5,0, 0,0.5,0, // grass is dark green
	0,0,1, 0,0,1, 0,0,1,                // front attic wall is blue
	1,0,0, 1,0,0, 1,0,0, 1,0,0,         // left slope is green
	0,1,0, 0,1,0, 0,1,0, 0,1,0,         // right slope is red
	0,0,1, 0,0,1, 0,0,1, };              // rear attic wall is red

										 // The index data stores the connectivity of the triangles; 
										 // index 0 refers to the first triangle defined above
int House::indices[] = {
	0,2,3,    0,1,2,      // front face
	4,6,7,    4,5,6,      // left face
	8,10,11,  8,9,10,     // back face
	12,14,15, 12,13,14,   // right face
	16,18,19, 16,17,18,   // top face
	20,22,23, 20,21,22,   // bottom face

	24,26,27, 24,25,26,   // grass
	28,29,30,             // front attic wall
	31,33,34, 31,32,33,   // left slope
	35,37,38, 35,36,37,   // right slope
	39,40,41 };            // rear attic wall


void House::draw(DrawData& data)
{

	//Apply the material properties
	//From here forward anything drawn will be drawn with this material
	material.apply();

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
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < sizeof(indices)/sizeof(indices[0]); i++) {
		glColor3f(colors[indices[i] * 3], colors[indices[i] * 3 + 1], colors[indices[i] * 3 + 2]);
		glVertex3f(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
	}

	glEnd();

	//The above glBegin, glEnd, glNormal and glVertex calls can be replaced with a glut convenience function
	//glutSolidCube(size);

	//Pop the save state off the matrix stack
	//This will undo the multiply we did earlier
	glPopMatrix();
}
