#ifndef M_MATRIX_H
#define M_MATRIX_H

#include "Global.h"

#include "M_Vector.h"

namespace Math
{
    class Matrix {
    public:
	    float m[4][4];

	    Matrix() {};
	    Matrix( float m0,  float m1,  float m2,  float m3,
		          float m4,  float m5,  float m6,  float m7,
		          float m8,  float m9,  float m10, float m11,
		          float m12, float m13, float m14, float m15);
	    Vector operator*( Vector &a );
	    Matrix operator*( Matrix &b );
    };
}

#endif