#include "Material.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

Material::Material()
{
    //Material setup goes hurr
    ambientColor = Color::ambientMaterialDefault();
    diffuseColor = Color::diffuseMaterialDefault();
    specularColor = Color::specularMaterialDefault();
    emissionColor = Color::emissionMaterialDefault();
    shininess = 10.0;
    
    //Set the initial color to a random pastel color
    //Can be removed once materials are implemented
    color = Color::randomPastel();
}

Material::~Material()
{
    //Delete any dynamically allocated memory/objects here
}

void Material::apply(void)
{
    //Set the material properties for ambient, diffuse, specular, emission, and shininess
    //Hint: Lookup how glMaterialfv works
	float default[4] = { 0.0, 0.0, 0.0, 0.0 };
	default[3] = 1.0;
	
	for (int i = 0; i < 3; i++) {
		default[i] = ambientColor[i];
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)default);
	for (int i = 0; i < 3; i++) {
		default[i] = diffuseColor[i];
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)default);
	for (int i = 0; i < 3; i++) {
		default[i] = specularColor[i];
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)default);
	for (int i = 0; i < 3; i++) {
		default[i] = emissionColor[i];
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat *)default);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat *)&shininess);
    
    //Set the color property (can be removed once materials are implemented)
    //Used for projects 1, 2, and 3
    //glColor3fv(color.ptr());
}

void Material::reset(void)
{
	ambientColor = Color::ambientMaterialDefault();
	diffuseColor = Color::diffuseMaterialDefault();
	specularColor = Color::specularMaterialDefault();
	emissionColor = Color::emissionMaterialDefault();
	shininess = 10.0;
}


