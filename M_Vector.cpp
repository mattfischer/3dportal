#include "M_Vector.h"

#include <math.h>

namespace Math
{
    Vector::Vector(float m_x, float m_y, float m_z)
    {
	    x=m_x;
	    y=m_y;
	    z=m_z;
    }

    float Vector::operator*(Vector &b)
    {
	    return x*b.x+y*b.y+z*b.z;
    }

    Vector Vector::operator*(float b)
    {
	    Vector newVector;

	    newVector.x=x*b;
	    newVector.y=y*b;
	    newVector.z=z*b;

	    return newVector;
    }

    Vector Vector::operator/(float b)
    {
	    Vector newVector;

	    newVector.x=x/b;
	    newVector.y=y/b;
	    newVector.z=z/b;

	    return newVector;
    }

    Vector Vector::operator+(Vector &b)
    {
	    Vector newVector;

	    newVector.x=x+b.x;
	    newVector.y=y+b.y;
	    newVector.z=z+b.z;

	    return newVector;
    }

    Vector Vector::operator-(Vector &b)
    {
	    Vector newVector;

	    newVector.x=x-b.x;
	    newVector.y=y-b.y;
	    newVector.z=z-b.z;

	    return newVector;
    }

    Vector Vector::operator%(Vector &b)
    {
	    Vector newVector;

	    newVector.x=y*b.z-z*b.y;
	    newVector.y=z*b.x-x*b.z;
	    newVector.z=x*b.y-y*b.x;

	    return newVector;
    }

    Vector &Vector::operator+=(Vector &b)
    {
	    x+=b.x;
	    y+=b.y;
	    z+=b.z;

	    return *this;
    }

    Vector &Vector::operator-=(Vector &b)
    {
	    x-=b.x;
	    y-=b.y;
	    z-=b.z;

	    return *this;
    }

    Vector &Vector::operator*=(float b)
    {
	    x*=b;
	    y*=b;
	    z*=b;

	    return *this;
    }

    Vector Vector::operator-()
    {
        return *this * -1;
    }

    Vector Vector::VectorProjection(Vector &b)
    {
	    return *this * ((*this * b)/( *this * *this));
    }

    float Vector::Magnitude()
    {
	    return sqrt(x*x+y*y+z*z);
    }

    float Vector::Magnitude2()
    {
	    return x*x+y*y+z*z;
    }

    float Vector::ScalarProjection(Vector &b)
    {
	    return (*this*b)/Magnitude();
    }

    void Vector::Normalize()
    {
	    float m=Magnitude();

	    x/=m;
	    y/=m;
	    z/=m;
    }

    Vector &Vector::operator=(const Vector &b)
    {
	    x=b.x;
	    y=b.y;
	    z=b.z;

	    return *this;
    }

    bool Vector::operator==(Vector &b)
    {
	    if(x==b.x && y==b.y && z==b.z) return true;
	    else return false;
    }

    bool Vector::operator!=(Vector &b)
    {
	    if(x!=b.x || y!=b.y || z!=b.z) return true;
	    else return false;
    }
}