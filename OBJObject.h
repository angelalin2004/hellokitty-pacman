#ifndef CSE167_OBJObject_h
#define CSE167_OBJObject_h

#include <iostream>
#include <vector>
#include <string>
#include "Vector3.h"
#include "Drawable.h"

struct Face
{
    int vertexIndices[3];
    int normalIndices[3];
    //Add more members as necessary
};

class OBJObject : public Drawable
{
    
protected:
    
    
    //Helper functions
    std::vector<std::string>& split(const std::string&, char, std::vector<std::string>&);
    std::vector<std::string> split(const std::string&, char);
    
    //Parse
    void parse(std::string&);
	void normalize();
    
public:

	//Storage vectors
	std::vector<Vector3*>* vertices;
	std::vector<Vector3*>* normals;
	std::vector<Face*>* faces;
    
    OBJObject(std::string);
    virtual ~OBJObject(void);
    
    virtual void draw(DrawData&);
	virtual void update(UpdateData&);

	float minx;
	float maxx;
	float miny;
	float maxy;
	float minz;
	float maxz;

	float midx;
	float midy;
	float midz;
	Vector3 center;
	Vector3 midpoint;
	float normal_factor;
	float maxdim;
    
};

#endif
