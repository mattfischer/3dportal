#include "C_Verb.h"

#include <boost/shared_ptr.hpp>
#include "W_Thing.h"
#include "W_Sector.h"
#include "JK_Level.h"

using boost::shared_ptr;

namespace Cog
{
    Type_Int GetCurFrame( Type_Thing thing_num )
    {
	    shared_ptr<World::Thing> thing;
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

    Type_Void MoveToFrame( Type_Thing thing_num, Type_Int frame, Type_Flex speed )
    {
	    shared_ptr<World::Thing> thing;
	    thing = currentLevel.things[thing_num];
	    if( thing )
	    {
		    thing->MoveToFrame( frame, speed );
	    }
    }

    Type_Sector GetThingSector( Type_Thing thing_num )
    {
	    shared_ptr<World::Thing> thing;
	    thing = currentLevel.things[thing_num];

	    if( thing )
		    return thing->GetSector()->GetNum();
	    else
		    return 0;
    }

    Type_Int IsThingMoving( Type_Thing thing_num )
    {
	    shared_ptr<World::Thing> thing;
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

    Type_Int IsMoving( Type_Thing thing_num )
    {
	    return IsThingMoving( thing_num );
    }


    Type_Thing GetLocalPlayerThing()
    {
	    return currentLevel.playerNum;
    }

    Type_Thing jkGetLocalPlayer()
    {
	    return currentLevel.playerNum;
    }

    Type_Void JumpToFrame( Type_Thing thing_num, Type_Int frame, Type_Sector sector_num )
    {
	    shared_ptr<World::Thing> thing;

	    thing = currentLevel.things[thing_num];
	    if( thing )
		    thing->JumpToFrame( frame, &currentLevel.sectors[sector_num] );
    }

    Type_Float GetThingHealth( Type_Thing thing_num )
    {
	    return 100;
    }

    Type_Float GetHealth( Type_Thing thing_num )
    {
	    return GetThingHealth( thing_num );
    }

    Type_Void DestroyThing( Type_Thing thing_num )
    {
        boost::shared_ptr<World::Thing> thing = currentLevel.things[thing_num];

        if(thing) 
        {
            thing->Destroy();
        }
    }

    Type_Thing CreateThing( Type_Thing template_num, Type_Thing position_num )
    {
	    shared_ptr<World::Thing> thing;
	    thing = currentLevel.things[position_num];

	    if( thing )
	    {
		    return World::Thing::CreateFromThing( thing.get(), currentLevel.templates[template_num] );
	    }

	    return -1;
    }

    Type_Void CaptureThing( Type_Thing thing_num, Script* script )
    {
	    shared_ptr<World::Thing> thing;

	    thing = currentLevel.things[thing_num];
	    if( thing )
	    {
		    thing->AddCogLink( script );
	    }
	    return;
    }

    Type_Vector GetThingLVec( Type_Thing thing_num )
    {
	    Math::Vector vec;

	    return vec;
    }
}