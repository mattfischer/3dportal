#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "W_Poly.h"
#include "W_Thing.h"

#include "R_Frame.h"
#include "R_Texture.h"

#include <math.h>

extern bool fullLight;
extern bool drawPolygons;
extern bool drawThings;

namespace World
{
    void Poly::Clip(Math::Plane &c)
    {
	    int i;
	    int v;
	    float ndotp;
	    int state, newstate;
	    Math::Vector newPoint;
	    float t;
	    Math::Vector a;
	    Math::Vector a1, a2;
	    Math::Vector p, n;
	    Math::Vector v1t, v2t, vt;

	    Render::Vertex v1,v2;
	    static Render::Vertex *newVertices=NULL;
	    static int numNewVerts=0;
    	
	    p=c.point;
	    n=c.normal;
	    ndotp=n*p;

	    if(numVerts==0) return;

	    if(newVertices==NULL)
	    {
		    newVertices=new Render::Vertex[numVerts+1];
		    numNewVerts=numVerts+1;
	    }
	    else if(numNewVerts<numVerts+1)
	    {
		    delete[] newVertices;
		    newVertices=new Render::Vertex[numVerts+1];
		    numNewVerts=numVerts+1;
	    }

	    if(vertices[numVerts-1].position*n<ndotp) state=0; else state=1;

	    v=0;
	    for(i=0;i<numVerts;i++)
	    {
		    newstate=state;
		    if(vertices[i].position*n < ndotp) 
			    newstate=0; else newstate=1;
    		
		    if((state==0 && newstate==1) || (state==1 && newstate==0))
		    {
			    v1=vertices[i];
			    v2=vertices[(i+numVerts-1)%numVerts];

			    a1=v1.position;
			    a2=v2.position;

			    a=a2-a1;

			    t=(ndotp-a1*n)/(a*n);
			    newPoint=a1+a*t;
			    newVertices[v].position=newPoint;
			    //v1t=m*v1.position;
			    //v2t=m*v2.position;
			    //vt=m*newPoint;

			    //t2=((vt.x-v1t.x)*(vt.x-v1t.x)+(vt.y-v1t.y)*(vt.y-v1t.y))/((v2t.x-v1t.x)*(v2t.x-v1t.x)+(v2t.y-v1t.y)*(v2t.y-v1t.y));

			    newVertices[v].intensity=v1.intensity+(v2.intensity-v1.intensity)*t;
			    newVertices[v].texture.u=v1.texture.u+(v2.texture.u-v1.texture.u)*t;
			    newVertices[v].texture.v=v1.texture.v+(v2.texture.v-v1.texture.v)*t;
			    v++;
		    }

		    if(newstate==1) newVertices[v++]=vertices[i];
		    state=newstate;
	    }

	    if(v>allocatedVerts)
	    {
		    delete[] vertices;
		    allocatedVerts*=2;
		    vertices=new Render::Vertex[allocatedVerts];
	    }
	    numVerts=v;
	    v=0;
	    for(i=0;i<numVerts;i++)
		    vertices[v++]=newVertices[i];
	    numVerts=v;
    }

