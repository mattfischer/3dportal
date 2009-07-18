#include "R_Model.h"

#include "M_Matrix.h"

#include "W_Sector.h"
#include "W_Surface.h"
#include "W_Thing.h"

#include "JK_Template.h"
#include "JK_Flags.h"
#include "JK_Level.h"

#include "C_Script.h"

#include "S_SoundClass.h"
#include "S_Sound.h"
#include "S_Manager.h"

#include "U_Matrix.h"

#include <math.h>
#include <assert.h>

W_Thing::W_Thing(Jk::Template *t, Math::Vector p, Math::Vector r, W_Sector *s) 
{
    thingTemplate=new Jk::Template(t->GetName(), t);
	position=p;
	rotation=r;
	sector=s;

	attachFlags=JK_ATTACH_FREE;
	velocity=Math::Vector(0,0,0);
	acceleration=Math::Vector(0,0,0);
	nudge=Math::Vector(0,0,0);
	crouched=false;
	
	pathMoving=false;
	currentFrame=0;
	destFrame=0;
	pathVector=Math::Vector(0,0,0);
	pathT=0;
	pathTSpeed=0;

	thrusting=false;

	flagValue=1;

	moveSound=NULL;

	walkTimer=GetTickCount();
	walkSide=0;

	sprite = NULL;
	spriteTime = 0;

	killTime = -1;

    pendingDestroy = false;
}

W_Thing::W_Thing(W_Thing &c) 
{
    thingTemplate=new Jk::Template(*c.thingTemplate);
	position=c.position;
	rotation=c.rotation;
	sector=c.sector;

	attachFlags=JK_ATTACH_FREE;
	velocity=Math::Vector(0,0,0);
	acceleration=Math::Vector(0,0,0);
	nudge=Math::Vector(0,0,0);
	crouched=false;
	
	pathMoving=false;
	currentFrame=0;
	destFrame=0;
	pathVector=Math::Vector(0,0,0);
	pathT=0;
	pathTSpeed=0;

	thrusting=false;

	flagValue=1;

	moveSound=NULL;

	walkTimer=GetTickCount();
	walkSide=0;

	keyTrack = c.keyTrack;

	sprite = c.sprite;
	spriteTime = c.spriteTime;

	killTime = c.killTime;
}

W_Thing::~W_Thing()
{
}

int W_Thing::Create( Jk::Template *t, Math::Vector p, Math::Vector r, W_Sector *s )
{
	shared_ptr<W_Thing> newThing( new W_Thing( t, p, r, s ) );

	newThing->ProcessTemplate();
	
	int i;
	for( i = 0; i < currentLevel.things.size(); i++ )
	{
		if( !currentLevel.things[i] )
		{
			break;
		}
	}

	if( i == currentLevel.things.size() )
	{
		currentLevel.things.push_back( newThing );
	}
	else
	{
		currentLevel.things[i] = newThing;
	}

	newThing->num = i;

	newThing->GetSector()->AddThing( newThing.get() );

	if( newThing->soundClass )
	{
		newThing->soundClass->Play( "create", i );
	}

	return i;
}

int W_Thing::CreateFromThing( W_Thing *thing, Jk::Template *newTemplate )
{
	shared_ptr<W_Thing> newThing( new W_Thing( *thing ) );
	newThing->GetTemplate()->CopyParams( newTemplate );
	newThing->ProcessTemplate();
	
	int i;
	for( i = 0; i < currentLevel.things.size(); i++ )
	{
		if( !currentLevel.things[i] )
		{
			break;
		}
	}

	if( i == currentLevel.things.size() )
	{
		currentLevel.things.push_back( newThing );
	}
	else
	{
		currentLevel.things[i] = newThing;
	}

	newThing->num = i;
	newThing->GetSector()->AddThing(newThing.get());

	if( newThing->soundClass )
	{
		newThing->soundClass->Play( "create", i );
	}

	return i;
}

int W_Thing::GetNum()
{
    return num;
}

Math::Vector W_Thing::GetPosition()
{
	return position;
}

void W_Thing::SetPosition(Math::Vector p)
{
	position=p;
}

