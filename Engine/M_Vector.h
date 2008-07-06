#ifndef M_VECTOR_H
#define M_VECTOR_H

#include "Global.h"

class M_Vector {
public:
	M_Vector() {}
	M_Vector( float m_x, float m_y, float m_z );
	float operator*( M_Vector &b );
	M_Vector operator*( float b );
	M_Vector operator/( float b );
	M_Vector operator+( M_Vector &b );
	M_Vector operator-( M_Vector &b );
	M_Vector operator%( M_Vector &b );
	M_Vector &operator=( const M_Vector &b );
	M_Vector &operator+=( M_Vector &b );
	M_Vector &operator-=( M_Vector &b );
	M_Vector &operator*=( float b );
	bool operator==( M_Vector &b );
	bool operator!=( M_Vector &b );

	float Magnitude();
	float Magnitude2();

	float ScalarProjection( M_Vector &b );
	M_Vector VectorProjection( M_Vector &b );
	void Normalize();

	float x;
	float y;
	float z;
};

#endif