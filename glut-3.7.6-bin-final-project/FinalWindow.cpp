#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

#include "FinalWindow.h"
#include <stdlib.h>
#include <time.h>

int FinalWindow::width = 512;   //Set window width in pixels here
int FinalWindow::height = 512;   //Set window height in pixels here
Matrix4 changef;
Camera cameraf;
Drawable * drawablef;
std::vector<Drawable*>* drawablesf, *drawables_body, *drawables_roof;
int lastf;
int framef = 0, timef, timebasef = 0;
int draw_itemf;
bool Lmousedownf, Rmousedownf;
Vector3 lastPointf, currPointf, directionf, rotAxisf;
float velocityf, rot_anglef;
Light * directionalf;
Light * pointf;
Light * spotf;
Sphere * pointSpheref;
Sphere * spotSpheref;
bool ctrlf;
float wfovf;
OBJObject * body1, *body2, *body3, *body4, *body5;
OBJObject *roof1, *roof2, *roof3, *roof4, *roof5;

void FinalWindow::initialize(void)
{
	//Setup the light
	Vector4 lightPos(10.0, 10.0, 0.0, 1.0);
	Globals::light.position = lightPos;
	Globals::light.quadraticAttenuation = 0.02;

	directionalf = new Light();
	directionalf->position = Vector4(0.0, 100.0, 100.0, 0.0);
	//directional->diffuseColor = Color::red();

	pointf = new Light();
	pointf->position = Vector4(0.0, 10.0, 0.0, 1.0);
	//point->diffuseColor = Color::green();
	pointSpheref = new Sphere(0.2, 50, 50);
	pointSpheref->material.emissionColor = Color::green();
	changef.makeTranslate(pointf->position[0], pointf->position[1], pointf->position[2]);
	pointSpheref->toWorld = changef * pointSpheref->toWorld;

	spotf = new Light();
	spotf->position = Vector4(0.0, -11.0, 0.0, 1.0);
	//spot->diffuseColor = Color::blue();
	spotf->isSpot = true;
	spotSpheref = new Sphere(0.2, 50, 50);
	spotSpheref->material.emissionColor = Color::blue();
	changef.makeTranslate(spotf->position[0], spotf->position[1], spotf->position[2]);
	spotSpheref->toWorld = changef * spotSpheref->toWorld;

	//Initialize sphere matrix:
	Globals::sphere.toWorld.identity();

	//Setup the cube's material properties
	//Color color(0x23ff27ff);

	Globals::sphere.material.diffuseColor = Color(1.0, 0.5, 0.0);


	drawablesf = new std::vector<Drawable*>();
	drawables_body = new std::vector<Drawable*>();
	drawables_roof = new std::vector<Drawable*>();
	lastf = 0;
	draw_itemf = 0;
	body1 = new OBJObject("../Body1T.obj");
		changef.makeScale(0.35);
		body1->toWorld = body1->toWorld * changef;
	body2 = new OBJObject("../Body2T.obj");
	body3 = new OBJObject("../Body3T.obj");
	body4 = new OBJObject("../Body4T.obj");
		changef.makeScale(0.65);
		body4->toWorld = body4->toWorld * changef;
	body5 = new OBJObject("../Body5T.obj");
		changef.makeScale(0.5);
		body5->toWorld = body5->toWorld * changef;
	drawables_body->push_back(body1);
	drawables_body->push_back(body2);
	drawables_body->push_back(body3);
	drawables_body->push_back(body4);
	drawables_body->push_back(body5);

	roof1 = new OBJObject("../Roof1T.obj");
	roof2 = new OBJObject("../Roof2T.obj");
	roof3 = new OBJObject("../Roof3T.obj");
	roof4 = new OBJObject("../Roof4T.obj");
	roof5 = new OBJObject("../Roof5T.obj");
	drawables_roof->push_back(roof1);
	drawables_roof->push_back(roof2);
	drawables_roof->push_back(roof3);
	drawables_roof->push_back(roof4);
	drawables_roof->push_back(roof5);
	
	

	// scale down objects
	for (int i = 0; i < drawables_body->size(); i++) {
		changef.makeScale(0.05);
		drawables_body->at(i)->toWorld = drawables_body->at(i)->toWorld * changef;
	}
	for (int i = 0; i < drawables_roof->size(); i++) {
		changef.makeTranslate(0.0, 7.0, 0.0);
		drawables_roof->at(i)->toWorld = changef*drawables_roof->at(i)->toWorld;
		changef.makeScale(0.05);
		drawables_roof->at(i)->toWorld = drawables_roof->at(i)->toWorld * changef;
	}
	srand(time(NULL));
	int brand = rand() % 5;
	std::cout << "brand: " << brand << std::endl;
	int rrand = rand() % 5;
	std::cout << "rrand: " << rrand << std::endl;
	drawablesf->push_back(drawables_body->at(brand));
	drawablesf->push_back(drawables_roof->at(rrand));

	for (int i = 0; i < drawablesf->size(); i++) {
		changef.makeTranslate(0.0, -5.0, 0.0);
		drawablesf->at(i)->toWorld = changef*drawablesf->at(i)->toWorld;
	}
	

	Lmousedownf = Rmousedownf = false;
	ctrlf = false;



	wfovf = 60.0;


}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void FinalWindow::idleCallback()
{
	//Set up a static time delta for update calls
	Globals::updateData.dt = 1.0 / 60.0;// 60 fps

	// fps stuff 
	framef++;
	timef = glutGet(GLUT_ELAPSED_TIME);
	if (timef - timebasef > 1000) {
		printf("FPS:%4.2f\n",
			framef*1000.0 / (timef - timebasef));
		timebasef = timef;
		framef = 0;
	}
	else
		displayCallback();

}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void FinalWindow::reshapeCallback(int w, int h)
{

	width = w;                                                       //Set the window width
	height = h;                                                      //Set the window height

	glViewport(0, 0, w, h);                                          //Set new viewport size
	glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
	glLoadIdentity();                                                //Clear the projection matrix by loading the identity
	gluPerspective(wfovf, double(width) / (double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void FinalWindow::displayCallback()
{

	glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
	glLoadIdentity();                                                //Clear the projection matrix by loading the identity
	gluPerspective(wfovf, double(width) / (double)height, 1.0, 1000.0); //Set perspective projection viewing frustum

	//Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the OpenGL matrix mode to ModelView
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraf.getInverseMatrix().ptr());

	//Push a matrix save point
	//This will save a copy of the current matrix so that we can
	//make changes to it and 'pop' those changes off later.
	glPushMatrix();

	//Replace the current top of the matrix stack with the inverse camera matrix
	//This will convert all world coordiantes into camera coordiantes
	glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());

	//Bind the light to slot 0.  We do this after the camera matrix is loaded so that
	//the light position will be treated as world coordiantes
	//(if we didn't the light would move with the camera, why is that?)
	Globals::light.bind(0);
	directionalf->bind(1);
	pointf->bind(2);
	spotf->bind(3);

	// draw light spheres
	//pointSphere->draw(Globals::drawData);
	//spotSphere->draw(Globals::drawData);

	//Draw the OBJObjects!
	for (int i = 0; i < drawablesf->size(); i++) {
		drawablesf->at(i)->draw(Globals::drawData);
	}


	//Pop off the changes we made to the matrix stack this frame
	glPopMatrix();

	//Tell OpenGL to clear any outstanding commands in its command buffer
	//This will make sure that all of our commands are fully executed before
	//we swap buffers and show the user the freshly drawn frame
	glFlush();

	//Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
	glutSwapBuffers();
}

//TODO: Keyboard callbacks!
void FinalWindow::processNormalKeys(unsigned char key, int x, int y) {
	/*
	if (key == 'b') {
		platoon->bounding = !(platoon->bounding);
		platoon2->bounding = !(platoon2->bounding);
	}
	*/
	if (key == 'x') {
		changef.makeTranslate(-1.0, 0.0, 0.0);

		drawablesf->at(lastf)->toWorld = changef * drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->left = changef * drawablesf->at(lastf)->left;

	}
	else if (key == 'X') {
		changef.makeTranslate(1.0, 0.0, 0.0);

		drawablesf->at(lastf)->toWorld = changef * drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->left = changef * drawablesf->at(lastf)->left;
	}
	else if (key == 'y') {
		changef.makeTranslate(0.0, -1.0, 0.0);

		drawablesf->at(lastf)->toWorld = changef*drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->left = changef * drawablesf->at(lastf)->left;
	}
	else if (key == 'Y') {
		changef.makeTranslate(0.0, 1.0, 0.0);

		drawablesf->at(lastf)->toWorld = changef*drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->left = changef * drawablesf->at(lastf)->left;
	}
	else if (key == 'z') {
		changef.makeTranslate(0.0, 0.0, -1.0);

		drawablesf->at(lastf)->toWorld = changef*drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->left = changef *drawablesf->at(lastf)->left;
	}
	else if (key == 'Z') {
		changef.makeTranslate(0.0, 0.0, 1.0);
		//drawable->toWorld = change*drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawablesf->at(lastf)->toWorld = changef*drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->left = changef * drawablesf->at(lastf)->left;
	}
	else if (key == 'r') {
		changef.identity();

		drawablesf->at(lastf)->toWorld = drawablesf->at(lastf)->toWorld * drawablesf->at(lastf)->right.inverse();
		drawablesf->at(lastf)->toWorld = drawablesf->at(lastf)->left.inverse() * drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->right.identity();
		drawablesf->at(lastf)->left.identity();

	}
	else if (key == 'o') {
		changef.makeRotateArbitrary(Vector3(0.0, 0.0, 1.0), 0.5);

		drawablesf->at(lastf)->toWorld = changef*drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->left = changef * drawablesf->at(lastf)->left;
	}
	else if (key == 'O') {
		changef.makeRotateArbitrary(Vector3(0.0, 0.0, 1.0), -0.5);

		drawablesf->at(lastf)->toWorld = changef*drawablesf->at(lastf)->toWorld;
		drawablesf->at(lastf)->left = changef * drawablesf->at(lastf)->left;
	}
	else if (key == 's') {
		changef.makeScale(0.75);

		drawablesf->at(lastf)->toWorld = drawablesf->at(lastf)->toWorld * changef;
		drawablesf->at(lastf)->right = drawablesf->at(lastf)->right * changef;
	}
	else if (key == 'S') {
		changef.makeScale(1.25);

		drawablesf->at(lastf)->toWorld = drawablesf->at(lastf)->toWorld * changef;
		drawablesf->at(lastf)->right = drawablesf->at(lastf)->right * changef;
	}
}

//TODO: Mouse callbacks!
void FinalWindow::processMouseButtons(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			Lmousedownf = true;
			lastPointf = trackBallMapping(Vector3(x, y, 0));
		}
		else if (state == GLUT_UP)
			Lmousedownf = false;
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			Rmousedownf = true;
			lastPointf = trackBallMapping(Vector3(x, y, 0));
		}
		else if (state == GLUT_UP)
			Rmousedownf = false;
	}

	if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		ctrlf = true;
	}
	else
		ctrlf = false;
}

