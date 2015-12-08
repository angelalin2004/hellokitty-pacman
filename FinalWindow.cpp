#include "Shader.h"
#include <mmsystem.h>
#include <Windows.h>
#include "RandHouse.h"
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
#pragma comment(lib, "winmm.lib")

int FinalWindow::width = 512;   //Set window width in pixels here
int FinalWindow::height = 512;   //Set window height in pixels here
Matrix4 changef;
Camera cameraf;
Drawable * drawablef;
std::vector<Drawable*>* drawablesf;
std::vector<OBJObject*> *drawables_body, *drawables_roof, *drawables_door;
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
OBJObject *door1, *door2, *door3, *door4, *door5;

OBJObject *kitty;
RandHouse * house, *specialhouse;
Kitty * kittyGeode;
Group * village;
MatrixTransform * villageTransform, *houseTransform, *kittyTransform;
std::vector<RandHouse*> * innerhouses, *allhouses;
float angle;
Vector3 c;
float minx, maxx, miny, maxy;

std::vector<std::vector<float>>* intervals;
float magnitude;
bool colliding;
Shader * shad;

void FinalWindow::initialize(void)
{
	PlaySound(TEXT("lol.wav"), NULL, SND_LOOP | SND_ASYNC);
	Globals::camera.set(Vector3(0.0, 24.14, 24.14), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));

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
	lastf = 0;
	draw_itemf = 0;
	Lmousedownf = Rmousedownf = false;
	ctrlf = false;
	wfovf = 60.0;
	angle = 0.0;
	colliding = false;

	shad = new Shader("../toon.vert", "../toon.frag", true);
	drawablesf = new std::vector<Drawable*>();
	kitty = new OBJObject("../Kitty.obj");
		changef.makeTranslate((-1.0)* kitty->center[0],
			(-0.5)* kitty->center[1],
			(-1.0)* kitty->center[2]);
		kitty->toWorld = changef*kitty->toWorld;
		changef.makeTranslate(0.0, 1.0, 0.0);
		kitty->toWorld = changef*kitty->toWorld;
		changef.makeRotateX(15.0 * 3.14159265 / 180.0);
		kitty->toWorld = kitty->toWorld * changef;
		changef.makeRotateZ((-1.0)*6.0 * 3.14159265 / 180.0);
		kitty->toWorld = kitty->toWorld * changef;
		
	changef.identity();
	kittyTransform = new MatrixTransform(changef);
		kittyTransform->rad = 1;
		kittyGeode = new Kitty(kitty);
			kittyGeode->rad = 4;
			kittyGeode->move = 1.0;
		kittyTransform->addChild(kittyGeode);
		changef.makeRotateY(90.0 * 3.14159265 / 180.0);
		kittyTransform->M = kittyTransform->M * changef;
	
	drawables_body = new std::vector<OBJObject*>();
	drawables_roof = new std::vector<OBJObject*>();
	drawables_door = new std::vector<OBJObject*>();
	innerhouses = new std::vector<RandHouse*>();
	allhouses = new std::vector<RandHouse*>();
	intervals = new std::vector<std::vector<float>>();

	body1 = new OBJObject("../Body1T.obj");
		changef.makeScale(0.35);
		body1->toWorld = body1->toWorld * changef;
	body2 = new OBJObject("../Body2T.obj");
	body3 = new OBJObject("../Body3T.obj");
	body4 = new OBJObject("../Body4T.obj");
		changef.makeScale(0.65, 0.65, 0.75);
		body4->toWorld = body4->toWorld * changef;
	body5 = new OBJObject("../Body5T.obj");
		changef.makeScale(1.0, 0.45, 1.0);
		body5->toWorld = body5->toWorld * changef;
	drawables_body->push_back(body1);
	drawables_body->push_back(body2);
	drawables_body->push_back(body3);
	drawables_body->push_back(body4);
	drawables_body->push_back(body5);
		

	roof1 = new OBJObject("../Roof1T.obj");
		changef.makeTranslate(0.0, -4.0, 0.0);
		roof1->toWorld = roof1->toWorld * changef;
	roof2 = new OBJObject("../Roof2T.obj");
	roof3 = new OBJObject("../Roof3T.obj");
	roof4 = new OBJObject("../Roof4T.obj");
	roof5 = new OBJObject("../Roof5T.obj");
	drawables_roof->push_back(roof1);
	drawables_roof->push_back(roof2);
	drawables_roof->push_back(roof3);
	drawables_roof->push_back(roof4);
	drawables_roof->push_back(roof5);
		

	
	door1 = new OBJObject("../Door1T.obj");
		changef.makeScale(0.9, 0.9, 1.5);
		door1->toWorld = door1->toWorld * changef;
	door2 = new OBJObject("../Door2T.obj");
	door3 = new OBJObject("../Door3T.obj");
		changef.makeScale(0.9);
		door3->toWorld = door3->toWorld * changef;
		changef.makeTranslate(0.0, 3.0, 0.0);
		door3->toWorld = door3->toWorld * changef;
	door4 = new OBJObject("../Door4T.obj");
	door5 = new OBJObject("../Door5T.obj");
		changef.makeScale(0.85);
		door5->toWorld = door5->toWorld * changef;
	drawables_door->push_back(door1);
	drawables_door->push_back(door2);
	drawables_door->push_back(door3);
	drawables_door->push_back(door4);
	drawables_door->push_back(door5);
	
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
	for (int i = 0; i < drawables_door->size(); i++) {
		changef.makeTranslate(0.0, 2.0, 4.25);
		drawables_door->at(i)->toWorld = changef*drawables_door->at(i)->toWorld;
		changef.makeScale(0.05);
		drawables_door->at(i)->toWorld = drawables_door->at(i)->toWorld * changef;
		// scale doors thicker
		changef.makeScale(1.0, 1.0, 2.0);
		drawables_door->at(i)->toWorld = drawables_door->at(i)->toWorld * changef;
		// rotate along X axis 90 degrees
		changef.makeRotateX((-1.0)*90 * 3.14159265 / 180.0);
		drawables_door->at(i)->toWorld = drawables_door->at(i)->toWorld * changef;
		// rotate along Z axis 90 degrees
		changef.makeRotateY(90 * 3.14159265 / 180.0);
		drawables_door->at(i)->toWorld = drawables_door->at(i)->toWorld * changef;
		
	}

	//drawablesf->push_back(drawables_body->at(4));
	//drawablesf->push_back(drawables_roof->at(3));
	//drawablesf->push_back(drawables_door->at(1));
	//drawablesf->push_back(kitty);
	/*
	for (int i = 0; i < drawablesf->size(); i++) {
		changef.makeTranslate(0.0, -5.0, 0.0);
		drawablesf->at(i)->toWorld = changef*drawablesf->at(i)->toWorld;
	}
	*/
	
	
	
	changef.identity();
	village = new Group();
		villageTransform = new MatrixTransform(changef);
		// add houses on borders
		for (int count = 0; count < 256; count++) {
			if (count / 16 == 0 || count / 16 == 15 || count % 16 == 0 || count % 16 == 15) {
				int brand = rand() % 5;
				int rrand = rand() % 5;
				int drand = rand() % 5;
				house = new RandHouse(drawables_body->at(brand), drawables_roof->at(rrand), drawables_door->at(drand));
					c = Vector3((count % 16 - 8) * 16, 0.0, (-1.0)*(count / 16 - 8) * 16);
					house->center = c;
				allhouses->push_back(house);
				intervals->push_back({ c[0] - (float)(7.0 / 2.0), c[0] + (float)(7.0 / 2.0), c[2] - (float)(7.0 / 2.0), c[2] + (float)(7.0 / 2.0), });
				houseTransform = new MatrixTransform(changef.makeTranslate(c));
				if (count / 16 == 0)
					houseTransform->M = houseTransform->M * changef.makeRotateY(180 * 3.14159265 / 180.0);
				else if (count % 16 == 0) {
					houseTransform->M = houseTransform->M * changef.makeRotateY(90 * 3.14159265 / 180.0);
				}
				else if (count % 16 == 15)
					houseTransform->M = houseTransform->M * changef.makeRotateY((-1.0)*90 * 3.14159265 / 180.0);
				houseTransform->addChild(house);
				villageTransform->addChild(houseTransform);
			}
		}
		// random houses in middle
		for (int i = 0; i < 24; i++) {
			int r = rand() % 6;
			int j = i % 4;
			int count = ((r % 3 + 3*j) + 12*(r/3)) + i/4*24;
			int brand = rand() % 5;
			int rrand = rand() % 5;
			int drand = rand() % 5;
			house = new RandHouse(drawables_body->at(brand), drawables_roof->at(rrand), drawables_door->at(drand));
				c = Vector3((count % 12 - 6) * 16, 0.0, (-1.0)*(count / 12) * 16);
				house->center = c;
			innerhouses->push_back(house);
			allhouses->push_back(house);
			intervals->push_back({ c[0] - (float)(7.0 / 2.0), c[0] + (float)(7.0 / 2.0), c[2] - (float)(7.0 / 2.0), c[2] + (float)(7.0 / 2.0), });
			
			houseTransform = new MatrixTransform(changef.makeTranslate(c));
			// apply random rotation
			//houseTransform->M = houseTransform->M * changef.makeRotateY(rand() % 360 * 3.14159265 / 180.0);
			houseTransform->addChild(house);
			villageTransform->addChild(houseTransform);
		}
		// make a random house red
		int special = rand() % 24;
		std::cout << "special: " << special << std::endl;
		specialhouse = innerhouses->at(special);
		specialhouse->makeRed();
		village->addChild(villageTransform);


		for (int i = 0; i < intervals->size(); i++) {
			if (i == 60)
				std::cout << "now middle houses" << std::endl;
			std::cout << "interval " << i << ": " << intervals->at(i)[0] << " " << intervals->at(i)[1] << " " << intervals->at(i)[2] << " " << intervals->at(i)[3] << std::endl;
		}
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
		//printf("FPS:%4.2f\n", framef*1000.0 / (timef - timebasef));
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
	//shad->bind();
	for (int i = 0; i < drawablesf->size(); i++) {
		drawablesf->at(i)->draw(Globals::drawData);
	}
	changef.identity();
	village->draw(changef);
	//shad->unbind();
	kittyTransform->draw(changef);

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

	if (key == 'b') {
		village->bounding = !(village->bounding);
		kittyTransform->bounding = !(kittyTransform->bounding);
	}
	
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
	else if (key == 't') {
		changef.makeRotateY(90 * 3.14159265 / 180.0);
		drawablesf->at(lastf)->toWorld = drawablesf->at(lastf)->toWorld * changef;
	}
	else if (key == 'T') {
		changef.makeRotateY((-1.0) * 90 * 3.14159265 / 180.0);
		drawablesf->at(lastf)->toWorld = drawablesf->at(lastf)->toWorld * changef;
	}
}

