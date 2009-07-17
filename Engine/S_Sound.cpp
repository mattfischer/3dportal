#include "S_Sound.h"

#include "JK_GOB.h"
#include <windows.h>

namespace Sound
{
    extern LPDIRECTSOUND lpDS;

    Buffer::~Buffer()
    {
	    lpDSB->Release();
    }

    Buffer::Buffer( const string& filename )
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

    void Buffer::duplicateBuffer(LPDIRECTSOUNDBUFFER *buffer)
    {
        lpDS->DuplicateSoundBuffer(lpDSB, buffer);
    }

    Track::Track(Buffer *b)
    {
        buf = b;

        b->duplicateBuffer(&lpDSB);

	    status = STATE_READY;
    }

    Track::~Track()
    {
	    lpDSB->Release();
    }

    Buffer *Track::buffer()
    {
        return buf;
    }

    void Track::Play(bool l)
    {
	    if(status==STATE_READY)
	    {
		    loop=l;
		    status=STATE_PENDING_START;
	    }
    }

    void Track::Stop()
    {
	    if(status==STATE_PLAYING) status=STATE_PENDING_STOP;
	    else if(status==STATE_PENDING_START) status=STATE_STOPPED;
    }

    void Track::SetVolume(float v)
    {
	    volume=v;
    }

    void Track::SetPan(float p)
    {
	    pan=p;
    }

    Track::PlayState Track::GetStatus()
    {
	    return status;
    }

    void Track::Update()
    {
	    DWORD bufferStatus;

	    lpDSB->SetVolume(-4000*(1-volume));
	    lpDSB->SetPan(10000*pan);

	    switch(status)
	    {
	    case STATE_PENDING_START:
		    lpDSB->Play(0,0, loop?DSBPLAY_LOOPING:0);
		    status=STATE_PLAYING;
		    break;
	    case STATE_PENDING_STOP:
		    lpDSB->Stop();
		    status=STATE_STOPPED;
		    break;
	    }

	    lpDSB->GetStatus(&bufferStatus);
	    if(!(bufferStatus&DSBSTATUS_PLAYING)) status=STATE_STOPPED;
    }
}