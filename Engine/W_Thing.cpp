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

#include "S_Manager.h"

#include <math.h>
#include <assert.h>

W_Thing::W_Thing(JK_Template *t, M_Vector p, M_Vector r, W_Sector *s) 
{
	thingTemplate=new JK_Template(t->GetName(), t);
	position=p;
	rotation=r;
	sector=s;

	attachFlags=JK_ATTACH_FREE;
	velocity=M_Vector(0,0,0);
	acceleration=M_Vector(0,0,0);
	nudge=M_Vector(0,0,0);
	crouched=false;
	
	pathMoving=false;
	currentFrame=0;
	destFrame=0;
	pathVector=M_Vector(0,0,0);
	pathT=0;
	pathTSpeed=0;

	thrusting=false;

	flagValue=1;

	moveSound=NULL;

	walkTimer=GetTickCount();
	walkSide=0;
}

W_Thing::W_Thing(W_Thing &c) 
{
	thingTemplate=new JK_Template(*c.thingTemplate);
	position=c.position;
	rotation=c.rotation;
	sector=c.sector;

	attachFlags=JK_ATTACH_FREE;
	velocity=M_Vector(0,0,0);
	acceleration=M_Vector(0,0,0);
	nudge=M_Vector(0,0,0);
	crouched=false;
	
	pathMoving=false;
	currentFrame=0;
	destFrame=0;
	pathVector=M_Vector(0,0,0);
	pathT=0;
	pathTSpeed=0;

	thrusting=false;

	flagValue=1;

	moveSound=NULL;

	walkTimer=GetTickCount();
	walkSide=0;

    keyInstance = c.keyInstance;
}

W_Thing::~W_Thing()
{
	//sector->RemoveThing( shared_ptr<W_Thing>( this ) );
}

M_Vector W_Thing::GetPosition()
{
	return position;
}

void W_Thing::SetPosition(M_Vector p)
{
	position=p;
}

M_Vector W_Thing::GetRotation()
{
	return rotation;
}

void W_Thing::SetRotation(M_Vector r)
{
	M_Vector delta;
	float xcos, xsin;
	
	delta=r-rotation;
	xcos=cos(delta.y*3.14/180);
	xsin=sin(delta.y*3.14/180);

	M_Matrix m(xcos,-xsin,0,0,
			  xsin,xcos,0,0,
			  0,0,1,0,
			  0,0,0,1
			  );

	rotation=r;
	velocity=m*velocity;
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

M_Vector W_Thing::GetCompositeRotation()
{
	return orient + rotation;
}

JK_Template *W_Thing::GetTemplate()
{
	return thingTemplate;
}

void W_Thing::SetTemplate(JK_Template *t)
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

R_Model *W_Thing::GetModel()
{
	return model;
}

M_Vector W_Thing::GetEyePosition()
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

M_Vector W_Thing::GetStandVector()
{
	if(crouched)
		return M_Vector(0,0,moveSize);
	else
		return insertOffset;
}

W_Surface *W_Thing::GetAttachSurface()
{
	return attachSurface;
}

int W_Thing::GetThingFlags()
{
	return thingFlags;
}

int W_Thing::GetPhysicsFlags()
{
	return physicsFlags;
}

int W_Thing::GetCollide()
{
	return collide;
}

M_Vector W_Thing::GetAcceleration()
{
	return acceleration;
}

M_Vector W_Thing::GetVelocity()
{
	return velocity;
}

float W_Thing::GetMass()
{
	return mass;
}

void W_Thing::AddCogLink(C_Script *cogScript)
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

M_Vector W_Thing::GetPathMoveDelta()
{
	return pathMoveDelta;
}

bool W_Thing::WasAttached()
{
	return oldAttachFlags&JK_ATTACH_WORLD_SURFACE;
}

void W_Thing::Destroy()
{
	//currentLevel.things.Remove(num);
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

int W_Thing::Create(JK_Template *newTemplate, W_Thing *thingPos) // **** Potential overflow
{
	int i;
	/*for(i=0;1;i++)
	{
		if(!currentLevel.things[i].Exists())
		{
			currentLevel.things[i]=*thingPos;
//			currentLevel.things[i].GetTemplate()->CopyParams(newTemplate);
			currentLevel.things[i].ProcessTemplate();
			currentLevel.things[i].SetNum(i);
			if(i>=currentLevel.numThings) currentLevel.numThings++;
			return i;
		}
	}*/
	return 0;
}
/*
void W_Thing::Lock()
{
	EnterCriticalSection(&critSec);
}

void W_Thing::Unlock()
{
	LeaveCriticalSection(&critSec);
}*/

void W_Thing::playKey( JK_Key *key, int flags )
{
    keyInstance.key = key;
    keyInstance.time = 0;
    keyInstance.flags = flags;
}