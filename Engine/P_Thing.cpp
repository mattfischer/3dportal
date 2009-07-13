#include "W_Thing.h"
#include "W_Sector.h"
#include "W_Poly.h"
#include "W_Surface.h"

#include "JK_Level.h"
#include "JK_Flags.h"

#include "R_Model.h"

#include "G_Console.h"

#include "S_Sound.h"
#include "S_SoundClass.h"

#include <math.h>

extern bool doThingCollisions;
extern bool updateThings;

extern shared_ptr<W_Thing> player;

void W_Thing::ApplyThrust(M_Vector thrust)
{
	M_Vector accel;

	if(thrust.Magnitude2()==0) return;

	accel=thrust;
	acceleration=acceleration+accel;

	thrusting=true;
}

void W_Thing::AddVelocity(M_Vector vel)
{
	velocity+=vel;
}

void W_Thing::SetVelocity(M_Vector vel)
{
	//if(attached)
		velocity=vel;
}

void W_Thing::Nudge(M_Vector n)
{
	nudge=n;
}



bool W_Thing::ThingFloorCollisions( W_Thing* thing)
{
	R_Mesh *mesh;
	W_Poly poly;
	int i;
	M_Vector normal;
	float xcos, xsin;
	M_Vector floorPos;
	
	if( !doThingCollisions ) return false;
	if( collide != JK_COLLIDE_FACE ) return false;
	if( thing->GetCollide() != JK_COLLIDE_SPHERE ) return false;
	if( !( thingFlags & JK_THING_CAN_STAND_ON ) ) return false;
	if( model == NULL ) return false;

	xcos = cos( rotation.y * 3.14 / 180 );
	xsin = sin( rotation.y * 3.14 / 180 );
	
	M_Matrix rotatex( xcos, -xsin, 0, 0,
					  xsin,  xcos, 0, 0,
					  0,     0,    1, 0,
					  0,     0,    0, 1
					 );
	M_Matrix translate( 1, 0, 0, position.x,
					    0, 1, 0, position.y,
					    0, 0, 1, position.z,
					    0, 0, 0, 1);
	M_Matrix transform = translate * rotatex;
	float t;
	M_Vector newPos;
	float planeD, newD;
	float offset;
	bool result;

	mesh = model->GetMesh( 0, 0 );
	
	result = false;
	for( i = 0 ; i < mesh->numFaces ; i++ )
	{
		poly = mesh->faces[i].poly;
		poly.Transform( transform );
		poly.BuildPlane();

		normal = poly.GetPlane().normal;
		planeD = normal * poly.GetPlane().point;

		if( thing->GetPosition() * normal < planeD ) continue;
		
		if( normal.z <= 0 ) continue;
		floorPos = thing->GetPosition() - thing->GetStandVector();
		
		newD = floorPos * normal;

		if( thing->WasAttached() ) offset = thing->GetStandVector().z;
		else offset = 0.01;

		if( newD > planeD + offset ) continue;
		t = ( planeD - newD ) / ( M_Vector( 0, 0, 1) * normal );
		newPos = floorPos + M_Vector( 0, 0, t );
		if( !poly.VectorIn( newPos ) ) continue;
		
		thing->Nudge( M_Vector( 0, 0, ( planeD - newD ) / normal.z ) );
		thing->SetAttached( &poly, this );
		thing->ArrestMotion( normal );
		result = true;
	}
	return result;
}