    void Poly::Clip(Render::Frustum frustum)
    {
	    Math::Vector v0, v1, v2;
	    Math::Plane p0, p1, p2, p3, p4, p5, p6, p7;
	    int i;

	    v0=Render::Frame::worldviewInverseMatrix*Math::Vector(0,0,0);
    /*
	    for(i=0;i<frustum.numPlanes;i++)
	    {
		    p0.point=v0;
		    p0.normal=rotationInverseMatrix*frustum.planeNormals[i];
		    Clip(p0);
	    }*/

	    v1=Math::Vector(frustum.x0, 1, frustum.y0);
	    v2=Math::Vector(frustum.x0, 1, frustum.y1);
    	
	    p0.point=v0;
	    p0.normal=v1%v2;
	    p0.normal.Normalize();
	    p0.normal=Render::Frame::rotationInverseMatrix*p0.normal;
    	
	    v1=Math::Vector(frustum.x0, 1, frustum.y1);
	    v2=Math::Vector(frustum.x1, 1, frustum.y1);
    	
	    p1.point=v0;
	    p1.normal=v1%v2;
	    p1.normal.Normalize();
	    p1.normal=Render::Frame::rotationInverseMatrix*p1.normal;
    	
	    v1=Math::Vector(frustum.x1, 1, frustum.y1);
	    v2=Math::Vector(frustum.x1, 1, frustum.y0);
    	
	    p2.point=v0;
	    p2.normal=v1%v2;
	    p2.normal.Normalize();
	    p2.normal=Render::Frame::rotationInverseMatrix*p2.normal;
    	
	    v1=Math::Vector(frustum.x1, 1, frustum.y0);
	    v2=Math::Vector(frustum.x0, 1, frustum.y0);
    	
	    p3.point=v0;
	    p3.normal=v1%v2;
	    p3.normal.Normalize();
	    p3.normal=Render::Frame::rotationInverseMatrix*p3.normal;
    	
	    Clip(p0);
	    Clip(p1);
	    Clip(p2);
	    Clip(p3);
    	
    	
	    if(frustum.x00d>frustum.x0)
	    {
		    v1=Math::Vector(frustum.x00d, 1, frustum.y0);
            v2=Math::Vector(frustum.x0, 1, (frustum.y0+frustum.x00d-frustum.x0));
    		
		    p4.point=v0;
		    p4.normal=v1%v2;
		    p4.normal.Normalize();
		    p4.normal=Render::Frame::rotationInverseMatrix*p4.normal;

		    Clip(p4);
	    }

	    if(frustum.x01d>frustum.x0)
	    {
            v1=Math::Vector(frustum.x0, 1, (frustum.y1-(frustum.x01d-frustum.x0)));
            v2=Math::Vector(frustum.x01d, 1, frustum.y1);
    		
		    p5.point=v0;
		    p5.normal=v1%v2;
		    p5.normal.Normalize();
		    p5.normal=Render::Frame::rotationInverseMatrix*p5.normal;

		    Clip(p5);
	    }

	    if(frustum.x11d<frustum.x1)
	    {
            v1=Math::Vector(frustum.x11d, 1, frustum.y1);
		    v2=Math::Vector(frustum.x1, 1, (frustum.y1-(frustum.x1-frustum.x11d)));
    		
		    p6.point=v0;
		    p6.normal=v1%v2;
		    p6.normal.Normalize();
		    p6.normal=Render::Frame::rotationInverseMatrix*p6.normal;

		    Clip(p6);
	    }

	    if(frustum.x10d<frustum.x1)
	    {
            v1=Math::Vector(frustum.x1, 1, (frustum.y0+frustum.x1-frustum.x10d));
            v2=Math::Vector(frustum.x10d, 1, frustum.y0);
    		
		    p7.point=v0;
		    p7.normal=v1%v2;
		    p7.normal.Normalize();
		    p7.normal=Render::Frame::rotationInverseMatrix*p7.normal;

		    Clip(p7);
	    }
    }

