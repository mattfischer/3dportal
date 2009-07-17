#include "W_Sector.h"
#include "W_Thing.h"
#include "W_Surface.h"

#include "M_Plane.h"

#include "R_Model.h"

#include "JK_Flags.h"
#include "JK_Level.h"

#include "S_Manager.h"

extern shared_ptr<W_Thing> player;

bool W_Sector::UpdateThingSector( W_Thing* thing, M_Vector oldPosition )
{
	float newD, planeD, oldD;
	M_Vector planePoint;
	int i;
	M_Plane plane;
	W_Sector *oldSector;
	M_Vector delta;
	M_Vector intersection;

	delta = thing->GetEyePosition() - oldPosition;
	delta.Normalize();

	for( i = 0 ; i < numSurfaces ; i++ )
	{
		plane = surfaces[i]->GetPoly().GetPlane();
		
		planeD = plane.normal * plane.point;
		newD = plane.normal * thing->GetEyePosition();
		
		if( newD < planeD )
		{
			oldD = plane.normal * oldPosition;

			intersection = oldPosition + delta * ( ( planeD - oldD ) / ( delta * plane.normal ) );

			if( surfaces[i]->GetPoly().VectorIn( intersection ) )
			{
				if( surfaces[i]->Adjoined() )
				{
					oldSector = thing->GetSector();
					thing->SetSector( surfaces[i]->GetAdjoin() );
					surfaces[i]->SendCogMessages( "crossed", thing->GetNum() );
					surfaces[i]->GetAdjoin()->SendCogMessages( "entered", thing->GetNum() );
					oldSector->SendCogMessages( "exited", thing->GetNum() );

					thing->GetSector()->UpdateThingSector( thing, oldPosition );
					
					return true;
				}
			}
		}
	}
	return false;
}



void W_Sector::SurfaceCollisions( W_Thing* thing, W_Surface *ignore )
{
	M_Vector planePoint;
	int i;
	M_Plane plane;
	M_Vector offset;

	if( recurseGuard ) return;
	recurseGuard = true;

	for( i = 0 ; i < numSurfaces ; i++ )
	{
		if( surfaces[i] == ignore ) continue;
		//if(surfaces[i]==thing->GetAttachSurface()) continue;
		
		surfaces[i]->SurfaceCollisions( thing );
	}

	recurseGuard = false;
}

bool W_Sector::FloorCollisions( W_Thing* thing, W_Surface *ignore )
{
	float newD, planeD, centerD;
	M_Vector planePoint;
	int i;
	M_Plane plane;
	M_Vector floorPos;
	M_Vector newPos;
	float t;
	float offset;

	if( recurseGuard ) return false;
	recurseGuard = true;
	for( i = 0 ; i < numSurfaces ; i++ )
	{
		if( surfaces[i] == ignore ) continue;
		if( surfaces[i]->FloorCollisions( thing ) )
		{
			recurseGuard = false;
			return true;
		}
	}
	recurseGuard = false;
	return false;
}



void W_Sector::ThingCollisions( W_Thing* collide_thing, W_Surface *ignore )
{
	M_Vector distance;
	M_Vector offset;
	float collideDistance;
	int i;
	M_Vector floorPoint;
	float newD, planeD;
	bool standPastSurface;
	bool garbage;
	if( recurseGuard ) return;
	recurseGuard = true;
	W_Thing *thing;

	for( i = 0 ; i < things.size() ; i++ )
	{
		thing = things[i];
		if( thing != collide_thing )
		{
			distance = collide_thing->GetPosition() - thing->GetPosition();
			collideDistance = collide_thing->GetSize() + thing->GetSize();
			if( distance.Magnitude2() <= collideDistance * collideDistance )
				thing->ThingCollisions( collide_thing );
		}
	}
	
	for( i = 0 ; i < numSurfaces ; i++ )
	{
		if( surfaces[i] == ignore ) continue;
		if( !surfaces[i]->Adjoined() ) continue;

		offset = surfaces[i]->GetPoly().SpherePlaneOffset( collide_thing->GetPosition(), collide_thing->GetSize() * 2, garbage );
		if( offset.Magnitude2() > 0 )
			surfaces[i]->GetAdjoin()->ThingCollisions( collide_thing, surfaces[i]->GetMirror() );
	}
	recurseGuard = false;
}

