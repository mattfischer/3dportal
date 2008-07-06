#include "C_Verb.h"

#include "JK_Level.h"
#include "S_Manager.h"
#include <boost/shared_ptr.hpp>
#include "W_Thing.h"

using boost::shared_ptr;

C_Type_Int PlaySoundLocal( C_Type_Sound sound_num, C_Type_Flex volume, C_Type_Flex pan, C_Type_Int flags) // ****
{
	S_PlayLocal( currentLevel.sounds[sound_num], volume, pan );
	return 0;
}
		
C_Type_Int PlaySoundPos( C_Type_Sound sound_num, C_Type_Vector position, C_Type_Flex volume, C_Type_Flex minDistance, C_Type_Flex maxDistance, C_Type_Int flags) // *****
{
	if( minDistance != -1 ) minDistance /= 10;
	if( maxDistance != -1 ) maxDistance /= 10;

	S_PlayPos( currentLevel.sounds[sound_num], position, volume, minDistance, maxDistance );
	return 0;
}

C_Type_Int PlaySoundThing( C_Type_Sound sound_num, C_Type_Thing thing_num, C_Type_Flex volume, C_Type_Flex minDistance, C_Type_Flex maxDistance, C_Type_Int flags ) // ****
{
	shared_ptr<W_Thing> thing;

	if( minDistance != -1 ) minDistance /= 10;
	if( maxDistance != -1 ) maxDistance /= 10;

	thing = currentLevel.things[thing_num];
	if( thing && sound_num != -1 )
	{
		S_PlayThing( currentLevel.sounds[sound_num], thing, false, volume, minDistance, maxDistance );
	}
	return 0;
}

C_Type_Int GetSoundLen( C_Type_Sound sound_num )
{
	return 0;
}