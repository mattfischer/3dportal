#ifndef W_POLY_H
#define W_POLY_H

#include "M_Vector.h"
#include "R_Vertex.h"
#include "R_Frame.h"
#include "M_Plane.h"

namespace Render
{
    struct Texture;
}

namespace World
{
    class Poly {
    public:
	    // Poly.cpp

	    Poly() { numVerts=0; allocatedVerts=0; vertices=0; }
	    Poly( int n, int a = 0 );	
	    ~Poly();
	    Poly( const Poly &p );
        Render::Vertex &operator[]( int n );
		const Render::Vertex &operator[]( int n ) const;
	    Poly &operator=( const Poly &p );

	    int NumVertices() const { return numVerts; }

	    void BuildPlane();
	    Math::Plane GetPlane() const;
	    void SetPlane( Math::Vector normal );

	    Render::Texture *GetTexture() const;
	    void SetTexture( Render::Texture *t );
    	
	    void Transform( Math::Matrix &m );

	    int GetNumCels() const;

	    // R_Poly.cpp
	    void Clip( Math::Plane &p );
	    void Clip( Render::Frustum frustum );
    	
	    void Draw( Math::Vector tint, float light, bool translucent, bool cullReverse, int cel, float offsetU, float offsetV ) const;
	    Render::Frustum CreateFrustum() const;

	    // P_Poly.cpp
	    int InFrontOfPlane( Math::Plane &c ) const;
	    int VectorIn( Math::Vector &v ) const;
    	
	    Math::Vector SpherePlaneOffset( Math::Vector position, float radius, bool &direct ) const;
    	
    protected:
	    Render::Texture *texture;
	    int numVerts;
	    int allocatedVerts;
	    Render::Vertex *vertices;
	    Math::Plane plane;
    };
}

#endif