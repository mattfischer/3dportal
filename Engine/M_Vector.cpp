#include "M_Vector.h"

#include <math.h>

M_Vector::M_Vector(float m_x, float m_y, float m_z)
{
	x=m_x;
	y=m_y;
	z=m_z;
}

float M_Vector::operator*(M_Vector &b)
{
	return x*b.x+y*b.y+z*b.z;
}

M_Vector M_Vector::operator*(float b)
{
	M_Vector newVector;

	newVector.x=x*b;
	newVector.y=y*b;
	newVector.z=z*b;

	return newVector;
}

M_Vector M_Vector::operator/(float b)
{
	M_Vector newVector;

	newVector.x=x/b;
	newVector.y=y/b;
	newVector.z=z/b;

	return newVector;
}

M_Vector M_Vector::operator+(M_Vector &b)
{
	M_Vector newVector;

	newVector.x=x+b.x;
	newVector.y=y+b.y;
	newVector.z=z+b.z;

	return newVector;
}

M_Vector M_Vector::operator-(M_Vector &b)
{
	M_Vector newVector;

	newVector.x=x-b.x;
	newVector.y=y-b.y;
	newVector.z=z-b.z;

	return newVector;
}

M_Vector M_Vector::operator%(M_Vector &b)
{
	M_Vector newVector;

	newVector.x=y*b.z-z*b.y;
	newVector.y=z*b.x-x*b.z;
	newVector.z=x*b.y-y*b.x;

	return newVector;
}

M_Vector &M_Vector::operator+=(M_Vector &b)
{
	x+=b.x;
	y+=b.y;
	z+=b.z;

	return *this;
}

M_Vector &M_Vector::operator-=(M_Vector &b)
{
	x-=b.x;
	y-=b.y;
	z-=b.z;

	return *this;
}

M_Vector &M_Vector::operator*=(float b)
{
	x*=b;
	y*=b;
	z*=b;

	return *this;
}

M_Vector M_Vector::VectorProjection(M_Vector &b)
{
	return *this * ((*this * b)/( *this * *this));
}

float M_Vector::Magnitude()
{
	return sqrt(x*x+y*y+z*z);
}

float M_Vector::Magnitude2()
{
	return x*x+y*y+z*z;
}

float M_Vector::ScalarProjection(M_Vector &b)
{
	return (*this*b)/Magnitude();
}

void M_Vector::Normalize()
{
	float m=Magnitude();

	x/=m;
	y/=m;
	z/=m;
}

M_Vector &M_Vector::operator=(const M_Vector &b)
{
	x=b.x;
	y=b.y;
	z=b.z;

	return *this;
}

bool M_Vector::operator==(M_Vector &b)
{
	if(x==b.x && y==b.y && z==b.z) return true;
	else return false;
}

bool M_Vector::operator!=(M_Vector &b)
{
	if(x!=b.x || y!=b.y || z!=b.z) return true;
	else return false;
}