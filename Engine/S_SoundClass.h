#ifndef S_CLASS_H
#define S_CLASS_H

#include <string>

class Thing;

using std::string;

namespace Sound
{
    class Buffer;
    class Track;

    struct Class
    {
    public:
	    Class( const string& filename );

	    Sound::Track *Play( const string& sound, int thing );

    protected:
	    int numEntries;

        struct Entry
        {
	        string name;
	        Sound::Buffer *sound;
	        int flags;
	        float minDist;
	        float maxDist;
	        float volume;
        };

	    Entry *entries;
    };
}

#endif