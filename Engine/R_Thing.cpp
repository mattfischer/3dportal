#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "W_Sector.h"
#include "W_Thing.h"

#include "R_Frame.h"
#include "R_Texture.h"
#include "R_Model.h"

#include "JK_Level.h"

extern shared_ptr<W_Thing> player;

extern bool drawThings;

bool SphereInFrustum(M_Vector position, float radius, R_Frustum frustum)
{
	M_Vector normal;
	
	position=worldviewMatrix*position;

	normal=M_Vector(1, 0, frustum.x0);
	normal.Normalize();
	if(normal*position<-radius) return false;

	normal=M_Vector(-1, 0, -frustum.x1);
	normal.Normalize();
	if(normal*position<-radius) return false;

	normal=M_Vector(0, 1, frustum.y0);
	normal.Normalize();
	if(normal*position<-radius) return false;

	normal=M_Vector(0, -1, -frustum.y1);
	normal.Normalize();
	if(normal*position<-radius) return false;

	if(frustum.x00d>frustum.x0)
	{
		normal=M_Vector(frustum.x00d, frustum.y0, -1)%M_Vector(frustum.x0, frustum.y0+frustum.x00d-frustum.x0, -1);
		normal.Normalize();
		if(normal*position<-radius) return false;
	}

	if(frustum.x01d>frustum.x0)
	{
		normal=M_Vector(frustum.x0, frustum.y1-(frustum.x01d-frustum.x0), -1)%M_Vector(frustum.x01d, frustum.y1, -1);
		normal.Normalize();
		if(normal*position<-radius) return false;
	}

	if(frustum.x11d<frustum.x1)
	{
		normal=M_Vector(frustum.x11d, frustum.y1, -1)%M_Vector(frustum.x1, frustum.y1-(frustum.x1-frustum.x11d), -1);
		normal.Normalize();
		if(normal*position<-radius) return false;
	}

	if(frustum.x10d<frustum.x1)
	{
		normal=M_Vector(frustum.x1, frustum.y0+frustum.x1-frustum.x10d, -1)%M_Vector(frustum.x10d, frustum.y0, -1);
		normal.Normalize();
		if(normal*position<-radius) return false;
	}

	return true;
}
	
void W_Thing::Draw( R_Frustum frustum, float light, M_Vector tint )
{
	float distance2;
	M_Vector compositeRotation;

	if( flagValue == globalFlag ) return;
	if( player.get() == this) return;

	if( model && !SphereInFrustum( position, model->GetRadius(), frustum ) ) return;

	distance2 = ( player->GetPosition() - position ).Magnitude2();

	compositeRotation = orient + rotation;
	
	glPushMatrix();
	glTranslatef( position.x, position.z, -position.y );
	glRotatef( compositeRotation.x, 1, 0, 0 );
	glRotatef( compositeRotation.y, 0, 1, 0 );
	glRotatef( compositeRotation.z, 0, 0, -1 );
		
	if( model )
		model->Draw( distance2, light, tint, &keyInstance );

	if( sprite )
		sprite->Draw();
	
	glPopMatrix();
	
	flagValue = globalFlag;
}