#include "C_Script.h"
#include "C_Stack.h"
#include "G_Console.h"

#include "M_Vector.h"

#include "R_Frame.h"

#include "W_Thing.h"
#include "W_Surface.h"

#include "JK_Level.h"
#include "JK_Template.h"

#include "G_Inventory.h"

#include "S_Manager.h"
#include "S_Sound.h"

#include <math.h>

extern R_Model* povModel;

void C_Script::ExecuteVerbCall( C_ASTNode *node, C_Context &c, bool expression )
{
	int paramsi[10];
	float paramsf[10];
	M_Vector paramsv[10];
	int resulti;
	float resultf;
	boost::shared_ptr<W_Thing> thing;

	int i;

	char buffer[100];
	bool found;

	if( !strcmp( (char*)node->lexData, "sendmessage" ) ) return; // ****
			
	for( i = node->numChildren - 1 ; i >= 0 ; i-- )
		ExecuteExpression( node->children[i], c );

	// ----- System -----

	if( !strcmp( (char*)node->lexData, "rand" ) )
	{
		node->type = C_TYPE_FLOAT;
		resultf = (float)rand();
		resultf /= (float)RAND_MAX;
		c.stack.PushFloat( resultf );
		return;
	}

	if( !strcmp( (char*)node->lexData, "getsenderid" ) )
	{
		found = false;

		for( i = 0 ; i < symbols.size() ; i++ )
			if( symbols[i].type == c.senderType && *(int*)symbols[i].data == c.sender)
			{
				c.stack.PushInt( symbols[i].linkId );
				found = true;
				break;
			}
		if( !found ) c.stack.PushInt( -1 );
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getsenderref" ) )
	{
		c.stack.PushInt( c.sender );
		node->type = (C_SymbolType)c.senderType;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getsourceref" ) )
	{
		c.stack.PushInt( c.source );
		return;
	}

	if( !strcmp( (char*)node->lexData, "sleep" ) )
	{
		Cast( node->children[0]->type, C_TYPE_FLOAT, c );
		paramsf[0] = c.stack.PopFloat();
		Sleep( paramsf[0] * 1000 );
		return;
	}

	if( !strcmp( (char*)node->lexData, "settimer" ) )
	{
		paramsf[0] = c.stack.PopFloat();
		StartTimer( paramsf[0] );
		return;
	}

	if( !strcmp( (char*)node->lexData, "getdifficulty" ) ) 
	{
		c.stack.PushInt( 0 );
		node->type = C_TYPE_INT;
		return;
	}
	
	if( !strcmp( (char*)node->lexData, "getleveltime" ) ) 
	{
		resultf = GetTickCount();
		resultf /= 1000.0;
		c.stack.PushFloat( resultf );
		node->type = C_TYPE_FLOAT;
		return;
	}
	
	if( !strcmp( (char*)node->lexData, "getautopickup" ) ) // ****
	{
		c.stack.PushInt( 0 );
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getautoreload" ) ) // ****
	{
		c.stack.PushInt( 0 );
		node->type = C_TYPE_INT;
		return;
	}

	
	// ----- Thing -----

	if( !strcmp( (char*)node->lexData, "isthingmoving" ) || 
		!strcmp( (char*)node->lexData, "ismoving" ) )
	{
		paramsi[0] = c.stack.PopInt();
		if( currentLevel.things[paramsi[0]]->IsMoving() )
			c.stack.PushInt( 1 );
		else
			c.stack.PushInt( 0 );
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "creatething" ) ) // ****
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		
		thing = shared_ptr<W_Thing>( new W_Thing( *currentLevel.things[paramsi[1]] ) );
		resulti = currentLevel.things.size();
		currentLevel.things.push_back( thing );
		
		thing->GetTemplate()->CopyParams( currentLevel.templates[paramsi[0]] );
		thing->ProcessTemplate();
		thing->GetSector()->AddThing( thing.get() );
		//resulti=W_Thing::Create(&currentLevel.templates[paramsi[0]], &currentLevel.things[paramsi[1]]);

		c.stack.PushInt( resulti );
		node->type = C_TYPE_THING;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getcurframe" ) )
	{
		paramsi[0] = c.stack.PopInt();
		thing = currentLevel.things[paramsi[0]];
		if( thing )
		{
			resulti = thing->GetCurrentFrame();
		}
		else
			resulti = 0;

		c.stack.PushInt( resulti );
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "movetoframe" ) )
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		paramsf[0] = c.stack.PopFloat();
		thing = currentLevel.things[paramsi[0]];
		if( thing )
		{
			thing->MoveToFrame( paramsi[1], paramsf[0] );
		}
		return;
	}

	if( !strcmp( (char*)node->lexData, "getthingsector" ) )
	{
		paramsi[0] = c.stack.PopInt();
		thing = currentLevel.things[paramsi[0]];
		if( thing )
		{
			c.stack.PushInt( thing->GetSector()->GetNum() );
		}
		else
			c.stack.PushInt( 0 );

		node->type = C_TYPE_SECTOR;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getlocalplayerthing" ) ||
		!strcmp( (char*)node->lexData, "jkgetlocalplayer" ) ) 
	{
		c.stack.PushInt( currentLevel.playerNum );
		node->type = C_TYPE_THING;		
		return;
	}

	if( !strcmp( (char*)node->lexData, "jumptoframe" ) ) 
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		paramsi[2] = c.stack.PopInt();
		thing = currentLevel.things[paramsi[0]];
		if( thing )
		{
			thing->JumpToFrame( paramsi[1], &currentLevel.sectors[paramsi[2]] );
		}
		return;
	}

	if( !strcmp( (char*)node->lexData, "takeitem" ) )
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();

		thing = currentLevel.things[paramsi[0]];
		if( thing )
		{
			thing->SendCogMessages( "taken", paramsi[1], true );

			currentLevel.things[paramsi[0]] = boost::shared_ptr<W_Thing>();
		}
		return;
	}

	if( !strcmp( (char*)node->lexData, "destroything" ) )
	{
		paramsi[0] = c.stack.PopInt();
		currentLevel.things[paramsi[0]] = boost::shared_ptr<W_Thing>();
		
		return;
	}

	if( !strcmp( (char*)node->lexData, "gethealth" ) ||
		!strcmp( (char*)node->lexData, "getthinghealth" ) )
	{
		paramsi[0] = c.stack.PopInt();
		c.stack.PushFloat( 100.0 );
		node->type = C_TYPE_FLOAT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "capturething" ) )
	{
		paramsi[0] = c.stack.PopInt();
		thing = currentLevel.things[paramsi[0]];
		if( thing )
		{
			thing->AddCogLink( this );
		}
		return;
	}


	// ----- Sound -----

	if( !strcmp( (char*)node->lexData, "playsoundlocal" ) ) // ****
	{
		paramsi[0] = c.stack.PopInt();
		paramsf[0] = c.stack.PopFlex( node->children[2]->type );
		paramsf[1] = c.stack.PopFlex( node->children[3]->type );
		paramsi[1] = c.stack.PopInt();

		S_PlayLocal( currentLevel.sounds[paramsi[0]], paramsf[0], paramsf[1] );
		if( expression )
		{
			c.stack.PushInt( 0 );
			node->type = C_TYPE_INT;
		}
		return;
	}

	if( !strcmp( (char*)node->lexData, "playsoundthing" ) ) // ****
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		paramsf[0] = c.stack.PopFlex( node->children[2]->type );
		paramsf[1] = c.stack.PopFlex( node->children[3]->type );
		paramsf[2] = c.stack.PopFlex( node->children[4]->type );
		paramsi[2] = c.stack.PopInt();

		if( paramsf[1] != -1 ) paramsf[1] /= 10;
		if( paramsf[2] != -1 ) paramsf[2] /= 10;
		thing = currentLevel.things[paramsi[1]];
		if( thing && paramsi[0]!=-1 )
		{
			S_PlayThing( currentLevel.sounds[paramsi[0]], thing, false, paramsf[0], paramsf[1], paramsf[2] );
		}

		if( expression )
		{
			c.stack.PushInt( 0 );
			node->type = C_TYPE_INT;
		}
		return;
	}

	if( !strcmp( (char*)node->lexData, "playsoundpos" ) ) // ****
	{
		paramsi[0] = c.stack.PopInt();
		paramsv[0] = c.stack.PopVector();
		paramsf[0] = c.stack.PopFlex( node->children[2]->type );
		paramsf[1] = c.stack.PopFlex( node->children[3]->type );
		paramsf[2] = c.stack.PopFlex( node->children[4]->type );
		paramsi[1] = c.stack.PopInt();

		if( paramsf[1] != -1 ) paramsf[1] /= 10;
		if( paramsf[2] != -1 ) paramsf[2] /= 10;
		S_PlayPos( currentLevel.sounds[paramsi[0]], paramsv[0], paramsf[0], paramsf[1], paramsf[2] );
		if( expression )
		{
			c.stack.PushInt( 0 );
			node->type = C_TYPE_INT;
		}
		return;
	}


	// ----- Sector -----

	if( !strcmp( (char*)node->lexData, "sectoradjoins" ) ||
		!strcmp( (char*)node->lexData, "setsectoradjoins" ) ) 
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		currentLevel.sectors[paramsi[0]].RenderAdjoins( paramsi[1] );

		return;
	}

	if( !strcmp( (char*)node->lexData, "sectorlight" ) ||
		!strcmp( (char*)node->lexData, "setsectorlight" ) ) 
	{
		paramsi[0] = c.stack.PopInt();
		paramsf[0] = c.stack.PopFloat();
		paramsf[1] = c.stack.PopFloat();
		currentLevel.sectors[paramsi[0]].SetLight( paramsf[0] );
		return;
	}



	// ----- Surface -----

	if( !strcmp( (char*)node->lexData, "setwallcel" ) ) 
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		c.stack.PushInt( currentLevel.surfaces[paramsi[0]].GetCel() );
		currentLevel.surfaces[paramsi[0]].SetCel( paramsi[1] );
		return;
	}

	if( !strcmp( (char*)node->lexData, "getwallcel" ) ) 
	{
		paramsi[0] = c.stack.PopInt();
		c.stack.PushInt( currentLevel.surfaces[paramsi[0]].GetCel() );
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getsurfacecenter" ) ||
		!strcmp( (char*)node->lexData, "surfacecenter" ) )  // ****
	{
		paramsi[0] = c.stack.PopInt();
		c.stack.PushVector( currentLevel.surfaces[paramsi[0]].GetPoly()[0].position );
		node->type = C_TYPE_VECTOR;
		return;
	}

	if( !strcmp( (char*)node->lexData, "surfaceanim" ) ) 
	{
		paramsi[0] = c.stack.PopInt();
		paramsf[0] = c.stack.PopFloat();
		paramsi[1] = c.stack.PopInt();
		
		currentLevel.surfaces[paramsi[0]].Animate( paramsf[0], paramsi[1] );
		return;
	}
	
	if( !strcmp( (char*)node->lexData, "stopsurfaceanim" ) ) 
	{
		paramsi[0] = c.stack.PopInt();
		
		currentLevel.surfaces[paramsi[0]].StopAnimate();
		return;
	}

	if( !strcmp( (char*)node->lexData, "slidewall" ) ) 
	{
		paramsi[0] = c.stack.PopInt();
		paramsv[0] = c.stack.PopVector();
		paramsf[0] = c.stack.PopFloat();

		currentLevel.surfaces[paramsi[0]].Slide( paramsv[0], paramsf[0] );
		return;
	}

	// ----- Anim/Key -----

	if( !strcmp( (char*)node->lexData, "jksetpovmodel" ) )
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();

		povModel = currentLevel.models[paramsi[1]];
		return;
	}

	if( !strcmp( (char*)node->lexData, "jkplaypovkey" ) ) // ****
	{
		c.stack.PopInt();
		c.stack.PopInt();
		c.stack.PopInt();
		c.stack.PopInt();

		c.stack.PushInt( 0 );
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getkeylen" ) ) // ****
	{
		c.stack.PopInt();

		c.stack.PushInt( 0 );
		node->type = C_TYPE_INT;
		return;
	}

	// ----- Inventory -----

	if( !strcmp( (char*)node->lexData, "getinv" ) )
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		c.stack.PushFloat( G_Items[paramsi[1]].value );
		node->type = C_TYPE_FLOAT;
		//c.stack.PushFloat(0);
		return;
	}

	if( !strcmp( (char*)node->lexData, "getinvmax" ) )
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		c.stack.PushFloat( G_Items[paramsi[1]].max );
		//c.stack.PushFloat(200);
		node->type = C_TYPE_FLOAT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "changeinv" ) )
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		paramsf[0] = c.stack.PopFloat();
		G_Items[paramsi[1]].value += paramsf[0];
		if( G_Items[paramsi[1]].value > G_Items[paramsi[1]].max )
			G_Items[paramsi[1]].value = G_Items[paramsi[1]].max;
		return;
	}



	// ----- Output -----

	if( !strcmp( (char*)node->lexData, "jkprintunistring" ) ) // ****
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		return;
	}
	

	
	// ----- Special Effects -----

	if( !strcmp( (char*)node->lexData, "adddynamictint" ) ) // ****
	{
		paramsi[0] = c.stack.PopInt();
		paramsf[0] = c.stack.PopFloat();
		paramsf[1] = c.stack.PopFloat();
		paramsf[2] = c.stack.PopFloat();
		return;
	}



	// ----- Weapon -----
	M_Vector fireOffset;
	M_Vector rotation;
	float xcos, xsin;
	float ycos, ysin;

	if( !strcmp( (char*)node->lexData, "fireprojectile" ) ) // ****
	{
		paramsi[0] = c.stack.PopInt();
		paramsi[1] = c.stack.PopInt();
		paramsi[2] = c.stack.PopInt();
		paramsi[3] = c.stack.PopInt();
		paramsv[0] = c.stack.PopVector();
		paramsv[1] = c.stack.PopVector();
		paramsf[0] = c.stack.PopFlex( node->children[6]->type );
		paramsi[4] = c.stack.PopInt();
		paramsf[1] = c.stack.PopFlex( node->children[8]->type );
		paramsf[2] = c.stack.PopFlex( node->children[9]->type );

		rotation=currentLevel.things[paramsi[0]]->GetRotation();
		xcos = cos( rotation.y * 3.14 / 180 );
		xsin = sin( rotation.y * 3.14 / 180 );

		M_Matrix m( xcos, -xsin, 0, 0,
				    xsin,  xcos, 0, 0,
				    0,     0,    1, 0,
				    0,     0,    0, 1
				  );

		ycos = cos( rotation.x * 3.14 / 180 );
		ysin = sin( rotation.x * 3.14 / 180 );

		M_Matrix m2( 1, 0,    0,    0,
				     0, ycos,-ysin, 0,
				     0, ysin, ycos, 0,
				     0, 0,    0,    1
				);
		fireOffset = m * m2 * paramsv[0];

		thing = shared_ptr<W_Thing>( new W_Thing( currentLevel.templates[paramsi[1]], 
			 									  currentLevel.things[paramsi[0]]->GetPosition() + fireOffset, 
												  M_Vector(0,0,0), 
												  currentLevel.things[paramsi[0]]->GetSector()
												)
									);
		resulti = currentLevel.things.size();
		thing->SetNum( resulti );
		currentLevel.things.push_back( thing );
		
		thing->ProcessTemplate();
		thing->SetVelocity( m * m2 * thing->GetVelocity() );
		thing->GetSector()->AddThing( thing.get() );

		S_PlayLocal( currentLevel.sounds[paramsi[2]], 1.0, 0 );

		c.stack.PushInt( resulti );
		node->type = C_TYPE_THING;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getweaponpriority" ) ) // ****
	{
		c.stack.PopInt();
		c.stack.PopInt();
		c.stack.PopInt();

		c.stack.PushInt(0);
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "getcurweapon" ) ) // ****
	{
		c.stack.PopInt();
		c.stack.PopInt();

		c.stack.PushInt(0);
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "selectweapon" ) ) // ****
	{
		c.stack.PopInt();
		c.stack.PopInt();

		c.stack.PushInt(0);
		node->type = C_TYPE_INT;
		return;
	}

	if( !strcmp( (char*)node->lexData, "autoselectweapon" ) ) // ****
	{
		c.stack.PopInt();
		c.stack.PopInt();

		c.stack.PushInt(0);
		node->type = C_TYPE_INT;
		return;
	}



	for( i = 0 ; i < node->numChildren ; i++ )
	{
		switch( BaseType( node->children[i]->type ) )
		{
		case C_TYPE_INT:
			c.stack.PopInt();
			break;
		case C_TYPE_FLOAT:
			c.stack.PopFloat();
			break;
		case C_TYPE_VECTOR:
			c.stack.PopVector();
			break;
		}
	}

	wsprintf( buffer, "%s: Call to undefined verb %s", name.c_str(), node->lexData );
	CON_Out( buffer );
}