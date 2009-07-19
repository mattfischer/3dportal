#include "W_Surface.h"

#include "R_Texture.h"

#include "C_Script.h"

#include "JK_Flags.h"
#include <windows.h>

namespace World
{
    Surface::Surface()
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

    bool Surface::Adjoined()
    {
	    return adjoined;
    }

    int Surface::GetFlags()
    {
	    return flags;
    }

    int Surface::GetGeo()
    {
	    return geo;
    }

    int Surface::GetFace()
    {
	    return face;
    }

    int Surface::GetAdjoinFlags()
    {
	    return adjoinFlags;
    }

    Poly Surface::GetPoly()
    {
	    return polygon;
    }

    Sector *Surface::GetAdjoin()
    {
	    return adjoin;
    }

    Surface *Surface::GetMirror()
    {
	    return mirror;
    }

    void Surface::AddCogLink( Cog::Script *cogScript )
    {
	    cogLinks.push_back( cogScript );
    }

    void Surface::SendCogMessages( const string& message, int source, bool synchronous )
    {
	    int i;

	    for( i = 0 ; i < cogLinks.size() ; i++ )
	    {
		    cogLinks[i]->Message( message, num, 6, source, synchronous );
	    }
    }

    void Surface::SetCel(int c)
    {
	    cel=c;
    }

    int Surface::GetCel()
    {
	    return cel;
    }

    void Surface::Animate(float fps, int flags)
    {
	    animate=true;
	    animateFPS=fps;
	    animateFlags=flags;
	    animateTimer=GetTickCount();
	    cel=0;
	    if(animateFlags&JK_SURFACE_ANIMATE_SKIP_LAST) cel=1;
	    if(animateFlags&JK_SURFACE_ANIMATE_SKIP_LAST_TWO) cel=2;
    }

    void Surface::StopAnimate()
    {
	    animate=false;
    }

    void Surface::Slide(Math::Vector direction, float speed)
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

    void Surface::Update()
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

    Math::Vector Surface::GetMoveDelta()
    {
	    return moveDelta;
    }
}