void W_Thing::ThingSurfaceCollisions( W_Thing* thing)
{
	R_Mesh *mesh;
	W_Poly poly;
	int i;
	M_Vector normal;
	M_Vector offset;
	float xcos, xsin;
	M_Vector floorPos;
	
	if( !doThingCollisions ) return;
	if( collide != JK_COLLIDE_FACE ) return;
	if( thing->GetCollide() != JK_COLLIDE_SPHERE ) return;
	if( model == NULL ) return;

	xcos = cos( rotation.y * 3.14 / 180 );
	xsin = sin( rotation.y * 3.14 / 180 );

	M_Matrix rotatex( xcos, -xsin, 0, 0,
					  xsin,  xcos, 0, 0,
					  0,     0,    1, 0,
					  0,     0,    0, 1
					);
	M_Matrix translate( 1, 0, 0, position.x,
					    0, 1, 0, position.y,
					    0, 0, 1, position.z,
					    0, 0, 0, 1);
	M_Matrix transform = translate * rotatex;
	float t;
	M_Vector newPos;
	float planeD, newD;
	bool direct;

	mesh = model->GetMesh( 0, 0 );

	for( i = 0 ; i < mesh->numFaces ; i++ )
	{
		poly = mesh->faces[i].poly;
		poly.Transform( transform );
		poly.BuildPlane();

		normal = poly.GetPlane().normal;
		planeD = normal * poly.GetPlane().point;

		if( thing->GetPosition() * normal < planeD ) continue;

		offset = poly.SpherePlaneOffset( thing->GetPosition(), thing->GetMoveSize(), direct );
		if( offset.Magnitude2() > 0 )
		{
			thing->SetPosition( thing->GetPosition() + offset );
			if( direct )
				thing->ArrestMotion( normal );
		}
	}
}

void W_Thing::ThingCollisions( W_Thing* thing)
{
	M_Vector direction;
	float relVel;
	float distance;
	float collideDistance;
	float difference;
	M_Vector normalAccel;
	float p, v, dp;
	
	if( !doThingCollisions ) return;
	if( collide!=JK_COLLIDE_SPHERE || thing->GetCollide()!=JK_COLLIDE_SPHERE ) return;
	
	
	direction = position - thing->GetPosition();
	distance = direction.Magnitude();
	direction = direction / distance;

	relVel = thing->GetVelocity() * direction - velocity * direction;

	if( relVel < 0 ) return;

	collideDistance = moveSize + thing->GetMoveSize();
	difference = collideDistance - distance - .005;
	if( difference > 0 )
	{
		thing->SetPosition( thing->GetPosition() - direction * difference );
		
		if( thing->GetAcceleration() * direction < 0 ) return;

		normalAccel = direction * ( direction * thing->GetAcceleration() );
		
		ApplyThrust( normalAccel * thing->GetMass() / ( mass + thing->GetMass() ) );
		thing->ApplyThrust( normalAccel * -mass / ( mass + thing->GetMass() ) );
	}

	thing->SendCogMessages( "touched", num );
}



void W_Thing::UpdatePath(float time)
{
	M_Vector newPos;
	M_Vector oldPos;

	if(!pathMoving) 
	{
		pathMoveDelta=M_Vector(0,0,0);
		return;
	}

	pathT+=pathTSpeed*time;
	
	if(pathT>=1)
	{
		currentFrame=destFrame;
		newPos=frames[currentFrame].position;
		pathMoving=false;
		pathT=0;
		pathTSpeed=0;
		SendCogMessages("arrived", -1);
		if(moveSound!=NULL)
			moveSound->Stop();
		moveSound=NULL;
		if(soundClass)
			soundClass->Play("stopmove", num);
	}
	else
		newPos=frames[currentFrame].position+pathVector*pathT;

	pathMoveDelta=newPos-position;
	oldPos=position;
	position=newPos;
	sector->UpdateThingSector( this, oldPos);
}

void W_Thing::MoveToFrame( int frame, float speed )
{
	if( move != MOVE_PATH ) return;
	if( frame >= frames.size() ) return;

	destFrame = frame;
	pathVector = frames[destFrame].position - frames[currentFrame].position;
	pathT = 0;
	if( maxVelocity > 0 && speed > maxVelocity ) speed = maxVelocity;
	speed /= 10.0;
	pathTSpeed = speed / pathVector.Magnitude();
	pathMoving = true;
	if( soundClass )
	{
		soundClass->Play( "startmove", num );
		moveSound = soundClass->Play( "moving", num );
	}
}

void W_Thing::JumpToFrame(int frame, W_Sector *s)
{
	position=frames[frame].position;
	rotation=frames[frame].rotation;
	currentFrame=frame;
	sector=s;
}

bool W_Thing::IsMoving()
{
	return pathMoving;
}

