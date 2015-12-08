#ifndef CSE167_Globals_h
#define CSE167_Globals_h


#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include "House.h"
#include "Rasterize.h"
#include "Sphere.h"

class Globals
{
    
public:
    
    static Camera camera;
    static Cube cube;
    static Light light;
    static DrawData drawData;
    static UpdateData updateData;
    //Feel free to add more member variables as needed
	static House house;
	static Rasterize rasterizer;
	static Sphere sphere;
	/*
	static OBJObject * body1, *body2, *body3, *body4, *body5;
	static OBJObject *roof1, *roof2, *roof3, *roof4, *roof5;
	static OBJObject *door1, *door2, *door3, *door4, *door5;
	static std::vector<Drawable*>* drawablesf, *drawables_body, *drawables_roof, *drawables_door;
	*/
};

#endif
