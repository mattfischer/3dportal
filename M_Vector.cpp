#include "M_Vector.h"

#include <math.h>

namespace Math
{
    Vector Vector::operator*(float b) const
    {
	    Vector newVector;

	    newVector.x=x*b;
	    newVector.y=y*b;
	    newVector.z=z*b;

	    return newVector;
    }

    Vector Vector::operator/(float b) const
    {
	    Vector newVector;

	    newVector.x=x/b;
	    newVector.y=y/b;
	    newVector.z=z/b;

	    return newVector;
    }

    Vector Vector::operator+(const Vector &b) const
    {
	    Vector newVector;

	    newVector.x=x+b.x;
	    newVector.y=y+b.y;
	    newVector.z=z+b.z;

	    return newVector;
    }

    Vector Vector::operator-(const Vector &b) const
    {
	    Vector newVector;

	    newVector.x=x-b.x;
	    newVector.y=y-b.y;
	    newVector.z=z-b.z;

	    return newVector;
    }

    Vector Vector::operator%(const Vector &b) const
    {
	    Vector newVector;

	    newVector.x=y*b.z-z*b.y;
	    newVector.y=z*b.x-x*b.z;
	    newVector.z=x*b.y-y*b.x;

	    return newVector;
    }

    Vector &Vector::operator+=(const Vector &b)
    {
	    x+=b.x;
	    y+=b.y;
	    z+=b.z;

	    return *this;
    }

    Vector &Vector::operator-=(const Vector &b)
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

    Vector Vector::operator-() const
    {
        return *this * -1;
    }

    Vector Vector::VectorProjection(const Vector &b) const
    {
	    return *this * ((*this * b)/( *this * *this));
    }

    float Vector::Magnitude() const
    {
	    return sqrt(x*x+y*y+z*z);
    }

    float Vector::Magnitude2() const
    {
	    return x*x+y*y+z*z;
    }

    float Vector::ScalarProjection(const Vector &b) const
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


    bool Vector::operator==(const Vector &b) const
    {
	    if(x==b.x && y==b.y && z==b.z) return true;
	    else return false;
    }

    bool Vector::operator!=(const Vector &b) const
    {
	    if(x!=b.x || y!=b.y || z!=b.z) return true;
	    else return false;
    }
}