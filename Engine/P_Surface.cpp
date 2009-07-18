#include "W_Surface.h"
#include "W_Sector.h"
#include "W_Thing.h"

#include "M_Vector.h"
#include "M_Plane.h"

#include "JK_Flags.h"

extern bool doThingCollisions;
extern shared_ptr<W_Thing> player;

bool W_Surface::PerformActivate( Math::Vector position, Math::Vector point )
{
	Math::Plane plane;
	float planeD, newD;
	float t;
	Math::Vector newPos;

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
	Math::Vector offset;
	Math::Vector normalVel;
	Math::Plane plane;
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
	Math::Vector newPos;
	float offset;
	Math::Vector floorPos;
	Math::Plane plane;

	floorPos = thing->GetPosition() - thing->GetStandVector();
	plane = polygon.GetPlane();
	
	if( plane.normal.z <= 0 ) return false;

	newD = plane.normal.ScalarProjection( floorPos );
	planeD = plane.normal.ScalarProjection( plane.point );
	centerD = plane.normal.ScalarProjection( thing->GetPosition() );

	t = ( planeD - newD ) / ( Math::Vector( 0, 0, 1 ) * plane.normal );
	newPos = floorPos + Math::Vector( 0, 0, t );

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
						thing->SetPosition( thing->GetPosition() + Math::Vector( 0, 0, ( planeD - centerD ) / plane.normal.z ) );
						thing->Nudge( Math::Vector( 0, 0, ( centerD - newD ) / plane.normal.z ) );
					}
					else
						thing->Nudge( Math::Vector( 0, 0, ( planeD - newD ) / plane.normal.z ) );
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