Vector3 FinalWindow::trackBallMapping(Vector3 point)    // The CPoint class is a specific Windows class. Either use separate x and y values for the mouse location, or use a Vector3 in which you ignore the z coordinate.
{
	Vector3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v[0] = (2.0*point[0] - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v[1] = (height - 2.0*point[1]) / height;   // this does the equivalent to the above for the mouse Y position
	v[2] = 0.0;   // initially the mouse z position is set to zero, but this will change below
	d = v.magnitude();    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d<1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v[2] = sqrt(1.001 - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	v.normalize(); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}

//TODO: Mouse Motion callbacks!
void FinalWindow::processMouseMotion(int x, int y) {
	
	if (Lmousedownf) {
		currPointf = trackBallMapping(Vector3(x, y, 0));
		directionf = currPointf - lastPointf;
		velocityf = directionf.magnitude();
		if (velocityf < 0.0001)	// if little movement - do nothing
			return;
		rotAxisf = lastPointf.cross(currPointf);
		rot_anglef = velocityf * 1.0;

		changef.makeRotateArbitrary(Vector3(rotAxisf[0], rotAxisf[1], rotAxisf[2]), rot_anglef);

		if (ctrlf) {
			if (directionf[1] < 0) {
				changef.makeScale(0.5);
				wfovf = wfovf*0.99;
				//Globals::camera.c = Globals::camera.c * change;
				//drawables->at(draw_item / 2)->toWorld = drawables->at(draw_item / 2)->toWorld * change;
			}
			else if (directionf[1] > 0 ) {
				changef.makeScale(1.5);
				wfovf = wfovf*1.01;
				//Globals::camera.c = Globals::camera.c * change;
				//drawables->at(draw_item / 2)->toWorld = drawables->at(draw_item / 2)->toWorld * change;
			}
		}
		else {
			//Globals::camera.c = change*Globals::camera.c;
			//drawables->at(draw_item / 2)->toWorld = change*drawables->at(draw_item / 2)->toWorld;
			Globals::camera.e = changef * Globals::camera.e;
			Globals::camera.set(Globals::camera.e, Globals::camera.d, Globals::camera.up);
		}

		lastPointf = currPointf;
	}
	else if (Rmousedownf) {
		currPointf = trackBallMapping(Vector3(x, y, 0));
		directionf = currPointf - lastPointf;
		directionf.scale(100000.0);
		directionf.set(2, 0.0);
		changef.makeTranslate(directionf);
		drawablesf->at(0)->toWorld = changef*drawablesf->at(0)->toWorld;
		lastPointf = currPointf;
	}
	
}