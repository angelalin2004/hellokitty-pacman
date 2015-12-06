#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

#include "RobotWindow.h"


int RobotWindow::width = 512;   //Set window width in pixels here
int RobotWindow::height = 512;   //Set window height in pixels here

Matrix4 transform, M;
Camera camera2;
int frame2 = 0, time2, timebase2 = 0;
MatrixTransform * Torso, * Torso2;
MatrixTransform * LeftLeg;
MatrixTransform * LeftArm;
MatrixTransform * Head; 
MatrixTransform * RightArm; 
MatrixTransform * RightLeg;
MatrixTransform * Limbs;
MatrixTransform * robotTransform;
MatrixTransform * platoonTransform, * platoonTransform2;
Sphere2 * s2;
Cube2 * c2;
Group * robot, * robot2, * platoon, * platoon2;
bool Ldown, Rdown, ctrl_down;
Vector3 lastP, currP, directionAx, rotAx;
float velocity2, rot_angle2;
Matrix4 change2;
int robotcount,armynum;
float fov;

void RobotWindow::initialize(void)
{
	//Setup the light
	Vector4 lightPos(0.0, 10.0, 10.0, 0.0);
	Globals::light.position = lightPos;
	Globals::light.quadraticAttenuation = 0.02;

	Globals::camera.set(Vector3(0.0, 24.14, 24.14), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));
	//Globals::camera.set(Vector3(-28.33, 11.66, 23.33), Vector3(-5.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.5));

	Ldown = Rdown = ctrl_down = false;
	change2.identity();

	transform.identity();
	robot = new Group();
	Torso = new MatrixTransform(transform.makeScale(3.0));
			Torso->radius = 4.0;
			s2 = new Sphere2(2, 50, 50);
			Torso->addChild(s2);
			Head = new MatrixTransform(transform.makeTranslate(Vector3(0.0, 1.5, 0.0))* transform.makeScale(2.0/3.0));
				Head->radius = 2.0/3.0;
				Head->center = Vector3(0.0, 1.5, 0.0);
				Head->addChild(s2);
			Torso->addChild(Head);
			Limbs = new MatrixTransform(transform.makeScale(0.5, 1.25, 0.75));
				Limbs->radius = 1.25;
				c2 = new Cube2(1.0);
				LeftArm = new MatrixTransform(transform.makeTranslate(Vector3(-2.0, 0.0, 0.0)));
					LeftArm->addChild(c2);
					LeftArm->center = Vector3(-2.0, 0.0, 0.0);
				Limbs->addChild(LeftArm);
				RightArm = new MatrixTransform(transform.makeTranslate(Vector3(2.0, 0.0, 0.0)));
					RightArm->addChild(c2);
					RightArm->center = Vector3(2.0, 0.0, 0.0);
				Limbs->addChild(RightArm);
				LeftLeg = new MatrixTransform(transform.makeTranslate(Vector3(-0.75, -1.0, 0.0)));
					LeftLeg->addChild(c2);
					LeftLeg->center = Vector3(-0.75, -1.0, 0.0);
				Limbs->addChild(LeftLeg);
				RightLeg = new MatrixTransform(transform.makeTranslate(Vector3(0.75, -1.0, 0.0)));
					RightLeg->addChild(c2);
					RightLeg->center = Vector3(0.75, -1.0, 0.0);
				Limbs->addChild(RightLeg);
			Torso->addChild(Limbs);
		robot->addChild(Torso);
	
	transform.identity();
	robot2 = new Group();
	Torso2 = new MatrixTransform(transform.makeRotateY(180.0* 3.14159265 / 180.0) * transform.makeScale(3.0));
			Torso2->radius = 4.0;
			Torso2->addChild(s2);
			Torso2->addChild(Head);
			Torso2->addChild(Limbs);
		robot2->addChild(Torso2);


	transform.identity();
	armynum = 8;
	robotcount = 0;
	platoon = new Group();
		platoonTransform = new MatrixTransform(transform);
			for (robotcount; robotcount < armynum; robotcount++) {
				robotTransform = new MatrixTransform(transform.makeTranslate(Vector3((robotcount%4 - 4)*10, 0.0, (-1.0)*(robotcount / 4)*10)));
					robotTransform->addChild(robot);
				platoonTransform->addChild(robotTransform);
			}
		platoon->addChild(platoonTransform);
	robotcount = 0;
	
	transform.identity();
	platoon2 = new Group();
		platoonTransform2 = new MatrixTransform(transform);
		for (robotcount; robotcount < armynum; robotcount++) {
			robotTransform = new MatrixTransform(transform.makeTranslate(Vector3((robotcount % 4 + 1) * 10, 0.0, (-1.0)*(robotcount / 4) * 10)));
				robotTransform->addChild(robot2);
			platoonTransform2->addChild(robotTransform);
		}
		platoon2->addChild(platoonTransform2);
	
	fov = 60.0;

}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void RobotWindow::idleCallback()
{
	//Set up a static time delta for update calls
	Globals::updateData.dt = 1.0 / 60.0;// 60 fps
	robot->update();

	// translate to origin
	LeftLeg->M = transform.makeTranslate(Vector3(-2.0, 0.0, 0.0)).inverse() * LeftLeg->M;
	RightArm->M = transform.makeTranslate(Vector3(2.0, 0.0, 0.0)).inverse() * RightArm->M;
	LeftArm->M = transform.makeTranslate(Vector3(-2.0, 0.0, 0.0)).inverse() * LeftArm->M;
	RightLeg->M = transform.makeTranslate(Vector3(2.0, 0.0, 0.0)).inverse() * RightLeg->M;
	// rotate
	if (robot->swingup) {
		LeftLeg->M = transform.makeRotateX(1.0* 3.14159265 / 180.0) * LeftLeg->M;
		RightArm->M = transform.makeRotateX(1.0* 3.14159265 / 180.0) * RightArm->M;
		LeftArm->M = transform.makeRotateX(-1.0* 3.14159265 / 180.0) * LeftArm->M;
		RightLeg->M = transform.makeRotateX(-1.0* 3.14159265 / 180.0) * RightLeg->M;

		
		
	}
	else {
		LeftLeg->M = transform.makeRotateX(-1.0* 3.14159265 / 180.0) * LeftLeg->M;
		RightArm->M = transform.makeRotateX(-1.0* 3.14159265 / 180.0) * RightArm->M;
		LeftArm->M = transform.makeRotateX(1.0* 3.14159265 / 180.0) * LeftArm->M;
		RightLeg->M = transform.makeRotateX(1.0* 3.14159265 / 180.0) * RightLeg->M;

		//platoonTransform->M = transform.makeTranslate(Vector3(0.0, 0.0, -0.1)) * platoonTransform->M;
		//platoonTransform2->M = transform.makeTranslate(Vector3(0.0, 0.0, 0.1)) * platoonTransform2->M;
	}
	// translate back
	LeftLeg->M = transform.makeTranslate(Vector3(-2.0, 0.0, 0.0)) * LeftLeg->M;
	RightArm->M = transform.makeTranslate(Vector3(2.0, 0.0, 0.0)) * RightArm->M;
	LeftArm->M = transform.makeTranslate(Vector3(-2.0, 0.0, 0.0)) * LeftArm->M;
	RightLeg->M = transform.makeTranslate(Vector3(2.0, 0.0, 0.0)) * RightLeg->M;

	if (robot->dir == 0) {
		platoonTransform->M = transform.makeTranslate(Vector3(0.0, 0.0, 0.02)) * platoonTransform->M;
		platoonTransform2->M = transform.makeTranslate(Vector3(0.0, 0.0, -0.02)) * platoonTransform2->M;
	}
	else if (robot->dir == 1 || robot->dir == 5 || robot->dir == 7 || robot->dir == 11) {
		//std::cout << robot->dist << std::endl;
		Torso->M = Torso->M * transform.makeRotateY(1.0* 3.14159265 / 180.0);
		Torso2->M = Torso2->M * transform.makeRotateY(1.0* 3.14159265 / 180.0);
	}
	else if (2 <= robot->dir && robot->dir <= 4) {
		platoonTransform->M = transform.makeTranslate(Vector3(0.2, 0.0, 0.0)) * platoonTransform->M;
		platoonTransform2->M = transform.makeTranslate(Vector3(-0.2, 0.0, 0.0)) * platoonTransform2->M;
	}
	else if (robot->dir == 6) {
		platoonTransform->M = transform.makeTranslate(Vector3(0.0, 0.0, -0.1)) * platoonTransform->M;
		platoonTransform2->M = transform.makeTranslate(Vector3(0.0, 0.0, 0.1)) * platoonTransform2->M;
	}
	else if (8 <= robot->dir && robot->dir <= 10) {
		platoonTransform->M = transform.makeTranslate(Vector3(-0.2, 0.0, 0.0)) * platoonTransform->M;
		platoonTransform2->M = transform.makeTranslate(Vector3(0.2, 0.0, 0.0)) * platoonTransform2->M;
	}
	else if (robot->dir == 12) {
		platoonTransform->M = transform.makeTranslate(Vector3(0.0, 0.0, 0.1)) * platoonTransform->M;
		platoonTransform2->M = transform.makeTranslate(Vector3(0.0, 0.0, -0.1)) * platoonTransform2->M;
	}
	// fps stuff 
	frame2++;
	time2 = glutGet(GLUT_ELAPSED_TIME);
	if (time2 - timebase2 > 1000) {
		printf("FPS:%4.2f\n",
			frame2*1000.0 / (time2 - timebase2));
		timebase2 = time2;
		frame2 = 0;
	}

	//Call the display routine to draw the cube
	RobotWindow::displayCallback();

}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void RobotWindow::reshapeCallback(int w, int h)
{

	width = w;                                                       //Set the window width
	height = h;                                                      //Set the window height

	glViewport(0, 0, w, h);                                          //Set new viewport size
	glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
	glLoadIdentity();                                                //Clear the projection matrix by loading the identity
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void RobotWindow::displayCallback()
{

	glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
	glLoadIdentity();                                                //Clear the projection matrix by loading the identity
	gluPerspective(fov, double(width) / (double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
	
	//Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the OpenGL matrix mode to ModelView
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera2.getInverseMatrix().ptr());

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

	//Draw the cube!
	transform.identity();

	/*
	transform.makeTranslate(Vector3(0.0, 5.0, 0.0));
	s2->draw(transform);
	
	transform.makeScale(1.5);
	s2->draw(transform);
	M = transform.makeScale(1.5, 4.0, 2.0);
	M = transform.makeTranslate(Vector3(3.5, 0.0, 0.0)) * M;
	c2->draw(M);
	M = transform.makeScale(1.5, 4.0, 2.0);
	M = transform.makeTranslate(Vector3(-3.5, 0.0, 0.0)) * M;
	c2->draw(M);
	// legs
	M = transform.makeScale(1.5, 4.0, 2.0);
	M = transform.makeTranslate(Vector3(1.5, -4.0, 0.0)) * M;
	c2->draw(M);
	M = transform.makeScale(1.5, 4.0, 2.0);
	M = transform.makeTranslate(Vector3(-1.5, -4.0, 0.0)) * M;
	c2->draw(M);
	*/
	platoon->draw(transform, Vector3(0.0, 0.0, 0.0), 1.0, 3);
	//std::cout << " finished drawing" << std::endl;
	platoon2->draw(transform, Vector3(0.0, 0.0, 0.0), 1.0, 3);
	//std::cout << " finished drawing" << std::endl;
	//robot->draw(transform);

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
void RobotWindow::processNormalKeys(unsigned char key, int x, int y) {
	if (key == 'b') {
		platoon->bounding = !(platoon->bounding);
		platoon2->bounding = !(platoon2->bounding);
	}

}

//TODO: Mouse callbacks!
void RobotWindow::processMouseButtons(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			Ldown = true;
			lastP = trackBallMapping(Vector3(x, y, 0));
		}
		else if (state == GLUT_UP)
			Ldown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			Rdown = true;
			lastP = trackBallMapping(Vector3(x, y, 0));
		}
		else if (state == GLUT_UP)
			Rdown = false;
	}

	if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		ctrl_down = true;
	}
	else
		ctrl_down = false;
}

