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

#include "U_Matrix.h"

#define _USE_MATH_DEFINES
#include <math.h>

extern bool doThingCollisions;
extern bool updateThings;

extern shared_ptr<World::Thing> player;

namespace World
{
    void Thing::ApplyThrust(Math::Vector thrust)
    {
	    Math::Vector accel;

	    if(thrust.Magnitude2()==0) return;

	    accel=thrust;
	    acceleration=acceleration+accel;

	    thrusting=true;
    }

    void Thing::AddVelocity(Math::Vector vel)
    {
	    velocity+=vel;
    }

    void Thing::SetVelocity(Math::Vector vel)
    {
	    //if(attached)
		    velocity=vel;
    }

    void Thing::Nudge(Math::Vector n)
    {
	    nudge=n;
    }



    bool Thing::ThingFloorCollisions( Thing* thing)
    {
        Render::Model::Mesh *mesh;
	    Poly poly;
	    int i;
	    Math::Vector normal;
	    float xcos, xsin;
	    Math::Vector floorPos;
    	
	    if( !doThingCollisions ) return false;
	    if( collide != JK_COLLIDE_FACE ) return false;
	    if( thing->GetCollide() != JK_COLLIDE_SPHERE ) return false;
	    if( !( thingFlags & JK_THING_CAN_STAND_ON ) ) return false;
	    if( model == NULL ) return false;

        Math::Matrix rotatex = Util::Matrix::RotateZ(rotation.y);
        Math::Matrix translate = Util::Matrix::Translate(position);
	    Math::Matrix transform = translate * rotatex;
	    float t;
	    Math::Vector newPos;
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
		    t = ( planeD - newD ) / ( Math::Vector( 0, 0, 1) * normal );
		    newPos = floorPos + Math::Vector( 0, 0, t );
		    if( !poly.VectorIn( newPos ) ) continue;
    		
		    thing->Nudge( Math::Vector( 0, 0, ( planeD - newD ) / normal.z ) );
		    thing->SetAttached( &poly, this );
		    thing->ArrestMotion( normal );
		    result = true;
	    }
	    return result;
    }

    void Thing::ThingSurfaceCollisions( Thing* thing)
    {
        Render::Model::Mesh *mesh;
	    Poly poly;
	    int i;
	    Math::Vector normal;
	    Math::Vector offset;
	    Math::Vector floorPos;
    	
	    if( !doThingCollisions ) return;
	    if( collide != JK_COLLIDE_FACE ) return;
	    if( thing->GetCollide() != JK_COLLIDE_SPHERE ) return;
	    if( model == NULL ) return;

        Math::Matrix rotatex = Util::Matrix::RotateZ(rotation.y);
	    Math::Matrix translate = Util::Matrix::Translate(position);
	    Math::Matrix transform = translate * rotatex;
	    float t;
	    Math::Vector newPos;
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

    void Thing::ThingCollisions( Thing* thing)
    {
	    Math::Vector direction;
	    float relVel;
	    float distance;
	    float collideDistance;
	    float difference;
	    Math::Vector normalAccel;
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



    void Thing::UpdatePath(float time)
    {
	    Math::Vector newPos;
	    Math::Vector oldPos;

	    if(!pathMoving) 
	    {
		    pathMoveDelta=Math::Vector(0,0,0);
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

    void Thing::MoveToFrame( int frame, float speed )
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

    void Thing::JumpToFrame(int frame, Sector *s)
    {
	    position=frames[frame].position;
	    rotation=frames[frame].rotation;
	    currentFrame=frame;
	    sector=s;
    }

    bool Thing::IsMoving()
    {
	    return pathMoving;
    }

    int Thing::GetCurrentFrame()
    {
	    return currentFrame;
    }



    void Thing::Activate()
    {
	    Math::Vector pointVector;
	    Surface *surface;
	    char buffer[100];

        Math::Matrix rotatex = Util::Matrix::RotateZ(rotation.y);
        Math::Matrix rotatey = Util::Matrix::RotateX(rotation.x);
    	
	    pointVector=rotatex*rotatey*Math::Vector(0,1,0);

	    sector->PerformActivate(position+eyeOffset, pointVector, NULL);
    }

    bool Thing::PerformActivate(Math::Vector position, Math::Vector point)
    {
        Render::Model::Mesh *mesh;
	    Poly poly;
	    int i;
	    Math::Vector normal;
	    Math::Vector offset;
	    Math::Vector floorPos;

        Math::Matrix rotatex = Util::Matrix::RotateZ(rotation.y);
        Math::Matrix translate = Util::Matrix::Translate(position);
	    Math::Matrix transform=translate*rotatex;
	    float t;
	    Math::Vector newPos;
	    float planeD, newD;
	    Math::Plane plane;

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




    void Thing::UpdateForces( float time )
    {
	    bool repeat;
	    Math::Vector nudgeVector;
	    float nudgeVelocity = .5;
	    Math::Vector oldPosition;

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

		    nudgeVector = Math::Vector( 0, 0, 0 );

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
		    nudge = Math::Vector( 0, 0, 0 );
	    }

        sector->SurfaceCollisions( this, NULL );
        sector->ThingSurfaceCollisions( this, NULL );
        sector->UpdateThingSector( this, oldPosition );

	    if( attachFlags & (JK_ATTACH_WORLD_SURFACE | JK_ATTACH_THING_FACE ) )
	    {
		    acceleration -= velocity * surfaceDrag;
		    if( physicsFlags & JK_PHYSICS_GRAVITY )
			    acceleration += Math::Vector( 0, 0, -currentLevel.gravity / 4 );
	    }
	    else
	    {
		    if( physicsFlags & JK_PHYSICS_GRAVITY )
			    acceleration += Math::Vector( 0, 0, -currentLevel.gravity );
		    acceleration -= velocity * airDrag;
	    }

        sector->ThingCollisions( this, NULL);
    }

    void Thing::UpdateFinalize(float time)
    {
	    Math::Vector normal;
	    Math::Vector normalAccel;
	    Math::Vector normalVel;
	    Math::Vector oldPosition;

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
			    velocity=Math::Vector(0,0,0);
	    }
	    oldPosition=GetEyePosition();

	    position=position+velocity*time;
    	
        sector->UpdateThingSector( this, oldPosition);

	    acceleration=Math::Vector(0,0,0);
	    thrusting=false;

	    if(type==PLAYER)
	    {
		    float xcos, xsin;
		    xcos=cos((rotation.y+orient.y)*M_PI/180);
		    xsin=sin((rotation.y+orient.y)*M_PI/180);
		    Math::Vector vector(xcos, xsin, 0);
		    rotation.z=vector*velocity*-3;
	    }
	    DoFoley();

	    if( keyTrack.key )
	    {
		    keyTrack.time += time;
	    }

        if( sprite )
        {
            spriteTime += time;
        }

	    if( killTime > -1 && GetTickCount() > killTime )
	    {
		    Destroy();
	    }

        if( pendingDestroy )
        {
            RealDestroy();
        }
    }

    void Thing::ArrestMotion(Math::Vector normal)
    {
	    if(acceleration*normal<0)
		    acceleration-=normal*(normal*acceleration);
	    if(velocity*normal<0)
		    velocity-=normal*(normal*velocity);
    }

    void Thing::UpdateThings( float time )
    {
	    int i;
	    shared_ptr<Thing> thing;

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
}