#include "C_Verb.h"
#include <boost/shared_ptr.hpp>
#include "W_Thing.h"
#include "JK_Level.h"

using boost::shared_ptr;

namespace Cog
{
    Type_Int jkGetSaberCam()
    {
        return 0; // ****
    }

    Type_Int GetCurrentCamera()
    {
        return 0; // ****
    }

    Type_Int GetPrimaryFocus()
    {
        return 0; // ****
    }

    Type_Void sleep( Type_Float delay )
    {
	    Sleep( delay * 1000 );
    }

    Type_Float Rand()
    {
	    return (float)rand() / (float)RAND_MAX;
    }

    Type_Void TakeItem( Type_Thing thing_num, Type_Thing player_num )
    {
        shared_ptr<World::Thing> thing;
    	
	    thing = currentLevel.things[thing_num];
	    if( thing )
	    {
		    thing->SendCogMessages( "taken", player_num, true );
		    thing->Destroy();
	    }
    }

    Type_Int GetAutoSwitch()
    {
        return 0; // ****
    }

    Type_Variant GetSenderRef( Context& c )
    {
	    Type_Variant retval;

	    retval.type = (C_SymbolType)c.senderType;
	    retval.v_int = c.sender;
	    return retval;
    }

    Type_Int GetSenderId( Context& c )
    {
	    return c.senderId;
    }

    Type_Int GetSourceRef( Context& c )
    {
	    return c.source;
    }

    Type_Int GetDifficulty() // ****
    {
	    return 0;
    }

    Type_Float GetLevelTime()
    {
	    return GetTickCount()/1000.0;
    }

    Type_Void SetTimer( Type_Flex time, Script* script )
    {
	    script->StartTimer( time );
    }

    Type_Vector VectorSet( Type_Flex x, Type_Flex y, Type_Flex z )
    {
	    return Math::Vector( x, y, z );
    }

    Type_Vector VectorScale( Type_Vector v, Type_Flex scale )
    {
	    return v * scale;
    }

    Type_Vector RandVec()
    {
	    return Math::Vector( (rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
    }
}