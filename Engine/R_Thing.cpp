#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "W_Sector.h"
#include "W_Thing.h"

#include "R_Frame.h"
#include "R_Texture.h"
#include "R_Model.h"

#include "JK_Level.h"

extern shared_ptr<World::Thing> player;

extern bool drawThings;

namespace World
{
    bool SphereInFrustum(Math::Vector position, float radius, Render::Frustum frustum)
    {
	    Math::Vector normal;
    	
	    position=Render::worldviewMatrix*position;

	    normal=Math::Vector(1, 0, frustum.x0);
	    normal.Normalize();
	    if(normal*position<-radius) return false;

	    normal=Math::Vector(-1, 0, -frustum.x1);
	    normal.Normalize();
	    if(normal*position<-radius) return false;

	    normal=Math::Vector(0, 1, frustum.y0);
	    normal.Normalize();
	    if(normal*position<-radius) return false;

	    normal=Math::Vector(0, -1, -frustum.y1);
	    normal.Normalize();
	    if(normal*position<-radius) return false;

	    if(frustum.x00d>frustum.x0)
	    {
		    normal=Math::Vector(frustum.x00d, frustum.y0, -1)%Math::Vector(frustum.x0, frustum.y0+frustum.x00d-frustum.x0, -1);
		    normal.Normalize();
		    if(normal*position<-radius) return false;
	    }

	    if(frustum.x01d>frustum.x0)
	    {
		    normal=Math::Vector(frustum.x0, frustum.y1-(frustum.x01d-frustum.x0), -1)%Math::Vector(frustum.x01d, frustum.y1, -1);
		    normal.Normalize();
		    if(normal*position<-radius) return false;
	    }

	    if(frustum.x11d<frustum.x1)
	    {
		    normal=Math::Vector(frustum.x11d, frustum.y1, -1)%Math::Vector(frustum.x1, frustum.y1-(frustum.x1-frustum.x11d), -1);
		    normal.Normalize();
		    if(normal*position<-radius) return false;
	    }

	    if(frustum.x10d<frustum.x1)
	    {
		    normal=Math::Vector(frustum.x1, frustum.y0+frustum.x1-frustum.x10d, -1)%Math::Vector(frustum.x10d, frustum.y0, -1);
		    normal.Normalize();
		    if(normal*position<-radius) return false;
	    }

	    return true;
    }
    	
    void Thing::Draw( Render::Frustum frustum, float light, Math::Vector tint )
    {
	    float distance2;
	    Math::Vector compositeRotation;

	    if( flagValue == globalFlag ) return;
	    if( player.get() == this) return;

	    if( model && !SphereInFrustum( position, model->GetRadius(), frustum ) ) return;

	    distance2 = ( player->GetPosition() - position ).Magnitude2();

	    compositeRotation = orient + rotation;
    	
	    glPushMatrix();
	    glTranslatef( position.x, position.z, -position.y );

	    glPushMatrix();

	    glRotatef( player->GetRotation().y, 0, 1, 0 );
	    glRotatef( player->GetRotation().x, 1, 0, 0 );
	    glRotatef( player->GetRotation().z, 0, 0, 1 );
    	
	    if( sprite )
		    sprite->Draw( spriteTime * 15 );

	    glPopMatrix();

        glRotatef( compositeRotation.y, 0, 1, 0 );
	    glRotatef( compositeRotation.x, 1, 0, 0 );
	    glRotatef( compositeRotation.z, 0, 0, 1 );
    		
	    if( model )
		    model->Draw( distance2, light, tint, &keyTrack );

	    glPopMatrix();
    	
	    flagValue = globalFlag;
    }
}