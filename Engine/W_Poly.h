#ifndef W_POLY_H
#define W_POLY_H

#include "Global.h"

#include "M_Vector.h"
#include "M_Matrix.h"
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
	M_Plane GetPlane();
	void SetPlane( M_Vector normal );

	R_Texture *GetTexture();
	void SetTexture( R_Texture *t );
	
	void Transform( M_Matrix &m );

	int GetNumCels();

	// R_Poly.cpp
	void Clip( M_Plane &p );
	void Clip( R_Frustum frustum );
	
	void Draw( M_Vector tint, float light, bool translucent, bool cullReverse, int cel, float offsetU, float offsetV );
	R_Frustum CreateFrustum();

	// P_Poly.cpp
	int InFrontOfPlane( M_Plane &c );
	int VectorIn( M_Vector &v );
	
	M_Vector SpherePlaneOffset( M_Vector position, float radius, bool &direct );
	
protected:
	R_Texture *texture;
	int numVerts;
	int allocatedVerts;
	R_Vertex *vertices;
	M_Plane plane;
};

#endif