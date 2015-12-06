#include "Camera.h"

Camera::Camera()
{
    c.identity();
    e.set(0.0, 0.0, 20.0);
    d.set(0.0, 0.0, 0.0);
    up.set(0.0, 1.0, 0.0);
    
    //Pre-define a camera matrix (and its inverse) that are shifted 'e' from the origin
    //This is used as a default camera position for Project 1
    c.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, e[0], e[1], e[2], 1);
    ci.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -e[0], -e[1], -e[2], 1);
}

Camera::~Camera()
{
    //Delete and dynamically allocated memory/objects here
}

Matrix4& Camera::getMatrix()
{
    return c;
}

Matrix4& Camera::getInverseMatrix()
{
    return ci;
}

void Camera::update()
{
    //Update the Camera Matrix using d, e, and up
    //Solve for the z, x, and y axes of the camera matrix
    //Use these axes and the e vector to create a camera matrix c
    //Use c to solve for an inverse camera matrix ci
	Vector3 z = (e.subtract(d)).multiply(1/(e.subtract(d)).magnitude());
	Vector3 x = (up.cross(z)).multiply(1 / (up.cross(z).magnitude()));
	Vector3 y = z.cross(x);
	c.set(x.operator[](0), x.operator[](1), x.operator[](2), 0.0,
		y.operator[](0), y.operator[](1), y.operator[](2), 0.0,
		z.operator[](0), z.operator[](1), z.operator[](2), 0.0,
		e.operator[](0), e.operator[](1), e.operator[](2), 1.0);
	ci = c.inverse();
}

void Camera::set(Vector3& e, Vector3& d, Vector3& up)
{
    this->e = e;
    this->d = d;
    this->up = up;
    update();
}
