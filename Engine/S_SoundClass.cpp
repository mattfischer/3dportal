#include "JK_Flags.h"
#include "JK_Level.h"

#include "W_Thing.h"

#include "S_SoundClass.h"
#include "S_Sound.h"
#include "S_Manager.h"

namespace Sound
{
    Sound::Track *Class::Play( const string& sound, int thing_num )
    {
	    int i;
	    shared_ptr<W_Thing> thing;
	    Sound::Track *returnVal;

	    thing = currentLevel.things[thing_num];
	    if( thing )
	    {
		    for( i = 0 ; i < numEntries ; i++ )
			    if( entries[i].name == sound )
			    {
                    returnVal = Sound::PlayThing( entries[i].sound, thing, entries[i].flags&JK_SOUND_LOOP, entries[i].volume, entries[i].minDist, entries[i].maxDist );
				    break;
			    }
		    return returnVal;
	    }

	    return NULL;
    }
}
