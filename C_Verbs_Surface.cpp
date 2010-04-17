#include "C_Verb.h"

#include "JK_Level.h"
#include "W_Surface.h"

namespace Cog
{
    Type_Int SetWallCel( Type_Surface surface_num, Type_Int cel )
    {
	    int oldCel = currentLevel.surfaces[surface_num].GetCel(); // ??????
	    currentLevel.surfaces[surface_num].SetCel( cel );
	    return oldCel;
    }

    Type_Int GetWallCel( Type_Surface surface_num )
    {
	    return currentLevel.surfaces[surface_num].GetCel();
    }

    Type_Vector GetSurfaceCenter( Type_Surface surface_num )
    {
	    return currentLevel.surfaces[surface_num].GetPoly()[0].position;
    }

    Type_Vector SurfaceCenter( Type_Surface surface_num )
    {
	    return GetSurfaceCenter( surface_num );
    }

    Type_Void SurfaceAnim( Type_Surface surface_num, Type_Flex fps, Type_Int flags )
    {
	    currentLevel.surfaces[surface_num].Animate( fps, flags );
    }

    Type_Void StopSurfaceAnim( Type_Surface surface_num )
    {
	    currentLevel.surfaces[surface_num].StopAnimate();
    }

    Type_Void SlideWall( Type_Surface surface_num, Type_Vector direction, Type_Flex speed )
    {
	    currentLevel.surfaces[surface_num].Slide( direction, speed );
    }
}
