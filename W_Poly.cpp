#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "W_Poly.h"
#include "W_Thing.h"

#include "R_Frame.h"
#include "R_Texture.h"

#include <math.h>

extern int fullLight;

namespace World
{
    Poly::Poly(int n, int a) 
    {
	    numVerts=n;
	    allocatedVerts=n+a;
	    vertices=new Render::Vertex[allocatedVerts];
    }

    Poly::~Poly() 
    { 
	    if(allocatedVerts>0) delete[] vertices; 
    }

    Poly::Poly(const Poly &p)
    {
	    int i;

	    texture=p.texture;
	    plane=p.plane;
	    numVerts=p.numVerts;
	    allocatedVerts=p.allocatedVerts;
	    vertices=new Render::Vertex[allocatedVerts];
	    for(i=0;i<numVerts;i++) vertices[i]=p.vertices[i];
    }

    Poly &Poly::operator=(const Poly &p)
    {
	    int i;

	    if(allocatedVerts>0) delete[] vertices;

	    texture=p.texture;
	    plane=p.plane;
	    numVerts=p.numVerts;
	    allocatedVerts=p.allocatedVerts;
	    vertices=new Render::Vertex[allocatedVerts];
	    for(i=0;i<numVerts;i++) vertices[i]=p.vertices[i];

	    return *this;
    }

    Render::Vertex& Poly::operator[](int n) 
    { 
	    return vertices[(n+numVerts)%numVerts]; 
    }

	const Render::Vertex& Poly::operator[](int n) const
    { 
	    return vertices[(n+numVerts)%numVerts]; 
    }

    void Poly::Transform(Math::Matrix &m)
    {
	    int i;

	    for(i=0;i<numVerts;i++) vertices[i].position=m*vertices[i].position;
    }

    void Poly::BuildPlane()
    {
	    plane.point=vertices[0].position;
	    plane.normal=(vertices[1].position-vertices[0].position)%(vertices[2].position-vertices[1].position);
	    plane.normal.Normalize();
    }

    Math::Plane Poly::GetPlane() const
    {
	    return plane;
    }

    void Poly::SetPlane(Math::Vector normal)
    {
	    plane.point=vertices[0].position;
	    plane.normal=normal;
    }

    Render::Texture *Poly::GetTexture() const
    {
	    return texture;
    }

    void Poly::SetTexture(Render::Texture *t)
    {
	    texture=t;
    }

    int Poly::GetNumCels() const
    {
	    return texture->NumCels();
    }
}