int W_Thing::GetCurrentFrame()
{
	return currentFrame;
}



void W_Thing::Activate()
{
	M_Vector pointVector;
	float xcos, xsin, ycos, ysin;
	W_Surface *surface;
	char buffer[100];

	xcos=cos(rotation.y*3.14/180);
	xsin=sin(rotation.y*3.14/180);

	ycos=cos(rotation.x*3.14/180);
	ysin=sin(rotation.x*3.14/180);

	M_Matrix rotatex(xcos,-xsin,0,0,
			  xsin,xcos,0,0,
			  0,0,1,0,
			  0,0,0,1
			  );

	M_Matrix rotatey(1,0,0,0,
					 0,ycos,-ysin,0,
			         0,ysin,ycos,0,
			    	 0,0,0,1
					 );
	
	pointVector=rotatex*rotatey*M_Vector(0,1,0);

	sector->PerformActivate(position+eyeOffset, pointVector, NULL);
}

bool W_Thing::PerformActivate(M_Vector position, M_Vector point)
{
	R_Mesh *mesh;
	W_Poly poly;
	int i;
	M_Vector normal;
	M_Vector offset;
	float xcos, xsin;
	M_Vector floorPos;

	xcos=cos(rotation.y*3.14/180);
	xsin=sin(rotation.y*3.14/180);

	M_Matrix rotatex(xcos,-xsin,0,0,
			  xsin,xcos,0,0,
			  0,0,1,0,
			  0,0,0,1
			  );
	M_Matrix translate(1,0,0,position.x,
					   0,1,0,position.y,
					   0,0,1,position.z,
					   0,0,0,1);
	M_Matrix transform=translate*rotatex;
	float t;
	M_Vector newPos;
	float planeD, newD;
	M_Plane plane;

	if(model==NULL) return false;

	if(collide==JK_COLLIDE_FACE)
	{
		mesh=model->GetMesh(0,0);
		
		for(i=0;i<mesh->numFaces;i++)
		{
			poly=mesh->faces[i].poly;
			poly.Transform(transform);
			poly.BuildPlane();

			plane=poly.GetPlane();
			planeD=plane.point*plane.normal;
			newD=position*plane.normal;

			t=-(newD-planeD)/(point*plane.normal);
			if(t<0 || t>.1) continue;
			newPos=position+point*t;
			if(poly.VectorIn(newPos))
			{
				SendCogMessages("activated", player->GetNum());
				return true;
			}
		}
	}
	return false;
}




void W_Thing::UpdateForces( float time )
{
	bool repeat;
	M_Vector nudgeVector;
	float nudgeVelocity = .5;
	M_Vector oldPosition;

	if( collide != JK_COLLIDE_SPHERE ) return;

	oldPosition = GetEyePosition();
	if( type==ACTOR || type==PLAYER )
	{
		if( attachFlags & JK_ATTACH_THING_FACE ) 
			position += attachThing->GetPathMoveDelta();
		if( attachFlags & JK_ATTACH_WORLD_SURFACE )
			position += attachSurface->GetMoveDelta();
		oldAttachFlags = attachFlags;
		attachFlags = JK_ATTACH_FREE;
		
		if( !jumping )
		{
			sector->ThingFloorCollisions( this, NULL );
		
			if( attachFlags == JK_ATTACH_FREE )
				sector->FloorCollisions( this, NULL);
			
			if( sector->UpdateThingSector( this, oldPosition ) )
			{
				oldPosition = GetEyePosition();
				sector->ThingFloorCollisions( this, NULL );
		
				if( attachFlags == JK_ATTACH_FREE )
					sector->FloorCollisions( this, NULL );
			}
		}

		nudgeVector = M_Vector( 0, 0, 0 );

		if( time > 0 )
		{
			if( nudge.Magnitude() < nudgeVelocity * time )
				nudgeVector = nudge;
			else
			{
				nudgeVector = nudge;
				nudgeVector.Normalize();
				nudgeVector = nudgeVector * nudgeVelocity * time;
			}
		}
		position += nudgeVector;
		nudge = M_Vector( 0, 0, 0 );
	}

	sector->SurfaceCollisions( this, NULL );
	sector->ThingSurfaceCollisions( this, NULL );
	sector->UpdateThingSector( this, oldPosition );

	if( attachFlags & (JK_ATTACH_WORLD_SURFACE | JK_ATTACH_THING_FACE ) )
	{
		acceleration -= velocity * surfaceDrag;
		if( physicsFlags & JK_PHYSICS_GRAVITY )
			acceleration += M_Vector( 0, 0, -currentLevel.gravity / 4 );
	}
	else
	{
		if( physicsFlags & JK_PHYSICS_GRAVITY )
			acceleration += M_Vector( 0, 0, -currentLevel.gravity );
		acceleration -= velocity * airDrag;
	}

	sector->ThingCollisions( this, NULL);
}

