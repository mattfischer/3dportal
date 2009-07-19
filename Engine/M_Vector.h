#ifndef M_VECTOR_H
#define M_VECTOR_H

namespace Math
{
    class Vector {
    public:
	    Vector() { x = y = z = 0; }
	    Vector( float m_x, float m_y, float m_z );
	    float operator*( Vector &b );
	    Vector operator*( float b );
	    Vector operator/( float b );
	    Vector operator+( Vector &b );
	    Vector operator-( Vector &b );
	    Vector operator%( Vector &b );
	    Vector &operator=( const Vector &b );
	    Vector &operator+=( Vector &b );
	    Vector &operator-=( Vector &b );
	    Vector &operator*=( float b );
	    bool operator==( Vector &b );
	    bool operator!=( Vector &b );
        Vector operator-();

	    float Magnitude();
	    float Magnitude2();

	    float ScalarProjection( Vector &b );
	    Vector VectorProjection( Vector &b );
	    void Normalize();

	    float x;
	    float y;
	    float z;
    };
}

#endif