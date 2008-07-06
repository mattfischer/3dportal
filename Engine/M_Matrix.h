#ifndef M_MATRIX_H
#define M_MATRIX_H

#include "Global.h"

#include "M_Vector.h"

class M_Matrix {
public:
	float m[4][4];

	M_Matrix() {};
	M_Matrix( float m0,  float m1,  float m2,  float m3,
		      float m4,  float m5,  float m6,  float m7,
		      float m8,  float m9,  float m10, float m11,
		      float m12, float m13, float m14, float m15);
	M_Vector operator*( M_Vector &a );
	M_Matrix operator*( M_Matrix &b );
};

#endif