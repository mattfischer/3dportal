#include "M_Matrix.h"

namespace Math
{
    Vector Matrix::operator*(Vector &a)
    {
	    Vector newVector;
	    float w;

	    newVector.x=m[0][0]*a.x+m[0][1]*a.y+m[0][2]*a.z+m[0][3];
	    newVector.y=m[1][0]*a.x+m[1][1]*a.y+m[1][2]*a.z+m[1][3];
	    newVector.z=m[2][0]*a.x+m[2][1]*a.y+m[2][2]*a.z+m[2][3];
	    w          =m[3][0]*a.x+m[3][1]*a.y+m[3][2]*a.z+m[3][3];

	    if(w!=1) 
	    {
		    newVector.x/=w;
		    newVector.y/=w;
		    newVector.z/=w;
	    }

	    return newVector;
    }

    Matrix::Matrix(float m0,float m1,float m2,float m3,
		       float m4,float m5,float m6,float m7,
		       float m8,float m9,float m10,float m11,
		       float m12,float m13,float m14,float m15)
    {
	    m[0][0]=m0;
	    m[0][1]=m1;
	    m[0][2]=m2;
	    m[0][3]=m3;
	    m[1][0]=m4;
	    m[1][1]=m5;
	    m[1][2]=m6;
	    m[1][3]=m7;
	    m[2][0]=m8;
	    m[2][1]=m9;
	    m[2][2]=m10;
	    m[2][3]=m11;
	    m[3][0]=m12;
	    m[3][1]=m13;
	    m[3][2]=m14;
	    m[3][3]=m15;
    }

    Matrix Matrix::operator*(Matrix &b)
    {
	    int i,j,k;
	    Matrix newMatrix;

	    for(i=0;i<4;i++)
		    for(j=0;j<4;j++)
		    {
			    newMatrix.m[i][j]=0;
			    for(k=0;k<4;k++)
				    newMatrix.m[i][j]+=m[i][k]*b.m[k][j];
		    }

	    return newMatrix;
    }
}
