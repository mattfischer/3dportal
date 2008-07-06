#include "W_Surface.h"
#include "W_Thing.h"
#include "W_Sector.h"

#include "R_Frame.h"
#include "R_Texture.h"

#include "JK_Level.h"
#include "JK_Flags.h"

#include <windows.h>
#include <gl/gl.h>
#include <math.h>

extern shared_ptr<W_Thing> player;

void W_Surface::Draw(M_Vector tint, R_Frustum frustum, float light)
{
	int k;
	int i;
	int skySizeX, skySizeY;
	float startXOffset, startYOffset;
	float x,y;
	M_Vector position, rotation;
	M_Vector tempVector;
	
	if(geo==JK_GEO_NO_DRAW) return;


	position=player->GetEyePosition();
	rotation=player->GetCompositeRotation();

	if(flags&JK_SURFACE_CEILING_SKY)
	{
		W_Poly poly2(polygon);

		for(i=0;i<poly2.NumVertices();i++)
		{
			poly2[i].position=poly2[i].position-position;
			poly2[i].position=poly2[i].position*(currentLevel.ceilingSkyZ/(poly2[i].position*M_Vector(0,0,1)));
			poly2[i].position=poly2[i].position+position;

			poly2[i].texture.u=currentLevel.ceilingSkyZ*(poly2[i].position-position).x;
			poly2[i].texture.v=currentLevel.ceilingSkyZ*(poly2[i].position-position).y;
		}
				
		poly2.Draw(tint, 1.0, false, false, 0, 0, 0);
		return;
	}

	else if(flags&JK_SURFACE_HORIZON_SKY)
	{
		W_Poly poly2(polygon);
		poly2.Clip(frustum);
		poly2.Transform(totalTransformationMatrix);

		R_Frame_DisableClipPlanes();
		glPushMatrix();
		glLoadIdentity();
			
		skySizeX=2*currentLevel.horizonDistance*cos(FOV*3.14/360);
		skySizeY=skySizeX*SY/SX;
		startXOffset=-(rotation.y*currentLevel.numPixelsPerRev)/360-currentLevel.horizonOffsetX;
		startYOffset=polygon.GetTexture()->SizeY()-skySizeY-(rotation.x*currentLevel.numPixelsPerRev)/360-currentLevel.horizonOffsetY;
		
		for(k=0;k<poly2.NumVertices();k++)
		{
			x=(poly2[k].position.x+SX)/(2*SX);
			y=(-poly2[k].position.y+SY)/(2*SY);
						
			poly2[k].texture.u=startXOffset+x*skySizeX;
			poly2[k].texture.v=startYOffset+y*skySizeY;
			
			tempVector=poly2[k].position;
			poly2[k].position.z=tempVector.y;
			poly2[k].position.y=-tempVector.z;
			poly2[k].position=poly2[k].position*100;
			
		}

		poly2.Draw(tint, 1.0, false, false, 0, 0, 0);
		glPopMatrix();
		R_Frame_SetupClipPlanes(frustum);
	}
	else
	{
		if(face&JK_FACE_TRANSLUCENT)
			polygon.Draw(tint, light, true, false, cel, offsetU, offsetV);
		else
			polygon.Draw(tint, light, false, false, cel, offsetU, offsetV);
	
	}
	Update();
}