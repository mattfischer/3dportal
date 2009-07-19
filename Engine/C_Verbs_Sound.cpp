#include "C_Verb.h"

#include "JK_Level.h"
#include "S_Manager.h"
#include <boost/shared_ptr.hpp>
#include "W_Thing.h"

using boost::shared_ptr;

namespace Cog
{
    Type_Int PlaySoundLocal( Type_Sound sound_num, Type_Flex volume, Type_Flex pan, Type_Int flags) // ****
    {
        Sound::PlayLocal( currentLevel.sounds[sound_num], volume, pan );
	    return 0;
    }
    		
    Type_Int PlaySoundPos( Type_Sound sound_num, Type_Vector position, Type_Flex volume, Type_Flex minDistance, Type_Flex maxDistance, Type_Int flags) // *****
    {
	    if( minDistance != -1 ) minDistance /= 10;
	    if( maxDistance != -1 ) maxDistance /= 10;

        Sound::PlayPos( currentLevel.sounds[sound_num], position, volume, minDistance, maxDistance );
	    return 0;
    }

    Type_Int PlaySoundThing( Type_Sound sound_num, Type_Thing thing_num, Type_Flex volume, Type_Flex minDistance, Type_Flex maxDistance, Type_Int flags ) // ****
    {
        shared_ptr<World::Thing> thing;

	    if( minDistance != -1 ) minDistance /= 10;
	    if( maxDistance != -1 ) maxDistance /= 10;

	    thing = currentLevel.things[thing_num];
	    if( thing && sound_num != -1 )
	    {
            Sound::PlayThing( currentLevel.sounds[sound_num], thing, false, volume, minDistance, maxDistance );
	    }
	    return 0;
    }

    Type_Int GetSoundLen( Type_Sound sound_num )
    {
	    return 0;
    }
}