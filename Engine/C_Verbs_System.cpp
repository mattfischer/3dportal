#include "C_Verb.h"
#include <boost/shared_ptr.hpp>
#include "W_Thing.h"
#include "JK_Level.h"

using boost::shared_ptr;

C_Type_Int jkGetSaberCam()
{
    return 0; // ****
}

C_Type_Int GetCurrentCamera()
{
    return 0; // ****
}

C_Type_Int GetPrimaryFocus()
{
    return 0; // ****
}

C_Type_Void sleep( C_Type_Float delay )
{
	Sleep( delay * 1000 );
}

C_Type_Float Rand()
{
	return (float)rand() / (float)RAND_MAX;
}

C_Type_Void TakeItem( C_Type_Thing thing_num, C_Type_Thing player_num )
{
	shared_ptr<W_Thing> thing;
	
	thing = currentLevel.things[thing_num];
	if( thing )
	{
		thing->SendCogMessages( "taken", player_num, true );
		thing->Destroy();
	}
}

C_Type_Int GetAutoSwitch()
{
    return 0; // ****
}

C_Type_Variant GetSenderRef( C_Context& c )
{
	C_Type_Variant retval;

	retval.type = (C_SymbolType)c.senderType;
	retval.v_int = c.sender;
	return retval;
}

C_Type_Int GetSenderId( C_Context& c )
{
	return c.senderId;
}

C_Type_Int GetSourceRef( C_Context& c )
{
	return c.source;
}

C_Type_Int GetDifficulty() // ****
{
	return 0;
}

C_Type_Float GetLevelTime()
{
	return GetTickCount()/1000.0;
}

C_Type_Void SetTimer( C_Type_Flex time, C_Script* script )
{
	script->StartTimer( time );
}

C_Type_Vector VectorSet( C_Type_Flex x, C_Type_Flex y, C_Type_Flex z )
{
	return Math::Vector( x, y, z );
}

C_Type_Vector VectorScale( C_Type_Vector v, C_Type_Flex scale )
{
	return v * scale;
}

C_Type_Vector RandVec()
{
	return Math::Vector( (rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
}