Math::Vector W_Thing::GetRotation()
{
	return rotation;
}

void W_Thing::SetRotation(Math::Vector r)
{
	Math::Vector delta;
	
	delta=r-rotation;
	
    rotation = r;
    velocity = Util::Matrix::RotateZ(delta.y) * Util::Matrix::RotateX(delta.x) * velocity;
}

W_Sector *W_Thing::GetSector()
{
	return sector;
}

void W_Thing::SetSector( W_Sector *newSector )
{
	sector->RemoveThing( this );
	sector = newSector;
	sector->AddThing( this );
}

Math::Vector W_Thing::GetCompositeRotation()
{
	return orient + rotation;
}

Jk::Template *W_Thing::GetTemplate()
{
	return thingTemplate;
}

void W_Thing::SetTemplate(Jk::Template *t)
{
	thingTemplate=t;
}

float W_Thing::GetMaxThrust()
{
	return maxThrust;
}

float W_Thing::GetMoveSize()
{
	return moveSize;
}

float W_Thing::GetSize()
{
	return size;
}

int W_Thing::GetFlagValue()
{
	return flagValue;
}

void W_Thing::SetAttached(W_Surface *s)
{
	attachFlags=JK_ATTACH_WORLD_SURFACE;
	attachSurface=s;
	attachThing=NULL;
}

void W_Thing::SetAttached(W_Poly *p, W_Thing *t)
{
	attachFlags=JK_ATTACH_THING_FACE;
	attachPoly=*p;
	attachThing=t;
	attachSurface=NULL;
}

bool W_Thing::IsAttached()
{
	return attachFlags&JK_ATTACH_WORLD_SURFACE;
}

Render::Model *W_Thing::GetModel()
{
	return model;
}

Math::Vector W_Thing::GetEyePosition()
{
	return position+eyeOffset;
}

void W_Thing::SetCrouched(bool c)
{
	crouched=c;
}

void W_Thing::Jump()
{
	//if(jumping) return;
	//if(attachFlags&(JK_ATTACH_WORLD_SURFACE | JK_ATTACH_THING_FACE))
	{
		jumping=true;
		velocity.z=jumpVelocity;
	}
	if(attachFlags&(JK_ATTACH_WORLD_SURFACE | JK_ATTACH_THING_FACE))
		switch(GetSurfaceType())
		{
		case 0: soundClass->Play("jump", num); break;
		case 1: soundClass->Play("jumpmetal", num); break;
		case 2: soundClass->Play("jumpearth", num); break;
		case 4: soundClass->Play("jumpwater", num); break;
		}
}

Math::Vector W_Thing::GetStandVector()
{
	if(crouched)
		return Math::Vector(0,0,moveSize);
	else
		return insertOffset;
}

W_Surface *W_Thing::GetAttachSurface()
{
	return attachSurface;
}

int W_Thing::GetType()
{
	return type;
}

int W_Thing::GetThingFlags()
{
	return thingFlags;
}

int W_Thing::GetTypeFlags()
{
	return typeFlags;
}

int W_Thing::GetPhysicsFlags()
{
	return physicsFlags;
}

int W_Thing::GetCollide()
{
	return collide;
}

Math::Vector W_Thing::GetAcceleration()
{
	return acceleration;
}

Math::Vector W_Thing::GetVelocity()
{
	return velocity;
}

float W_Thing::GetMass()
{
	return mass;
}

void W_Thing::AddCogLink(Cog::Script *cogScript)
{
	cogLinks.push_back( cogScript );
}

void W_Thing::SendCogMessages( const string& message, int source, bool synchronous )
{
	int i;

	for( i = 0 ; i < cogLinks.size() ; i++ )
	{
		cogLinks[i]->Message( message, num, 3, source, synchronous );
	}
}

bool W_Thing::IsPathMove()
{
	if(move==MOVE_PATH) return true;
	else return false;
}

bool W_Thing::IsPhysicsMove()
{
	if(move==MOVE_PHYSICS) return true;
	else return false;
}

Math::Vector W_Thing::GetPathMoveDelta()
{
	return pathMoveDelta;
}

