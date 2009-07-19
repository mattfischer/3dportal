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
	    Poly( Poly &p );
        Render::Vertex &operator[]( int n );
	    Poly &operator=( Poly &p );

	    int NumVertices() { return numVerts; }

	    void BuildPlane();
	    Math::Plane GetPlane();
	    void SetPlane( Math::Vector normal );

	    Render::Texture *GetTexture();
	    void SetTexture( Render::Texture *t );
    	
	    void Transform( Math::Matrix &m );

	    int GetNumCels();

	    // R_Poly.cpp
	    void Clip( Math::Plane &p );
	    void Clip( Render::Frustum frustum );
    	
	    void Draw( Math::Vector tint, float light, bool translucent, bool cullReverse, int cel, float offsetU, float offsetV );
	    Render::Frustum CreateFrustum();

	    // P_Poly.cpp
	    int InFrontOfPlane( Math::Plane &c );
	    int VectorIn( Math::Vector &v );
    	
	    Math::Vector SpherePlaneOffset( Math::Vector position, float radius, bool &direct );
    	
    protected:
	    Render::Texture *texture;
	    int numVerts;
	    int allocatedVerts;
	    Render::Vertex *vertices;
	    Math::Plane plane;
    };
}

#endif