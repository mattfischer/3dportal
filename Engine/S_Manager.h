#ifndef S_MANAGER_H
#define S_MANAGER_H
#include <windows.h>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

class S_Sound;
class S_SoundInstance;
class W_Thing;
class M_Vector;

void S_Initialize( HWND hWnd );
void S_PlaySector( S_Sound *sound, float volume );
void S_PlayLocal( S_Sound *sound, float volume, float pan );
S_SoundInstance *S_PlayThing( S_Sound *sound, shared_ptr<W_Thing> thing, bool loop, float volume, float minDist, float maxDist );
void S_PlayPos( S_Sound *sound, M_Vector position, float volume, float minDist, float maxDist );
void S_Update();

#endif