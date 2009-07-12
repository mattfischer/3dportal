#include "W_Thing.h"

#include "JK_Template.h"
#include "JK_Level.h"

#include "S_SoundClass.h"

#include "R_Model.h"

#include "C_Script.h"


void W_Thing::ProcessTemplate()
{
	string s;
	int i;
	float height;

	model = NULL;
	if( thingTemplate->GetString( "model3d", s ) )
		model = currentLevel.models[s];
		
	orient = M_Vector( 0, 0, 0 );
	thingTemplate->GetVector( "orient", orient );

	maxVelocity = 0;
	thingTemplate->GetFloat( "maxvel", maxVelocity );

	maxThrust = 0;
	thingTemplate->GetFloat( "maxthrust", maxThrust );

	maxRotVelocity = 0;
	thingTemplate->GetFloat( "maxrotvel", maxRotVelocity );

	maxRotThrust = 0;
	thingTemplate->GetFloat( "maxrotthrust", maxRotThrust );

	mass = 0;
	thingTemplate->GetFloat( "mass", mass );

	surfaceDrag = 0;
	thingTemplate->GetFloat( "surfdrag", surfaceDrag );

	staticDrag = 0;
	thingTemplate->GetFloat( "staticdrag", staticDrag );

	airDrag = 0;
	thingTemplate->GetFloat( "airdrag", airDrag );

	eyeOffset = M_Vector( 0, 0, 0 );
	thingTemplate->GetVector( "eyeoffset", eyeOffset );

	moveSize = 0;
	thingTemplate->GetFloat( "movesize", moveSize );

	size = 0;
	thingTemplate->GetFloat( "size", size );

	move = MOVE_NONE;
	thingTemplate->GetString( "move", s );
	if( s == "none" ) move = MOVE_NONE;
	if( s == "path" ) move = MOVE_PATH;
	if( s == "physics" ) move = MOVE_PHYSICS;

	thingFlags = 0;
	thingTemplate->GetHex( "thingflags", thingFlags );

	physicsFlags = 0;
	thingTemplate->GetHex( "physflags", physicsFlags );

	typeFlags = 0;
	thingTemplate->GetHex( "typeflags", typeFlags );

	rotVelocity = M_Vector( 0, 0, 0 );
	thingTemplate->GetVector( "angvel", rotVelocity );

	height = 0;
	if( thingTemplate->GetFloat( "height", height ) )
		insertOffset = M_Vector( 0, 0, height / 2 );
	else if( model != NULL )
		insertOffset = model->GetInsertOffset();
	else
		insertOffset = M_Vector( 0, 0, 0 );

	collide = 0;
	thingTemplate->GetInt( "collide", collide );

	jumpVelocity = 0;
	thingTemplate->GetFloat( "jumpspeed", jumpVelocity );
	
	thingTemplate->GetFrames( frames );

	if( thingTemplate->GetString( "cog", s ) )
		AddCogLink( currentLevel.cogScripts[s] );

	if( thingTemplate->GetString( "soundclass", s ) )
		soundClass = currentLevel.soundClasses[s];
	else soundClass = NULL;

	if( thingTemplate->GetString( "type", s ) )
	{
		if( s == "actor" )     type = ACTOR;
		if( s == "weapon" )    type = WEAPON;
		if( s == "item" )      type = ITEM;
		if( s == "explosion" ) type = EXPLOSION;
		if( s == "cog" )       type = COG;
		if( s == "ghost" )     type = GHOST;
		if( s == "corpse" )    type = CORPSE;
		if( s == "player" )    type = PLAYER;
	}
	else type = GHOST;

	velocity = M_Vector( 0, 0, 0 );
	thingTemplate->GetVector( "vel", velocity );
	
	animClass = NULL;
	if( thingTemplate->GetString( "puppet", s ) )
	{
		animClass = currentLevel.animClasses[s];
		if(animClass->modes.size() > 0)
		{
		    std::string filename = animClass->modes[0].submodes["stand"].key;
		    if(filename != "")
		    {
			keyInstance = JK_Key_Instance( currentLevel.keyframes[filename], 0, 0 );
		    }
		}
	}
    
	explodeTemplate = NULL;
	if( thingTemplate->GetString( "explode", s ) )
	{
		explodeTemplate = currentLevel.templates[s];
	}

	sprite = NULL;
	if( thingTemplate->GetString( "sprite", s ) )
	{
		sprite = currentLevel.sprites[s];
	}
}

void W_Thing::Explode()
{
	if( explodeTemplate )
	{
		W_Thing::Create( explodeTemplate, position, rotation, sector );
	}

	Destroy();
}