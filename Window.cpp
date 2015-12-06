#include "Flag.h"
#include <iostream>
#include <vector>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <math.h>

#include "Window.h"
#include "Cube.h"
#include "Sphere.h"
#include "Matrix4.h"
#include "Globals.h"
#include "OBJObject.h"
#include "Cube2.h"
#include "Sphere2.h"
#include "Skybox.h"


int Window::width  = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here
float dir = 1.0;
Matrix4 change;
Camera camera;
OBJObject *obj;
OBJObject * bunny;
OBJObject * dragon;
OBJObject * bear;
Drawable * drawable;
std::vector<Drawable*>* drawables;
int last;
bool multiple;
bool rasterizing;
int frame = 0, time, timebase = 0;
int draw_item;
bool Lmousedown, Rmousedown;
Vector3 lastPoint, currPoint, direction, rotAxis;
float velocity, rot_angle;
Light * currLight;
Light * directional;
Light * point;
Light * spot;
Sphere * pointSphere;
Sphere * spotSphere;
Sphere * currSphere;
bool controllingLight;
float h;
bool ctrl;
bool newbie;
Skybox * skybox;
Flag * flag;
float wfov;

void Window::initialize(void)
{
    //Setup the light
    Vector4 lightPos(10.0, 10.0, 0.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.02;

	directional = new Light();
	directional->position = Vector4(0.0, 100.0, 100.0, 0.0);
	//directional->diffuseColor = Color::red();
	currLight = directional;
	currSphere = 0;

	point = new Light();
	point->position = Vector4(0.0, 10.0, 0.0, 1.0);
	//point->diffuseColor = Color::green();
	pointSphere = new Sphere(0.2, 50, 50);
	pointSphere->material.emissionColor = Color::green();
	change.makeTranslate(point->position[0], point->position[1], point->position[2]);
	pointSphere->toWorld = change * pointSphere->toWorld;

	spot = new Light();
	spot->position = Vector4(0.0, -11.0, 0.0, 1.0);
	//spot->diffuseColor = Color::blue();
	spot->isSpot = true;
	spotSphere = new Sphere(0.2, 50, 50);
	spotSphere->material.emissionColor = Color::blue();
	change.makeTranslate(spot->position[0], spot->position[1], spot->position[2]);
	spotSphere->toWorld = change * spotSphere->toWorld;

	//Initialize sphere matrix:
	Globals::sphere.toWorld.identity();

	//Setup the cube's material properties
	//Color color(0x23ff27ff);
	
	Globals::sphere.material.diffuseColor = Color(1.0, 0.5, 0.0);

	obj = 0;
	bunny = 0;
	dragon = 0;
	bear = 0;

	skybox = new Skybox();
	drawable = skybox;
	drawables = new std::vector<Drawable*>();
	drawables->push_back(skybox);
	last = 0;
	multiple = false;
	rasterizing = false;
	draw_item = 0;
	Globals::rasterizer.level = 1;
	Globals::rasterizer.toDraw = drawables->at(0);
	Globals::rasterizer.debug = false;
	Lmousedown = Rmousedown = false;
	controllingLight = false;
	ctrl = false;
	newbie = false;

	flag = new Flag();
	wfov = 60.0;
	
	
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
    //Set up a static time delta for update calls
    Globals::updateData.dt = 1.0/60.0;// 60 fps
    
	// fps stuff 
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		printf("FPS:%4.2f\n",
		frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}

    //Call the display routine to draw the cube
	if (rasterizing) {
		//std::cout << "rasterizing!!" << std::endl;
		Globals::rasterizer.draw(draw_item, Globals::camera, Globals::house);
	}
	else
		displayCallback();
	
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
	
	width = w;                                                       //Set the window width
    height = h;                                                      //Set the window height

	Globals::rasterizer.D.makeViewport(0.0, (float)w, 0.0, (float)h);
	Globals::rasterizer.P.makePerspectiveProjection(60.0, w, h, 1.0, 1000.0);
	Globals::rasterizer.resizePixelBuffer(w, h);
	Globals::rasterizer.resizeZBuffer(w, h);

    glViewport(0, 0, w, h);                                          //Set new viewport size
    glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
    glLoadIdentity();                                                //Clear the projection matrix by loading the identity
    gluPerspective(wfov, double(width)/(double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	
	glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
	glLoadIdentity();                                                //Clear the projection matrix by loading the identity
	gluPerspective(wfov, double(width) / (double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
	
    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set the OpenGL matrix mode to ModelView
    glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.getInverseMatrix().ptr());
    
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
	directional->bind(1);
	point->bind(2);
	spot->bind(3);

	// draw light spheres
	//pointSphere->draw(Globals::drawData);
	//spotSphere->draw(Globals::drawData);
    
    //Draw the cube!
	/*
	if (!rasterizing) {
		if (draw_item != 2)
			drawables->at(0)->draw(Globals::drawData);
		else { // draw_item == 2
			drawables->at(1)->draw(Globals::drawData);
			if (draw_item == 2 && multiple) {
				for (int i = 2; i < drawables->size(); i++) {
					drawables->at(i)->draw(Globals::drawData);
				}
			}
		}

	}
	else {
		Globals::rasterizer.draw(draw_item, Globals::camera, Globals::house);
	}
	*/
	

	skybox->draw(Globals::drawData);
	flag->draw(Globals::drawData);

    //Pop off the changes we made to the matrix stack this frame
    glPopMatrix();
    
    //Tell OpenGL to clear any outstanding commands in its command buffer
    //This will make sure that all of our commands are fully executed before
    //we swap buffers and show the user the freshly drawn frame
    glFlush();
    
    //Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
    glutSwapBuffers();
}

void Window::printPosition() {
	Vector3 printv = Vector3(Globals::cube.toWorld.get(3,0), Globals::cube.toWorld.get(3,1), Globals::cube.toWorld.get(3,2));
	printv.print("");
}

//TODO: Keyboard callbacks!
void Window::processNormalKeys(unsigned char key, int x, int y) {
	int temp_last = last;
	if (draw_item != 2)
		last = 0;

	if (key == 'c') {
		dir = dir * (-1.0);
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'x') {
		change.makeTranslate(-1.0, 0.0, 0.0);
		//drawable->toWorld = change * drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawables->at(last)->toWorld = change * drawables->at(last)->toWorld;
		drawables->at(last)->left = change * drawables->at(last)->left;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'X') {
		change.makeTranslate(1.0, 0.0, 0.0);
		//drawable->toWorld = change * drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawables->at(last)->toWorld = change * drawables->at(last)->toWorld;
		drawables->at(last)->left = change * drawables->at(last)->left;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'y') {
		change.makeTranslate(0.0, -1.0, 0.0);
		//drawable->toWorld = change*drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawables->at(last)->toWorld = change*drawables->at(last)->toWorld;
		drawables->at(last)->left = change * drawables->at(last)->left;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'Y') {
		change.makeTranslate(0.0, 1.0, 0.0);
		//drawable->toWorld = change*drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawables->at(last)->toWorld = change*drawables->at(last)->toWorld;
		drawables->at(last)->left = change * drawables->at(last)->left;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'z') {
		change.makeTranslate(0.0, 0.0, -1.0);
		//drawable->toWorld = change*drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawables->at(last)->toWorld = change*drawables->at(last)->toWorld;
		drawables->at(last)->left = change *drawables->at(last)->left;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'Z') {
		change.makeTranslate(0.0, 0.0, 1.0);
		//drawable->toWorld = change*drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawables->at(last)->toWorld = change*drawables->at(last)->toWorld;
		drawables->at(last)->left = change * drawables->at(last)->left;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'r') {
		change.identity();
		//drawable->toWorld = drawable->toWorld * drawable->right.inverse();
		//drawable->toWorld = drawable->left.inverse() * drawable->toWorld;
		//drawable->right.identity();
		//drawable->left.identity();

		drawables->at(last)->toWorld = drawables->at(last)->toWorld * drawables->at(last)->right.inverse();
		drawables->at(last)->toWorld = drawables->at(last)->left.inverse() * drawables->at(last)->toWorld;
		drawables->at(last)->right.identity();
		drawables->at(last)->left.identity();

		if (draw_item == 0)
			printPosition();
		
	}
	else if (key == 'o') {
		change.makeRotateArbitrary(Vector3(0.0, 0.0, 1.0), 0.5);
		//drawable->toWorld = change*drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawables->at(last)->toWorld = change*drawables->at(last)->toWorld;
		drawables->at(last)->left = change * drawables->at(last)->left;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'O') {
		change.makeRotateArbitrary(Vector3(0.0, 0.0, 1.0), -0.5);
		//drawable->toWorld = change*drawable->toWorld;
		//drawable->left = change * drawable->left;

		drawables->at(last)->toWorld = change*drawables->at(last)->toWorld;
		drawables->at(last)->left = change * drawables->at(last)->left;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 's') {
		change.makeScale(0.75);
		//drawable->toWorld = drawable->toWorld * change;
		//drawable->right = drawable->right * change;

		drawables->at(last)->toWorld = drawables->at(last)->toWorld * change;
		drawables->at(last)->right = drawables->at(last)->right * change;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'S') {
		change.makeScale(1.25);
		//drawable->toWorld = drawable->toWorld * change;
		//drawable->right = drawable->right * change;

		drawables->at(last)->toWorld = drawables->at(last)->toWorld * change;
		drawables->at(last)->right = drawables->at(last)->right * change;
		if (draw_item == 0)
			printPosition();
	}
	else if (key == 'm') {
		if (draw_item != 2)
			return;
		if (multiple == false) {
			multiple = true;
			std::cout << "doing multiple objs now" << std::endl;
		}
		else {
			multiple = false;
			if (drawables->size() > 2) {
				for (int i = 2; i < drawables->size(); i++) {
					drawables->pop_back();
				}
			}
			std::cout << "not doing multiple objs anymore" << std::endl;
		}
		
	}
	else if (key == 'e') {
		rasterizing = !(rasterizing);
		std::cout << "rasterizing is " << rasterizing << "!" << std::endl;
	}
	else if (key == '+') {
		if (Globals::rasterizer.level < 4)
			Globals::rasterizer.level++;
		std::cout << "Pressed key '+', now at Part " << Globals::rasterizer.level << std::endl;
	}
	else if (key == '-') {
		if (Globals::rasterizer.level > 1)
			Globals::rasterizer.level--;
		std::cout << "Pressed key '-', now at Part " << Globals::rasterizer.level << std::endl;
	}
	else if (key == 'd') {
		if (Globals::rasterizer.debug == false ){
			Globals::rasterizer.debug = true;
			std::cout << "Debug mode ON" << std::endl;
		}
		else {
			Globals::rasterizer.debug = false;
			std::cout << "Debug mode OFF" << std::endl;
		}
	}
	else if (key == '1') {
		controllingLight = true;
		currLight = directional;
		currSphere = 0;
		std::cout << "Pressed 1, controlling DIRECTIONAL light" << std::endl;
	}
	else if (key == '2') {
		controllingLight = true;
		currLight = point;
		currSphere = pointSphere;
		std::cout << "Pressed 2, controlling POINT light" << std::endl;
	}
	else if (key == '3') {
		controllingLight = true;
		currLight = spot;
		currSphere = spotSphere;
		std::cout << "Pressed 3, controlling SPOT light" << std::endl;
	}
	else if (key == '0') {
		controllingLight = false;
		std::cout << "Pressed 0, back to OBJECT" << std::endl;
	}
	last = temp_last;

}

//TODO: Function Key callbacks!
void Window::processFunctionKeys(int key, int x, int y) {
	if (key == GLUT_KEY_F1) {
		glEnable(GL_LIGHTING);
		draw_item = 0;
		Globals::camera = Camera();
		//drawable = &(Globals::cube);
		//drawables->at(0) = &(Globals::cube);
		drawables->at(0) = &(Globals::sphere);
	}
	else if (key == GLUT_KEY_F2) {	// bunny
		glEnable(GL_LIGHTING);
		Globals::camera = Camera();
		if (multiple) {
			drawables->push_back(new OBJObject("../bunny.obj"));
			last++;
			std::cout << "+1 bunny" << std::endl;
		}
		else {
			if (bunny == 0) {
				bunny = new OBJObject("../bunny.obj");
				//bunny->material.diffuseColor = Color(1.0, 0.0, 0.0);
				bunny->material.ambientColor = Color(1.0, 0.0, 0.0);
				bunny->material.shininess = 128.0;
				
				newbie = true;
			}
			if (drawables->size() == 1)
				drawables->push_back(bunny);
			else
				drawables->at(1) = bunny;
			last = 1;
			Globals::rasterizer.toDraw = drawables->at(1);
		}
		draw_item = 2;
		h = 8.5;
	}
	else if (key == GLUT_KEY_F3) {	// dragon
		glEnable(GL_LIGHTING);
		Globals::camera = Camera();
		if (multiple) {
			drawables->push_back(new OBJObject("../dragon.obj"));
			last++;
			std::cout << "+1 dragon" << std::endl;
		}
		else {
			if (dragon == 0) {
				dragon = new OBJObject("../dragon.obj");
				dragon->material.specularColor = Color(0.0, 0.0, 1.0);
				newbie = true;
			}
			if (drawables->size() == 1)
				drawables->push_back(dragon);
			else
				drawables->at(1) = dragon;
			last = 1;
			Globals::rasterizer.toDraw = drawables->at(1);
		}
		draw_item = 2;
		h = 10.75;
	}
	else if (key == GLUT_KEY_F4) {	// bear
		glEnable(GL_LIGHTING);
		Globals::camera = Camera();
		if (multiple) {
			drawables->push_back(new OBJObject("../bear.obj"));
			last++;
			std::cout << "+1 bear" << std::endl;
		}
		else {
			if (bear == 0) {
				bear = new OBJObject("../bear.obj");
				bear->material.emissionColor = Color(0.0, 0.5, 0.0);
				newbie = true;
			}
			if (drawables->size() == 1)
				drawables->push_back(bear);
			else
				drawables->at(1) = bear;
			last = 1;
			Globals::rasterizer.toDraw = drawables->at(1);
		}
		draw_item = 2;
		h = 10.5;
	}
	else {}

	if (draw_item == 2 && newbie) {
		
		change.makeTranslate((-1.0)* ((OBJObject*)(drawables->at(1)))->center[0], 
							(-1.0)* ((OBJObject*)(drawables->at(1)))->center[1], 
							(-1.0)* ((OBJObject*)(drawables->at(1)))->center[2]);
		drawables->at(1)->toWorld = change*drawables->at(1)->toWorld;
		
		change.makeScale(h/(((OBJObject*)(drawables->at(1)))->normal_factor));
		drawables->at(1)->toWorld = change*drawables->at(1)->toWorld;
		newbie = false;
	}
}

//TODO: Mouse callbacks!
void Window::processMouseButtons(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			Lmousedown = true;
			lastPoint = trackBallMapping(Vector3(x, y, 0));
		}
		else if (state == GLUT_UP)
			Lmousedown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			Rmousedown = true;
			lastPoint = trackBallMapping(Vector3(x, y, 0));
		}
		else if (state == GLUT_UP)
			Rmousedown = false;
	}

	if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		ctrl = true;
	}
	else
		ctrl = false;
}

