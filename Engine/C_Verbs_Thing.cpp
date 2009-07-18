#include "C_Verb.h"

#include <boost/shared_ptr.hpp>
#include "W_Thing.h"
#include "JK_Level.h"

using boost::shared_ptr;

C_Type_Int GetCurFrame( C_Type_Thing thing_num )
{
	shared_ptr<W_Thing> thing;
	int frame;

	thing = currentLevel.things[thing_num];
	if( thing )
	{
		frame = thing->GetCurrentFrame();
	}
	else
		frame = 0;

	return frame;
}

C_Type_Void MoveToFrame( C_Type_Thing thing_num, C_Type_Int frame, C_Type_Flex speed )
{
	shared_ptr<W_Thing> thing;
	thing = currentLevel.things[thing_num];
	if( thing )
	{
		thing->MoveToFrame( frame, speed );
	}
}

C_Type_Sector GetThingSector( C_Type_Thing thing_num )
{
	shared_ptr<W_Thing> thing;
	thing = currentLevel.things[thing_num];

	if( thing )
		return thing->GetSector()->GetNum();
	else
		return 0;
}

C_Type_Int IsThingMoving( C_Type_Thing thing_num )
{
	shared_ptr<W_Thing> thing;
	thing = currentLevel.things[thing_num];
	
	if( thing )
	{
		if( thing->IsMoving() )
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

C_Type_Int IsMoving( C_Type_Thing thing_num )
{
	return IsThingMoving( thing_num );
}


C_Type_Thing GetLocalPlayerThing()
{
	return currentLevel.playerNum;
}

C_Type_Thing jkGetLocalPlayer()
{
	return currentLevel.playerNum;
}

C_Type_Void JumpToFrame( C_Type_Thing thing_num, C_Type_Int frame, C_Type_Sector sector_num )
{
	shared_ptr<W_Thing> thing;

	thing = currentLevel.things[thing_num];
	if( thing )
		thing->JumpToFrame( frame, &currentLevel.sectors[sector_num] );
}

C_Type_Float GetThingHealth( C_Type_Thing thing_num )
{
	return 100;
}

C_Type_Float GetHealth( C_Type_Thing thing_num )
{
	return GetThingHealth( thing_num );
}

C_Type_Void DestroyThing( C_Type_Thing thing_num )
{
    boost::shared_ptr<W_Thing> thing = currentLevel.things[thing_num];

    if(thing) 
    {
        thing->Destroy();
    }
}

C_Type_Thing CreateThing( C_Type_Thing template_num, C_Type_Thing position_num )
{
	shared_ptr<W_Thing> thing;
	thing = currentLevel.things[position_num];

	if( thing )
	{
		return W_Thing::CreateFromThing( thing.get(), currentLevel.templates[template_num] );
	}

	return -1;
}

C_Type_Void CaptureThing( C_Type_Thing thing_num, C_Script* script )
{
	shared_ptr<W_Thing> thing;

	thing = currentLevel.things[thing_num];
	if( thing )
	{
		thing->AddCogLink( script );
	}
	return;
}

C_Type_Vector GetThingLVec( C_Type_Thing thing_num )
{
	Math::Vector vec;

	return vec;
}