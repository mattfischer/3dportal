#ifndef W_POLY_H
#define W_POLY_H

#include "Global.h"

#include "M_Vector.h"
#include "R_Vertex.h"
#include "R_Frame.h"
#include "M_Plane.h"

struct R_Texture;

class W_Poly {
public:
	// W_Poly.cpp

	W_Poly() { numVerts=0; allocatedVerts=0; vertices=NULL; }
	W_Poly( int n, int a = 0 );	
	~W_Poly();
	W_Poly( W_Poly &p );
	R_Vertex &operator[]( int n );
	W_Poly &operator=( W_Poly &p );

	int NumVertices() { return numVerts; }

	void BuildPlane();
	Math::Plane GetPlane();
	void SetPlane( Math::Vector normal );

	R_Texture *GetTexture();
	void SetTexture( R_Texture *t );
	
	void Transform( Math::Matrix &m );

	int GetNumCels();

	// R_Poly.cpp
	void Clip( Math::Plane &p );
	void Clip( R_Frustum frustum );
	
	void Draw( Math::Vector tint, float light, bool translucent, bool cullReverse, int cel, float offsetU, float offsetV );
	R_Frustum CreateFrustum();

	// P_Poly.cpp
	int InFrontOfPlane( Math::Plane &c );
	int VectorIn( Math::Vector &v );
	
	Math::Vector SpherePlaneOffset( Math::Vector position, float radius, bool &direct );
	
protected:
	R_Texture *texture;
	int numVerts;
	int allocatedVerts;
	R_Vertex *vertices;
	Math::Plane plane;
};

#endif