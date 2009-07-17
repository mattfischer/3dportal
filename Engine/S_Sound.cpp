#include "S_Sound.h"

#include "JK_GOB.h"
#include <windows.h>

S_Sound::~S_Sound()
{
	lpDSB->Release();
}

S_Sound::S_Sound( const string& filename )
{
	WAVEFORMATEX wfx;
	DSBUFFERDESC desc;
	DWORD numBytes;
	LPVOID lock1, lock2;
	DWORD lockLen1, lockLen2;
	char *cursor;
	char *data;
	int size;
	string fullname;
	bool found;

	fullname = "sound\\" + filename;
	found = false;
    if( Jk::Gob::getFile( fullname, &data, &size ) )
		found = true;
	else
	{
		fullname = "voice\\" + filename;
        if( Jk::Gob::getFile( fullname, &data, &size ) ) found = true;
		else found = false;
	}

	if( !found )
	{
		return;
	}

	cursor = data+20;
	memcpy( &wfx, cursor, sizeof( WAVEFORMATEX ) - 2 );
	wfx.cbSize = sizeof( WAVEFORMATEX );
	cursor += sizeof( WAVEFORMATEX ) - 2;

	cursor += 4;
	desc.dwSize = sizeof( DSBUFFERDESC );
	numBytes = *( DWORD* )( cursor );
	desc.dwBufferBytes = numBytes;
	desc.lpwfxFormat = &wfx;
	desc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS; 
	desc.dwReserved = 0;
	desc.guid3DAlgorithm = DS3DALG_DEFAULT;

	cursor += 4;

	lpDS->CreateSoundBuffer( &desc, &lpDSB, NULL );
	
	lpDSB->Lock( 0, 0, &lock1, &lockLen1, &lock2, &lockLen2, DSBLOCK_ENTIREBUFFER );
	memcpy( lock1, cursor, numBytes );
	lpDSB->Unlock( lock1, lockLen1, lock2, lockLen2 );
}

LPDIRECTSOUNDBUFFER S_Sound::buffer()
{
    return lpDSB;
}

S_SoundInstance::S_SoundInstance(S_Sound *s)
{
    snd = s;

    lpDS->DuplicateSoundBuffer(s->buffer(), &lpDSB);

	status = S_READY;
}

S_SoundInstance::~S_SoundInstance()
{
	lpDSB->Release();
}

S_Sound *S_SoundInstance::sound()
{
    return snd;
}

void S_SoundInstance::Play(bool l)
{
	if(status==S_READY)
	{
		loop=l;
		status=S_PENDING_START;
	}
}

void S_SoundInstance::Stop()
{
	if(status==S_PLAYING) status=S_PENDING_STOP;
	else if(status==S_PENDING_START) status=S_STOPPED;
}

void S_SoundInstance::SetVolume(float v)
{
	volume=v;
}

void S_SoundInstance::SetPan(float p)
{
	pan=p;
}

S_PlayState S_SoundInstance::GetStatus()
{
	return status;
}

void S_SoundInstance::Update()
{
	DWORD bufferStatus;

	lpDSB->SetVolume(-4000*(1-volume));
	lpDSB->SetPan(10000*pan);

	switch(status)
	{
	case S_PENDING_START:
		lpDSB->Play(0,0, loop?DSBPLAY_LOOPING:0);
		status=S_PLAYING;
		break;
	case S_PENDING_STOP:
		lpDSB->Stop();
		status=S_STOPPED;
		break;
	}

	lpDSB->GetStatus(&bufferStatus);
	if(!(bufferStatus&DSBSTATUS_PLAYING)) status=S_STOPPED;
}
