#include "W_Sector.h"
#include "W_Surface.h"
#include "W_Thing.h"

#include "R_Frame.h"
#include "R_Model.h"

#include "JK_Flags.h"
#include "JK_Level.h"

#include <gl/gl.h>
#include <gl/glu.h>

extern bool updateThings;
extern bool drawThings;

namespace World
{
    void Sector::Draw( Render::Frustum frustum, Surface *ignore )
    {
	    int i;
	    Render::Frustum newFrustum;
	    Poly clipped;
	    Math::Plane plane;
	    shared_ptr<Thing> thing;
    	
	    if( drawing ) return;
    	
	    drawing = true;
    	
	    for( i = 0 ; i < numSurfaces ; i++ )
	    {
		    if( ignore != NULL && surfaces[i] == ignore ) continue;
    	
		    plane = surfaces[i]->GetPoly().GetPlane();
		    if( (plane.point - currentLevel.player->GetEyePosition() ) * plane.normal > 0) continue;

		    if( surfaces[i]->Adjoined() )
		    {
			    if( renderAdjoins && surfaces[i]->GetAdjoinFlags() & JK_ADJOIN_RENDER_PAST )
			    {
				    clipped = surfaces[i]->GetPoly();
				    clipped.Clip( frustum );
    				
				    if( clipped.NumVertices() == 0 ) continue;
    					
				    newFrustum = clipped.CreateFrustum();

				    if( newFrustum.x0 >= newFrustum.x1 || newFrustum.y0 >= newFrustum.y1 )
					    continue;

    				surfaces[i]->GetAdjoin()->Draw( newFrustum, surfaces[i]->GetMirror() );
			    }
			    surfaces[i]->Draw( tint, extraLight );
		    }
		    else
			    surfaces[i]->Draw( tint, extraLight );
	    }
    	
	    drawing = false;	

	    if( drawThings )
	    {
		    if( things.size() > 0 )
		    {
			    for( i = 0 ; i < things.size() ; i++ )
			    {
                    things[i]->Draw( frustum, extraLight + ambientLight, tint );
			    }
		    }
	    }
    }
}