Vector3 Window::trackBallMapping(Vector3 point)    // The CPoint class is a specific Windows class. Either use separate x and y values for the mouse location, or use a Vector3 in which you ignore the z coordinate.
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
void Window::processMouseMotion( int x, int y) {
	if (Lmousedown) {
		currPoint = trackBallMapping(Vector3(x, y, 0));
		direction = currPoint - lastPoint;
		velocity = direction.magnitude();
		if (velocity < 0.0001)	// if little movement - do nothing
			return;
		rotAxis = lastPoint.cross(currPoint);
		rot_angle = velocity * 1.0;

		change.makeRotateArbitrary(Vector3(rotAxis[0], rotAxis[1], rotAxis[2]), rot_angle);
		if (controllingLight) {
			if (ctrl) {
				if (direction[1] < 0) {
					change.makeScale(0.99);
					currLight->position = change * currLight->position;
					if (currSphere != 0)
						currSphere->toWorld = change*currSphere->toWorld;
				}
				else if (direction[1] > 0) {
					change.makeScale(1.01);
					currLight->position = change * currLight->position;
					if (currSphere != 0)
						currSphere->toWorld = change*currSphere->toWorld;
				}

			}
			else {
				currLight->position = change*currLight->position;
				if (currSphere != 0)
					currSphere->toWorld = change*currSphere->toWorld;
			}
		}
		else {
			
			if (ctrl) {
				if (direction[1] < 0) {
					change.makeScale(0.5);
					wfov = wfov*0.99;
					//Globals::camera.c = Globals::camera.c * change;
					//drawables->at(draw_item / 2)->toWorld = drawables->at(draw_item / 2)->toWorld * change;
				}
				else if (direction[1] > 0 ) {
					change.makeScale(1.5);
					wfov = wfov*1.01;
					//Globals::camera.c = Globals::camera.c * change;
					//drawables->at(draw_item / 2)->toWorld = drawables->at(draw_item / 2)->toWorld * change;
				}
			}
			else {
				//Globals::camera.c = change*Globals::camera.c;
				//drawables->at(draw_item / 2)->toWorld = change*drawables->at(draw_item / 2)->toWorld;
				Globals::camera.e = change * Globals::camera.e;
				Globals::camera.set(Globals::camera.e, Globals::camera.d, Globals::camera.up);
			}
		}

		lastPoint = currPoint;
	}
	else if (Rmousedown) {
		currPoint = trackBallMapping(Vector3(x, y, 0));
		direction = currPoint - lastPoint;
		direction.scale(100000.0);
		direction.set(2, 0.0);
		change.makeTranslate(direction);
		if (controllingLight) {
			if (currLight->isSpot) {
				// mouse up/down should make the spot wider/narrower
				if (direction[1] < 0) {
					currLight->spotCutoff -= 0.1;
					if (currLight->spotCutoff < 0.0)
						currLight->spotCutoff = 0.0;
				}
				else if (direction[1] > 0) {
					currLight->spotCutoff += 0.1;
					if (currLight->spotCutoff > 90.0)
						currLight->spotCutoff = 90.0;
				}
				std::cout << "cutoff: " << currLight->spotCutoff << std::endl;

				// mouse left/right should make the spot edge sharper/blurrier
				if (direction[0] < 0) {
					currLight->spotExponent -= 0.05;
					if (currLight->spotExponent < 0.0)
						currLight->spotExponent = 0.0;
				}
				else if (direction[0] > 0) {
					currLight->spotExponent += 0.05;
					if (currLight->spotExponent > 128.0)
						currLight->spotExponent = 128.0;
				}
				std::cout << "exponent: " << currLight->spotExponent << std::endl;
			}
		}
		else
			drawables->at(draw_item/2)->toWorld = change*drawables->at(draw_item/2)->toWorld;
		lastPoint = currPoint;
	}
}


Window::~Window()
{
	//Delete any dynamically allocated memory/objects here
	if (drawables->size() > 1) {
		for (int i = 1; i < drawables->size(); i++) {
			delete drawables->at(i);
		}
	}
	
}