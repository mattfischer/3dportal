#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "W_Poly.h"
#include "W_Thing.h"

#include "R_Frame.h"
#include "R_Texture.h"

#include <math.h>

extern int fullLight;

W_Poly::W_Poly(int n, int a) 
{
	numVerts=n;
	allocatedVerts=n+a;
	vertices=new R_Vertex[allocatedVerts];
}

W_Poly::~W_Poly() 
{ 
	if(allocatedVerts>0) delete[] vertices; 
}

W_Poly::W_Poly(W_Poly &p)
{
	int i;

	texture=p.texture;
	plane=p.plane;
	numVerts=p.numVerts;
	allocatedVerts=p.allocatedVerts;
	vertices=new R_Vertex[allocatedVerts];
	for(i=0;i<numVerts;i++) vertices[i]=p.vertices[i];
}

W_Poly &W_Poly::operator=(W_Poly &p)
{
	int i;

	if(allocatedVerts>0) delete[] vertices;

	texture=p.texture;
	plane=p.plane;
	numVerts=p.numVerts;
	allocatedVerts=p.allocatedVerts;
	vertices=new R_Vertex[allocatedVerts];
	for(i=0;i<numVerts;i++) vertices[i]=p.vertices[i];

	return *this;
}

R_Vertex& W_Poly::operator[](int n) 
{ 
	return vertices[(n+numVerts)%numVerts]; 
}

void W_Poly::Transform(Math::Matrix &m)
{
	int i;

	for(i=0;i<numVerts;i++) vertices[i].position=m*vertices[i].position;
}

void W_Poly::BuildPlane()
{
	plane.point=vertices[0].position;
	plane.normal=(vertices[1].position-vertices[0].position)%(vertices[2].position-vertices[1].position);
	plane.normal.Normalize();
}

Math::Plane W_Poly::GetPlane()
{
	return plane;
}

void W_Poly::SetPlane(Math::Vector normal)
{
	plane.point=vertices[0].position;
	plane.normal=normal;
}

R_Texture *W_Poly::GetTexture()
{
	return texture;
}

void W_Poly::SetTexture(R_Texture *t)
{
	texture=t;
}

int W_Poly::GetNumCels()
{
	return texture->NumCels();
}