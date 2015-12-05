#include "Vector3.h"
#include <math.h>
#include <iostream>
#include <cstring>
#include "Vector4.h"

Vector3::Vector3()
{
    std::memset(m, 0, sizeof(m));
}

Vector3::Vector3(float m0, float m1, float m2)
{
    m[0] = m0;
    m[1] = m1;
    m[2] = m2;
}

Vector3::~Vector3()
{
    //
}

float* Vector3::ptr()
{
    return &m[0];
}

void Vector3::set(float x, float y, float z)
{
    m[0] = x;
    m[1] = y;
    m[2] = z;
}

void Vector3::set(int index, float value)
{
    m[index] = value;
}

float& Vector3::operator [] (int loc)
{
    return m[loc];
}

Vector3 Vector3::add(Vector3 a)
{
    Vector3 b;
    
    b = Vector3(m[0] + a.m[0], m[1] + a.m[1], m[2] + a.m[2]);
    
    return b;
}

Vector3 Vector3::operator + (Vector3 a)
{
    return add(a);
}

Vector3 Vector3::subtract(Vector3 a)
{
    Vector3 b;
    
    b = Vector3(m[0] - a.m[0], m[1] - a.m[1], m[2] - a.m[2]);
    
    return b;
}

Vector3 Vector3::operator - (Vector3 a)
{
    return subtract(a);
}

Vector3 Vector3::negate(void)
{
    Vector3 b;
	float zero = 0.0f;
    b = Vector3(zero - m[0], zero - m[1], zero - m[2]);
    
    return b;
}

Vector3 Vector3::scale(float s)
{
    Vector3 b;
    
    b = Vector3(m[0]*s, m[1]*s, m[2]*s);
    
    return b;
}

Vector3 Vector3::multiply(float a)
{
    Vector3 b;
    
	b = Vector3(m[0] * a, m[1] * a, m[2] * a);
    
    return b;
}

Vector3 Vector3::operator * (float a)
{
    return multiply(a);
}

Vector3 Vector3::multiply(Vector3 a)
{
    Vector3 b;
    
    //Coomponent-wise multiplication
	b = Vector3(m[0] * a.m[0], m[1] * a.m[1], m[2] * a.m[2]);
    
    return b;
}

Vector3 Vector3::operator * (Vector3 a)
{
    return multiply(a);
}


float Vector3::dot(Vector3 a)
{
    return m[0]*a.m[0] + m[1]*a.m[1] + m[2]*a.m[2];

}

Vector3 Vector3::cross(Vector3 a)
{
    Vector3 b;
    
	b = Vector3(m[1] * a.m[2] - m[2] * a.m[1],
					m[2] * a.m[0] - m[0] * a.m[2],
					m[0] * a.m[1] - m[1] * a.m[0]);
    
    return b;
}

float Vector3::angle(Vector3 a)
{
    
    return acos(dot(a) / (magnitude()*a.magnitude()));
}

float Vector3::magnitude(void)
{
    return sqrt( pow(m[0],2) + pow(m[1],2) + pow(m[2],2) );

}

Vector3 Vector3::normalize(void)
{
    Vector3 b;
    
	if (magnitude() == 0.0)
		b = Vector3(0.0, 0.0, 0.0);
	else
		b = scale(1 / magnitude());
    
    return b;
}

Vector4 Vector3::toVector4(float w)
{
    Vector4 b(m[0], m[1], m[2], w);
    return b;
}

void Vector3::print(std::string comment)
{
    std::cout << comment << std::endl;
    std::cout << "<x:" << m[0] <<  ", y:" << m[1] << ", z:" << m[2] << ">" << std::endl;
}
