#ifndef CSE167_Rasterize_h
#define CSE167_Rasterize_h

#include "Matrix4.h"
#include "Drawable.h"
#include "Color.h"
#include "OBJObject.h"
#include "Camera.h"
#include "House.h"

class Rasterize	  // OpenGL output window related routines
{

public:

	int width, height; 	            // window size
	float* pixels;

	float colors[126];

	Matrix4 P;
	Matrix4 D;
	Drawable * toDraw;
	int level;
	bool debug;
	float* zbuffer;


	Rasterize();

	void resizePixelBuffer(int, int);
	void draw(int, Camera, House);
	void drawPoint(int, int, float, float, float);
	void rasterizeVertex(int,Camera, House);
	void clearBuffers(void);
	void rasterizeTriangle(int, Camera, House);
	bool barycentric(Camera, int);
	void resizeZBuffer(int, int);
	/*
	void perPixel(Vector3, Vector3, Vector3);
	
	static void loadData(void);
	static void rasterizeTriangle(void);
	static void reshape(int, int);	
	*/
};

#endif

