#include "C_Verb.h"

#include "JK_Level.h"
#include "W_Surface.h"

C_Type_Int SetWallCel( C_Type_Surface surface_num, C_Type_Int cel )
{
	int oldCel = currentLevel.surfaces[surface_num].GetCel(); // ??????
	currentLevel.surfaces[surface_num].SetCel( cel );
	return oldCel;
}

C_Type_Int GetWallCel( C_Type_Surface surface_num )
{
	return currentLevel.surfaces[surface_num].GetCel();
}

C_Type_Vector GetSurfaceCenter( C_Type_Surface surface_num )
{
	return currentLevel.surfaces[surface_num].GetPoly()[0].position;
}

C_Type_Vector SurfaceCenter( C_Type_Surface surface_num )
{
	return GetSurfaceCenter( surface_num );
}

C_Type_Void SurfaceAnim( C_Type_Surface surface_num, C_Type_Flex fps, C_Type_Int flags )
{
	currentLevel.surfaces[surface_num].Animate( fps, flags );
}

C_Type_Void StopSurfaceAnim( C_Type_Surface surface_num )
{
	currentLevel.surfaces[surface_num].StopAnimate();
}

C_Type_Void SlideWall( C_Type_Surface surface_num, C_Type_Vector direction, C_Type_Flex speed )
{
	currentLevel.surfaces[surface_num].Slide( direction, speed );
}

