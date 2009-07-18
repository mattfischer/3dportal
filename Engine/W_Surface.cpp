#include "W_Surface.h"

#include "R_Texture.h"

#include "C_Script.h"

#include "JK_Flags.h"
#include <windows.h>

W_Surface::W_Surface()
{
	cel = 0;
	animate = false;

	offsetU = offsetV = 0;
	slideU = 0;
	slideV = 0;
    flag = 0;
	moveDelta = Math::Vector(0,0,0);
	moveVector = Math::Vector(0,0,0);
}

bool W_Surface::Adjoined()
{
	return adjoined;
}

int W_Surface::GetFlags()
{
	return flags;
}

int W_Surface::GetGeo()
{
	return geo;
}

int W_Surface::GetFace()
{
	return face;
}

int W_Surface::GetAdjoinFlags()
{
	return adjoinFlags;
}

W_Poly W_Surface::GetPoly()
{
	return polygon;
}

W_Sector *W_Surface::GetAdjoin()
{
	return adjoin;
}

W_Surface *W_Surface::GetMirror()
{
	return mirror;
}

void W_Surface::AddCogLink( C_Script *cogScript )
{
	cogLinks.push_back( cogScript );
}

void W_Surface::SendCogMessages( const string& message, int source, bool synchronous )
{
	int i;

	for( i = 0 ; i < cogLinks.size() ; i++ )
	{
		cogLinks[i]->Message( message, num, 6, source, synchronous );
	}
}

void W_Surface::SetCel(int c)
{
	cel=c;
}

int W_Surface::GetCel()
{
	return cel;
}

void W_Surface::Animate(float fps, int flags)
{
	animate=true;
	animateFPS=fps;
	animateFlags=flags;
	animateTimer=GetTickCount();
	cel=0;
	if(animateFlags&JK_SURFACE_ANIMATE_SKIP_LAST) cel=1;
	if(animateFlags&JK_SURFACE_ANIMATE_SKIP_LAST_TWO) cel=2;
}

void W_Surface::StopAnimate()
{
	animate=false;
}

void W_Surface::Slide(Math::Vector direction, float speed)
{
	Math::Vector x, x2;
	float u,v;
	float distance;

	x=polygon[1].position-polygon[0].position;
	distance=x.Magnitude();
	x.Normalize();
	x2=polygon.GetPlane().normal%x;

	u=polygon[1].texture.u-polygon[0].texture.u;
	v=polygon[1].texture.v-polygon[0].texture.v;

	u/=distance;
	v/=distance;

	slideU=u*(direction*x)+v*(direction*x2);
	slideV=-v*(direction*x)-u*(direction*x2);

	slideU*=speed/10;
	slideV*=speed/10;
	
	slideTimer=GetTickCount();
	moveVector=direction*speed/10;
}

void W_Surface::Update()
{
	int startFrame;
	long timeDifference;

	startFrame=0;
	if(animateFlags&JK_SURFACE_ANIMATE_SKIP_LAST) startFrame=1;
	if(animateFlags&JK_SURFACE_ANIMATE_SKIP_LAST_TWO) startFrame=2;

	timeDifference=GetTickCount()-slideTimer;

	offsetU+=slideU*timeDifference/1000;
	offsetV+=slideV*timeDifference/1000;

	slideTimer=GetTickCount();
	
	moveDelta=moveVector*timeDifference/1000;
	if(!animate) return;

	if(GetTickCount()>animateTimer+1000/animateFPS)
	{
		animateTimer=GetTickCount();
		if(cel<polygon.GetNumCels()) cel++;
		else
			if(animateFlags&JK_SURFACE_ANIMATE_LOOP) cel=startFrame;
			else animate=false;
	}
}

Math::Vector W_Surface::GetMoveDelta()
{
	return moveDelta;
}