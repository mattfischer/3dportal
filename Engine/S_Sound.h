#ifndef S_SOUND_H
#define S_SOUND_H

#include <dsound.h>

#include "U_Collection.h"

extern LPDIRECTSOUND lpDS;

enum S_PlayState { S_EMPTY, S_READY, S_PENDING_START, S_PLAYING, S_PENDING_STOP, S_STOPPED };

class S_Sound : public U_NameableItem, public U_NumerableItem
{
public:
	S_Sound( const string& filename );
	S_Sound( S_Sound &s );
	
	~S_Sound();
	
	void Play( bool loop );
	void Stop();
	void SetVolume( float v );
	void SetPan( float p );
	void Update();
	S_PlayState GetStatus();
	
protected:
	LPDIRECTSOUNDBUFFER lpDSB;
	float volume;
	float pan;
	bool loop;
	S_PlayState status;
};

#endif