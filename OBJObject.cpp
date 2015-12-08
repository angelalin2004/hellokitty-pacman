#include "OBJObject.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "math.h"
#include <sstream>
#include <fstream>
#include <algorithm>

#define deleteVector(__type__, __vect__) do {\
                                   std::vector<__type__>::iterator iter; \
                                   std::vector<__type__>::iterator end; \
                                   iter = __vect__->begin();\
                                   end = __vect__->end();\
                                   while(iter != end) delete (*(iter++));\
                                   delete __vect__;\
                               } while(false)


OBJObject::OBJObject(std::string filename) : Drawable()
{
    this->vertices = new std::vector<Vector3*>();
    this->normals = new std::vector<Vector3*>();
    this->faces = new std::vector<Face*>();
    
    parse(filename);
	normalize();
}

OBJObject::~OBJObject()
{
    //Delete any dynamically allocated memory/objects here
    
    deleteVector(Vector3*, vertices);
    deleteVector(Vector3*, normals);
    deleteVector(Face*, faces);

	maxx = maxy = maxz = 0.0;
	minx = miny = minz = -1.0;
}

void OBJObject::draw(DrawData& data)
{
    material.apply();
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glMultMatrixf(toWorld.ptr());
    
    glBegin(GL_TRIANGLES);
    
    
    //Loop through the faces
    //For each face:
    //  Look up the vertices, normals (if they exist), and texcoords (if they exist)
    //  Draw them as triplets:
    
    //      glNorm(normals->at(face.normalIndices[0]))
    //      glVert(vertices->at(face.vertexIndices[0]))
    //      Etc.
    //
	
	for (int i = 0; i < faces->size(); i++) {
		for (int j = 0; j < 3; j++) {
			int v = (*faces)[i]->vertexIndices[j];
			int n = (*faces)[i]->normalIndices[j];
			glNormal3f(normals->at(n)->operator[](0), normals->at(n)->operator[](1), normals->at(n)->operator[](2));
			glVertex3f(vertices->at(v)->operator[](0), vertices->at(v)->operator[](1), vertices->at(v)->operator[](2));
		}
	}
    
    glEnd();
    
    glPopMatrix();
}

void OBJObject::update(UpdateData& data)
{
    //
}

void OBJObject::parse(std::string& filename)
{
    std::ifstream infile(filename);
    std::string line;
    std::vector<std::string> tokens;
    std::string token;
    
    int lineNum = 0;
    
    std::cout << "Starting parse..." << std::endl;
	bool printed = false;
    
    //While all your lines are belong to us
    while (std::getline(infile, line))
    {
        //Progress
        if(++lineNum % 10000 == 0)
            std::cout << "At line " << lineNum << std::endl;
        
        //Split a line into tokens by delimiting it on spaces
        //"Er Mah Gerd" becomes ["Er", "Mah", "Gerd"]
        tokens.clear();
		/*
		if (lineNum > 65000)
			std::cout << "About to split line " << lineNum << " " << line.length() << " " << line << std::endl;
        */
		if (line.length() == 0)
			continue;
		tokens = split(line, ' ', tokens);

        //If first token is a v then it gots to be a vertex
        if(tokens.at(0).compare("v") == 0)
        {
            //Parse the vertex line
            float x = std::stof(tokens.at(1));
            float y = std::stof(tokens.at(2));
            float z = std::stof(tokens.at(3));

			if (minx == -1)
				minx = x;
			if (miny == -1)
				miny = y;
			if (minz == -1)
				minz = z;

			if (x > maxx)
				maxx = x;
			else if (y > maxy)
				maxy = y;
			else if (z > maxz)
				maxz = z;
			else if (x < minx)
				minx = x;
			else if (y < miny)
				miny = y;
			else if (z < minz)
				minz = z;
            
            vertices->push_back(new Vector3(x, y, z));
        }
        else if(tokens.at(0).compare("vn") == 0)
        {
            //Parse the normal line
			float x = std::stof(tokens.at(1));
			float y = std::stof(tokens.at(2));
			float z = std::stof(tokens.at(3));

			normals->push_back(new Vector3(x, y, z));
        }
        else if(tokens.at(0).compare("f") == 0)
        {
            Face* face = new Face;

			//Parse the face line
			std::vector<std::string> temp_tokens;
			temp_tokens = split(tokens.at(1), '/', temp_tokens);
			face->vertexIndices[0] = std::stoi(temp_tokens.at(0))-1;
			face->normalIndices[0] = std::stoi(temp_tokens.at(2))-1;

			temp_tokens = split(tokens.at(2), '/', temp_tokens);
			face->vertexIndices[1] = std::stoi(temp_tokens.at(3))-1;
			face->normalIndices[1] = std::stoi(temp_tokens.at(5))-1;

			temp_tokens = split(tokens.at(3), '/', temp_tokens);
			face->vertexIndices[2] = std::stoi(temp_tokens.at(6))-1;
			face->normalIndices[2] = std::stoi(temp_tokens.at(8))-1;
			/*
			if (lineNum % 10000 == 0)
				printed = false;
			if (!printed) {
				std::cout << face->vertexIndices[0] << " " << face->normalIndices[0] << " " << lineNum << std::endl;
				std::cout << face->vertexIndices[1] << " " << face->normalIndices[1] << " " << lineNum << std::endl;
				std::cout << face->vertexIndices[2] << " " << face->normalIndices[2] << " " << lineNum << std::endl;
				printed = true;
			}
			*/

			faces->push_back(face);
        }
        else if(tokens.at(0).compare("How does I are C++?!?!!") == 0)
        {
            //Parse as appropriate
            //There are more line types than just the above listed
            //See the Wavefront Object format specification for details
        }
		else {}
    }
    
    std::cout << "Done parsing." << std::endl;
	/*
	std::cout << vertices->size() << std::endl;
	std::cout << normals->size() << std::endl;
	std::cout << faces->size() << std::endl;
	*/
}

//Split functions from the interwebs
//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string>& OBJObject::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> OBJObject::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void OBJObject::normalize()
{
	if (vertices->empty())
		return;
	/*
	std::cout << minx << " " << maxx << std::endl;
	std::cout << miny << " " << maxy << std::endl;
	std::cout << minz << " " << maxz << std::endl;
	*/
	midx = (maxx + minx) / 2;
	midy = (maxy + miny) / 2;
	midz = (maxz + minz) / 2;
	center = Vector3((maxx + minx) / 2, (maxy + miny) / 2, (maxz + minz) / 2);
	center.print("");
	/*
	for (int i = 0; i < (sizeof vertices) / (sizeof vertices->at(0)); i++) {
		Vector3 temp = *(vertices->at(i));
		temp[0] = temp[0] - midx;
		temp[1] = temp[1] - midy;
		temp[2] = temp[2] - midz;
		vertices->at(i) = &temp;
	}
	*/
	normal_factor = std::max(std::max((maxx - minx) / 2, (maxy - miny) / 2), (maxz - minz) / 2);
	maxdim = std::max(std::max((maxx - minx), (maxy - miny)), (maxz - minz));
}