bool W_Sector::ThingFloorCollisions( W_Thing* collide_thing, W_Surface *ignore )
{
	M_Vector distance;
	M_Vector offset;
	float collideDistance;
	int i;
	M_Vector floorPoint;
	float newD, planeD;
	bool standPastSurface;
	W_Thing *thing;

	if( recurseGuard ) return false;
	recurseGuard = true;

	floorPoint = collide_thing->GetPosition() - collide_thing->GetStandVector();
		
	for( i = 0 ; i < things.size() ; i++ )
	{
		thing = things[i];
		if( thing != collide_thing )
		{
			distance = floorPoint - thing->GetPosition();
			collideDistance = thing->GetMoveSize();
			if( distance.Magnitude2() <= collideDistance * collideDistance )
				if( thing->ThingFloorCollisions( collide_thing ) ) 
					{
						recurseGuard = false;
						return true;
					}
		}
	}

	for( i = 0 ; i < numSurfaces ; i++ )
	{
		if( surfaces[i] == ignore ) continue;
		if( !surfaces[i]->Adjoined() ) continue;
		planeD = surfaces[i]->GetPoly().GetPlane().normal * surfaces[i]->GetPoly().GetPlane().point;
		newD = floorPoint * surfaces[i]->GetPoly().GetPlane().normal;

		if( newD < planeD + .01 )
		{
			if( surfaces[i]->GetPoly().VectorIn( floorPoint ) )
				if( surfaces[i]->GetAdjoin()->ThingFloorCollisions( collide_thing, surfaces[i]->GetMirror() ) )
				{
					recurseGuard = false;
					return true;
				}
		}
	}
	recurseGuard = false;
	return false;
}

void W_Sector::ThingSurfaceCollisions( W_Thing* collide_thing, W_Surface *ignore )
{
	M_Vector distance;
	M_Vector offset;
	float collideDistance;
	int i;
	M_Vector floorPoint;
	float newD, planeD;
	bool standPastSurface;
	bool garbage;
	W_Thing *thing;

	if( recurseGuard ) return;
	recurseGuard = true;

	for( i = 0 ; i < things.size() ; i++ )
	{
		thing = things[i];
		if( thing != collide_thing )
		{
			distance = collide_thing->GetPosition() - thing->GetPosition();
			collideDistance = collide_thing->GetMoveSize() + thing->GetMoveSize();
			if( distance.Magnitude2() <= collideDistance * collideDistance )
				thing->ThingSurfaceCollisions( collide_thing );
		}
	}

	for( i = 0 ; i < numSurfaces ; i++ )
	{
		if( surfaces[i] == ignore ) continue;
		if( !surfaces[i]->Adjoined() ) continue;
		
		offset = surfaces[i]->GetPoly().SpherePlaneOffset( collide_thing->GetPosition(), collide_thing->GetMoveSize() * 2, garbage );
		if( offset.Magnitude2() > 0 )
			surfaces[i]->GetAdjoin()->ThingSurfaceCollisions( collide_thing, surfaces[i]->GetMirror() );
	}
	recurseGuard = false;
}

bool W_Sector::PerformActivate( M_Vector position, M_Vector point, W_Surface *ignore )
{
	int i;
	float planeD, newD;
	float t;
	M_Plane plane;
	M_Vector newPos;
	W_Poly poly;
	M_Vector distance;
	float modelSize;
	W_Thing *thing;

	for( i = 0 ; i < things.size() ; i++) 
	{
		thing = things[i];
		if( thing->GetModel() != NULL )
		{
			distance = position - thing->GetPosition();
			modelSize = thing->GetModel()->GetRadius();

			if( distance.Magnitude2() <= .1 * .1 + modelSize * modelSize )
				if( thing->PerformActivate( position, point ) ) return true;
		}
	}

	for( i = 0 ; i < numSurfaces ; i++ )
	{
		if( surfaces[i] == ignore ) continue;
		if( surfaces[i]->PerformActivate( position, point ) ) return true ;
	}
	return false;
}