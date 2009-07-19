#ifndef S_MANAGER_H
#define S_MANAGER_H
#include <windows.h>
#include <boost/shared_ptr.hpp>

#include "M_Vector.h"

using boost::shared_ptr;

namespace World
{
    class Thing;
}

namespace Sound
{
    class Buffer;
    class Track;

    void Initialize( HWND hWnd );
    void PlaySector( Sound::Buffer *sound, float volume );
    void PlayLocal( Sound::Buffer *sound, float volume, float pan );
    Sound::Track *PlayThing( Sound::Buffer *sound, shared_ptr<World::Thing> thing, bool loop, float volume, float minDist, float maxDist );
    void PlayPos( Sound::Buffer *sound, Math::Vector position, float volume, float minDist, float maxDist );
    void Update();
}
#endif