bool W_Thing::WasAttached()
{
	return oldAttachFlags&JK_ATTACH_WORLD_SURFACE;
}

void W_Thing::Destroy()
{
    pendingDestroy = true;
}

void W_Thing::RealDestroy()
{
    sector->RemoveThing( this );
    sector = NULL;
	currentLevel.things[num] = shared_ptr<W_Thing> ();
}

void W_Thing::DoFoley()
{
	int surfaceType;

	if(soundClass==NULL) return;

	surfaceType=GetSurfaceType();

	if(attachFlags&(JK_ATTACH_WORLD_SURFACE | JK_ATTACH_THING_FACE))
	{
		if(oldAttachFlags==JK_ATTACH_FREE)
		{
			switch(surfaceType)
			{
			case 0: soundClass->Play("landhard", num); break;
			case 1: soundClass->Play("landmetal", num); break;
			case 2: soundClass->Play("landearth", num); break;
			case 3: soundClass->Play("landpuddle", num); break;
			case 4: soundClass->Play("landwater", num); break;
			}
		}

		if(velocity.Magnitude2()>0)
		{
			if(velocity.Magnitude()<.7)
			{
				if(GetTickCount()>walkTimer+150/velocity.Magnitude())
				{
					if(walkSide==0)
					{
						switch(surfaceType)
						{
						case 0: soundClass->Play("lwalkhard", num); break;
						case 1: soundClass->Play("lwalkmetal", num); break;
						case 2: soundClass->Play("lwalkearth", num); break;
						case 3: soundClass->Play("lwalkpuddle", num); break;
						case 4: soundClass->Play("lwalkwater", num); break;
						}
						walkSide=1;
					}
					else
					{
						switch(surfaceType)
						{
						case 0: soundClass->Play("rwalkhard", num); break;
						case 1: soundClass->Play("rwalkmetal", num); break;
						case 2: soundClass->Play("rwalkearth", num); break;
						case 3: soundClass->Play("rwalkpuddle", num); break;
						case 4: soundClass->Play("rwalkwater", num); break;
						}
						walkSide=0;
					}
					walkTimer=GetTickCount();
				}
			}
			else
			{
				if(GetTickCount()>walkTimer+150/velocity.Magnitude())
				{
					if(walkSide==0)
					{
						switch(surfaceType)
						{
						case 0: soundClass->Play("lrunhard", num); break;
						case 1: soundClass->Play("lrunmetal", num); break;
						case 2: soundClass->Play("lrunearth", num); break;
						case 3: soundClass->Play("lrunpuddle", num); break;
						case 4: soundClass->Play("lrunwater", num); break;
						}
						walkSide=1;
					}
					else
					{
						switch(surfaceType)
						{
						case 0: soundClass->Play("rrunhard", num); break;
						case 1: soundClass->Play("rrunmetal", num); break;
						case 2: soundClass->Play("rrunearth", num); break;
						case 3: soundClass->Play("rrunpuddle", num); break;
						case 4: soundClass->Play("rrunwater", num); break;
						}
						walkSide=0;
					}
					walkTimer=GetTickCount();
				}
			}
		}
	}
}

int W_Thing::GetSurfaceType()
{
	int surfaceType;

	surfaceType=0;
	if(attachFlags&JK_ATTACH_WORLD_SURFACE)
	{
		if(attachSurface->GetFlags()&JK_SURFACE_METAL) surfaceType=1;
		if(attachSurface->GetFlags()&JK_SURFACE_DIRT) surfaceType=2;
		if(attachSurface->GetFlags()&JK_SURFACE_WATER_SHALLOW) surfaceType=3;
		if(attachSurface->GetFlags()&JK_SURFACE_WATER_DEEP) surfaceType=4;
	}
	else if(attachFlags&JK_ATTACH_THING_FACE)
	{
		if(attachThing->GetThingFlags()&JK_THING_METAL) surfaceType=1;
		if(attachThing->GetThingFlags()&JK_THING_DIRT) surfaceType=2;
	}

	return surfaceType;
}

void W_Thing::playKey( Jk::Key *key, int flags )
{
    keyTrack = Jk::Key::Track(key, 0, flags);
}