void W_Thing::UpdateFinalize(float time)
{
	M_Vector normal;
	M_Vector normalAccel;
	M_Vector normalVel;
	M_Vector oldPosition;

	rotation+=rotVelocity*time;

	if(attachFlags&(JK_ATTACH_WORLD_SURFACE | JK_ATTACH_THING_FACE))
	{
		if(attachFlags&JK_ATTACH_WORLD_SURFACE)
			normal=attachSurface->GetPoly().GetPlane().normal;
		else if(attachFlags&JK_ATTACH_THING_FACE)
			normal=attachPoly.GetPlane().normal;

		normalAccel=normal*(normal*acceleration);
		acceleration-=normalAccel;
	}

	velocity=velocity+acceleration*time;
	
	if(attachFlags&(JK_ATTACH_WORLD_SURFACE | JK_ATTACH_THING_FACE))
	{
		normalVel=normal*(normal*velocity);
		velocity-=normalVel;
	}

	if(velocity.z<=0) jumping=false;

	if(attachFlags&(JK_ATTACH_WORLD_SURFACE | JK_ATTACH_THING_FACE))
	{
		if(!thrusting && velocity.Magnitude()<staticDrag)
			velocity=M_Vector(0,0,0);
	}
	oldPosition=GetEyePosition();

	position=position+velocity*time;
	
	sector->UpdateThingSector( this, oldPosition);
	acceleration=M_Vector(0,0,0);
	thrusting=false;

	if(type==PLAYER)
	{
		float xcos, xsin;
		xcos=cos((rotation.y+orient.y)*3.14/180);
		xsin=sin((rotation.y+orient.y)*3.14/180);
		M_Vector vector(xcos, xsin, 0);
		rotation.z=vector*velocity*-3;
	}
	DoFoley();

	if( keyInstance.key )
	{
		keyInstance.time += time;
	}

	if( killTime > -1 && GetTickCount() > killTime )
	{
		Destroy();
	}
}

void W_Thing::ArrestMotion(M_Vector normal)
{
	if(acceleration*normal<0)
		acceleration-=normal*(normal*acceleration);
	if(velocity*normal<0)
		velocity-=normal*(normal*velocity);
}

void W_Thing::UpdateThings( float time )
{
	int i;
	shared_ptr<W_Thing> thing;

	if( !updateThings ) return;

	for( i = 0 ; i < currentLevel.things.size() ; i++ )
	{
		thing = currentLevel.things[i];
		if( thing && thing->IsPathMove() )
			thing->UpdatePath( time );
	}

	for( i = 0 ; i < currentLevel.things.size() ; i++ )
	{
		thing = currentLevel.things[i];
		if( thing && thing->IsPhysicsMove() && ( thing==player || thing->GetFlagValue()==globalFlag-1 || i%currentLevel.things.size() == globalFlag%currentLevel.things.size() ) )
			thing->UpdateForces( time );
	}

	for( i = 0 ;  i < currentLevel.things.size() ; i++ )
	{
		thing = currentLevel.things[i];
		if( thing && ( thing==player || thing->GetFlagValue()==globalFlag-1 || i%currentLevel.things.size() == globalFlag%currentLevel.things.size() ) )
			thing->UpdateFinalize( time );
	}
}