Vector3 RobotWindow::trackBallMapping(Vector3 point)    // The CPoint class is a specific Windows class. Either use separate x and y values for the mouse location, or use a Vector3 in which you ignore the z coordinate.
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
void RobotWindow::processMouseMotion(int x, int y) {
	if (Ldown) {
		currP = trackBallMapping(Vector3(x, y, 0));
		directionAx = currP - lastP;
		velocity2 = directionAx.magnitude();
		if (velocity2 < 0.0001)	// if little movement - do nothing
			return;
		rotAx = lastP.cross(currP);
		rot_angle2 = velocity2 * 1.0;

		change2.makeRotateArbitrary(Vector3(rotAx[0], rotAx[1], rotAx[2]), rot_angle2);
		
		if (ctrl_down) {
			if (directionAx[1] < 0) {
				change2.makeScale(0.99);
				fov = fov*0.99;
				//Torso->M = Torso->M * change2;
				//Globals::camera.e = Globals::camera.e.add(Globals::camera.e.subtract(Globals::camera.d));
			}
			else if (directionAx[1] > 0) {
				change2.makeScale(1.01);
				fov = fov*1.01;
				//Torso->M = Torso->M * change2;
			}
		}
		else
			Torso->M = change2*Torso->M;
		

		lastP = currP;
	}
	else if (Rdown) {
		currP = trackBallMapping(Vector3(x, y, 0));
		directionAx = currP - lastP;
		directionAx.scale(100000.0);
		directionAx.set(2, 0.0);
		change2.makeTranslate(directionAx);
		//Globals::camera.c = change2*Globals::camera.c;
		Torso->M = change2*Torso->M;
		lastP = currP;
	}
}

RobotWindow::~RobotWindow()
{
	

}