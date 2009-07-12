#include "W_Surface.h"
#include "W_Sector.h"
#include "W_Thing.h"

#include "M_Vector.h"
#include "M_Plane.h"

#include "JK_Flags.h"

extern bool doThingCollisions;
extern shared_ptr<W_Thing> player;

bool W_Surface::PerformActivate( M_Vector position, M_Vector point )
{
	M_Plane plane;
	float planeD, newD;
	float t;
	M_Vector newPos;

	plane = polygon.GetPlane();
	
	if( point * plane.normal == 0 ) return false;
	newD = plane.normal.ScalarProjection( position );
	planeD = plane.normal.ScalarProjection( plane.point );

	t = -( newD - planeD ) / ( point * plane.normal );
	if( t<0 || t>.1 ) return false;
	newPos = position + point * t;
	if( polygon.VectorIn( newPos ) )
	{
		if( adjoined )
		{
			if( adjoin->PerformActivate( position, point, mirror ) ) return true;
		}
		else 
		{
			SendCogMessages( "activated", player->GetNum() );
			return true;
		}
	}
	return false;
}

void W_Surface::SurfaceCollisions( W_Thing* thing )
{
	M_Vector offset;
	M_Vector normalVel;
	M_Plane plane;
	bool direct;

	offset = polygon.SpherePlaneOffset( thing->GetPosition(), thing->GetMoveSize(), direct );
	if( offset.Magnitude() > 0 )
	{
		if( !adjoined || ( doThingCollisions && flags&JK_SURFACE_IMPASSABLE ) )
		{
			plane = polygon.GetPlane();

			thing->SetPosition( thing->GetPosition() + offset );
			if( direct )
				thing->ArrestMotion( plane.normal );

			if( thing->GetType() == W_Thing::WEAPON && ( thing->GetTypeFlags() & JK_WEAPON_EXPLODE_SURFACE ) )
			{
				thing->Explode();
			}
		}
		else
			adjoin->SurfaceCollisions( thing, mirror );
	}
}

bool W_Surface::FloorCollisions( W_Thing* thing )
{
	float newD, planeD, centerD;
	float t;
	M_Vector newPos;
	float offset;
	M_Vector floorPos;
	M_Plane plane;

	floorPos = thing->GetPosition() - thing->GetStandVector();
	plane = polygon.GetPlane();
	
	if( plane.normal.z <= 0 ) return false;

	newD = plane.normal.ScalarProjection( floorPos );
	planeD = plane.normal.ScalarProjection( plane.point );
	centerD = plane.normal.ScalarProjection( thing->GetPosition() );

	t = ( planeD - newD ) / ( M_Vector( 0, 0, 1 ) * plane.normal );
	newPos = floorPos + M_Vector( 0, 0, t );

	if( thing->WasAttached() ) offset = thing->GetStandVector().z;
	else offset = 0.01;

	if( newD <= planeD + offset )
	{
		if( polygon.VectorIn( newPos ) )
		{
			if( !adjoined || (doThingCollisions && flags&JK_SURFACE_IMPASSABLE ) )
			{
				if( flags & JK_SURFACE_FLOOR )
				{
					if( centerD < planeD )
					{
						thing->SetPosition( thing->GetPosition() + M_Vector( 0, 0, ( planeD - centerD ) / plane.normal.z ) );
						thing->Nudge( M_Vector( 0, 0, ( centerD - newD ) / plane.normal.z ) );
					}
					else
						thing->Nudge( M_Vector( 0, 0, ( planeD - newD ) / plane.normal.z ) );
				thing->SetAttached( this );
				thing->ArrestMotion( plane.normal );
				return true;
				}
			}
			else
				return adjoin->FloorCollisions( thing, mirror );
		}
	}
	return false;
}