//TODO: Function Key callbacks!
void FinalWindow::processFunctionKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		changef.makeRotateY(4.0 * 3.14159265 / 180.0);
		//kitty->toWorld = kitty->toWorld * changef;
		kittyTransform->M = kittyTransform->M * changef;
		kittyGeode->angle += 4.0;
		Globals::camera.e = Globals::camera.e + (Vector3(0.0, 0.0, 0.0) - Globals::camera.d);
		Globals::camera.e = changef * Globals::camera.e;
		Globals::camera.e = Globals::camera.e - (Vector3(0.0, 0.0, 0.0) - Globals::camera.d);
		Globals::camera.update();
	}
	else if (key == GLUT_KEY_RIGHT) {
		changef.makeRotateY((-1.0)*4.0 * 3.14159265 / 180.0);
		//kitty->toWorld = kitty->toWorld * changef;
		kittyTransform->M = kittyTransform->M * changef;
		kittyGeode->angle -= 4.0;
		Globals::camera.e = Globals::camera.e + (Vector3(0.0, 0.0, 0.0) - Globals::camera.d);
		Globals::camera.e = changef * Globals::camera.e;
		Globals::camera.e = Globals::camera.e - (Vector3(0.0, 0.0, 0.0) - Globals::camera.d);
		Globals::camera.update();
	}
	else if (key == GLUT_KEY_UP) {
		changef.makeRotateZ(-6.0 * 3.14159265 / 180.0);
		kitty->toWorld = kitty->toWorld * changef;
		changef.makeTranslate(0.5, 0.0, 0.0);
		kittyTransform->M = kittyTransform->M * changef;
		kittyGeode->center = kittyGeode->center.add(Vector3(sin(kittyGeode->angle * 3.14159265 / 180.0)*(-0.5), 0.0, 
															cos(kittyGeode->angle * 3.14159265 / 180.0)*(-0.5)));
		Globals::camera.e = Globals::camera.e.add(Vector3(sin(kittyGeode->angle * 3.14159265 / 180.0)*(-0.5), 0.0, cos(kittyGeode->angle * 3.14159265 / 180.0)*(-0.5)));
		Globals::camera.d = Globals::camera.d.add(Vector3(sin(kittyGeode->angle * 3.14159265 / 180.0)*(-0.5), 0.0, cos(kittyGeode->angle * 3.14159265 / 180.0)*(-0.5)));
		Globals::camera.update();
		
	}
	else if (key == GLUT_KEY_DOWN) {
		changef.makeRotateZ(6.0 * 3.14159265 / 180.0);
		kitty->toWorld = kitty->toWorld * changef;
		changef.makeTranslate(-0.5, 0.0, 0.0);
		kittyTransform->M = kittyTransform->M * changef;
		kittyGeode->center = kittyGeode->center.add(Vector3(sin(kittyGeode->angle * 3.14159265 / 180.0)*(0.5), 0.0, 
															cos(kittyGeode->angle * 3.14159265 / 180.0)*(0.5)));
		Globals::camera.e = Globals::camera.e.add(Vector3(sin(kittyGeode->angle * 3.14159265 / 180.0)*(-0.5), 0.0, cos(kittyGeode->angle * 3.14159265 / 180.0)*(0.5)));
		Globals::camera.d = Globals::camera.d.add(Vector3(sin(kittyGeode->angle * 3.14159265 / 180.0)*(-0.5), 0.0, cos(kittyGeode->angle * 3.14159265 / 180.0)*(0.5)));
		Globals::camera.update();
		
	}
	if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
		kittyGeode->center.print("kittyGeode center: ");
		// go through intervals array
		minx = kittyGeode->center[0] - (float)(kittyGeode->rad / 2.0);
		maxx = kittyGeode->center[0] + (float)(kittyGeode->rad / 2.0);
		miny = kittyGeode->center[2] - (float)(kittyGeode->rad / 2.0);
		maxy = kittyGeode->center[2] + (float)(kittyGeode->rad / 2.0);
		colliding = false;
		for (int i = 0; i < innerhouses->size(); i++) {
			c = innerhouses->at(i)->center;
			c = c - kittyGeode->center;
			magnitude = c.magnitude();
			if (magnitude < innerhouses->at(i)->rad + kittyGeode->rad) {
				innerhouses->at(i)->colliding = true;
				colliding = true;
			}
			else {
				innerhouses->at(i)->colliding = false;
			}
		}
		kittyGeode->colliding = colliding;
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