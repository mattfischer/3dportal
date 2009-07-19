#include "W_Surface.h"
#include "W_Thing.h"
#include "W_Sector.h"

#include "R_Frame.h"
#include "R_Texture.h"

#include "JK_Level.h"
#include "JK_Flags.h"

#include <windows.h>
#include <gl/gl.h>

#define _USE_MATH_DEFINES
#include <math.h>

extern shared_ptr<World::Thing> player;

namespace World
{
    void Surface::Draw(Math::Vector tint, float light)
    {
	    int k;
	    int i;
	    int skySizeX, skySizeY;
	    float startXOffset, startYOffset;
	    float x,y;
	    Math::Vector position, rotation;
	    Math::Vector tempVector;
    	
	    if(geo==JK_GEO_NO_DRAW) return;
        if(flag == globalFlag) return;

	    position=player->GetEyePosition();
	    rotation=player->GetCompositeRotation();

        glEnable(GL_CULL_FACE);

	    if(flags&JK_SURFACE_CEILING_SKY)
	    {
		    Poly poly2(polygon);

		    for(i=0;i<poly2.NumVertices();i++)
		    {
			    poly2[i].position=poly2[i].position-position;
			    poly2[i].position=poly2[i].position*(currentLevel.ceilingSkyZ/(poly2[i].position*Math::Vector(0,0,1)));
			    poly2[i].position=poly2[i].position+position;

			    poly2[i].texture.u=currentLevel.ceilingSkyZ*(poly2[i].position-position).x;
			    poly2[i].texture.v=currentLevel.ceilingSkyZ*(poly2[i].position-position).y;
		    }
    				
		    poly2.Draw(tint, 1.0, false, false, 0, 0, 0);
		    return;
	    }

	    else if(flags&JK_SURFACE_HORIZON_SKY)
	    {
		    Poly poly2(polygon);
            poly2.Clip(Render::Frame::WindowFrustum);
            poly2.Transform(Render::Frame::totalTransformationMatrix);

		    glPushMatrix();
		    glLoadIdentity();
    			
		    skySizeX=2*currentLevel.horizonDistance*cos(FOV*M_PI/360);
		    skySizeY=skySizeX*Render::Frame::SY/Render::Frame::SX;
		    startXOffset=-(rotation.y*currentLevel.numPixelsPerRev)/360-currentLevel.horizonOffsetX;
		    startYOffset=polygon.GetTexture()->SizeY()-skySizeY-(rotation.x*currentLevel.numPixelsPerRev)/360-currentLevel.horizonOffsetY;
    		
		    for(k=0;k<poly2.NumVertices();k++)
		    {
			    x=(poly2[k].position.x+Render::Frame::SX)/(2*Render::Frame::SX);
			    y=-(poly2[k].position.y+Render::Frame::SY)/(2*Render::Frame::SY);
    						
			    poly2[k].texture.u=startXOffset+x*skySizeX;
			    poly2[k].texture.v=startYOffset+y*skySizeY;
    			
                //poly2[i].position.y *= -1;
			    poly2[k].position=poly2[k].position*100;
    			
		    }

		    poly2.Draw(tint, 1.0, false, false, 0, 0, 0);
		    glPopMatrix();
	    }
	    else
	    {
		    if(face&JK_FACE_TRANSLUCENT)
			    polygon.Draw(tint, light, true, false, cel, offsetU, offsetV);
		    else
			    polygon.Draw(tint, light, false, false, cel, offsetU, offsetV);
    	
	    }
	    Update();
        flag = globalFlag;
    }
}