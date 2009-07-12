#include "W_Poly.h"

int W_Poly::VectorIn(M_Vector &v)
{
	int i;
	M_Vector line;
	M_Vector normal;

	for(i=0;i<numVerts;i++)
	{
		line=vertices[(i+1)%numVerts].position-vertices[i].position;
		normal=plane.normal%line;

		if((v-vertices[i].position)*normal<0) return 0;
	}

	return 1;
}

int W_Poly::InFrontOfPlane(M_Plane &c)
{
	float ndotp;
	M_Vector p, n;
	int i;

	p=c.point;
	n=c.normal;
	ndotp=n*p;

	for(i=0;i<numVerts;i++) if(vertices[i].position*n>ndotp) return 1;

	return 0;
}

M_Vector W_Poly::SpherePlaneOffset(M_Vector position, float radius, bool &direct)
{
	float planeD, posD;
	int i;
	M_Vector line, point;
	M_Vector lineNorm;
	float distance;
	float lineMag;
	float t;
	M_Vector normal;
	M_Vector result;

	planeD=plane.point*plane.normal;
	posD=position*plane.normal;

	direct=true;
	if(posD>planeD+radius) return M_Vector(0,0,0);

	if(!VectorIn(position))
	{
		direct=false;
		for(i=0;i<numVerts;i++)
		{
			point=vertices[i].position;
			line=vertices[(i+1)%numVerts].position-vertices[i].position;
			lineMag=line.Magnitude();
			lineNorm=line/lineMag;
			distance=((position-point)%lineNorm).Magnitude();
			if(distance>radius) continue;
			t=lineNorm*(position-point);
			if(t<0 || t>lineMag) continue;
			result=(position-point)-lineNorm*t;
			result.Normalize();
			result=result*(radius-distance);
			return result;
		}
		for(i=0;i<numVerts;i++)
		{
			if((vertices[i].position-position).Magnitude2()<=radius*radius)
			{
				result=position-vertices[i].position;
				distance=result.Magnitude();
				result.Normalize();
				result=result*(radius-distance);
				return result;
			}
		}
		return M_Vector(0,0,0);
	}

	return plane.normal*(planeD-posD+radius);
}