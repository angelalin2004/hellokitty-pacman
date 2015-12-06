#ifndef CSE167_Window_h
#define CSE167_Window_h

#include "Vector3.h"

class Window	  // OpenGL output window related routines
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

	static void printPosition();
	

	virtual ~Window(void);
};

#endif

