#ifndef CSE167_FinalWindow_h
#define CSE167_FinalWindow_h

#include "Vector3.h"
#include "Cube.h"
#include "Sphere.h"
#include "Globals.h"
#include "OBJObject.h"
#include "Group.h"
#include "MatrixTransform.h"
#include "Kitty.h"

class FinalWindow	  // OpenGL output window related routines
{

public:

	static int width, height; 	            // window size

	static void initialize(void);
	static void idleCallback(void);
	static void reshapeCallback(int, int);
	static void displayCallback(void);

	
	static void processNormalKeys(unsigned char, int, int);
	static void processFunctionKeys(int, int, int);
	static void processMouseButtons(int, int, int, int);
	static Vector3 trackBallMapping(Vector3);
	static void processMouseMotion(int, int);


	virtual ~FinalWindow(void);
};

#endif