    Render::Frustum Poly::CreateFrustum() const
    {
	    Poly newPoly(*this);
	    Render::Frustum frustum;
	    int i;
    	
	    newPoly.Transform(Render::Frame::totalTransformationMatrix);
    	
	    frustum.x0=frustum.x1=newPoly[0].position.x;
	    frustum.y0=frustum.y1=newPoly[0].position.y;
        
	    for(i=1;i<newPoly.NumVertices();i++)
	    {
		    if(newPoly[i].position.x<frustum.x0) frustum.x0=newPoly[i].position.x;
		    if(newPoly[i].position.x>frustum.x1) frustum.x1=newPoly[i].position.x;
		    if(newPoly[i].position.y<frustum.y0) frustum.y0=newPoly[i].position.y;
		    if(newPoly[i].position.y>frustum.y1) frustum.y1=newPoly[i].position.y;
	    }

	    frustum.x00d=frustum.x1;
	    frustum.x10d=frustum.x0;
	    frustum.x01d=frustum.x1;
	    frustum.x11d=frustum.x0;

	    for(i=0;i<newPoly.NumVertices();i++)
	    {
		    if(newPoly[i].position.x+(newPoly[i].position.y-frustum.y0)<frustum.x00d)
			    frustum.x00d=newPoly[i].position.x+(newPoly[i].position.y-frustum.y0)-.001;
		    if(newPoly[i].position.x-(newPoly[i].position.y-frustum.y0)>frustum.x10d)
			    frustum.x10d=newPoly[i].position.x-(newPoly[i].position.y-frustum.y0)+.001;
		    if(newPoly[i].position.x+(frustum.y1-newPoly[i].position.y)<frustum.x01d)
			    frustum.x01d=newPoly[i].position.x+(frustum.y1-newPoly[i].position.y)-.001;
		    if(newPoly[i].position.x-(frustum.y1-newPoly[i].position.y)>frustum.x11d)
			    frustum.x11d=newPoly[i].position.x-(frustum.y1-newPoly[i].position.y)+.001;
	    }
    /*
	    frustum.numPlanes=newPoly.NumVertices();
	    frustum.planeNormals=new Math::Vector[frustum.numPlanes];

	    for(i=0;i<newPoly.NumVertices();i++)
		    frustum.planeNormals[i]=newPoly[i+1].position%newPoly[i].position;
    */
	    return frustum;
    }

    void Poly::Draw(Math::Vector tint, float light, bool translucent, bool cullReverse, int cel, float offsetU, float offsetV) const
    {
	    int k;
    	
	    if(numVerts<=0) return;

	    if(cel>=texture->NumCels()) cel=texture->NumCels()-1;

	    if(texture)
		    texture->Select(0, cel);
	    else
		    glBindTexture(GL_TEXTURE_2D, 0);
    	
	    if(translucent || (texture && texture->NeedsBlending())) glEnable(GL_BLEND);
	    else glDisable(GL_BLEND);

	    if(cullReverse) glEnable(GL_CULL_FACE);
	    else glDisable(GL_CULL_FACE);

	    glNormal3f(plane.normal.x,plane.normal.y,plane.normal.z);
    	
	    if(fullLight)
	    {
		    glBegin(GL_POLYGON);
		    for(k=0;k<numVerts;k++)
		    {
			    glTexCoord2f((vertices[k].texture.u+offsetU)/texture->SizeX(), (vertices[k].texture.v+offsetV)/texture->SizeY());
			    glColor3f(1.0, 1.0, 1.0);
			    glVertex3f(vertices[k].position.x, vertices[k].position.y, vertices[k].position.z);
		    }
		    glEnd();
	    }
	    else if(translucent)
	    {
		    glBegin(GL_POLYGON);
		    for(k=0;k<numVerts;k++)
		    {
			    glTexCoord2f((vertices[k].texture.u+offsetU)/texture->SizeX(), (vertices[k].texture.v+offsetV)/texture->SizeY());
			    glColor4f(vertices[k].intensity+light, vertices[k].intensity+light, vertices[k].intensity+light, 0.25);
			    glVertex3f(vertices[k].position.x, vertices[k].position.y, vertices[k].position.z);
		    }
		    glEnd();
	    }
	    else
	    {
		    glBegin(GL_POLYGON);
		    for(k=0;k<numVerts;k++)
		    {
			    glTexCoord2f((vertices[k].texture.u+offsetU)/texture->SizeX(), (vertices[k].texture.v+offsetV)/texture->SizeY());
			    glColor3f(vertices[k].intensity+light, vertices[k].intensity+light, vertices[k].intensity+light);
			    glVertex3f(vertices[k].position.x, vertices[k].position.y, vertices[k].position.z);
		    }
		    glEnd();
	    }
    }
}