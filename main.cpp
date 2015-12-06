#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"
#include "MathTestBench.h"
#include "RobotWindow.h"
#include "FinalWindow.h"



int main(int argc, char *argv[])
{
	// manually change to true if HW5, false otherwise
	bool robotwindow = false;
	// manually change to true if final project, false otherwise
	bool finalproj = true;

    //Math Test Bench
    //MathTestBench::runTests();
    
    //GLUT and OpenGL Configuration
    glutInit(&argc, argv);                                      //Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   //Open an OpenGL context with double buffering, RGB colors, and depth buffering
    glutInitWindowSize(Window::width, Window::height);          //Set initial window size
    glutCreateWindow("UCSD CSE 167 - Project 1 - OpenGL Cube"); //Open window and set window title
    
    glEnable(GL_DEPTH_TEST);                                    //Enable depth buffering
    glClear(GL_DEPTH_BUFFER_BIT);                               //Clear depth buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);                           //Set clear color to black
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                  //Set polygon drawing mode to fill front and back of each polygon
    glDisable(GL_CULL_FACE);                                    //Disable backface culling to render both sides of polygons
    glShadeModel(GL_SMOOTH);                                    //Set shading to smooth
    
    //glEnable(GL_COLOR_MATERIAL);                                //Enable color materials
    glEnable(GL_LIGHTING);                                      //Enable lighting
    glEnable(GL_NORMALIZE);                                     //Auto normalize surface normals
    glEnable(GL_TEXTURE_2D);                                    //Enable 2D textures
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);        //Enable Local Viewer Light Model
    
	if (robotwindow) {
		glutDisplayFunc(RobotWindow::displayCallback);
		glutReshapeFunc(RobotWindow::reshapeCallback);
		glutIdleFunc(RobotWindow::idleCallback);
		//Register the callback for the keyboard
		glutKeyboardFunc(RobotWindow::processNormalKeys);
		//Register the callback for the mouse
		glutMouseFunc(RobotWindow::processMouseButtons);
		//Register the callback for the mouse motion
		glutMotionFunc(RobotWindow::processMouseMotion);
	}
	else if (finalproj) {
		//Register callback functions:
		glutDisplayFunc(FinalWindow::displayCallback);
		glutReshapeFunc(FinalWindow::reshapeCallback);
		glutIdleFunc(FinalWindow::idleCallback);
		//Register the callback for the keyboard
		glutKeyboardFunc(FinalWindow::processNormalKeys);
		//Register the callback for the keyboard function keys
		//glutSpecialFunc(FinalWindow::processFunctionKeys);
		//Register the callback for the mouse
		glutMouseFunc(FinalWindow::processMouseButtons);
		//Register the callback for the mouse motion
		glutMotionFunc(FinalWindow::processMouseMotion);
		//Register the callback for the mouse passive motion
	}
	else {
		//Register callback functions:
		glutDisplayFunc(Window::displayCallback);
		glutReshapeFunc(Window::reshapeCallback);
		glutIdleFunc(Window::idleCallback);
		//Register the callback for the keyboard
		glutKeyboardFunc(Window::processNormalKeys);
		//Register the callback for the keyboard function keys
		glutSpecialFunc(Window::processFunctionKeys);
		//Register the callback for the mouse
		glutMouseFunc(Window::processMouseButtons);
		//Register the callback for the mouse motion
		glutMotionFunc(Window::processMouseMotion);
		//Register the callback for the mouse passive motion
	}


    //Print Shader Debug Information:
    printf("%s\n%s\n",
           glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
           glGetString(GL_VERSION)    // e.g. 3.2 INTEL-8.0.61
           );
    //std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    //Initialize the Window:
    //The body of this function is a great place to load textures, shaders, etc.
    //and do any operations/calculations/configurations that only need to happen once.
	if (robotwindow) {
		RobotWindow::initialize();
		
	}
	else if (finalproj) {
		FinalWindow::initialize();
	}
	else {
		Window::initialize();
	}
    
    //Start up the render loop!
    glutMainLoop();
    
    return 0;
}
