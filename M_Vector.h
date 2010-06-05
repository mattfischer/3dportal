#ifndef M_VECTOR_H
#define M_VECTOR_H

namespace Math
{
    class Vector {
    public:
	    Vector() { x = y = z = 0; }
		Vector( float m_x, float m_y, float m_z ) { x = m_x; y = m_y; z = m_z; }
	    float operator*( const Vector &b ) const { return x*b.x+y*b.y+z*b.z; }
	    Vector operator*( float b ) const;
	    Vector operator/( float b ) const;
	    Vector operator+( const Vector &b ) const;
	    Vector operator-( const Vector &b ) const;
	    Vector operator%( const Vector &b ) const;
	    Vector &operator=( const Vector &b ) { x=b.x; y=b.y; z=b.z; return *this; }
	    Vector &operator+=( const Vector &b );
	    Vector &operator-=( const Vector &b );
	    Vector &operator*=( float b );
	    bool operator==( const Vector &b ) const;
	    bool operator!=( const Vector &b ) const;
        Vector operator-() const;

	    float Magnitude() const;
	    float Magnitude2() const;

	    float ScalarProjection( const Vector &b ) const;
	    Vector VectorProjection( const Vector &b ) const;
	    void Normalize();

	    float x;
	    float y;
	    float z;
    };
}

#endif